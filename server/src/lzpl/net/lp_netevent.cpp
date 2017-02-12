#include "lp_netevent.h"
#include "lp_processerror.h"
#include "lp_net.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPINT32 NET_EVENT::ms_dwNetEventCount = 0;

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

BOOL LPAPI LPEventMgr::Init(LPNetImpl* pNetImpl, ILPNetMessageHandler* pNetMessageHandler, LPINT32 nEventListCount)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pNetMessageHandler);
	LOG_PROCESS_ERROR(pNetImpl);
	LOG_PROCESS_ERROR(nEventListCount > 0);

	m_pNetMessageHandler = pNetMessageHandler;
	m_pNetImpl = pNetImpl;

	m_pEventListRecvLoopBuf = new LPLoopBuf[m_nEventListCount];
	LOG_PROCESS_ERROR(m_pEventListRecvLoopBuf);
	for (LPINT32 i = 0; i < m_nEventListCount; ++i)
	{
		nResult = m_pEventListRecvLoopBuf[i].Init(m_pNetImpl->GetNetConfig().dwNetRecvEventBufSize);
		LOG_PROCESS_ERROR(nResult);
	}
	m_pEventListRecvLoopBufLock = new LPLock[m_nEventListCount];
	LOG_PROCESS_ERROR(m_pEventListRecvLoopBufLock);
	
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
	PROCESS_SUCCESS(m_bInit == FALSE);
	m_bInit = FALSE;

	SAFE_DELETE_SZ(m_pEventListRecvLoopBuf);
	SAFE_DELETE_SZ(m_pEventListRecvLoopBufLock);
	SAFE_DELETE_SZ(m_pPacketTempBuf);
	SAFE_DELETE_SZ(m_pEventListLock);
	SAFE_DELETE_SZ(m_pEventList);

Exit1:
	return TRUE;
}

BOOL LPAPI LPEventMgr::PushRecvEvent(ILPSockerImpl* pSocker, LPUINT32 dwSockerId, ILPLoopBuf& oLoopBuf, LPUINT32 dwLen)
{
	LPINT32 nResult = 0;
	LPINT32 nRetryCount = 0;
	LPUINT32 dwLineSize = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pSocker->GetSockerId() == dwSockerId);
	LOG_PROCESS_ERROR(oLoopBuf.GetTotalReadableLen() >= dwLen);

	pstEvent = NET_EVENT::NewNetEvent(eEventType_Recv);
	LOG_PROCESS_ERROR(pstEvent != nullptr);
	LOG_PROCESS_ERROR(pstEvent->pRecvEvent != nullptr);

	pstEvent->eEventType = eEventType_Recv;
	pstEvent->dwFlag = dwSockerId;
	pstEvent->pRecvEvent->pSocker = pSocker;
	pstEvent->pRecvEvent->dwLen = dwLen;

	//lock
	m_pEventListRecvLoopBufLock[pstEvent->dwFlag % m_nEventListCount].Lock();

	while (m_pEventListRecvLoopBuf[pstEvent->dwFlag % m_nEventListCount].GetTotalWritableLen() < dwLen)
	{
		WRN("function %s in file %s at line %d : buf not enough, sleep and try again !", __FUNCTION__, __FILE__, __LINE__);
		lpSleep(1);
		nRetryCount++;

		// 超过30秒，丢弃
		if (nRetryCount > 30000)
		{
			oLoopBuf.FinishRead(dwLen);

			//unlock
			m_pEventListRecvLoopBufLock[pstEvent->dwFlag % m_nEventListCount].UnLock();
			LOG_PROCESS_ERROR(FALSE);
		}
	}

	dwLineSize = oLoopBuf.GetOnceReadableLen();
	if (dwLineSize > dwLen)
	{
		dwLineSize = dwLen;
	}

	nResult = m_pEventListRecvLoopBuf[pstEvent->dwFlag % m_nEventListCount].Write(oLoopBuf.ReadPtr(), dwLineSize);
	LOG_CHECK_ERROR(nResult);

	oLoopBuf.FinishRead(dwLineSize);

	if (dwLineSize < dwLen)
	{
		nResult = m_pEventListRecvLoopBuf[pstEvent->dwFlag % m_nEventListCount].Write(oLoopBuf.ReadPtr(), dwLen - dwLineSize);
		LOG_CHECK_ERROR(nResult);

		oLoopBuf.FinishRead(dwLen - dwLineSize);
	}

	//unlock
	m_pEventListRecvLoopBufLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return TRUE;

Exit0:

	NET_EVENT::DeleteNetEvent(pstEvent);

	return FALSE;
}

void LPAPI LPEventMgr::PushTerminateEvent(ILPSockerImpl* pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose)
{
	LPINT32 nResult = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pSocker);

	pstEvent = NET_EVENT::NewNetEvent(eEventType_Terminate);
	LOG_PROCESS_ERROR(pstEvent != nullptr);
	LOG_PROCESS_ERROR(pstEvent->pTerminateEvent != nullptr);

	pstEvent->eEventType = eEventType_Terminate;
	pstEvent->dwFlag = dwSockerId;
	pSocker->SetPassiveClose(bPassiveClose);
	pstEvent->pTerminateEvent->pSocker = pSocker;
	pstEvent->pTerminateEvent->dwSockerId = dwSockerId;

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return;

