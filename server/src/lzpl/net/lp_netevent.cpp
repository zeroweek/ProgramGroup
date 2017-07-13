#include "lp_netevent.h"
#include "lp_processerror.h"
#include "lp_net.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPINT32 NET_EVENT::ms_dwNetEventCount = 0;

LPEventMgr::LPEventMgr()
{
    m_bInit = FALSE;
    m_pPacketTempBuf = nullptr;
    m_nEventListCount = 0;
    m_vectEventList.clear();
    m_vectEventListRecvLoopBuf.clear();
    m_pNetMessageHandler = nullptr;
    m_pNetImpl = nullptr;
}

LPEventMgr::~LPEventMgr()
{
    UnInit();
}

BOOL LPAPI LPEventMgr::Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPNetMessageHandler> pNetMessageHandler, LPINT32 nEventListCount)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPLoopBuf> pLoopBuf = nullptr;

    LOG_PROCESS_ERROR(pNetMessageHandler);
    LOG_PROCESS_ERROR(pNetImpl);
    LOG_PROCESS_ERROR(nEventListCount > 0);

    m_pNetMessageHandler = pNetMessageHandler;
    m_pNetImpl = pNetImpl;
    m_nEventListCount = nEventListCount;

    for(LPINT32 i = 0; i < m_nEventListCount; ++i)
    {
        pLoopBuf = ILPLoopBuf::CreateBuf(m_pNetImpl->GetNetConfig().dwNetRecvEventBufSize);
        LOG_PROCESS_ERROR(pLoopBuf);

        m_vectEventListRecvLoopBuf.push_back(pLoopBuf);
    }

    for(int i = 0; i < m_nEventListCount; i++)
    {
        m_vectEventListRecvLoopBufLock.push_back(lp_make_shared<std::mutex>());
    }

    m_pPacketTempBuf = new char[MAX_PACKET_LEN];
    LOG_PROCESS_ERROR(m_pPacketTempBuf);

    for(int i = 0; i < m_nEventListCount; ++i)
    {
        m_vectEventList.push_back(EventList());
    }

    for(int i = 0; i < m_nEventListCount; i++)
    {
        m_vectEventListLock.push_back(lp_make_shared<std::mutex>());
    }

    m_bInit = TRUE;

    return TRUE;

Exit0:

    nResult = UnInit();
    LOG_CHECK_ERROR(nResult);

    return FALSE;
}

BOOL LPAPI LPEventMgr::UnInit()
{
    lp_shared_ptr<ILPLoopBuf> pLoopBuf = nullptr;

    PROCESS_SUCCESS(m_bInit == FALSE);
    m_bInit = FALSE;

    while(m_vectEventListRecvLoopBuf.size() > 0)
    {
        pLoopBuf = m_vectEventListRecvLoopBuf.back();
        m_vectEventListRecvLoopBuf.pop_back();
        ILPLoopBuf::ReleaseBuf(pLoopBuf);
    }

    SAFE_DELETE_SZ(m_pPacketTempBuf);

    m_vectEventList.clear();
    m_vectEventListLock.clear();
    m_vectEventListRecvLoopBufLock.clear();

Exit1:
    return TRUE;
}

