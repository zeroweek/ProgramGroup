#ifdef _WIN32

#include "lp_iocp.h"
#include "lpi_logger.h"
#include "lp_processerror.h"
#include "lp_string.h"
#include "lp_processerror.h"
#include "lpi_logger.h"
#include "lp_dump.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



#define IOCP_THREAD_PER_CPU     (2)



LZPL::LPNetOption::LPNetOption()
{

}

LZPL::LPNetOption::~LPNetOption()
{

}



LPLock LPSocker::m_oSendBufLock;




const char *LPAPI SOCK_ERR_CODE::GetDescription()
{
	switch (eSockErrCode)
	{
	case LZPL::eSockErrCode_Fail:
		return "失败";
		break;
	case LZPL::eSockErrCode_Success:
		return "成功";
		break;
	case LZPL::eSockErrCode_AcceptFail:
		return "accept失败";
		break;
	case LZPL::eSockErrCode_ConnectFail:
		return "connect失败";
		break;
	case LZPL::eSockErrCode_SendBufOverflow:
		return "发送缓冲区溢出";
		break;
	case LZPL::eSockErrCode_PostRecvFail:
		return "执行异步接收操作失败";
		break;
	case LZPL::eSockErrCode_PostSendFail:
		return "执行异步发送操作失败";
		break;
	case LZPL::eSockErrCode_ReactorErrorEvent:
		return "反应器错误事件";
		break;
	case LZPL::eSockErrCode_RecvError:
		return "接收数据错误";
		break;
	case LZPL::eSockErrCode_Max:
		LOG_CHECK_ERROR(FALSE);
		return "eSockErrCode_Max";
		break;
	default:
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:

	return "*** Unknow e_SockErrCode ***";
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

void LPAPI LPSocker::Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose)
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
			lpShutDown(GetSock(), SD_SEND);
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
	DWORD dwReadLen;
	DWORD dwFlags = 0;

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

	nResult = WSARecv(GetSock(), &m_stRecvPerIoData.stWsaBuf, 1, &dwReadLen, &dwFlags, &m_stRecvPerIoData.stOverlapped, NULL);
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
		nResult = WSASend(GetSock(), &m_stSendPerIoData.stWsaBuf, 1, &dwNumberOfBytesSend, 0, &m_stSendPerIoData.stOverlapped, NULL);
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

void LPAPI LPSocker::OnRecv(DWORD dwBytes)
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

void LPAPI LPSocker::OnSend(DWORD dwBytes)
{
	LOG_PROCESS_ERROR(m_pSendLoopBuf);

	m_pSendLoopBuf->FinishRead(dwBytes);

	//有数据正在发送标记清除
	m_bSending = false;

	if (!IsConnect() && m_pSendLoopBuf->GetTotalReadableLen() == 0)
	{
		lpShutDown(GetSock(), SD_SEND);
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
	//延迟关闭
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




LZPL::LPSockerMgr::LPSockerMgr()
{
	m_dwMaxSockId = INVALID_SOCKER_ID;

	m_oValidList.clear();
	m_oValidMap.clear();
	m_oDelayCloseList.clear();
	m_oDelayReleaseList.clear();

	m_bCheckDelayRun = FALSE;
	m_bPostSendRun = FALSE;

	m_pNetImpl = NULL;
	m_pRecvLoopBufPool = NULL;
	m_pSendLoopBufPool = NULL;

	m_tLastCheckTime = 0;
}

LPSockerMgr::~LPSockerMgr()
{
	UnInit();
}

BOOL LPAPI LPSockerMgr::Init(LPNetImpl* pNetImpl, LPUINT32 dwSize)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pNetImpl);
	m_pNetImpl = pNetImpl;

	IMP("m_oSockerPool init size : %d", dwSize);

	nResult = m_oSockerPool.Init(dwSize, TRUE);
	LOG_PROCESS_ERROR(nResult);

	m_pRecvLoopBufPool = new LPLoopBufPool();
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	nResult = m_pRecvLoopBufPool->Init(m_pNetImpl->GetNetConfig().dwRecvBufSize, TRUE, m_pNetImpl->GetNetConfig().dwConnectCount);
	LOG_PROCESS_ERROR(nResult);

	m_pSendLoopBufPool = new LPLoopBufPool();
	LOG_PROCESS_ERROR(m_pSendLoopBufPool);
	nResult = m_pSendLoopBufPool->Init(m_pNetImpl->GetNetConfig().dwSendBufSize, TRUE, m_pNetImpl->GetNetConfig().dwConnectCount);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oCheckDelayThread.Start(_CheckDelayThreadProc, this);
	LOG_PROCESS_ERROR(nResult);
	m_bCheckDelayRun = TRUE;

	nResult = m_oPostSendThread.Start(_PostSendThreadProc, this);
	LOG_PROCESS_ERROR(nResult);
	m_bPostSendRun = TRUE;

	return TRUE;

Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL LPAPI LPSockerMgr::UnInit()
{
	if (m_bPostSendRun)
	{
		m_bPostSendRun = FALSE;
		m_oPostSendThread.Wait();
	}

	if (m_bCheckDelayRun)
	{
		m_bCheckDelayRun = FALSE;
		m_oCheckDelayThread.Wait();
	}

	SAFE_DELETE(m_pRecvLoopBufPool);
	SAFE_DELETE(m_pSendLoopBufPool);

	return TRUE;
}

LPSocker* LPAPI LPSockerMgr::Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate)
{
	LPINT32 nResult = 0;
	LPSocker* pSocker = NULL;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	LOG_PROCESS_ERROR(m_pSendLoopBufPool);
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool->GetBufSize() == m_pNetImpl->GetNetConfig().dwRecvBufSize);
	LOG_PROCESS_ERROR(m_pSendLoopBufPool->GetBufSize() == m_pNetImpl->GetNetConfig().dwSendBufSize);

	pSocker = _Create(pPacketParser, dwParentId, bAcceptCreate);

	return pSocker;

Exit0:

	SAFE_DELETE(m_pRecvLoopBufPool);
	SAFE_DELETE(m_pSendLoopBufPool);

	return pSocker;
}

void LPAPI LPSockerMgr::Release(LPSocker* pSocker)
{
	LOG_CHECK_ERROR(pSocker);

	_Release(pSocker);
}

void LPAPI LPSockerMgr::DelayClose(LPSocker * pSocker)
{
	LOG_PROCESS_ERROR(pSocker);

	m_oDelayLock.Lock();
	pSocker->SetDelayCloseBeginTick(lpGetTickCountEx());
	if (pSocker->IsPassiveClose())
	{
		//被动关闭，无需延迟close socket
		pSocker->SetDelayCloseDuration(0);
	}
	else
	{
		//主动关闭，需要等未发送完的数据发送完
		pSocker->SetDelayCloseDuration(DELAY_CLOSE_SOCKET_TIME);
	}
	m_oDelayCloseList.push_back(pSocker);
	m_oDelayLock.UnLock();

Exit0:
	return;
}

void LPAPI LPSockerMgr::DelayRelease(LPSocker* pSocker)
{
	LOG_PROCESS_ERROR(pSocker);

	m_oDelayLock.Lock();
	pSocker->SetDelayReleaseBeginTick(lpGetTickCountEx());
	if (pSocker->IsPassiveClose())
	{
		//被动关闭，无需延迟close socket
		//无需考虑还有正在执行的io操作被退回而产生事件，导致访问已被释放的socker对象，
		//在socker的send、recv网络事件接口回调已经对io退回事件做了处理
		pSocker->SetDelayReleaseDuration(0);
	}
	else
	{
		//主动关闭，需要延迟释放，因为socker的close操作只关闭了发，
		//如果不延迟释放，可能在收的过程访问到被释放的socker
		pSocker->SetDelayReleaseDuration(DELAY_RELEASE_SOCKER_TIME);
	}
	m_oDelayReleaseList.push_back(pSocker);
	m_oDelayLock.UnLock();

Exit0:
	return;
}

void LPAPI LPSockerMgr::CheckDelay()
{
	LPINT32 nResult = 0;
	LPUINT64 qwTick = 0;
	LPSocker* pSocker = NULL;
	LPConnector* pConnector = NULL;
	std::list<SOCKET> ltCancelHandle;
	std::list<SOCKET>::iterator ch_lit;
	std::list<LPUINT32> ltConnectSockerParentId;
	std::list<LPUINT32>::iterator csp_lit;

	qwTick = lpGetTickCountEx();

	//检查延迟的关闭连接
	m_oDelayLock.Lock();
	for (LPListSocker::iterator it = m_oDelayCloseList.begin(); it != m_oDelayCloseList.end();)
	{
		pSocker = (*it);
		if (NULL == pSocker)
		{
			LOG_CHECK_ERROR(FALSE);
			it = m_oDelayCloseList.erase(it);
			continue;
		}

		if (qwTick >= pSocker->GetDelayCloseBeginTick() + pSocker->GetDelayCloseDuration())
		{
			if (INVALID_SOCKET != pSocker->GetSock())
			{
				ltCancelHandle.push_back(pSocker->GetSock());
			}
			else
			{
				LOG_CHECK_ERROR(FALSE);
			}

			it = m_oDelayCloseList.erase(it);
			DelayRelease(pSocker);

			continue;
		}

		++it;
	}

	//检查延迟释放的连接
	for (LPListSocker::iterator it = m_oDelayReleaseList.begin(); it != m_oDelayReleaseList.end();)
	{
		pSocker = (*it);
		if (NULL == pSocker)
		{
			LOG_CHECK_ERROR(FALSE);
			it = m_oDelayReleaseList.erase(it);
			continue;
		}

		if (qwTick >= pSocker->GetDelayReleaseBeginTick() + pSocker->GetDelayReleaseDuration())
		{
			it = m_oDelayReleaseList.erase(it);

			//如果是主动连接，需要通知连接器
			//需要在完全关闭，且释放socker之后再通知，防止重连后与旧的连接信息冲突
			if (!pSocker->IsAcceptCreate())
			{
				ltConnectSockerParentId.push_back(pSocker->GetParentId());
			}
			_Release(pSocker);
			continue;
		}

		++it;
	}
	m_oDelayLock.UnLock();

	//完全关闭连接close socket
	if (!ltCancelHandle.empty())
	{
		for (ch_lit = ltCancelHandle.begin(); ch_lit != ltCancelHandle.end(); ++ch_lit)
		{
			lpCancelIoEx((*ch_lit));
			lpCloseSocket((*ch_lit));
		}
	}

	//通知连接器连接已断开
	if (!ltConnectSockerParentId.empty())
	{
		for (csp_lit = ltConnectSockerParentId.begin(); csp_lit != ltConnectSockerParentId.end(); ++csp_lit)
		{
			pConnector = m_pNetImpl->FindConnector((*csp_lit));
			LOG_CHECK_ERROR(pConnector);
			if (pConnector)
			{
				pConnector->OnClose();
			}
		}
	}

	return;
}

