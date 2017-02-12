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



//begin声明所处的名字空间
NS_LZPL_BEGIN



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

ILPSockerImpl* LPAPI ILPSockerImpl::NewSockerImpl(LPUINT32 dwIoType)
{
	switch (dwIoType)
	{
	case eIoType_CompletionPort:
		{
			return new LPWinNetSocker();
		}
		break;
	case eIoType_Epoll:
		{
			return new LPLinuxNetSocker();
		}
		break;
	case eIoType_None:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:
	return nullptr;
}

void LPAPI ILPSockerImpl::DeleteSockerImpl(ILPSockerImpl* & pSocker)
{
	SAFE_DELETE(pSocker);
}



LPSocker::LPSocker()
{
	Reset();
}

LPSocker::~LPSocker()
{

}

void LPAPI LPSocker::Reset()
{
	SetConnect(false);

	SetAcceptCreate(FALSE);
	SetSock(INVALID_SOCKET);
	SetSockerId(INVALID_SOCKER_ID);

	m_dwRemoteIp = 0;
	m_wRemotePort = 0;
	m_dwLocalIp = 0;
	m_wLocalPort = 0;

	SetParentId(0);

	m_pPacketParser = nullptr;
	SetNetImpl(nullptr);

	m_pRecvLoopBuf = nullptr;
	m_pSendLoopBuf = nullptr;

	m_qwDelayCloseBeginTick = 0;
	m_qwDelayCloseDuration = 0;
	m_qwDelayReleaseBeginTick = 0;
	m_qwDelayReleaseDuration = 0;

	m_stRecvOrSendPerIoData.eIoOptType = eIoOptType_RecvOrSend;
	m_stRecvOrSendPerIoData.eHandlerType = eEventHandlerType_Socker;
}

void LPAPI LPSocker::SetSock(SOCKET sock)
{
	m_hSock = sock;
}

SOCKET LPAPI LPSocker::GetSock()
{
	return m_hSock;
}

HANDLE LPAPI LPSocker::GetHandle()
{
	return (HANDLE)m_hSock;
}

void LPAPI LPSocker::SetSockerId(LPUINT32 dwSockerId)
{
	m_dwSockerId = dwSockerId;
}

LPUINT32 LPAPI LPSocker::GetSockerId()
{
	return m_dwSockerId;
}

void LPAPI LPSocker::SetRemoteIp(LPUINT32 dwIp)
{
	char szRemoteIpStr[IP_LEN];

	m_dwRemoteIp = dwIp;
	lpFastZeroCharArray(szRemoteIpStr);
	lpStrCpyN(szRemoteIpStr, inet_ntoa((in_addr&)m_dwRemoteIp), IP_LEN);
	m_strRemoteIp.append(szRemoteIpStr);
}

LPUINT32 LPAPI LPSocker::GetRemoteIp()
{
	return m_dwRemoteIp;
}

std::string& LPAPI LPSocker::GetRemoteIpStr()
{
	return m_strRemoteIp;
}

void LPAPI LPSocker::SetRemotePort(LPUINT16 wPort)
{
	m_wRemotePort = wPort;
}

LPUINT16 LPAPI LPSocker::GetRemotePort()
{
	return m_wRemotePort;
}

void LPAPI LPSocker::SetLocalIp(LPUINT32 dwIp)
{
	char szLocalIpStr[IP_LEN];

	m_dwLocalIp = dwIp;
	lpFastZeroCharArray(szLocalIpStr);
	memcpy(szLocalIpStr, inet_ntoa((in_addr&)m_dwLocalIp), IP_LEN);
	m_strLocalIp.append(szLocalIpStr);
}

LPUINT32 LPAPI LPSocker::GetLocalIp()
{
	return m_dwLocalIp;
}

std::string& LPAPI LPSocker::GetLocalIpStr()
{
	return m_strLocalIp;
}

void LPAPI LPSocker::SetLocalPort(LPUINT16 wPort)
{
	m_wLocalPort = wPort;
}

LPUINT16 LPAPI LPSocker::GetLocalPort()
{
	return m_wLocalPort;
}

void LPAPI LPSocker::SetConnect(bool bConnect)
{
	m_bConnect = bConnect;
}

BOOL LPAPI LPSocker::IsConnect()
{
	return m_bConnect ? TRUE : FALSE;
}

void LPAPI LPSocker::SetPassiveClose(BOOL bPassiveClose)
{
	m_bPassiveClose = bPassiveClose;
}

BOOL LPAPI LPSocker::IsPassiveClose()
{
	return m_bPassiveClose;
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

e_EventHandlerType LPAPI LPSocker::GetEventHandlerType()
{
	return eEventHandlerType_Socker;
}

PER_IO_DATA* LPAPI LPSocker::GetEventHandlerData()
{
	return &m_stRecvOrSendPerIoData;
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

void LPAPI LPSocker::SetNetImpl(LPNetImpl * pNetImpl)
{
	m_pNetImpl = pNetImpl;
}

LPNetImpl* LPAPI LPSocker::GetNetImpl()
{
	return m_pNetImpl;
}



LPWinNetSocker::LPWinNetSocker()
{
	Reset();
}

LPWinNetSocker::~LPWinNetSocker()
{

}

void LPAPI LPWinNetSocker::Reset()
{
	LPSocker::Reset();
	m_bSending = false;

	m_stRecvPerIoData.eIoOptType = eIoOptType_Recv;
	m_stRecvPerIoData.eHandlerType = eEventHandlerType_Socker;
	m_stSendPerIoData.eIoOptType = eIoOptType_Send;
	m_stSendPerIoData.eHandlerType = eEventHandlerType_Socker;
}

void LPAPI LPWinNetSocker::OnNetEvent(PER_IO_DATA* pstPerIoData)
{
	LPINT32 nLastError;

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Send
		|| pstPerIoData->eIoOptType == eIoOptType_Recv);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Socker);

	//这里WSAGetLastError获得的结果是否是对应本次io操作？？？或许需要用GetLastError比较准确？？？
	nLastError = WSAGetLastError();

	//调用lpCloseSocket时，会触发收回连接句柄关联的正在执行的io操作，
	//此处进行判断拦截，防止访问已经被释放的socker对象
	if (!pstPerIoData->bOperateRet && ERROR_OPERATION_ABORTED == nLastError)
	{
		PROCESS_SUCCESS(TRUE);
	}

	if (pstPerIoData->bOperateRet)
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

BOOL LPAPI LPWinNetSocker::Send(const char* pData, LPUINT32 dwLen)
{
	LPINT32 nResult = 0;
	BOOL bOverflow = FALSE;

	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(dwLen > 0);
	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	//检测是否sock是否连接中
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

void LPAPI LPWinNetSocker::Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose)
{
	IMP("socker close, socker_id=%d, socket=%d, eSockErrCode=%d, dwParam=%d, dwErrno=%d, desc=%s",
		GetSockerId(), GetHandle(), stSockErrCode.eSockErrCode, stSockErrCode.dwParam, stSockErrCode.dwErrno, stSockErrCode.GetDescription());

	//检测是否连接
	PROCESS_SUCCESS(!IsConnect());
	SetConnect(false);

	//关闭接收链接，但不关闭发送链接
	LOG_CHECK_ERROR(INVALID_SOCKET != GetSock());
	if (INVALID_SOCKET != GetSock())
	{
		if (!m_bSending && m_pSendLoopBuf->GetTotalReadableLen() == 0)
		{
			lpShutDown(GetSock(), SOCK_SEND);
		}
	}

	GetNetImpl()->GetEventMgr().PushTerminateEvent(this, GetSockerId(), bPassiveClose);

Exit1:
	return;
}

void LPAPI LPWinNetSocker::PostRecv()
{
	LPINT32 nResult = 0;

#	ifdef _WIN32
	DWORD dwReadLen;
	DWORD dwFlags = 0;
#	endif

	LOG_PROCESS_ERROR(m_pRecvLoopBuf);

	//接收不用判断是否链接，无需担心socker已被释放的
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
			ERR("WSARecv failed : %d", WSAGetLastError());
			Close(SOCK_ERR_CODE(eSockErrCode_PostRecvFail, 1, WSAGetLastError()), TRUE);
		}
	}

Exit0:
	return;
}