BOOL LPAPI LPEventMgr::PushRecvEvent(lp_shared_ptr<ILPSockerImpl> pSocker, LPUINT32 dwSockerId, lp_shared_ptr<ILPLoopBuf> pLoopBuf, LPUINT32 dwLen)
{
    LPINT32 nResult = 0;
    LPINT32 nRetryCount = 0;
    LPUINT32 dwLineSize = 0;
    lp_shared_ptr<NET_EVENT> pstEvent = nullptr;

    PROCESS_SUCCESS(!m_bInit);

    LOG_PROCESS_ERROR(pSocker);
    LOG_PROCESS_ERROR(pSocker->GetSockerId() == dwSockerId);
    LOG_PROCESS_ERROR(pLoopBuf);
    LOG_PROCESS_ERROR(pLoopBuf->GetTotalReadableLen() >= dwLen);

    pstEvent = NET_EVENT::NewNetEvent(eEventType_Recv);
    LOG_PROCESS_ERROR(pstEvent != nullptr);
    LOG_PROCESS_ERROR(pstEvent->pRecvEvent != nullptr);

    pstEvent->eEventType = eEventType_Recv;
    pstEvent->dwFlag = dwSockerId;
    pstEvent->pRecvEvent->pSocker = pSocker;
    pstEvent->pRecvEvent->dwLen = dwLen;

    //lock
    m_vectEventListRecvLoopBufLock[pstEvent->dwFlag % m_nEventListCount]->lock();

    if(m_vectEventListRecvLoopBuf[pstEvent->dwFlag % m_nEventListCount]->GetTotalWritableLen() < dwLen)
    {
        m_vectEventListRecvLoopBufLock[pstEvent->dwFlag % m_nEventListCount]->unlock();

        ERR("function %s in file %s at line %d : buf not enough, close socket !", __FUNCTION__, __FILE__, __LINE__);
        pSocker->Close(SOCK_ERR_CODE(eSockErrCode_MessageEventBufOverflow, 1, 0), FALSE);
        LOG_PROCESS_ERROR(FALSE);
    }

    dwLineSize = pLoopBuf->GetOnceReadableLen();
    if(dwLineSize > dwLen)
    {
        dwLineSize = dwLen;
    }

    nResult = m_vectEventListRecvLoopBuf[pstEvent->dwFlag % m_nEventListCount]->Write(pLoopBuf->ReadPtr(), dwLineSize);
    LOG_CHECK_ERROR(nResult);

    pLoopBuf->FinishRead(dwLineSize);

    if(dwLineSize < dwLen)
    {
        nResult = m_vectEventListRecvLoopBuf[pstEvent->dwFlag % m_nEventListCount]->Write(pLoopBuf->ReadPtr(), dwLen - dwLineSize);
        LOG_CHECK_ERROR(nResult);

        pLoopBuf->FinishRead(dwLen - dwLineSize);
    }

    //unlock
    m_vectEventListRecvLoopBufLock[pstEvent->dwFlag % m_nEventListCount]->unlock();

    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->lock();
    m_vectEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->unlock();

Exit1:

    return TRUE;

Exit0:

    NET_EVENT::DeleteNetEvent(pstEvent);

    return FALSE;
}

void LPAPI LPEventMgr::PushTerminateEvent(lp_shared_ptr<ILPSockerImpl> pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<NET_EVENT> pstEvent = nullptr;

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

    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->lock();
    m_vectEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->unlock();

Exit1:

    return;

Exit0:

    NET_EVENT::DeleteNetEvent(pstEvent);

    return;
}

void LPAPI LPEventMgr::PushEstablishEvent(lp_shared_ptr<ILPSockerImpl> pSocker, BOOL bAccept)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<NET_EVENT> pstEvent = nullptr;

    PROCESS_SUCCESS(!m_bInit);

    LOG_PROCESS_ERROR(pSocker);

    pstEvent = NET_EVENT::NewNetEvent(eEventType_Establish);
    LOG_PROCESS_ERROR(pstEvent != nullptr);
    LOG_PROCESS_ERROR(pstEvent->pEstablishEvent != nullptr);

    pstEvent->eEventType = eEventType_Establish;
    pstEvent->dwFlag = pSocker->GetSockerId();
    pstEvent->pEstablishEvent->pSocker = pSocker;
    pstEvent->pEstablishEvent->bAccept = bAccept;

    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->lock();
    m_vectEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->unlock();

Exit1:

    return;

Exit0:

    NET_EVENT::DeleteNetEvent(pstEvent);

    return;
}

void LPAPI LPEventMgr::PushConnectErrorEvent(lp_shared_ptr<ILPConnectorImpl> pConnector, LPUINT32 dwErrorNo)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<NET_EVENT> pstEvent = nullptr;

    PROCESS_SUCCESS(!m_bInit);

    LOG_PROCESS_ERROR(pConnector);

    pstEvent = NET_EVENT::NewNetEvent(eEventType_ConnectError);
    LOG_PROCESS_ERROR(pstEvent != nullptr);
    LOG_PROCESS_ERROR(pstEvent->pConnectErrorEvent != nullptr);

    pstEvent->eEventType = eEventType_ConnectError;
    pstEvent->dwFlag = pConnector->GetId();
    pstEvent->pConnectErrorEvent->pConnector = pConnector;
    pstEvent->pConnectErrorEvent->dwErrorNo = dwErrorNo;

    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->lock();
    m_vectEventList[pstEvent->dwFlag % m_nEventListCount].push_back(pstEvent);
    m_vectEventListLock[pstEvent->dwFlag % m_nEventListCount]->unlock();