LPINT32 LPAPI LPSockerMgr::PostSend()
{
	LPINT32 nResult = 0;
	LPSocker* pSocker = NULL;

	m_oLock.Lock();
	for (LPListSocker::iterator it = m_oValidList.begin(); it != m_oValidList.end(); ++it)
	{
		pSocker = (*it);
		if (NULL == pSocker)
		{
			LOG_CHECK_ERROR(FALSE);
			continue;
		}

		if (INVALID_SOCKET != pSocker->GetSock())
		{
			if (pSocker->PostSend())
			{
				++nResult;
			}
		}
		else
		{
			if (pSocker->IsConnect())
			{
				LOG_CHECK_ERROR(FALSE);
			}
		}
	}
	m_oLock.UnLock();

	return nResult;
}

LPSocker* LPAPI LPSockerMgr::_Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate)
{
	LPINT32 nResult = 0;
	LPSocker* pSocker = NULL;
	LPLoopBuf* pRecvLoopBuf = NULL;
	LPLoopBuf* pSendLoopBuf = NULL;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	LOG_PROCESS_ERROR(m_pSendLoopBufPool);

	pSocker = _MallocSocker();
	LOG_PROCESS_ERROR(pSocker);

	pRecvLoopBuf = m_pRecvLoopBufPool->Create();
	pSendLoopBuf = m_pSendLoopBufPool->Create();
	LOG_PROCESS_ERROR(pRecvLoopBuf);
	LOG_PROCESS_ERROR(pSendLoopBuf);

	pSocker->AttachRecvBuf(pRecvLoopBuf);
	pSocker->AttachSendBuf(pSendLoopBuf);

	pSocker->SetSockerId(_CreateSockId());
	pSocker->AttachPacketParser(pPacketParser);

	pSocker->SetParentId(dwParentId);
	pSocker->SetAcceptCreate(bAcceptCreate);

	pSocker->SetNetImpl(m_pNetImpl);

	m_oLock.Lock();
	m_oValidList.push_back(pSocker);
	m_oValidMap.insert(make_pair(pSocker->GetSockerId(), pSocker));
	m_oLock.UnLock();

	return pSocker;

Exit0:

	if (pSocker) _FreeSocker(pSocker);
	if (pRecvLoopBuf) m_pRecvLoopBufPool->Release(pRecvLoopBuf);
	if (pSendLoopBuf) m_pSendLoopBufPool->Release(pSendLoopBuf);

	return pSocker;
}

void LPAPI LPSockerMgr::_Release(LPSocker* pSocker)
{
	LPINT32 nResult = 0;
	LPLoopBuf* pRecvLoopBuf = NULL;
	LPLoopBuf* pSendLoopBuf = NULL;

	LOG_PROCESS_ERROR(pSocker);

	pRecvLoopBuf = pSocker->DetachRecvBuf();
	pSendLoopBuf = pSocker->DetachSendBuf();
	LOG_CHECK_ERROR(pRecvLoopBuf);
	LOG_CHECK_ERROR(pSendLoopBuf);

	pSocker->SetSock(INVALID_SOCKET);
	pSocker->SetSockerId(INVALID_SOCKER_ID);
	pSocker->DetachPacketParser();
	pSocker->SetNetImpl(NULL);

	m_oLock.Lock();
	m_oValidList.remove(pSocker);
	m_oValidMap.erase(pSocker->GetSockerId());
	m_oLock.UnLock();

	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	m_pRecvLoopBufPool->Release(pRecvLoopBuf);

	LOG_PROCESS_ERROR(m_pSendLoopBufPool);
	m_pSendLoopBufPool->Release(pSendLoopBuf);

	_FreeSocker(pSocker);

Exit0:
	return;
}

LPUINT32 LPAPI LPSockerMgr::_CreateSockId()
{
	return ++m_dwMaxSockId;
}


LPSocker* LPAPI LPSockerMgr::_MallocSocker()
{
	return m_oSockerPool.Malloc();
}

void LPAPI LPSockerMgr::_FreeSocker(LPSocker* pSocker)
{
	LOG_PROCESS_ERROR(pSocker);

	m_oSockerPool.Free(pSocker);

Exit0:
	return;
}
THREAD_FUNC_DECLARE(LPSockerMgr::_CheckDelayThreadProc)(void * pParam)
{
	LPINT32 nResult = 0;
	LPSockerMgr* pSockerMgr = NULL;

	LOG_PROCESS_ERROR(pParam);
	pSockerMgr = (LPSockerMgr*)pParam;
	LOG_PROCESS_ERROR(pSockerMgr);

	__TRY__
	{
		while (pSockerMgr->m_bCheckDelayRun)
		{
			pSockerMgr->CheckDelay();
			lpSleep(5);
		}
	}
	__EXCEPT__
	{

	}

	pSockerMgr->CheckDelay();


Exit0:

	return 0;
}
THREAD_FUNC_DECLARE(LPSockerMgr::_PostSendThreadProc)(void * pParam)
{
	LPINT32 nResult = 0;
	LPSockerMgr* pSockerMgr = NULL;

	LOG_PROCESS_ERROR(pParam);
	pSockerMgr = (LPSockerMgr*)pParam;
	LOG_PROCESS_ERROR(pSockerMgr);

	while (pSockerMgr->m_bPostSendRun)
	{
		if (pSockerMgr->PostSend() == 0)
		{
			lpSleep(1);
		}
	}

	pSockerMgr->PostSend();

Exit0:

	return 0;
}
LPSocker* LPSockerMgr::Find(LPUINT32 dwSockerId)
{
	LPSocker* pSocker = NULL;
	LPMapSocker::iterator fit;

	m_oLock.Lock();
	fit = m_oValidMap.find(dwSockerId);
	if (fit != m_oValidMap.end())
	{
		pSocker = fit->second;
	}
	m_oLock.UnLock();

	return pSocker;
}

LPUINT32 LPAPI LPSockerMgr::GetCurValidConnectCount()
{
	LPUINT32 dwValidCount = 0;

	m_oLock.Lock();
	dwValidCount = (LPUINT32)m_oValidMap.size();
	m_oLock.UnLock();

	return dwValidCount;
}


LPConnector::LPConnector()
{
	m_dwId = 0;
	SetState(eCommonState_NoInit);
	m_szIp[0] = '\0';
	m_dwPort = 0;
	SetReconnect(FALSE);
	m_hConnectSock = INVALID_SOCKET;
	m_pPacketParser = NULL;
	m_pNetImpl = NULL;
	m_pSocker = NULL;

	m_lpfnConnectEx = NULL;
	m_pstPerIoData = NULL;
}

LPConnector::~LPConnector()
{
	UnInit();
}

BOOL LPAPI LPConnector::Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(pNetImpl);

	m_dwId = dwId;
	m_pPacketParser = pPacketParser;
	m_pNetImpl = pNetImpl;
	m_pPacketParser->AddRef();

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

BOOL LPAPI LPConnector::UnInit()
{
	PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

	SetState(eCommonState_UnIniting);

	if (INVALID_SOCKET != m_hConnectSock)
	{
		CancelIo((HANDLE)m_hConnectSock);
		closesocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
	}

	if (NULL != m_pPacketParser)
	{
		m_pPacketParser->Release();
		m_pPacketParser = NULL;
	}

	m_pSocker = NULL;
	m_lpfnConnectEx = NULL;

	SAFE_DELETE(m_pstPerIoData);

	SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPConnector::Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReconnect)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pcszIp);

	LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
	SetState(eCommonState_Initing);

	m_dwPort = dwPort;
	lpStrCpyN(m_szIp, pcszIp, IP_LEN);

	SetReconnect(bReconnect);

	nResult = _InitConnectEx();
	LOG_PROCESS_ERROR(nResult);

	if (NULL == m_pstPerIoData)
	{
		m_pstPerIoData = new PER_IO_DATA();
	}
	LOG_PROCESS_ERROR(m_pstPerIoData);

	//lpSleep(10);
	SetState(eCommonState_Inited);

	LPPRINTF("start connecting %s:%d !\n", m_szIp, m_dwPort);
	IMP("start connecting %s:%d !", m_szIp, m_dwPort);

	nResult = _PostConnectEx(m_pstPerIoData);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

void LPAPI LPConnector::Stop()
{
	PROCESS_SUCCESS(GetState() >= eCommonState_Close);

	LOG_PROCESS_ERROR(GetState() == eCommonState_Inited);
	SetState(eCommonState_Close);

	// stop时，不需关闭socket，统一由ILPSocker的Close关闭
	//if (INVALID_SOCKET != m_hConnectSock)
	//{
	//	CancelIo((HANDLE)m_hConnectSock);
	//	closesocket(m_hConnectSock);
	//	m_hConnectSock = INVALID_SOCKET;
	//}


Exit1:
Exit0:
	return;
}

LPUINT32 LPAPI LPConnector::GetId()
{
	return m_dwId;
}

void LPAPI LPConnector::SetReconnect(BOOL bReconnect)
{
	m_bReconnect = bReconnect;
}

BOOL LPAPI LPConnector::IsReconnect()
{
	return m_bReconnect;
}

BOOL LPAPI LPConnector::Reconnect()
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(eCommonState_Inited == GetState());
	LOG_PROCESS_ERROR(NULL == GetSocker() && INVALID_SOCKET == m_hConnectSock);

	nResult = _PostConnectEx(m_pstPerIoData);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