BOOL LPAPI LPWinNetSocker::PostSend()
{
	LPINT32 nResult = 0;
	BOOL bSend = FALSE;
	DWORD dwNumberOfBytesSend = 0;
	LPUINT32 dwOnceReadableLen = 0;

	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	PROCESS_SUCCESS(!IsConnect());

	//判断是否有数据正在发送
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
				ERR("WSASend failed : %d", WSAGetLastError());
				Close(SOCK_ERR_CODE(eSockErrCode_PostSendFail, 1, WSAGetLastError()), TRUE);
			}
		}
	}

Exit1:
Exit0:

	nResult = bSend;
	return nResult;
}

void LPAPI LPWinNetSocker::OnRecv(LPUINT32 dwBytes)
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
			//接收区已满，但还没有装下一个包，只好断开连接
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

			//m_pRecvLoopBuf通过引用而不是指针来传递，可以防止函数内部存储指针
			nResult = GetNetImpl()->GetEventMgr().PushRecvEvent(this, GetSockerId(), *m_pRecvLoopBuf, idwUsed);
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

	PostRecv();

	return;
Exit0:
	return;
}

void LPAPI LPWinNetSocker::OnSend(LPUINT32 dwBytes)
{
	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	m_pSendLoopBuf->FinishRead(dwBytes);

	//有数据正在发送标记清除
	m_bSending = false;

	if (!IsConnect() && m_pSendLoopBuf->GetTotalReadableLen() == 0)
	{
		lpShutDown(GetSock(), SOCK_SEND);
	}

Exit0:
	return;
}