Exit0:

	NET_EVENT::DeleteNetEvent(pstEvent);

	return;
}

void LPAPI LPEventMgr::PushEstablishEvent(ILPSockerImpl* pSocker, BOOL bAccept)
{
	LPINT32 nResult = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pSocker);

	pstEvent = NET_EVENT::NewNetEvent(eEventType_Establish);
	LOG_PROCESS_ERROR(pstEvent != nullptr);
	LOG_PROCESS_ERROR(pstEvent->pEstablishEvent != nullptr);
	
	pstEvent->eEventType = eEventType_Establish;
	pstEvent->dwFlag = pSocker->GetSockerId();
	pstEvent->pEstablishEvent->pSocker = pSocker;
	pstEvent->pEstablishEvent->bAccept = bAccept;

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return;

Exit0:

	NET_EVENT::DeleteNetEvent(pstEvent);

	return;
}

void LPAPI LPEventMgr::PushConnectErrorEvent(std::shared_ptr<ILPConnectorImpl> pConnector, LPUINT32 dwErrorNo)
{
	LPINT32 nResult = 0;
	NET_EVENT* pstEvent = NULL;

	PROCESS_SUCCESS(!m_bInit);

	LOG_PROCESS_ERROR(pConnector);

	pstEvent = NET_EVENT::NewNetEvent(eEventType_ConnectError);
	LOG_PROCESS_ERROR(pstEvent != nullptr);
	LOG_PROCESS_ERROR(pstEvent->pConnectErrorEvent != nullptr);
	
	pstEvent->eEventType = eEventType_ConnectError;
	pstEvent->dwFlag = pConnector->GetId();
	pstEvent->pConnectErrorEvent->pConnector = pConnector;
	pstEvent->pConnectErrorEvent->dwErrorNo = dwErrorNo;

	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].Lock();
	m_pEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
	m_pEventListLock[pstEvent->dwFlag % m_nEventListCount].UnLock();

Exit1:

	return;

Exit0:

	NET_EVENT::DeleteNetEvent(pstEvent);

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
					_ProcRecvEvent(pstEvent->pRecvEvent, pstEvent->dwFlag);
				}
				break;
			case eEventType_Terminate:
				{
					_ProcTerminateEvent(pstEvent->pTerminateEvent);
				}
				break;
			case eEventType_Establish:
				{
					_ProcEstablishEvent(pstEvent->pEstablishEvent);
				}
				break;
			case eEventType_ConnectError:
				{
					_ProcConnectErrorEvent(pstEvent->pConnectErrorEvent);
				}
				break;
			default:
				LOG_PROCESS_ERROR(FALSE);
				break;
		}
	}

Exit1:

Exit0:

	NET_EVENT::DeleteNetEvent(pstEvent);

	return;
}

void LPAPI LPEventMgr::_ProcRecvEvent(std::shared_ptr<RECV_EVENT> pstRecvEvent, LPUINT32 dwFlag)
{
	LPINT32 nResult = 0;
	ILPSockerImpl* pSocker = NULL;

	LOG_PROCESS_ERROR(pstRecvEvent);
	LOG_PROCESS_ERROR(m_pNetMessageHandler);
	LOG_PROCESS_ERROR(pstRecvEvent->dwLen < MAX_PACKET_LEN);

	m_pEventListRecvLoopBufLock[dwFlag % m_nEventListCount].Lock();
	nResult = m_pEventListRecvLoopBuf[dwFlag % m_nEventListCount].Read(m_pPacketTempBuf, pstRecvEvent->dwLen, TRUE, TRUE);
	m_pEventListRecvLoopBufLock[dwFlag % m_nEventListCount].UnLock();
	LOG_PROCESS_ERROR(nResult);

	pSocker = pstRecvEvent->pSocker;
	LOG_PROCESS_ERROR(pSocker);

	m_pNetMessageHandler->OnMessage(pSocker, m_pPacketTempBuf, pstRecvEvent->dwLen);

Exit0:

	return;
}

void LPAPI LPEventMgr::_ProcTerminateEvent(std::shared_ptr<TERMINATE_EVENT> pstTerminateEvent)
{
	LPINT32 nResult = 0;
	std::shared_ptr<ILPConnectorImpl> pConnector;
	ILPSockerImpl* pSocker = NULL;

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

void LPAPI LPEventMgr::_ProcEstablishEvent(std::shared_ptr<ESTABLISH_EVENT> pstEstablishEvent)
{
	LPINT32 nResult = 0;
	ILPSockerImpl* pSocker = NULL;

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

	pSocker->PostRecv();

Exit0:

	return;
}

void LPAPI LPEventMgr::_ProcConnectErrorEvent(std::shared_ptr<CONNECT_ERROR_EVENT> pstConnectErrorEvent)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pstConnectErrorEvent);
	LOG_PROCESS_ERROR(m_pNetMessageHandler);

	m_pNetMessageHandler->OnConnectError(pstConnectErrorEvent->pConnector, pstConnectErrorEvent->dwErrorNo);

Exit0:

	return;
}



//end声明所处的名字空间
NS_LZPL_END