void LPAPI LPConnector::Release()
{
	delete this;
}

HANDLE LPAPI LPConnector::GetHandle()
{
	return (HANDLE)m_hConnectSock;
}

e_EventHandlerType LPAPI LPConnector::GetEventHandlerType()
{
	return eEventHandlerType_Connector;
}

void LPConnector::OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	LPSocker* pSocker = NULL;
	sockaddr_in stLocalAddr;
	LPINT32 idwLocalAddrLen = 0;
	LPINT32 idwRetLocal = 0;
	sockaddr_in stRemoteAddr;
	LPINT32 idwRemoteAddrLen = 0;
	LPINT32 idwRetRemote = 0;
	const char cArg = 1;

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(eIoOptType_Connect == pstPerIoData->eIoOptType);

	LOG_PROCESS_ERROR(pstPerIoData->hSock == m_hConnectSock);

	switch (GetState())
	{
		case eCommonState_NoInit:
			{
				::closesocket(m_hConnectSock);
				LOG_PROCESS_ERROR(FALSE);
			}
			break;
		case eCommonState_Initing:
			{
				INF("connector is initing, discard connect", __FUNCTION__);
				::closesocket(m_hConnectSock);
				m_hConnectSock = INVALID_SOCKET;
				nResult = _PostConnectEx(m_pstPerIoData);
				LOG_PROCESS_ERROR(nResult);
				PROCESS_ERROR(FALSE);
			}
			break; 
		case eCommonState_Inited:
			{
				if (!bSuccess)
				{
					m_pNetImpl->GetEventMgr().PushConnectErrorEvent(this, WSAGetLastError());
					::closesocket(m_hConnectSock);
					m_hConnectSock = INVALID_SOCKET;
					SetSocker(NULL);
					nResult = _PostConnectEx(m_pstPerIoData);
					LOG_PROCESS_ERROR(nResult);
				}
				else
				{
					pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, FALSE);
					if (NULL == pSocker)
					{
						LOG_CHECK_ERROR(FALSE);
						::closesocket(m_hConnectSock);
						m_hConnectSock = INVALID_SOCKET;
						nResult = _PostConnectEx(m_pstPerIoData);
						LOG_PROCESS_ERROR(nResult);
						PROCESS_ERROR(FALSE);
					}

					IMP("connector create socker, socker_id=%d, hSock=%d !", pSocker->GetSockerId(), m_hConnectSock);

					//获取本地地址
					memset(&stLocalAddr, 0, sizeof(stLocalAddr));
					idwLocalAddrLen = sizeof(stLocalAddr);
					idwRetLocal = getsockname(m_hConnectSock, (sockaddr*)&stLocalAddr, &idwLocalAddrLen);
					if (0 != idwRetLocal)
					{
						LOG_CHECK_ERROR(FALSE);
						m_pNetImpl->GetSockerMgr().Release(pSocker);
						::closesocket(m_hConnectSock);
						m_hConnectSock = INVALID_SOCKET;
						nResult = _PostConnectEx(m_pstPerIoData);
						LOG_PROCESS_ERROR(nResult);
						PROCESS_ERROR(FALSE);
					}

					//获取远端地址
					memset(&stRemoteAddr, 0, sizeof(stRemoteAddr));
					idwRemoteAddrLen = sizeof(stRemoteAddr);
					idwRetRemote = getpeername(m_hConnectSock, (sockaddr*)&stRemoteAddr, &idwRemoteAddrLen);
					if (0 != idwRetLocal)
					{
						LOG_CHECK_ERROR(FALSE);
						m_pNetImpl->GetSockerMgr().Release(pSocker);
						::closesocket(m_hConnectSock);
						m_hConnectSock = INVALID_SOCKET;
						nResult = _PostConnectEx(m_pstPerIoData);
						LOG_PROCESS_ERROR(nResult);
						PROCESS_ERROR(FALSE);
					}

					//设置sock选项
					::setsockopt(m_hConnectSock, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
					::setsockopt(m_hConnectSock, IPPROTO_TCP, TCP_NODELAY, &cArg, sizeof(cArg));

					//设置LPSocker对象
					pSocker->SetSock(m_hConnectSock);
					pSocker->SetConnect(true);
					pSocker->SetRemoteIp(stRemoteAddr.sin_addr.s_addr);
					pSocker->SetRemotePort(ntohs(stRemoteAddr.sin_port));
					pSocker->SetLocalIp(stLocalAddr.sin_addr.s_addr);
					pSocker->SetLocalPort(ntohs(stLocalAddr.sin_port));

					//设置本连接器关联的socker对象
					//注意顺序：需要在socker收发数据之前设置，否则reactor处理获取socker为NULL
					SetSocker(pSocker);

					//push连接建立事件
					m_pNetImpl->GetEventMgr().PushEstablishEvent(pSocker, FALSE);
				}
			}
			break;
		case eCommonState_UnIniting:
		case eCommonState_UnInited:
			{
				::closesocket(m_hConnectSock);
				m_hConnectSock = INVALID_SOCKET;
				LOG_PROCESS_ERROR(FALSE);
			}
			break;
		case eCommonState_Close:
		{
			::closesocket(m_hConnectSock);
			m_hConnectSock = INVALID_SOCKET;
			PROCESS_SUCCESS(TRUE);
		}
		break;
	default:
		::closesocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit1:
Exit0:
	return;
}

void LPAPI LPConnector::OnClose()
{
	LPINT32 nResult = 0;

	m_hConnectSock = INVALID_SOCKET;
	SetSocker(NULL);

	if (IsReconnect())
	{
		nResult = Reconnect();
		LOG_PROCESS_ERROR(nResult);
	}

Exit0:
	return;
}

void LPAPI LPConnector::SetSocker(LPSocker* pSocker)
{
	m_pSocker = pSocker;
}

LPSocker* LPAPI LPConnector::GetSocker()
{
	return m_pSocker;
}

void LPAPI LPConnector::SetState(LPUINT32 dwState)
{
	m_dwState = dwState;
}

LPUINT32 LPAPI LPConnector::GetState()
{
	return m_dwState;
}

BOOL LPAPI LPConnector::_InitConnectEx()
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	DWORD dwBytes = 0;
	GUID stGuidAcceptEx = WSAID_CONNECTEX;

	hSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hSock)
	{
		FTL("%s ::WSASocket failed, %s:%d, errno %d", __FUNCTION__, m_szIp, m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}


	nResult = ::WSAIoctl(
		hSock,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&stGuidAcceptEx,
		sizeof(stGuidAcceptEx),
		&m_lpfnConnectEx,
		sizeof(LPFN_CONNECTEX),
		&dwBytes,
		NULL,
		NULL
		);
	LOG_PROCESS_ERROR(nResult == 0);

	//该sock只用于获取函数指针
	closesocket(hSock);

	LOG_PROCESS_ERROR(m_lpfnConnectEx);

	return TRUE;
Exit0:

	if (INVALID_SOCKET != hSock)
	{
		closesocket(hSock);
	}
	return FALSE;
}