Exit1:

    return;

Exit0:

    NET_EVENT::DeleteNetEvent(pstEvent);

    return;
}

BOOL LPEventMgr::HaveEventForHandled()
{
    for(LPINT32 i = 0; i < m_nEventListCount; ++i)
    {
        if(!m_vectEventList[i].empty())
        {
            return TRUE;
        }
    }

    return FALSE;
}

void LPEventMgr::HandleOneEvent()
{
    LPINT32 nResult = 0;
    lp_shared_ptr<NET_EVENT> pstEvent = nullptr;

    PROCESS_SUCCESS(!m_bInit);

    for(LPINT32 i = 0; i < m_nEventListCount; ++i)
    {
        if(m_vectEventList[i].empty())
        {
            continue;
        }

        m_vectEventListLock[i]->lock();
        if(m_vectEventList[i].empty())
        {
            m_vectEventListLock[i]->unlock();
            continue;
        }

        pstEvent = m_vectEventList[i].front();
        m_vectEventList[i].pop_front();
        m_vectEventListLock[i]->unlock();

        LOG_PROCESS_ERROR(pstEvent);

        switch(pstEvent->eEventType)
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

void LPAPI LPEventMgr::_ProcRecvEvent(lp_shared_ptr<RECV_EVENT> pstRecvEvent, LPUINT32 dwFlag)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPSockerImpl> pSocker = nullptr;

    LOG_PROCESS_ERROR(pstRecvEvent);
    LOG_PROCESS_ERROR(m_pNetMessageHandler);
    LOG_PROCESS_ERROR(pstRecvEvent->dwLen < MAX_PACKET_LEN);

    m_vectEventListRecvLoopBufLock[dwFlag % m_nEventListCount]->lock();
    nResult = m_vectEventListRecvLoopBuf[dwFlag % m_nEventListCount]->Read(m_pPacketTempBuf, pstRecvEvent->dwLen, TRUE, TRUE);
    m_vectEventListRecvLoopBufLock[dwFlag % m_nEventListCount]->unlock();
    LOG_PROCESS_ERROR(nResult);

    pSocker = pstRecvEvent->pSocker;
    LOG_PROCESS_ERROR(pSocker);

    m_pNetMessageHandler->OnMessage(pSocker, m_pPacketTempBuf, pstRecvEvent->dwLen);

Exit0:

    return;
}

void LPAPI LPEventMgr::_ProcTerminateEvent(lp_shared_ptr<TERMINATE_EVENT> pstTerminateEvent)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPConnectorImpl> pConnector;
    lp_shared_ptr<ILPSockerImpl> pSocker = nullptr;

    LOG_PROCESS_ERROR(pstTerminateEvent);
    pSocker = pstTerminateEvent->pSocker;
    LOG_PROCESS_ERROR(pSocker);
    LOG_PROCESS_ERROR(m_pNetMessageHandler);

    if(pSocker->GetSockerId() != pstTerminateEvent->dwSockerId)
    {
        ERR("function %s in file %s at line %d : socker id error, %d:%d !", __FUNCTION__, __FILE__, __LINE__,
            pSocker->GetSockerId(), pstTerminateEvent->dwSockerId);
    }

    if(!pSocker->IsAcceptCreate())
    {
        pConnector = m_pNetImpl->FindConnector(pSocker->GetParentId());
        if(NULL == pConnector)
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

void LPAPI LPEventMgr::_ProcEstablishEvent(lp_shared_ptr<ESTABLISH_EVENT> pstEstablishEvent)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPSockerImpl> pSocker = nullptr;

    LOG_PROCESS_ERROR(pstEstablishEvent);
    LOG_PROCESS_ERROR(m_pNetMessageHandler);

    pSocker = pstEstablishEvent->pSocker;
    LOG_PROCESS_ERROR(pSocker);

    if(pstEstablishEvent->bAccept)
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

void LPAPI LPEventMgr::_ProcConnectErrorEvent(lp_shared_ptr<CONNECT_ERROR_EVENT> pstConnectErrorEvent)
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