#include "lp_socker.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_dump.h"
#include "lp_net.h"
#include "lp_global.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif



//begin�������������ֿռ�
NS_LZPL_BEGIN



LPLock LPSocker::m_oSendBufLock;

const char *LPAPI SOCK_ERR_CODE::GetDescription()
{
	switch (eSockErrCode)
	{
	case LZPL::eSockErrCode_Fail:
		return lpGetErrorString(eErrorString_Fail);
	case LZPL::eSockErrCode_Success:
		return lpGetErrorString(eErrorString_Success);
	case LZPL::eSockErrCode_AcceptFail:
	case LZPL::eSockErrCode_ConnectFail:
	case LZPL::eSockErrCode_SendBufOverflow:
	case LZPL::eSockErrCode_PostRecvFail:
	case LZPL::eSockErrCode_PostSendFail:
	case LZPL::eSockErrCode_ReactorErrorEvent:
	case LZPL::eSockErrCode_RecvError:
	case LZPL::eSockErrCode_Total:
		return lpGetErrorString(eSockErrCode - eSockErrCode_AcceptFail + eErrorString_Socket_AcceptFail);
	default:
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:
	return lpGetErrorString(eErrorString_Unknow);
}

ILPSockerImpl* LPAPI ILPSockerImpl::NewSockerImpl()
{
	return new LPSocker();
}

void LPAPI ILPSockerImpl::DeleteSockerImpl(ILPSockerImpl* & pSocker)
{
	SAFE_DELETE(pSocker);
}

LPSocker::LPSocker()
{
	Reset();

	m_stRecvPerIoData.eIoOptType = eIoOptType_Recv;
	m_stRecvPerIoData.eHandlerType = eEventHandlerType_Socker;
	m_stSendPerIoData.eIoOptType = eIoOptType_Send;
	m_stSendPerIoData.eHandlerType = eEventHandlerType_Socker;
}

LPSocker::~LPSocker()
{

}

SOCKET LPAPI LPSocker::GetSock()
{
	return m_hSock;
}

LPUINT32 LPAPI LPSocker::GetSockerId()
{
	return m_dwSockerId;
}

LPUINT32 LPAPI LPSocker::GetRemoteIp()
{
	return m_dwRemoteIp;
}

const char* LPAPI LPSocker::GetRemoteIpStr()
{
	return m_szRemoteIpStr;
}

LPUINT16 LPAPI LPSocker::GetRemotePort()
{
	return m_wRemotePort;
}

LPUINT32 LPAPI LPSocker::GetLocalIp()
{
	return m_dwLocalIp;
}

const char* LPAPI LPSocker::GetLocalIpStr()
{
	return m_szLocalIpStr;
}

LPUINT16 LPAPI LPSocker::GetLocalPort()
{
	return m_wLocalPort;
}

BOOL LPAPI LPSocker::IsConnect()
{
	return m_bConnect ? TRUE : FALSE;
}

BOOL LPAPI LPSocker::Send(const char* pData, LPUINT32 dwLen)
{
	LPINT32 nResult = 0;
	BOOL bOverflow = FALSE;

	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(dwLen > 0);
	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	//����Ƿ�sock�Ƿ�������
	PROCESS_SUCCESS(!IsConnect());

	if (m_pSendLoopBuf->GetTotalWritableLen() < dwLen)
	{
		bOverflow = TRUE;
	}
	else
	{
		nResult = m_pSendLoopBuf->Write(pData, dwLen);
		LOG_PROCESS_ERROR(nResult);
	}

	if (bOverflow)
	{
		Close(SOCK_ERR_CODE(eSockErrCode_SendBufOverflow, 1, 0), FALSE);
		LOG_PROCESS_ERROR(FALSE);
	}

Exit1:
	return TRUE;
Exit0:
	return FALSE;
}

void LPAPI LPSocker::Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose)
{
	IMP("socker close, socker_id=%d, socket=%d, eSockErrCode=%d, dwParam=%d, dwErrno=%d, desc=%s",
		GetSockerId(), GetHandle(), stSockErrCode.eSockErrCode, stSockErrCode.dwParam, stSockErrCode.dwErrno, stSockErrCode.GetDescription());

	//����Ƿ�����
	PROCESS_SUCCESS(!IsConnect());
	SetConnect(false);

	//�رս������ӣ������رշ�������
	LOG_CHECK_ERROR(INVALID_SOCKET != GetSock());
	if (INVALID_SOCKET != GetSock())
	{
		if (!m_bSending && m_pSendLoopBuf->GetTotalReadableLen() == 0)
		{
			lpShutDown(GetSock(), SOCK_SEND);
		}
	}

	m_pNetImpl->GetEventMgr().PushTerminateEvent(this, GetSockerId(), bPassiveClose);

Exit1:
	return;
}