BOOL LPAPI LPConnector::_PostConnectEx(PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	DWORD dwBytes = 0;
	sockaddr_in stAddr;
	sockaddr_in stLocalAddr;

	LOG_PROCESS_ERROR(pstPerIoData);

	m_hConnectSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_hConnectSock)
	{
		FTL("%s WSASocket failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	//需要如果不bind，后面调用ConnectEx会出现10022错误码
	memset(&stLocalAddr, 0, sizeof(stLocalAddr));
	stLocalAddr.sin_family = AF_INET;
	nResult = ::bind(m_hConnectSock, (sockaddr*)&stLocalAddr, sizeof(stLocalAddr));
	if (nResult < 0)
	{
		FTL("%s bind failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	//注意：connectorctr在此注册，之后连接创建的socker不需要重新注册（注册也会返回失败），
	//即socker与connectorctrl公用一个回调接口
	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	memset(&stAddr, 0, sizeof(stAddr));
	stAddr.sin_family = AF_INET;
	stAddr.sin_addr.s_addr = inet_addr(m_szIp);
	stAddr.sin_port = htons(m_dwPort);

	pstPerIoData->hSock = m_hConnectSock;
	pstPerIoData->eIoOptType = eIoOptType_Connect;
	pstPerIoData->eHandlerType = eEventHandlerType_Connector;
	memset(&pstPerIoData->stOverlapped, 0, sizeof(pstPerIoData->stOverlapped));

	dwBytes = 0;
	nResult = m_lpfnConnectEx(
		m_hConnectSock,
		(const sockaddr*)&stAddr,
		sizeof(stAddr),
		NULL,
		0,
		NULL,
		&pstPerIoData->stOverlapped
		);

	if (!nResult)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			FTL("%s m_lpfnConnectEx failed, errno %d", __FUNCTION__, WSAGetLastError());
			::closesocket(m_hConnectSock);
			m_hConnectSock = INVALID_SOCKET;
			PROCESS_ERROR(FALSE);
		}
	}

	return TRUE;
Exit0:

	if (INVALID_SOCKET != m_hConnectSock)
	{
		::closesocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
	}

	return FALSE;
}



LPListener::LPListener()
{
	m_dwId = 0;
	_SetState(eCommonState_NoInit);
	m_szIp[0] = '\0';
	m_dwPort = 0;
	m_bReUseAddr = FALSE;
	m_hListenSock = INVALID_SOCKET;
	m_pPacketParser = NULL;
	m_pNetImpl = NULL;

	m_lpfnAcceptEx = NULL;
	m_lpfnGetAcceptExSockaddrs = NULL;
	m_pstPerIoDataArray = NULL;
}

LPListener::~LPListener()
{
	UnInit();
}

BOOL LPAPI LPListener::Init(LPNetImpl* pNetImpl, ILPPacketParser * pPacketParser, LPUINT32 dwId)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(pNetImpl);

	m_dwId = dwId;
	m_pPacketParser = pPacketParser;
	m_pNetImpl = pNetImpl;
	m_pPacketParser->AddRef();

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

BOOL LPAPI LPListener::UnInit()
{
	PROCESS_SUCCESS(_GetState() == eCommonState_NoInit || _GetState() >= eCommonState_UnIniting);

	_SetState(eCommonState_UnIniting);

	if (INVALID_SOCKET != m_hListenSock)
	{
		CancelIo((HANDLE)m_hListenSock);
		closesocket(m_hListenSock);
		m_hListenSock = INVALID_SOCKET;
	}

	if (NULL != m_pPacketParser)
	{
		m_pPacketParser->Release();
		m_pPacketParser = NULL;
	}

	m_lpfnAcceptEx = NULL;
	m_lpfnGetAcceptExSockaddrs = NULL;

	if (NULL != m_pstPerIoDataArray)
	{
		SAFE_DELETE_SZ(m_pstPerIoDataArray);
	}

	_SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPListener::Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReUseAddr)
{
	LPINT32 nResult = 0;
	LPINT32 nReUse = 0;
	LINGER stLinger;
	sockaddr_in stAddr;

	LOG_PROCESS_ERROR(pcszIp);

	LOG_PROCESS_ERROR(eCommonState_NoInit == _GetState());
	_SetState(eCommonState_Initing);

	m_dwPort = dwPort;
	m_bReUseAddr = bReUseAddr;
	lpStrCpyN(m_szIp, pcszIp, IP_LEN);

	m_hListenSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (-1 == m_hListenSock)
	{
		FTL("%s create socket error, %s:%d, errno %d", __FUNCTION__, m_szIp, m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	stLinger.l_onoff = 1;
	stLinger.l_linger = 0;
	nResult = ::setsockopt(m_hListenSock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	LOG_CHECK_ERROR(0 == nResult);

	nReUse = 1;
	if (m_bReUseAddr)
	{
		::setsockopt(m_hListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&nReUse, sizeof(nReUse));
	}

	memset(&stAddr, 0, sizeof(stAddr));
	stAddr.sin_family = AF_INET;
	if (0 == strcmp(m_szIp, "0"))
	{
		stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		stAddr.sin_addr.s_addr = inet_addr(m_szIp);
	}
	stAddr.sin_port = htons(m_dwPort);

	nResult = ::bind(m_hListenSock, (sockaddr*)&stAddr, sizeof(stAddr));
	if (nResult < 0)
	{
		FTL("%s bind at %s:%d failed, errno %d", __FUNCTION__, m_szIp, m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = ::listen(m_hListenSock, 128);
	if (nResult < 0)
	{
		FTL("%s listen at %s:%d failed, errno %d", __FUNCTION__, m_szIp, m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	nResult = _InitAcceptEx();
	LOG_PROCESS_ERROR(nResult);

	if (NULL == m_pstPerIoDataArray)
	{
		m_pstPerIoDataArray = new PER_IO_DATA[POST_ACCEPTEX_COUNT];
	}
	LOG_PROCESS_ERROR(m_pstPerIoDataArray);

	//lpSleep(300);
	_SetState(eCommonState_Inited);

	IMP("start listening %s:%d !", m_szIp, m_dwPort);

	for (LPINT32 i = 0; i < POST_ACCEPTEX_COUNT; ++i)
	{
		nResult = _PostAcceptEx(&m_pstPerIoDataArray[i]);
		LOG_PROCESS_ERROR(nResult);
	}

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

void LPAPI LPListener::Stop()
{
	PROCESS_SUCCESS(_GetState() >= eCommonState_Close);

	LOG_PROCESS_ERROR(_GetState() == eCommonState_Inited);
	_SetState(eCommonState_Close);

	if (INVALID_SOCKET != m_hListenSock)
	{
		CancelIo((HANDLE)m_hListenSock);
		closesocket(m_hListenSock);
		m_hListenSock = INVALID_SOCKET;
	}

Exit1:
Exit0:
	return;
}

LPUINT32 LPAPI LPListener::GetId()
{
	return m_dwId;
}

void LPAPI LPListener::Release()
{
	delete this;
}

HANDLE LPAPI LPListener::GetHandle()
{
	return (HANDLE)m_hListenSock;
}

e_EventHandlerType LPAPI LPListener::GetEventHandlerType()
{
	return eEventHandlerType_Listener;
}

void LPAPI LPListener::OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	LPSocker* pSocker = NULL;
	const char cArg = 1;
	LPUINT32 dwCurValidConnectCount = 0;

	LPINT32 idwAddrLen = 0;
	LPINT32 idwRemoteAddrLen = 0;
	LPINT32 idwLocalAddrLen = 0;
	sockaddr_in* pstRemoteAddr = NULL;
	sockaddr_in* pstLocalAddr = NULL;

	LOG_PROCESS_ERROR(pstPerIoData);

	hSock = pstPerIoData->hSock;

	switch (_GetState())
	{
		case eCommonState_NoInit:
			{
				::closesocket(hSock);
				LOG_PROCESS_ERROR(FALSE);
			}
			break;
		case eCommonState_Initing:
			{
				INF("listener is initing, discard accept", __FUNCTION__);
				::closesocket(hSock);
				nResult = _PostAcceptEx(pstPerIoData);
				LOG_PROCESS_ERROR(nResult);
				PROCESS_ERROR(FALSE);
			}
			break; 
		case eCommonState_Inited:
			{
				if (!bSuccess)
				{
					FTL("%s accept failed, errno %d", __FUNCTION__, WSAGetLastError());
					closesocket(hSock);
					nResult = _PostAcceptEx(pstPerIoData);
					LOG_PROCESS_ERROR(nResult);
					PROCESS_ERROR(FALSE);
				}
				else
				{
					nResult = _PostAcceptEx(pstPerIoData);
					LOG_CHECK_ERROR(nResult);

					dwCurValidConnectCount = m_pNetImpl->GetSockerMgr().GetCurValidConnectCount();
					if (dwCurValidConnectCount >= m_pNetImpl->GetNetConfig().dwConnectCount)
					{
						INF("encounter max connect count, connect count(current/max): %d/%d !", 
							dwCurValidConnectCount, m_pNetImpl->GetNetConfig().dwConnectCount);
						closesocket(hSock);
						PROCESS_ERROR(FALSE);
					}

					pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, TRUE);
					if (NULL == pSocker)
					{
						LOG_CHECK_ERROR(FALSE);
						FTL("%s create LPSocker failed", __FUNCTION__);
						::closesocket(hSock);
						PROCESS_ERROR(FALSE);
					}

					IMP("listener create socker, socker_id=%d, hSock=%d !", pSocker->GetSockerId(), hSock);

					//设置sock选项
					::setsockopt(hSock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_hListenSock, sizeof(SOCKET));
					::setsockopt(hSock, IPPROTO_TCP, TCP_NODELAY, &cArg, sizeof(cArg));

					idwRemoteAddrLen = sizeof(sockaddr_in);
					idwLocalAddrLen = sizeof(sockaddr_in);
					idwAddrLen = sizeof(sockaddr_in) + 16;

					m_lpfnGetAcceptExSockaddrs(
						pstPerIoData->szBuf,
						0,
						idwAddrLen,
						idwAddrLen,
						(SOCKADDR**)&pstLocalAddr,
						&idwLocalAddrLen,
						(SOCKADDR**)&pstRemoteAddr,
						&idwRemoteAddrLen
						);

					//设置LPSocker对象
					pSocker->SetSock(hSock);
					pSocker->SetConnect(true);
					pSocker->SetRemoteIp(pstRemoteAddr->sin_addr.s_addr);
					pSocker->SetRemotePort(pstRemoteAddr->sin_port);
					pSocker->SetLocalIp(pstLocalAddr->sin_addr.s_addr);
					pSocker->SetLocalPort(pstLocalAddr->sin_port);

					//注册事件处理器
					nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(pSocker);
					if (!nResult)
					{
						LOG_CHECK_ERROR(FALSE);
						m_pNetImpl->GetSockerMgr().Release(pSocker);
						::closesocket(hSock);
						PROCESS_ERROR(FALSE);
					}

					//push连接建立事件
					m_pNetImpl->GetEventMgr().PushEstablishEvent(pSocker, TRUE);
				}
			}
			break;
		case eCommonState_UnIniting:
		case eCommonState_UnInited:
			{
				LOG_CHECK_ERROR(bSuccess == FALSE);
				::closesocket(hSock);
				PROCESS_SUCCESS(TRUE);
			}
			break;
		case eCommonState_Close:
		{
			::closesocket(hSock);
			PROCESS_SUCCESS(TRUE);
		}
		break;
	default:
		::closesocket(hSock);
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit1:
Exit0:
	return;
}

LPUINT32 LPAPI LPListener::_GetState()
{
	return m_dwState;
}

void LPAPI LPListener::_SetState(LPUINT32 dwState)
{
	m_dwState = dwState;
}

BOOL LPAPI LPListener::_InitAcceptEx()
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	DWORD dwBytes = 0;
	GUID stGuidAcceptEx = WSAID_ACCEPTEX;
	GUID stGuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;

	hSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hSock)
	{
		FTL("%s WSASocket failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = ::WSAIoctl(
		hSock,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&stGuidAcceptEx,
		sizeof(stGuidAcceptEx),
		&m_lpfnAcceptEx,
		sizeof(LPFN_ACCEPTEX),
		&dwBytes,
		NULL,
		NULL
		);
	LOG_PROCESS_ERROR(nResult == 0);

	dwBytes = 0;
	nResult = ::WSAIoctl(
		hSock,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&stGuidGetAcceptExSockaddrs,
		sizeof(stGuidGetAcceptExSockaddrs),
		&m_lpfnGetAcceptExSockaddrs,
		sizeof(LPFN_GETACCEPTEXSOCKADDRS),
		&dwBytes,
		NULL,
		NULL
		);
	LOG_PROCESS_ERROR(nResult == 0);

	//该sock只用于获取函数指针
	closesocket(hSock);

	LOG_PROCESS_ERROR(m_lpfnAcceptEx);
	LOG_PROCESS_ERROR(m_lpfnGetAcceptExSockaddrs);

	return TRUE;
Exit0:

	if (INVALID_SOCKET != hSock)
	{
		closesocket(hSock);
	}
	return FALSE;
}

BOOL LPAPI LPListener::_PostAcceptEx(PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	SOCKET hNewSock = INVALID_SOCKET;
	DWORD dwBytes = 0;

	LOG_PROCESS_ERROR(pstPerIoData);

	hNewSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hNewSock)
	{
		FTL("%s WSASocket failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	pstPerIoData->hSock = hNewSock;
	pstPerIoData->eIoOptType = eIoOptType_Accept;
	pstPerIoData->eHandlerType = eEventHandlerType_Listener;
	memset(&pstPerIoData->stOverlapped, 0, sizeof(pstPerIoData->stOverlapped));

	nResult = m_lpfnAcceptEx(
		m_hListenSock,
		hNewSock,
		pstPerIoData->szBuf,
		0,
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		&dwBytes,
		&pstPerIoData->stOverlapped
		);

	if (!nResult)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			FTL("%s m_lpfnAcceptEx failed, errno %d", __FUNCTION__, WSAGetLastError());
			::closesocket(hNewSock);
			PROCESS_ERROR(FALSE);
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}



LPEventMgr::LPEventMgr()
{
	m_bInit = FALSE;
	m_pNetMessageHandler = NULL;
	m_pNetImpl = NULL;
	m_nEventListCount = 0;
	m_pEventList = NULL;
	m_pEventListLock = NULL;
}

LPEventMgr::~LPEventMgr()
{
	UnInit();
}

BOOL LPAPI LPEventMgr::Init(LPNetImpl* pNetImpl, ILPNetMessageHandler* pNetMessageHandler, LPUINT32 dwSize, LPINT32 nEventListCount)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pNetMessageHandler);
	LOG_PROCESS_ERROR(pNetImpl);
	LOG_PROCESS_ERROR(nEventListCount > 0);
	LOG_PROCESS_ERROR(dwSize > 0);

	m_pNetMessageHandler = pNetMessageHandler;
	m_pNetImpl = pNetImpl;

	nResult = m_oEventPool.Init(dwSize, TRUE);
	LOG_PROCESS_ERROR(nResult);

	m_pRecvLoopBuf = new LPLoopBuf();
	LOG_PROCESS_ERROR(m_pRecvLoopBuf);

	nResult = m_pRecvLoopBuf->Init(m_pNetImpl->GetNetConfig().dwNetRecvEventBufSize);
	LOG_PROCESS_ERROR(nResult);

	m_pPacketTempBuf = new char[MAX_PACKET_LEN];
	LOG_PROCESS_ERROR(m_pPacketTempBuf);

	m_nEventListCount = nEventListCount;
	m_pEventList = new LPListEvent[m_nEventListCount];
	LOG_PROCESS_ERROR(m_pEventList);
	m_pEventListLock = new LPLock[m_nEventListCount];
	LOG_PROCESS_ERROR(m_pEventListLock);

	m_bInit = TRUE;

	return TRUE;

Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL LPAPI LPEventMgr::UnInit()
{
	m_bInit = FALSE;

	SAFE_DELETE(m_pRecvLoopBuf);
	SAFE_DELETE_SZ(m_pPacketTempBuf);
	SAFE_DELETE_SZ(m_pEventListLock);
	SAFE_DELETE_SZ(m_pEventList);

	return TRUE;
}

BOOL LPAPI LPEventMgr::PushRecvEvent(LPSocker* pSocker, LPUINT32 dwSockerId, ILPLoopBuf* pLoopBuf, LPUINT32 dwLen)
{
	LPINT32 nResult = 0;
	LPINT32 nRetryCount = 0;
	LPUINT32 dwLineSize = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pLoopBuf);
	LOG_PROCESS_ERROR(pSocker->GetSockerId() == dwSockerId);
	LOG_PROCESS_ERROR(pLoopBuf->GetTotalReadableLen() >= dwLen);

	pstEvent = _MallocEvent();
	LOG_PROCESS_ERROR(pstEvent);

	while (m_pRecvLoopBuf->GetTotalWritableLen() < dwLen)
	{
		WRN("function %s in file %s at line %d : buf not enough, sleep and try again !", __FUNCTION__, __FILE__, __LINE__);
		lpSleep(1);
		nRetryCount++;

		// 超过30秒，丢弃
		if (nRetryCount > 30000)
		{
			pLoopBuf->FinishRead(dwLen);
			LOG_PROCESS_ERROR(FALSE);
		}
	}

	dwLineSize = pLoopBuf->GetOnceReadableLen();
	if (dwLineSize > dwLen)
	{
		dwLineSize = dwLen;
	}

	nResult = m_pRecvLoopBuf->Write(pLoopBuf->ReadPtr(), dwLineSize);
	LOG_CHECK_ERROR(nResult);

	pLoopBuf->FinishRead(dwLineSize);

	if (dwLineSize < dwLen)
	{
		nResult = m_pRecvLoopBuf->Write(pLoopBuf->ReadPtr(), dwLen - dwLineSize);
		LOG_CHECK_ERROR(nResult);

		pLoopBuf->FinishRead(dwLen - dwLineSize);
	}

	pstEvent->eEventType = eEventType_Recv;
	pstEvent->dwFlag = dwSockerId;
	pstEvent->stUn.stRecvEvent.pSocker = pSocker;
	pstEvent->stUn.stRecvEvent.dwLen = dwLen;

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return TRUE;

Exit0:

	if (pstEvent) _FreeEvent(pstEvent);

	return FALSE;
}

void LPAPI LPEventMgr::PushTerminateEvent(LPSocker* pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose)
{
	LPINT32 nResult = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pSocker);

	pstEvent = _MallocEvent();
	LOG_PROCESS_ERROR(pstEvent);

	pstEvent->eEventType = eEventType_Terminate;
	pstEvent->dwFlag = dwSockerId;
	pSocker->SetPassiveClose(bPassiveClose);
	pstEvent->stUn.stTerminateEvent.pSocker = pSocker;
	pstEvent->stUn.stTerminateEvent.dwSockerId = dwSockerId;

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return;

Exit0:

	if (pstEvent) _FreeEvent(pstEvent);

	return;
}

void LPAPI LPEventMgr::PushEstablishEvent(LPSocker* pSocker, BOOL bAccept)
{
	LPINT32 nResult = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pSocker);

	pstEvent = _MallocEvent();
	LOG_PROCESS_ERROR(pstEvent);

	pstEvent->eEventType = eEventType_Establish;
	pstEvent->dwFlag = pSocker->GetSockerId();
	pstEvent->stUn.stEstablishEvent.pSocker = pSocker;
	pstEvent->stUn.stEstablishEvent.bAccept = bAccept;

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return;

Exit0:

	if (pstEvent) _FreeEvent(pstEvent);

	return;
}

void LPAPI LPEventMgr::PushConnectErrorEvent(LPConnector * pConnector, LPUINT32 dwErrorNo)
{
	LPINT32 nResult = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pConnector);

	pstEvent = _MallocEvent();
	LOG_PROCESS_ERROR(pstEvent);

	pstEvent->eEventType = eEventType_ConnectError;
	pstEvent->dwFlag = pConnector->GetId();
	pstEvent->stUn.stConnectErrorEvent.pConnector = pConnector;
	pstEvent->stUn.stConnectErrorEvent.dwErrorNo = dwErrorNo;

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return;

Exit0:

	if (pstEvent) _FreeEvent(pstEvent);

	return;
}

BOOL LPEventMgr::HaveEventForHandled()
{
	for (LPINT32 i = 0; i < m_nEventListCount; ++i)
	{
		if (!m_pEventList[i].empty())
		{
			return TRUE;
		}
	}

	return FALSE;
}

void LPEventMgr::HandleOneEvent()
{
	LPINT32 nResult = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	for (LPINT32 i = 0; i < m_nEventListCount; ++i)
	{
		if (m_pEventList[i].empty())
		{
			continue;
		}

		m_pEventListLock[i].Lock();
		if (m_pEventList[i].empty())
		{
			m_pEventListLock[i].UnLock();
			continue;
		}

		pstEvent = m_pEventList[i].front();
		m_pEventList[i].pop_front();
		m_pEventListLock[i].UnLock();

		LOG_PROCESS_ERROR(pstEvent);

		switch (pstEvent->eEventType)
		{
			case eEventType_Recv:
				{
					_ProcRecvEvent(&(pstEvent->stUn.stRecvEvent));
				}
				break;
			case eEventType_Terminate:
				{
					_ProcTerminateEvent(&(pstEvent->stUn.stTerminateEvent));
				}
				break;
			case eEventType_Establish:
				{
					_ProcEstablishEvent(&(pstEvent->stUn.stEstablishEvent));
				}
				break;
			case eEventType_ConnectError:
				{
					_ProcConnectErrorEvent(&(pstEvent->stUn.stConnectErrorEvent));
				}
				break;
			default:
				LOG_PROCESS_ERROR(FALSE);
				break;
		}
	}

Exit1:

Exit0:

	if (pstEvent) _FreeEvent(pstEvent);

	return;
}

LZPL::NET_EVENT* LPAPI LPEventMgr::_MallocEvent()
{
	return m_oEventPool.Malloc();
}

void LPAPI LPEventMgr::_FreeEvent(NET_EVENT* pstEvent)
{
	if (NULL != pstEvent)
	{
		m_oEventPool.Free(pstEvent);
	}
}

void LPAPI LPEventMgr::_ProcRecvEvent(RECV_EVENT* pstRecvEvent)
{
	LPINT32 nResult = 0;
	LPSocker* pSocker = NULL;

	LOG_PROCESS_ERROR(pstRecvEvent);
	LOG_PROCESS_ERROR(m_pNetMessageHandler);
	LOG_PROCESS_ERROR(pstRecvEvent->dwLen < MAX_PACKET_LEN);

	nResult = m_pRecvLoopBuf->Read(m_pPacketTempBuf, pstRecvEvent->dwLen, TRUE, TRUE);
	LOG_PROCESS_ERROR(nResult);

	pSocker = pstRecvEvent->pSocker;
	LOG_PROCESS_ERROR(pSocker);

	m_pNetMessageHandler->OnMessage(pSocker, m_pPacketTempBuf, pstRecvEvent->dwLen);

Exit0:

	return;
}

void LPAPI LPEventMgr::_ProcTerminateEvent(TERMINATE_EVENT* pstTerminateEvent)
{
	LPINT32 nResult = 0;
	LPConnector* pConnector = NULL;
	LPSocker* pSocker = NULL;

	LOG_PROCESS_ERROR(pstTerminateEvent);
	pSocker = pstTerminateEvent->pSocker;
	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(m_pNetMessageHandler);

	if (pSocker->GetSockerId() != pstTerminateEvent->dwSockerId)
	{
		ERR("function %s in file %s at line %d : socker id error, %d:%d !", __FUNCTION__, __FILE__, __LINE__,
			pSocker->GetSockerId(), pstTerminateEvent->dwSockerId);
	}

	if (!pSocker->IsAcceptCreate())
	{
		pConnector = m_pNetImpl->FindConnector(pSocker->GetParentId());
		if (NULL == pConnector)
		{
			FTL("%s *** can't find the connector ***, socker_id=%d, parent_id=%d", __FUNCTION__, 
				pSocker->GetSockerId(), pSocker->GetParentId());
		}
		else
		{
			m_pNetMessageHandler->OnConnectDisconnected(pSocker, pConnector);
		}
	}
	else
	{
		m_pNetMessageHandler->OnDisconnected(pSocker);
	}

	pSocker->OnClose();

Exit0:

	return;
}

void LPAPI LPEventMgr::_ProcEstablishEvent(ESTABLISH_EVENT* pstEstablishEvent)
{
	LPINT32 nResult = 0;
	LPSocker* pSocker = NULL;

	LOG_PROCESS_ERROR(pstEstablishEvent);
	LOG_PROCESS_ERROR(m_pNetMessageHandler);

	pSocker = pstEstablishEvent->pSocker;
	LOG_PROCESS_ERROR(pSocker);

	if (pstEstablishEvent->bAccept)
	{
		m_pNetMessageHandler->OnAccepted(pSocker);
	}
	else
	{
		m_pNetMessageHandler->OnConnected(pSocker);
	}

	nResult = pSocker->PostRecv();
	LOG_PROCESS_ERROR(nResult);

Exit0:

	return;
}

void LPAPI LPEventMgr::_ProcConnectErrorEvent(CONNECT_ERROR_EVENT* pstConnectErrorEvent)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pstConnectErrorEvent);
	LOG_PROCESS_ERROR(m_pNetMessageHandler);

	m_pNetMessageHandler->OnConnectError(pstConnectErrorEvent->pConnector, pstConnectErrorEvent->dwErrorNo);

Exit0:

	return;
}



DECLARE const char * LPAPI lpGetEventHandlerTypeName(e_EventHandlerType eType)
{
	switch (eType)
	{
	case LZPL::eEventHandlerType_None:
		LOG_CHECK_ERROR(FALSE);
		return "eEventHandlerType_None";
		break;
	case LZPL::eEventHandlerType_Connector:
		return "eEventHandlerType_Connector";
		break;
	case LZPL::eEventHandlerType_Listener:
		return "eEventHandlerType_Listener";
		break;
	case LZPL::eEventHandlerType_Socker:
		return "eEventHandlerType_Socker";
		break;
	case LZPL::eEventHandlerType_Max:
		LOG_CHECK_ERROR(FALSE);
		return "eEventHandlerType_Max";
		break;
	default:
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:

	return "*** Unknow e_EventHandlerType ***";
}

DECLARE const char * LPAPI lpGetIoOptTypeName(e_IoOptType eType)
{
	switch (eType)
	{
	case LZPL::eIoOptType_None:
		LOG_CHECK_ERROR(FALSE);
		return "eIoOptType_None";
		break;
	case LZPL::eIoOptType_Recv:
		return "eIoOptType_Recv";
		break;
	case LZPL::eIoOptType_Send:
		return "eIoOptType_Send";
		break;
	case LZPL::eIoOptType_Accept:
		return "eIoOptType_Accept";
		break;
	case LZPL::eIoOptType_Connect:
		return "eIoOptType_Connect";
		break;
	case LZPL::eIoOptType_Max:
		LOG_CHECK_ERROR(FALSE);
		return "eIoOptType_Max";
		break;
	default:
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:

	return "*** Unknow e_IoOptType ***";
}

BOOL LPAPI LPReactorImpl::RegisterEventHandler(ILPEventHandler* pEventHandler)
{
	LPUINT32 dwIndex = 0;

	LOG_PROCESS_ERROR(pEventHandler);
	LOG_PROCESS_ERROR(m_pCompletionPort);
	LOG_PROCESS_ERROR(m_nCompletionPortCount > 0);

	dwIndex = (LPUINT64)(pEventHandler->GetHandle()) % m_nCompletionPortCount;

	if (NULL == CreateIoCompletionPort(pEventHandler->GetHandle(), m_pCompletionPort[dwIndex], (ULONG_PTR)pEventHandler, 0))
	{
		FTL("function %s in file %s at line %d : errno %d", __FUNCTION__, __FILE__, __LINE__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPReactorImpl::UnRegisterEventHandler(ILPEventHandler* pEventHandler)
{
	return FALSE;

}

unsigned LPAPI LZPL::LPReactorImpl::ThreadFunc(LPVOID pParam)
{
	REACTOR_THREAD_PARAM stThreadParam;
	REACTOR_THREAD_PARAM* pThreadParam = (REACTOR_THREAD_PARAM*)pParam;

	LOG_PROCESS_ERROR(pThreadParam);
	stThreadParam.pReactorImpl = pThreadParam->pReactorImpl;
	stThreadParam.nCompletionPortIndex = pThreadParam->nCompletionPortIndex;

	SAFE_DELETE(pThreadParam);

	LOG_PROCESS_ERROR(stThreadParam.pReactorImpl);
	stThreadParam.pReactorImpl->OnExecute(stThreadParam.nCompletionPortIndex);

Exit0:
	return 0;
}

BOOL LPAPI LZPL::LPReactorImpl::Init(LPNetImpl* pNetImpl, BOOL bOneCompletionPortOneThread)
{
	LPINT32 nResult = 0;
	SYSTEM_INFO stSysInfo;
	UINT dwThreadId = 0;

	LOG_PROCESS_ERROR(eCommonState_NoInit == _GetState());
	_SetState(eCommonState_Initing);

	LOG_PROCESS_ERROR(pNetImpl);
	m_pNetImpl = pNetImpl;

	GetSystemInfo(&stSysInfo);

	if (bOneCompletionPortOneThread)
	{
		m_nCompletionPortCount = stSysInfo.dwNumberOfProcessors * IOCP_THREAD_PER_CPU;
		m_nWorkerCountPerCompIo = 1;
	}
	else
	{
		m_nCompletionPortCount = 1;
		m_nWorkerCountPerCompIo = stSysInfo.dwNumberOfProcessors * IOCP_THREAD_PER_CPU;
	}

	m_pCompletionPort = new HANDLE[m_nCompletionPortCount];
	LOG_PROCESS_ERROR(m_pCompletionPort);
	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		m_pCompletionPort[i] = INVALID_HANDLE_VALUE;
	}
	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		m_pCompletionPort[i] = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		LOG_PROCESS_ERROR(m_pCompletionPort[i] != INVALID_HANDLE_VALUE);
	}

	m_ppWorkerArray = new HANDLE*[m_nCompletionPortCount];
	LOG_PROCESS_ERROR(m_ppWorkerArray);
	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		m_ppWorkerArray[i] = NULL;
	}
	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		m_ppWorkerArray[i] = new HANDLE[m_nWorkerCountPerCompIo];
		LOG_PROCESS_ERROR(m_ppWorkerArray[i]);
		for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
		{
			m_ppWorkerArray[i][j] = INVALID_HANDLE_VALUE;
		}
	}

	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
		{
			REACTOR_THREAD_PARAM* pThreadParam = new REACTOR_THREAD_PARAM();
			LOG_PROCESS_ERROR(pThreadParam);
			pThreadParam->pReactorImpl = this;
			pThreadParam->nCompletionPortIndex = i;

			m_ppWorkerArray[i][j] = (HANDLE)_beginthreadex(NULL,          // Security
				0,             // Stack size - use default
				ThreadFunc,    // Thread fn entry point
				(void*)pThreadParam,   // Param for thread
				0,             // Init flag
				&dwThreadId);  // Thread address

			LOG_PROCESS_ERROR(m_ppWorkerArray[i][j] != INVALID_HANDLE_VALUE);
		}
	}

	_SetState(eCommonState_Inited);

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

BOOL LPAPI LZPL::LPReactorImpl::UnInit()
{
	LPINT32 nResult = 0;

	PROCESS_SUCCESS(_GetState() == eCommonState_NoInit || _GetState() >= eCommonState_UnIniting); 

	_SetState(eCommonState_UnIniting);
	IMP("reactor uniniting ...");
	LPPRINTF("reactor uniniting ...\n");

	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
		{
			nResult = PostQueuedCompletionStatus(m_pCompletionPort[i], 0, NULL, NULL);
			LOG_CHECK_ERROR(nResult);
		}
	}

	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		nResult = WaitForMultipleObjects(m_nWorkerCountPerCompIo, m_ppWorkerArray[i], TRUE, 60*1000);
		LOG_CHECK_ERROR(nResult != WAIT_FAILED);
		LOG_CHECK_ERROR(nResult != WAIT_TIMEOUT);

		for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
		{
			CloseHandle(m_ppWorkerArray[i][j]);
			m_ppWorkerArray[i][j] = INVALID_HANDLE_VALUE;
		}
	}

	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		SAFE_DELETE_SZ(m_ppWorkerArray[i]);
	}
	SAFE_DELETE_SZ(m_ppWorkerArray);


	for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
	{
		CloseHandle(m_pCompletionPort[i]);
		m_pCompletionPort[i] = INVALID_HANDLE_VALUE;
	}
	SAFE_DELETE_SZ(m_pCompletionPort);

	IMP("reactor uninit success !");
	LPPRINTF("reactor uninit success !\n");
	_SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