void LPAPI LPWinNetSocker::OnClose()
{
	//延迟关闭
	LOG_PROCESS_ERROR(GetNetImpl());
	GetNetImpl()->GetSockerMgr().DelayClose(this);

Exit0:
	return;
}

LPLinuxNetSocker::LPLinuxNetSocker()
{
	Reset();
}

LPLinuxNetSocker::~LPLinuxNetSocker()
{

}

void LPAPI LPLinuxNetSocker::Reset()
{
	LPSocker::Reset();
}

void LPAPI LPLinuxNetSocker::OnNetEvent(PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	LPINT32 nLastError = 0;

	LOG_PROCESS_ERROR(m_pRecvLoopBuf);
	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_RecvOrSend);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Socker);

	if (pstPerIoData->ptEpollEvent->events & EPOLLRDHUP
		|| pstPerIoData->ptEpollEvent->events & EPOLLHUP
		|| pstPerIoData->ptEpollEvent->events & EPOLLERR)
	{
		Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 1, WSAGetLastError()), TRUE);
	}
	else if (pstPerIoData->ptEpollEvent->events & EPOLLIN)
	{
		do
		{
#			ifndef _WIN32
			{
				nResult = recv(GetSock(), m_pRecvLoopBuf->WritePtr(), m_pRecvLoopBuf->GetOnceWritableLen(), 0);
			}
#			endif

			if (nResult > 0)
			{
				OnRecv(nResult);
			}
			else if (nResult < 0)
			{
				nLastError = WSAGetLastError();
				if (nLastError == EINTR)
				{
					nResult = 1;//设置为大于0，重新recv
				}
				else if (EAGAIN != nLastError && EWOULDBLOCK != nLastError)
				{
					Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 2, WSAGetLastError()), TRUE);
				}
			}
			else //nResult == 0
			{
				Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 3, WSAGetLastError()), TRUE);
			}

		} while (nResult > 0);
	}
	else if (pstPerIoData->ptEpollEvent->events & EPOLLOUT)
	{
		//这里不调用PostSend，只能单线程发送
	}
	else
	{
		DBG("LPLinuxNetSocker::OnNetEvent recv unkonw events");
	}

