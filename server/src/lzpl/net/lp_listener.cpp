#include "lp_listener.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_net.h"
#include "lp_global.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



std::shared_ptr<ILPListenerImpl> LPAPI ILPListenerImpl::NewListenerImpl()
{
    return std::make_shared<LPListener>();
}

void LPAPI ILPListenerImpl::DeleteListenerImpl(std::shared_ptr<ILPListenerImpl>& pListener)
{
    pListener = nullptr;
}



LPListener::LPListener()
{
    m_dwId = 0;
    m_dwPort = 0;
    m_bReUseAddr = FALSE;
    m_pPacketParser = NULL;
    m_pNetImpl = NULL;

    SetState(eCommonState_NoInit);
}

LPListener::~LPListener()
{
    UnInit();
}

BOOL LPAPI LPListener::Init(LPNetImpl* pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId)
{
    LPINT32 nResult = 0;

    LOG_PROCESS_ERROR(pNetImpl);
    LOG_PROCESS_ERROR(pReactor);
    LOG_PROCESS_ERROR(pPacketParser);

    LOG_PROCESS_ERROR(GetState() == eCommonState_NoInit);
    SetState(eCommonState_Initing);

    m_dwId = dwId;
    m_pPacketParser = pPacketParser;
    m_pNetImpl = pNetImpl;
    m_pReactor = pReactor;

    SetState(eCommonState_Inited);
    return TRUE;
Exit0:

    UnInit();

    return FALSE;
}

BOOL LPAPI LPListener::UnInit()
{
    system::error_code err;

    PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);
    SetState(eCommonState_UnIniting);

    m_pReactor = nullptr;

    if(m_pAcceptor)
    {
        m_pAcceptor->close(err);
        LOG_CHECK_ERROR(system::errc::success == err);
        m_pAcceptor = nullptr;
    }

    if(nullptr != m_pPacketParser)
    {
        m_pPacketParser = nullptr;
    }

    SetState(eCommonState_UnInited);

Exit1:
    return TRUE;
}

BOOL LPAPI LPListener::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr)
{
    LPINT32 nResult = 0;
    ip::tcp::endpoint ep(ip::address::from_string(strIP), dwPort);

    LOG_PROCESS_ERROR(GetState() == eCommonState_Inited);

    m_strIP = strIP;
    m_dwPort = dwPort;
    m_bReUseAddr = bReUseAddr;

    m_pAcceptor = lp_make_shared<ip::tcp::acceptor>(m_pReactor->GetIoService(GetId()), ep);
    LOG_PROCESS_ERROR(m_pAcceptor);

    if(bReUseAddr)
    {
        m_pAcceptor->set_option(ip::tcp::socket::reuse_address(true));
    }

    m_pAcceptor->set_option(ip::tcp::socket::linger(true, 0));

    nResult = PostAccept();
    LOG_PROCESS_ERROR(nResult);

    IMP("start listening %s:%d !", m_strIP.c_str(), m_dwPort);

    return TRUE;
Exit0:

    return FALSE;
}

void LPAPI LPListener::Close()
{
    system::error_code err;

    PROCESS_SUCCESS(GetState() >= eCommonState_Close);

    LOG_PROCESS_ERROR(GetState() == eCommonState_Inited);
    SetState(eCommonState_Close);

    if(m_pAcceptor)
    {
        m_pAcceptor->cancel(err);
    }

Exit1:
Exit0:
    return;
}

LPUINT32 LPAPI LPListener::GetId()
{
    return m_dwId;
}

LPUINT32 LPAPI LPListener::GetState()
{
    return m_dwState;
}

void LPAPI LPListener::SetState(LPUINT32 dwState)
{
    m_dwState = dwState;
}

BOOL LPAPI LPListener::PostAccept()
{
    system::error_code err;
    lp_shared_ptr<ip::tcp::socket> pSocket;

    pSocket = lp_make_shared<ip::tcp::socket>(m_pReactor->GetIoService(GetId()));
    LOG_PROCESS_ERROR(pSocket);

    m_pAcceptor->async_accept(*pSocket, boost::bind(&LPListener::HandleAccept, this, pSocket, _1), err);
    if(system::errc::success != err)
    {
        ERR("call async_accept error %d !", err);
        LOG_PROCESS_ERROR(FALSE);
    }

    return TRUE;
Exit0:
    return FALSE;
}

void LPAPI LPListener::HandleAccept(lp_shared_ptr<ip::tcp::socket> pSocket, const system::error_code& errAccept)
{
    LPINT32 nResult = 0;
    system::error_code err;
    lp_shared_ptr<ILPSockerImpl> pSocker = nullptr;
    LPUINT32 dwCurValidConnectCount = 0;

    LOG_PROCESS_ERROR(pSocket);

    switch(GetState())
    {
    case eCommonState_Initing:
        {
            INF("listener is initing, discard accept", __FUNCTION__);
            pSocket->close(err);
            nResult = PostAccept();
            LOG_PROCESS_ERROR(nResult);
            PROCESS_ERROR(FALSE);
        }
        break;
    case eCommonState_Inited:
        {
            if(system::errc::success != errAccept)
            {
                FTL("%s accept failed, errno %d", __FUNCTION__, WSAGetLastError());
                pSocket->close(err);
                nResult = PostAccept();
                LOG_PROCESS_ERROR(nResult);
                PROCESS_ERROR(FALSE);
            }
            else
            {
                nResult = PostAccept();
                LOG_CHECK_ERROR(nResult);

                dwCurValidConnectCount = m_pNetImpl->GetSockerMgr().GetCurValidConnectCount();
                if(dwCurValidConnectCount >= m_pNetImpl->GetNetConfig().dwConnectCount)
                {
                    INF("encounter max connect count, connect count(current/max): %d/%d !",
                        dwCurValidConnectCount, m_pNetImpl->GetNetConfig().dwConnectCount);
                    pSocket->close(err);
                    PROCESS_ERROR(FALSE);
                }

                pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, TRUE);
                if(NULL == pSocker)
                {
                    LOG_CHECK_ERROR(FALSE);
                    FTL("%s create LPSocker failed", __FUNCTION__);
                    pSocket->close(err);
                    PROCESS_ERROR(FALSE);
                }

                IMP("listener create socker, socker_id=%d, hSock=%d !", pSocker->GetSockerId(), (LPUINT32)pSocket->native());

                //设置sock选项
                pSocket->set_option(ip::tcp::no_delay(true));

                //设置LPSocker对象
                pSocker->SetSock((LPUINT32)pSocket->native());
                pSocker->SetSocket(pSocket);
                pSocker->SetConnect(true);

                //设置地址 <UNDONE> ...
                //

                //push连接建立事件
                m_pNetImpl->GetEventMgr().PushEstablishEvent(pSocker, TRUE);
            }
        }
        break;
    case eCommonState_NoInit:
    case eCommonState_UnIniting:
    case eCommonState_UnInited:
    case eCommonState_Close:
    default:
        LOG_CHECK_ERROR(system::errc::success != errAccept);
        pSocket->close(err);
        LOG_PROCESS_ERROR(FALSE);
        break;
    }

Exit0:
    return;
}



//end声明所处的名字空间
NS_LZPL_END