void LPAPI LZPL::LPReactorImpl::OnExecute(LPINT32 nCompletionPortIndex)
{
	BOOL             bRet;
	DWORD            dwByteTransferred;
	ILPEventHandler* pEventHandler;
	PER_IO_DATA*     pstPerIoData;

	LOG_PROCESS_ERROR(nCompletionPortIndex >= 0 && nCompletionPortIndex < m_nCompletionPortCount);

	while (TRUE)
	{
		pEventHandler = NULL;
		pstPerIoData = NULL;
		dwByteTransferred = 0;

		//IMP("LPReactorImpl::OnExecute start GetQueuedCompletionStatus ...");
		bRet = GetQueuedCompletionStatus(
			m_pCompletionPort[nCompletionPortIndex],
			&dwByteTransferred,
			(PDWORD_PTR)&pEventHandler,
			(LPOVERLAPPED*)&pstPerIoData,
			INFINITE);

		//检查线程退出
		if (_GetState() == eCommonState_Inited)
		{
			LOG_PROCESS_ERROR(pEventHandler);
		}
		else if (_GetState() >= eCommonState_Close)
		{
			PROCESS_SUCCESS(TRUE);
		}
		else
		{
			LOG_PROCESS_ERROR(FALSE);
		}

		//io绑定数据检测
		if (NULL == pstPerIoData)
		{
			LOG_CHECK_ERROR(pstPerIoData);
			continue;
		}

		if (bRet)
		{
			DBG("LPIocpCtrl::OnExecute bRet=TRUE, e_EventHandlerType=%s, e_IoOptType=%s",
				lpGetEventHandlerTypeName(pstPerIoData->eHandlerType), lpGetIoOptTypeName(pstPerIoData->eIoOptType));
		}
		else
		{
			DBG("LPIocpCtrl::OnExecute bRet=FALSE, e_EventHandlerType=%s, e_IoOptType=%s",
				lpGetEventHandlerTypeName(pstPerIoData->eHandlerType), lpGetIoOptTypeName(pstPerIoData->eIoOptType));
		}

		switch (pstPerIoData->eIoOptType)
		{
		case eIoOptType_Accept:
			{
				_OnAccept(bRet, pEventHandler, pstPerIoData);
			}
			break;
		case eIoOptType_Connect:
			{
				_OnConnect(bRet, pEventHandler, pstPerIoData);
			}
			break;
		case eIoOptType_Recv:
		case eIoOptType_Send:
			{
				_OnSendRecv(bRet, pEventHandler, pstPerIoData, dwByteTransferred);
			}
			break;
		default:
			LOG_CHECK_ERROR(FALSE);
			break;
		}
	}

	//	if (!bRet)
	//	{
	//		DBG("LPIocpCtrl::OnExecute bRet=FALSE, e_EventHandlerType=%s, e_IoOptType=%s",
	//			lpGetEventHandlerTypeName(pstPerIoData->eHandlerType), lpGetIoOptTypeName(pstPerIoData->eIoOptType));

	//		if (eEventHandlerType_Listener == pstPerIoData->eHandlerType)
	//		{
	//			pListenter = dynamic_cast<LPListener*>(pEventHandler);
	//			if (NULL == pListenter)
	//			{
	//				LOG_CHECK_ERROR(FALSE);
	//				continue;
	//			}

	//			pListenter->OnAccept(FALSE, pstPerIoData);
	//		}
	//		else if (eEventHandlerType_Connector == pstPerIoData->eHandlerType)
	//		{
	//			pConnector = dynamic_cast<LPConnector*>(pEventHandler);
	//			if (NULL == pConnector)
	//			{
	//				LOG_CHECK_ERROR(FALSE);
	//				continue;
	//			}

	//			if (eIoOptType_Connect == pstPerIoData->eIoOptType)
	//			{
	//				pConnector->OnConnect(FALSE, pstPerIoData);
	//			}
	//			else
	//			{
	//				if (NULL == pConnector->GetSocker() || !pConnector->GetSocker()->IsConnect() || eCommonState_Inited != pConnector->GetState())
	//				{
	//					LOG_CHECK_ERROR(FALSE);
	//					continue;
	//				}
	//				pConnector->GetSocker()->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 1, WSAGetLastError()), TRUE);
	//			}
	//		}
	//		else if (eEventHandlerType_Socker == pstPerIoData->eHandlerType)
	//		{
	//			nErrno = WSAGetLastError();
	//			pSocker = dynamic_cast<LPSocker*>(pEventHandler);
	//			if (NULL == pSocker)
	//			{
	//				if (ERROR_OPERATION_ABORTED != nErrno)
	//				{
	//					LOG_CHECK_ERROR(FALSE);
	//				}
	//				continue;
	//			}

	//			pSocker->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 2, nErrno), TRUE);
	//		}
	//		else
	//		{
	//			LOG_CHECK_ERROR(FALSE);
	//		}

	//		continue;
	//	}

	//	DBG("LPIocpCtrl::OnExecute bRet=TRUE, e_EventHandlerType=%s, e_IoOptType=%s",
	//		lpGetEventHandlerTypeName(pstPerIoData->eHandlerType), lpGetIoOptTypeName(pstPerIoData->eIoOptType));

	//	if (eEventHandlerType_Listener == pstPerIoData->eHandlerType)
	//	{
	//		pListenter = dynamic_cast<LPListener*>(pEventHandler);
	//		if (NULL == pListenter)
	//		{
	//			LOG_CHECK_ERROR(FALSE);
	//			continue;
	//		}

	//		pListenter->OnAccept(TRUE, pstPerIoData);
	//	}
	//	else if (eEventHandlerType_Connector == pstPerIoData->eHandlerType)
	//	{
	//		pConnector = dynamic_cast<LPConnector*>(pEventHandler);
	//		if (NULL == pConnector)
	//		{
	//			LOG_CHECK_ERROR(FALSE);
	//			continue;
	//		}

	//		if (eIoOptType_Connect == pstPerIoData->eIoOptType)
	//		{
	//			pConnector->OnConnect(TRUE, pstPerIoData);
	//		}
	//		else
	//		{
	//			if (NULL == pConnector->GetSocker() || !pConnector->GetSocker()->IsConnect() || eCommonState_Inited != pConnector->GetState())
	//			{
	//				LOG_CHECK_ERROR(FALSE);
	//				continue;
	//			}

	//			if (0 == dwByteTransferred)
	//			{
	//				//这里WSAGetLastError获得的结果是否是对应本次io操作？？？或许需要用GetLastError比较准确？？？
	//				pConnector->GetSocker()->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 3, WSAGetLastError()), TRUE);
	//				continue;
	//			}

	//			if (eIoOptType_Recv == pstPerIoData->eIoOptType)
	//			{
	//				pConnector->GetSocker()->OnRecv(dwByteTransferred);
	//			}
	//			else if (eIoOptType_Send == pstPerIoData->eIoOptType)
	//			{
	//				pConnector->GetSocker()->OnSend(dwByteTransferred);
	//			}
	//			else
	//			{
	//				LOG_CHECK_ERROR(FALSE);
	//			}
	//		}
	//	}
	//	else if (eEventHandlerType_Socker == pstPerIoData->eHandlerType)
	//	{
	//		pSocker = dynamic_cast<LPSocker*>(pEventHandler);
	//		if (NULL == pSocker)
	//		{
	//			LOG_CHECK_ERROR(FALSE);
	//			continue;
	//		}

	//		if (0 == dwByteTransferred)
	//		{
	//			//这里WSAGetLastError获得的结果是否是对应本次io操作？？？或许需要用GetLastError比较准确？？？
	//			pSocker->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 4, WSAGetLastError()), TRUE);
	//			continue;
	//		}

	//		if (eIoOptType_Recv == pstPerIoData->eIoOptType)
	//		{
	//			pSocker->OnRecv(dwByteTransferred);
	//		}
	//		else if (eIoOptType_Send == pstPerIoData->eIoOptType)
	//		{
	//			pSocker->OnSend(dwByteTransferred);
	//		}
	//		else
	//		{
	//			LOG_CHECK_ERROR(FALSE);
	//		}
	//	}
	//	else
	//	{
	//		LOG_CHECK_ERROR(FALSE);
	//	}
	//}

Exit1:
Exit0:
	return;
}