HANDLE LPAPI LPSocker::GetHandle()
{
	return (HANDLE)m_hSock;
}

e_EventHandlerType LPAPI LPSocker::GetEventHandlerType()
{
	return eEventHandlerType_Socker;
}

void LPAPI LPSocker::OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nLastError;

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Send
		|| pstPerIoData->eIoOptType == eIoOptType_Recv);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Socker);

	//����WSAGetLastError��õĽ���Ƿ��Ƕ�Ӧ����io����������������Ҫ��GetLastError�Ƚ�׼ȷ������
	nLastError = WSAGetLastError();

	//����lpCloseSocketʱ���ᴥ���ջ����Ӿ������������ִ�е�io������
	//�˴������ж����أ���ֹ�����Ѿ����ͷŵ�socker����
	if (!bOperateRet && ERROR_OPERATION_ABORTED == nLastError)
	{
		PROCESS_SUCCESS(TRUE);
	}

	if (bOperateRet)
	{
		if (0 == pstPerIoData->qwByteTransferred)
		{
			Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 2, nLastError), TRUE);
		}
		else
		{
			if (eIoOptType_Recv == pstPerIoData->eIoOptType)
			{
				OnRecv((LPUINT32)pstPerIoData->qwByteTransferred);
			}
			else
			{
				OnSend((LPUINT32)pstPerIoData->qwByteTransferred);
			}
		}
	}
	else
	{
		Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 1, nLastError), TRUE);
	}

Exit1:
Exit0:
	return;
}

void LPAPI LPSocker::SetConnect(bool bConnect)
{
	m_bConnect = bConnect;
}

void LPAPI LPSocker::SetSock(SOCKET sock)
{
	m_hSock = sock;
}

void LPAPI LPSocker::AttachPacketParser(ILPPacketParser* pPacketParser)
{
	LOG_PROCESS_ERROR(pPacketParser);

	m_pPacketParser = pPacketParser;
	m_pPacketParser->AddRef();

Exit0:
	return;
}

void LPAPI LPSocker::DetachPacketParser()
{
	LOG_PROCESS_ERROR(m_pPacketParser);

	m_pPacketParser->Release();
	m_pPacketParser = NULL;

Exit0:
	return;
}

void LPAPI LPSocker::Reset()
{
	SetConnect(false);

	m_bSending = false;

	SetAcceptCreate(FALSE);
	SetSock(INVALID_SOCKET);
	SetSockerId(INVALID_SOCKER_ID);
	SetParentId(0);

	m_pPacketParser = NULL;
	m_pNetImpl = NULL;

	m_dwRemoteIp = 0;
	m_wRemotePort = 0;
	m_szRemoteIpStr[0] = '\0';
	m_dwLocalIp = 0;
	m_wLocalPort = 0;
	m_szLocalIpStr[0] = '\0';

	m_pRecvLoopBuf = NULL;
	m_pSendLoopBuf = NULL;

	m_qwDelayCloseBeginTick = 0;
	m_qwDelayCloseDuration = 0;
	m_qwDelayReleaseBeginTick = 0;
	m_qwDelayReleaseDuration = 0;
}

