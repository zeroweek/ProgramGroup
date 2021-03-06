#include "lp_sockermgr.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_dump.h"
#include "lp_net.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPSockerMgr::LPSockerMgr()
{
    m_oValidMap.clear();
    m_dwMaxSockId           = INVALID_SOCKER_ID;
    m_nCurValidConnectCount = 0;

    m_pNetImpl              = NULL;
    m_pRecvLoopBufPool      = nullptr;
    m_pSendLoopBufPool      = nullptr;
}

LPSockerMgr::~LPSockerMgr()
{
    UnInit();
}

BOOL LPAPI LPSockerMgr::Init(lp_shared_ptr<LPNetImpl> pNetImpl)
{
    LPINT32 nResult = 0;

    LOG_PROCESS_ERROR(pNetImpl);
    m_pNetImpl = pNetImpl;

    m_pRecvLoopBufPool = ILPLoopBufPool::CreatePool();
    LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
    nResult = m_pRecvLoopBufPool->Init(m_pNetImpl->GetNetConfig().dwRecvBufSize, TRUE, m_pNetImpl->GetNetConfig().dwConnectCount);
    LOG_PROCESS_ERROR(nResult);

    m_pSendLoopBufPool = ILPLoopBufPool::CreatePool();
    LOG_PROCESS_ERROR(m_pSendLoopBufPool);
    nResult = m_pSendLoopBufPool->Init(m_pNetImpl->GetNetConfig().dwSendBufSize, TRUE, m_pNetImpl->GetNetConfig().dwConnectCount);
    LOG_PROCESS_ERROR(nResult);

    return TRUE;

Exit0:

    nResult = UnInit();
    LOG_CHECK_ERROR(nResult);

    return FALSE;
}

BOOL LPAPI LPSockerMgr::UnInit()
{
    ILPLoopBufPool::ReleasePool(m_pRecvLoopBufPool);
    ILPLoopBufPool::ReleasePool(m_pSendLoopBufPool);

    return TRUE;
}

lp_shared_ptr<ILPSockerImpl> LPAPI LPSockerMgr::Create(lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPSockerImpl> pSocker = nullptr;

    LOG_PROCESS_ERROR(pPacketParser);
    LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
    LOG_PROCESS_ERROR(m_pSendLoopBufPool);
    LOG_PROCESS_ERROR(m_pRecvLoopBufPool->GetBufSize() == m_pNetImpl->GetNetConfig().dwRecvBufSize);
    LOG_PROCESS_ERROR(m_pSendLoopBufPool->GetBufSize() == m_pNetImpl->GetNetConfig().dwSendBufSize);

    pSocker = _Create(pPacketParser, dwParentId, bAcceptCreate);

    return pSocker;

Exit0:

    return pSocker;
}

void LPAPI LPSockerMgr::Release(lp_shared_ptr<ILPSockerImpl> pSocker)
{
    BOOL bAcceptCreate = FALSE;
    LPUINT32 nParentId = 0;
    lp_shared_ptr<ILPConnectorImpl> pConnector;

    LOG_PROCESS_ERROR(pSocker);
    nParentId = pSocker->GetParentId();
    bAcceptCreate = pSocker->IsAcceptCreate();

    _Release(pSocker);
    pSocker = nullptr;

    //如果是主动连接，需要通知连接器
    //需要在完全关闭，且释放socker之后再通知，防止重连后与旧的连接信息冲突
    if(!bAcceptCreate)
    {
        pConnector = m_pNetImpl->FindConnector(nParentId);
        LOG_PROCESS_ERROR(pConnector);
        pConnector->OnConnectionClose();
    }

Exit0:
    return;
}

lp_shared_ptr<ILPSockerImpl> LPAPI LPSockerMgr::_Create(lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPSockerImpl> pSocker = nullptr;
    lp_shared_ptr<ILPLoopBuf> pRecvLoopBuf = nullptr;
    lp_shared_ptr<ILPLoopBuf> pSendLoopBuf = nullptr;

    LOG_PROCESS_ERROR(pPacketParser);
    LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
    LOG_PROCESS_ERROR(m_pSendLoopBufPool);

    pSocker = ILPSockerImpl::NewSockerImpl();
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

    m_oValidMutex.lock();
    ++m_nCurValidConnectCount;
    m_oValidMap.insert(make_pair(pSocker->GetSockerId(), pSocker));
    m_oValidMutex.unlock();

    return pSocker;

Exit0:

    ILPSockerImpl::DeleteSockerImpl(pSocker);
    if(pRecvLoopBuf) m_pRecvLoopBufPool->Release(pRecvLoopBuf);
    if(pSendLoopBuf) m_pSendLoopBufPool->Release(pSendLoopBuf);

    return pSocker;
}

void LPAPI LPSockerMgr::_Release(lp_shared_ptr<ILPSockerImpl> pSocker)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPLoopBuf> pRecvLoopBuf = nullptr;
    lp_shared_ptr<ILPLoopBuf> pSendLoopBuf = nullptr;

    LOG_PROCESS_ERROR(pSocker);

    pRecvLoopBuf = pSocker->DetachRecvBuf();
    pSendLoopBuf = pSocker->DetachSendBuf();
    LOG_CHECK_ERROR(pRecvLoopBuf);
    LOG_CHECK_ERROR(pSendLoopBuf);

    pSocker->SetSocket(nullptr);
    pSocker->SetSockerId(INVALID_SOCKER_ID);
    pSocker->DetachPacketParser();
    pSocker->SetNetImpl(NULL);

    m_oValidMutex.lock();
    --m_nCurValidConnectCount;
    m_oValidMap.erase(pSocker->GetSockerId());
    m_oValidMutex.unlock();

    LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
    m_pRecvLoopBufPool->Release(pRecvLoopBuf);

    LOG_PROCESS_ERROR(m_pSendLoopBufPool);
    m_pSendLoopBufPool->Release(pSendLoopBuf);

    ILPSockerImpl::DeleteSockerImpl(pSocker);

Exit0:
    return;
}

LPUINT32 LPAPI LPSockerMgr::_CreateSockId()
{
    return ++m_dwMaxSockId;
}

lp_shared_ptr<ILPSockerImpl> LPSockerMgr::Find(LPUINT32 dwSockerId)
{
    lp_shared_ptr<ILPSockerImpl> pSocker = NULL;
    LPMapSocker::iterator fit;

    m_oValidMutex.lock();
    fit = m_oValidMap.find(dwSockerId);
    if(fit != m_oValidMap.end())
    {
        pSocker = fit->second;
    }
    m_oValidMutex.unlock();

    return pSocker;
}

LPUINT32 LPAPI LPSockerMgr::GetCurValidConnectCount()
{
    return m_nCurValidConnectCount;
}



//end声明所处的名字空间
NS_LZPL_END