LPUINT32 LPAPI LPReactorImpl::_GetState()
{
	return m_dwState;
}

void LPAPI LPReactorImpl::_SetState(LPUINT32 dwState)
{
	m_dwState = dwState;
}

void LPAPI LPReactorImpl::_OnConnect(BOOL bOperateRet, ILPEventHandler * pEventHandler, PER_IO_DATA * pstPerIoData)
{
	LPConnector* pConnector = NULL;

	LOG_PROCESS_ERROR(pEventHandler);
	LOG_PROCESS_ERROR(pstPerIoData);

	pConnector = dynamic_cast<LPConnector*>(pEventHandler);
	LOG_PROCESS_ERROR(pConnector);

	if (bOperateRet)
	{
		pConnector->OnConnect(TRUE, pstPerIoData);
	}
	else
	{
		pConnector->OnConnect(FALSE, pstPerIoData);
	}

Exit0:
	return;
}

void LPAPI LPReactorImpl::_OnAccept(BOOL bOperateRet, ILPEventHandler * pEventHandler, PER_IO_DATA * pstPerIoData)
{
	LPListener* pListener = NULL;

	LOG_PROCESS_ERROR(pEventHandler);
	LOG_PROCESS_ERROR(pstPerIoData);

	pListener = dynamic_cast<LPListener*>(pEventHandler);
	LOG_PROCESS_ERROR(pListener);

	if (bOperateRet)
	{
		pListener->OnAccept(TRUE, pstPerIoData);
	}
	else
	{
		pListener->OnAccept(FALSE, pstPerIoData);
	}

Exit0:
	return;
}