BOOL LPAPI LPSocker::PostRecv()
{
	LPINT32 nResult = 0;

#	ifdef _WIN32
	DWORD dwReadLen;
	DWORD dwFlags = 0;
#	endif

	LOG_PROCESS_ERROR(m_pRecvLoopBuf);

	//���ղ����ж��Ƿ����ӣ����赣��socker�ѱ��ͷŵ�
	//PROCESS_SUCCESS(!IsConnect());

	memset(&m_stRecvPerIoData.stOverlapped, 0, sizeof(m_stRecvPerIoData.stOverlapped));
	m_stRecvPerIoData.stWsaBuf.buf = m_pRecvLoopBuf->WritePtr();
	m_stRecvPerIoData.stWsaBuf.len = m_pRecvLoopBuf->GetOnceWritableLen();
	if (IsAcceptCreate())
	{
		m_stRecvPerIoData.eHandlerType = eEventHandlerType_Socker;
	}
	else
	{
		m_stRecvPerIoData.eHandlerType = eEventHandlerType_Connector;
	}

#	ifdef _WIN32
	{
		nResult = WSARecv(GetSock(), &m_stRecvPerIoData.stWsaBuf, 1, &dwReadLen, &dwFlags, &m_stRecvPerIoData.stOverlapped, NULL);
	}
#	endif
	if (0 != nResult)
	{
		if (WSA_IO_PENDING != WSAGetLastError() && IsConnect())
		{
			ERR("WSARecv failed : %d", WSAGetLastError);
			Close(SOCK_ERR_CODE(eSockErrCode_PostRecvFail, 1, WSAGetLastError()), TRUE);
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPSocker::PostSend()
{
	LPINT32 nResult = 0;
	BOOL bSend = FALSE;
	DWORD dwNumberOfBytesSend = 0;
	LPUINT32 dwOnceReadableLen = 0;

	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	PROCESS_SUCCESS(!IsConnect());

	//�ж��Ƿ����������ڷ���
	if (m_bSending)
	{
		PROCESS_ERROR(FALSE);
	}

	dwOnceReadableLen = m_pSendLoopBuf->GetOnceReadableLen();
	if (dwOnceReadableLen > 0)
	{
		bSend = TRUE;
		m_bSending = true;

		m_stSendPerIoData.stWsaBuf.buf = m_pSendLoopBuf->ReadPtr();
		m_stSendPerIoData.stWsaBuf.len = dwOnceReadableLen;
	}

	if (bSend)
	{
		dwNumberOfBytesSend = 0;
		memset(&m_stSendPerIoData.stOverlapped, 0, sizeof(m_stSendPerIoData.stOverlapped));
		if (IsAcceptCreate())
		{
			m_stSendPerIoData.eHandlerType = eEventHandlerType_Socker;
		}
		else
		{
			m_stSendPerIoData.eHandlerType = eEventHandlerType_Connector;
		}

#		ifdef _WIN32
		{
			nResult = WSASend(GetSock(), &m_stSendPerIoData.stWsaBuf, 1, &dwNumberOfBytesSend, 0, &m_stSendPerIoData.stOverlapped, NULL);
		}
#		endif
		if (0 != nResult)
		{
			if (WSA_IO_PENDING != WSAGetLastError() && IsConnect())
			{
				ERR("WSASend failed : %d", WSAGetLastError);
				Close(SOCK_ERR_CODE(eSockErrCode_PostSendFail, 1, WSAGetLastError()), TRUE);
			}
		}
	}

Exit1:
Exit0:

	nResult = bSend;
	return nResult;
}

void LPAPI LPSocker::OnRecv(LPUINT32 dwBytes)
{
	LPINT32 nResult = 0;
	LPINT32 idwUsed = 0;

	LOG_PROCESS_ERROR(m_pRecvLoopBuf);
	LOG_PROCESS_ERROR(m_pPacketParser);

	m_pRecvLoopBuf->FinishWrite(dwBytes);

	while (m_pRecvLoopBuf->GetTotalReadableLen() > 0)
	{
		idwUsed = m_pPacketParser->Parse(m_pRecvLoopBuf);
		if (0 == idwUsed)
		{
			//����������������û��װ��һ������ֻ�öϿ�����
			if (m_pRecvLoopBuf->GetTotalWritableLen() <= 0)
			{
				Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 1, 0), FALSE);
				LOG_PROCESS_ERROR(FALSE);
			}

			break;
		}
		else if (idwUsed > 0)
		{
			if ((LPUINT32)idwUsed > m_pRecvLoopBuf->GetTotalReadableLen())
			{
				Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 2, 0), FALSE);
				LOG_PROCESS_ERROR(FALSE);
			}

			nResult = m_pNetImpl->GetEventMgr().PushRecvEvent(this, GetSockerId(), m_pRecvLoopBuf, idwUsed);
			if (!nResult)
			{
				Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 3, 0), FALSE);
				LOG_PROCESS_ERROR(FALSE);
			}
		}
		else
		{
			Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 4, 0), FALSE);
			LOG_PROCESS_ERROR(FALSE);
		}
	}

	nResult = PostRecv();
	LOG_PROCESS_ERROR(nResult);

	return;
Exit0:
	return;
}