Exit0:
	return;
}

BOOL LPAPI LPLinuxNetSocker::Send(const char* pData, LPUINT32 dwLen)
{
	LPINT32 nResult = 0;
	BOOL bOverflow = FALSE;

	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(dwLen > 0);
	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	//检测是否sock是否连接中
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

void LPAPI LPLinuxNetSocker::Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose)
{
	IMP("socker close, socker_id=%d, socket=%d, eSockErrCode=%d, dwParam=%d, dwErrno=%d, desc=%s",
		GetSockerId(), GetHandle(), stSockErrCode.eSockErrCode, stSockErrCode.dwParam, stSockErrCode.dwErrno, stSockErrCode.GetDescription());

	//检测是否连接
	PROCESS_SUCCESS(!IsConnect());
	SetConnect(false);

	//关闭接收链接，但不关闭发送链接
	LOG_CHECK_ERROR(INVALID_SOCKET != GetSock());
	if (INVALID_SOCKET != GetSock())
	{
		if (m_pSendLoopBuf->GetTotalReadableLen() == 0)
		{
			lpShutDown(GetSock(), SOCK_SEND);
		}
	}

	GetNetImpl()->GetEventMgr().PushTerminateEvent(this, GetSockerId(), bPassiveClose);

Exit1:
	return;
}

BOOL LPAPI LPLinuxNetSocker::PostSend()
{
	LPINT32 nResult = 0;
	LPINT32 nError = 0;

	LOG_PROCESS_ERROR(m_pSendLoopBuf);
	PROCESS_SUCCESS(!IsConnect());

	if (m_pSendLoopBuf->GetOnceReadableLen() > 0)
	{
#		ifndef _WIN32
		{
			nResult = write(GetSock(), m_pSendLoopBuf->ReadPtr(), dwOnceReadableLen);
		}
#		endif

		if (nResult < 0)
		{
			nError = WSAGetLastError();
			if ((EAGAIN != nError && EINTR != nError && EWOULDBLOCK != nError) && IsConnect())
			{
				Close(SOCK_ERR_CODE(eSockErrCode_PostSendFail, 1, nError), TRUE);
				LOG_PROCESS_ERROR_WITH_MSG(FALSE, "write failed, errno=%d", nError);
			}
		}
		else
		{
			OnSend(nResult);
		}
	}
	else
	{
		PROCESS_ERROR(FALSE);
	}

	return (m_pSendLoopBuf->GetTotalReadableLen() > 0);
Exit1:
Exit0:

	return FALSE;
}

void LPAPI LPLinuxNetSocker::OnRecv(LPUINT32 dwBytes)
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
			//接收区已满，但还没有装下一个包，只好断开连接
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

			//m_pRecvLoopBuf通过引用而不是指针来传递，可以防止函数内部存储指针
			nResult = GetNetImpl()->GetEventMgr().PushRecvEvent(this, GetSockerId(), *m_pRecvLoopBuf, idwUsed);
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

	return;
Exit0:
	return;
}

void LPAPI LPLinuxNetSocker::OnSend(LPUINT32 dwBytes)
{
	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	m_pSendLoopBuf->FinishRead(dwBytes);

	if (!IsConnect() && m_pSendLoopBuf->GetTotalReadableLen() == 0)
	{
		lpShutDown(GetSock(), SOCK_SEND);
	}

Exit0:
	return;
}

void LPAPI LPLinuxNetSocker::OnClose()
{
	//延迟关闭
	LOG_PROCESS_ERROR(GetNetImpl());
	GetNetImpl()->GetSockerMgr().DelayClose(this);

Exit0:
	return;
}



//end声明所处的名字空间
NS_LZPL_END