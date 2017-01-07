#include "lp_socker.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_dump.h"
#include "lp_net.h"



//begin声明所处的名字空间
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

	//这里WSAGetLastError获得的结果是否是对应本次io操作？？？或许需要用GetLastError比较准确？？？
	nLastError = WSAGetLastError();

	//调用lpCloseSocket时，会触发收回连接句柄关联的正在执行的io操作，
	//此处进行判断拦截，防止访问已经被释放的socker对象
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

void LPAPI LPSocker::OnSend(LPUINT32 dwBytes)
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




LPSockerMgr::LPSockerMgr()
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
	std::shared_ptr<LPConnector> pConnector;
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



//end声明所处的名字空间
NS_LZPL_END