void LPAPI LPSocker::OnSend(LPUINT32 dwBytes)
{
	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	m_pSendLoopBuf->FinishRead(dwBytes);

	//���������ڷ��ͱ�����
	m_bSending = false;

	if (!IsConnect() && m_pSendLoopBuf->GetTotalReadableLen() == 0)
	{
		lpShutDown(GetSock(), SOCK_SEND);
	}

Exit0:
	return;
}

void LPAPI LPSocker::AttachRecvBuf(LPLoopBuf* pLoopBuf)
{
	m_pRecvLoopBuf = pLoopBuf;
}

LPLoopBuf* LPAPI LPSocker::DetachRecvBuf()
{
	LPLoopBuf* pLoopBuf = m_pRecvLoopBuf;
	m_pRecvLoopBuf = NULL;
	return pLoopBuf;
}

void LPAPI LPSocker::AttachSendBuf(LPLoopBuf* pLoopBuf)
{
	m_pSendLoopBuf = pLoopBuf;
}

LPLoopBuf* LPAPI LPSocker::DetachSendBuf()
{
	LPLoopBuf* pLoopBuf = m_pSendLoopBuf;
	m_pSendLoopBuf = NULL;
	return pLoopBuf;
}

void LPAPI LPSocker::SetRemoteIp(LPUINT32 dwIp)
{
	m_dwRemoteIp = dwIp;
	lpStrCpyN(m_szRemoteIpStr, inet_ntoa((in_addr&)m_dwRemoteIp), IP_LEN);
}

void LPAPI LPSocker::SetRemotePort(LPUINT16 wPort)
{
	m_wRemotePort = wPort;
}

void LPAPI LPSocker::SetLocalIp(LPUINT32 dwIp)
{
	m_dwLocalIp = dwIp;
	memcpy(m_szLocalIpStr, inet_ntoa((in_addr&)m_dwLocalIp), IP_LEN);
}

void LPAPI LPSocker::SetLocalPort(LPUINT16 wPort)
{
	m_wLocalPort = wPort;
}

void LPAPI LPSocker::SetDelayCloseBeginTick(LPUINT64 qwTick)
{
	m_qwDelayCloseBeginTick = qwTick;
}

LPUINT64 LPAPI LPSocker::GetDelayCloseBeginTick()
{
	return m_qwDelayCloseBeginTick;
}

void LPAPI LPSocker::SetDelayCloseDuration(LPUINT64 qwDuration)
{
	m_qwDelayCloseDuration = qwDuration;
}

LPUINT64 LPAPI LPSocker::GetDelayCloseDuration()
{
	return m_qwDelayCloseDuration;
}

void LPAPI LPSocker::SetDelayReleaseBeginTick(LPUINT64 qwTick)
{
	m_qwDelayReleaseBeginTick = qwTick;
}

LPUINT64 LPAPI LPSocker::GetDelayReleaseBeginTick()
{
	return m_qwDelayReleaseBeginTick;
}

void LPAPI LPSocker::SetDelayReleaseDuration(LPUINT64 qwDuration)
{
	m_qwDelayReleaseDuration = qwDuration;
}

LPUINT64 LPAPI LPSocker::GetDelayReleaseDuration()
{
	return m_qwDelayReleaseDuration;
}

void LPAPI LPSocker::SetSockerId(LPUINT32 dwSockerId)
{
	m_dwSockerId = dwSockerId;
}

void LPAPI LPSocker::OnClose()
{
	//�ӳٹر�
	LOG_PROCESS_ERROR(m_pNetImpl);
	m_pNetImpl->GetSockerMgr().DelayClose(this);

Exit0:
	return;
}

void LPAPI LPSocker::SetParentId(LPUINT32 dwParentId)
{
	m_dwParentId = dwParentId;
}

LPUINT32 LPAPI LPSocker::GetParentId()
{
	return m_dwParentId;
}

void LPAPI LPSocker::SetAcceptCreate(BOOL bAcceptCreate)
{
	m_bAcceptCreate = bAcceptCreate;
}

BOOL LPAPI LPSocker::IsAcceptCreate()
{
	return m_bAcceptCreate;
}

void LPAPI LPSocker::SetPassiveClose(BOOL bPassiveClose)
{
	m_bPassiveClose = bPassiveClose;
}

BOOL LPAPI LPSocker::IsPassiveClose()
{
	return m_bPassiveClose;
}

void LPAPI LPSocker::SetNetImpl(LPNetImpl * pNetImpl)
{
	m_pNetImpl = pNetImpl;
}



//end�������������ֿռ�
NS_LZPL_END