void LPAPI LPReactorImpl::_OnSendRecv(BOOL bOperateRet, ILPEventHandler * pEventHandler, PER_IO_DATA * pstPerIoData, LPUINT32 dwByteTransferred)
{
	LPINT32 nLastError;
	LPSocker* pSocker = NULL;
	LPConnector* pConnector = NULL;

	LOG_PROCESS_ERROR(pEventHandler);
	LOG_PROCESS_ERROR(pstPerIoData);

	LOG_PROCESS_ERROR(eIoOptType_Recv == pstPerIoData->eIoOptType ||
		eIoOptType_Send == pstPerIoData->eIoOptType);
	LOG_PROCESS_ERROR(eEventHandlerType_Socker == pstPerIoData->eHandlerType ||
		eEventHandlerType_Connector == pstPerIoData->eHandlerType);

	//这里WSAGetLastError获得的结果是否是对应本次io操作？？？或许需要用GetLastError比较准确？？？
	nLastError = WSAGetLastError();

	//调用closesocket时，会触发收回连接句柄关联的正在执行的io操作，
	//此处进行判断拦截，防止访问已经被释放的socker对象
	if (!bOperateRet && ERROR_OPERATION_ABORTED == nLastError)
	{
		PROCESS_SUCCESS(TRUE);
	}

	if (eEventHandlerType_Socker == pstPerIoData->eHandlerType)
	{
		pSocker = dynamic_cast<LPSocker*>(pEventHandler);
		LOG_PROCESS_ERROR(pSocker);
	}
	else
	{
		pConnector = dynamic_cast<LPConnector*>(pEventHandler);
		LOG_PROCESS_ERROR(pConnector);
		//LOG_PROCESS_ERROR(eCommonState_Inited == pConnector->GetState());

		pSocker = pConnector->GetSocker();
		LOG_PROCESS_ERROR(pSocker);
		//LOG_PROCESS_ERROR(pSocker->IsConnect());
	}

	if (bOperateRet)
	{
		if (0 == dwByteTransferred)
		{
			pSocker->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 2, nLastError), TRUE);
		}
		else
		{
			if (eIoOptType_Recv == pstPerIoData->eIoOptType)
			{
				pSocker->OnRecv(dwByteTransferred);
			}
			else
			{
				pSocker->OnSend(dwByteTransferred);
			}
		}
	}
	else
	{
		pSocker->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 1, nLastError), TRUE);
	}

Exit1:
Exit0:
	return;
}

LZPL::LPReactorImpl::LPReactorImpl()
{
	_SetState(eCommonState_NoInit);
	m_nCompletionPortCount = 0;
	m_pCompletionPort = NULL;
	m_nWorkerCountPerCompIo = 0;
	m_ppWorkerArray = NULL;
	m_pNetImpl = NULL;
}

LZPL::LPReactorImpl::~LPReactorImpl()
{
	UnInit();
}


DECLARE ILPNet* LPAPI lpCreateNetModule(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig)
{
	LPINT32 nResult = 0;
	LPNetImpl* pNetImpl = NULL;

	PRINTF_PROCESS_ERROR(lpGetLzplLoggerCtrl());

	LOG_PROCESS_ERROR(pNetMessageHandler);
	LOG_PROCESS_ERROR(pNetConfig);

	pNetImpl = new LPNetImpl();
	LOG_PROCESS_ERROR(pNetImpl);

	pNetImpl->AddRef();
	nResult = pNetImpl->Init(pNetMessageHandler, pNetConfig);
	LOG_PROCESS_ERROR(nResult);

	return pNetImpl;

Exit0:

	if (pNetImpl)
	{
		pNetImpl->Release();
	}

	return NULL;
}



LZPL::LPNetImpl::LPNetImpl(void)
{
	m_dwRef = 0;
	m_dwMaxCreateId = 0;
	m_pNetMessageHandler = NULL;

	memset(&m_oNetConfig, 0, sizeof(m_oNetConfig));
	memset(&m_stModuleVersion, 0, sizeof(m_stModuleVersion));

	m_mapListener.clear();
	m_mapConnector.clear();
}

LZPL::LPNetImpl::~LPNetImpl(void)
{
	UnInit();
}

void LPAPI LZPL::LPNetImpl::AddRef(void)
{
	++m_dwRef;
}

LPUINT32 LPAPI LZPL::LPNetImpl::QueryRef(void)
{
	return m_dwRef;
}

void LPAPI LZPL::LPNetImpl::Release(void)
{
	LOG_CHECK_ERROR(m_dwRef > 0);
	if (m_dwRef > 0)
	{
		--m_dwRef;
	}

	if (m_dwRef == 0)
	{
		delete this;
	}
}

MODULE_VERSION LPAPI LZPL::LPNetImpl::GetVersion(void)
{
	return LPNET_MODULEVERSION;
}

const char* LPAPI LZPL::LPNetImpl::GetModuleName(void)
{
	return LPNET_MODULENAME;
}

ILPListener* LPAPI LPNetImpl::CreateListenerCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser)
{
	LPINT32 nResult = 0;
	LPListener* pListener = NULL;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(eIoType_CompletionPort == eIoType);

	pListener = new LPListener();
	LOG_PROCESS_ERROR(pListener);

	nResult = pListener->Init(this, pPacketParser, _CreateId());
	LOG_PROCESS_ERROR(nResult);

	m_mapListener.insert(make_pair(pListener->GetId(), pListener));

	return pListener;
Exit0:
	return NULL;
}

ILPConnector* LPAPI LPNetImpl::CreateConnectorCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser)
{
	LPINT32 nResult = 0;
	LPConnector* pConnector = NULL;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(eIoType_CompletionPort == eIoType);

	pConnector = new LPConnector();
	LOG_PROCESS_ERROR(pConnector);

	nResult = pConnector->Init(this, pPacketParser, _CreateId());
	LOG_PROCESS_ERROR(nResult);

	m_mapConnector.insert(make_pair(pConnector->GetId(), pConnector));

	return pConnector;
Exit0:
	return NULL;
}

LPListener* LPAPI LPNetImpl::FindListener(LPUINT32 dwId)
{
	LPListener* pListener = NULL;
	MAP_LISTENER::iterator fit;

	fit = m_mapListener.find(dwId);
	if (fit != m_mapListener.end())
	{
		pListener = fit->second;
	}

	return pListener;
}

LPConnector* LPAPI LPNetImpl::FindConnector(LPUINT32 dwId)
{
	LPConnector* pConnector = NULL;
	MAP_CONNECTOR::iterator fit;

	fit = m_mapConnector.find(dwId);
	if (fit != m_mapConnector.end())
	{
		pConnector = fit->second;
	}

	return pConnector;
}

BOOL LPAPI LZPL::LPNetImpl::Run(LPINT32 nCount /*= -1*/)
{
	LPINT32 nResult = 0;

	do
	{
		nResult = m_oEventMgr.HaveEventForHandled();
		PROCESS_SUCCESS(!nResult);

		m_oEventMgr.HandleOneEvent();

	} while (--nCount != 0);

	nResult = m_oEventMgr.HaveEventForHandled();
	PROCESS_SUCCESS(!nResult);

	return FALSE;

Exit1:
	return TRUE;
}

BOOL LPAPI LZPL::LPNetImpl::Init(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig)
{
	LPINT32 nResult = 0;
	WSADATA stData;

	::WSAStartup(MAKEWORD(2, 2), &stData);

	m_pNetMessageHandler = pNetMessageHandler;
	LOG_PROCESS_ERROR(m_pNetMessageHandler);

	LOG_PROCESS_ERROR(pNetConfig);
	memcpy(&m_oNetConfig, pNetConfig, sizeof(m_oNetConfig));

	nResult = m_oSockerMgr.Init(this, m_oNetConfig.dwConnectCount);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oEventMgr.Init(this, m_pNetMessageHandler, m_oNetConfig.dwNetEventPoolSize, m_oNetConfig.dwNetEventListCount);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oReactorImpl.Init(this, FALSE);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;

Exit0:

	UnInit();

	return FALSE;
}

LPSockerMgr &LPAPI LPNetImpl::GetSockerMgr(void)
{
	return m_oSockerMgr;;
}

LPEventMgr &LPAPI LPNetImpl::GetEventMgr(void)
{
	return m_oEventMgr;
}

LPReactorImpl &LPAPI LPNetImpl::GetReactorImpl(void)
{
	return m_oReactorImpl;
}

NET_CONFIG& LPAPI LPNetImpl::GetNetConfig(void)
{
	return m_oNetConfig;
}

void LPAPI LZPL::LPNetImpl::UnInit()
{
	LPINT32 nResult = 0;
	LPUINT64 qwTickStart = 0;
	ILPListener* pListener = NULL;
	ILPConnector* pConnector = NULL;
	MAP_LISTENER::iterator iterListener;
	MAP_CONNECTOR::iterator iterConnector;

	IMP("net uniniting ...");
	LPPRINTF("net uniniting ...\n");

	//等待所有关闭的socker数据发送完毕，并且确定所有socker都关闭
	qwTickStart = lpGetTickCountEx();
	while (lpGetTickCountEx() < qwTickStart + (DELAY_CLOSE_SOCKET_TIME + DELAY_RELEASE_SOCKER_TIME))
	{
		Run();
	}

	nResult = m_oReactorImpl.UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = m_oEventMgr.UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = m_oSockerMgr.UnInit();
	LOG_CHECK_ERROR(nResult);

	for (iterListener = m_mapListener.begin(); iterListener != m_mapListener.end(); ++iterListener)
	{
		pListener = iterListener->second;
		LOG_CHECK_ERROR(pListener);
		IF_NULL_CONTINUE(pListener);
		pListener->Release();
	}
	m_mapListener.clear();

	for (iterConnector = m_mapConnector.begin(); iterConnector != m_mapConnector.end(); ++iterConnector)
	{
		pConnector = iterConnector->second;
		LOG_CHECK_ERROR(pConnector);
		IF_NULL_CONTINUE(pConnector);
		pConnector->Release();
	}
	m_mapConnector.clear();

	::WSACleanup();

	IMP("net uninit success !");
	LPPRINTF("net uninit success !\n");
}

LPUINT32 LPAPI LPNetImpl::_CreateId()
{
	LPUINT32 dwNewId = 0;

	m_oLock.Lock();
	dwNewId = ++m_dwMaxCreateId;
	m_oLock.UnLock();

	return dwNewId;
}
















//end声明所处的名字空间
NS_LZPL_END

#endif /* END OF _WIN32*/