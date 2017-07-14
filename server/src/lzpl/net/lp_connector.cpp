#include "lp_connector.h"
#include "lp_processerror.h"
#include "lp_net.h"
#include "lp_global.h"
#include "lpi_reactor.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



lp_shared_ptr<ILPConnectorImpl> LPAPI ILPConnectorImpl::NewConnectorImpl()
{
    return std::make_shared<LPConnector>();
}

void LPAPI ILPConnectorImpl::DeleteConnectorImpl(lp_shared_ptr<ILPConnectorImpl>& pConnector)
{
    pConnector = nullptr;
}



LPConnector::LPConnector()
{
    m_dwId = 0;
    SetState(eCommonState_NoInit);
    m_dwPort = 0;
    SetReconnect(FALSE);
    m_pPacketParser = NULL;
    m_pNetImpl = NULL;
    SetSocker(nullptr);
}

LPConnector::~LPConnector()
{
    UnInit();
}

BOOL LPAPI LPConnector::Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId)
{
    LPINT32 nResult = 0;

    LOG_PROCESS_ERROR(pPacketParser);
    LOG_PROCESS_ERROR(pNetImpl);
    LOG_PROCESS_ERROR(pReactor);

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

BOOL LPAPI LPConnector::UnInit()
{
    system::error_code err;

    PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);
    SetState(eCommonState_UnIniting);

    m_pReactor = nullptr;
    m_pNetImpl = nullptr;

    if(nullptr != m_pPacketParser)
    {
        m_pPacketParser = nullptr;
    }

    if(m_pSocket)
    {
        m_pSocket->close(err);
        m_pSocket = nullptr;
    }

    SetSocker(nullptr);

    SetState(eCommonState_UnInited);

Exit1:
    return TRUE;
}

BOOL LPAPI LPConnector::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect)
{
    LPINT32 nResult = 0;

    m_strIP = strIP;
    m_dwPort = dwPort;

    SetReconnect(bReconnect);

    nResult = PostConnect();
    LOG_PROCESS_ERROR(nResult);

    LPPRINTF("start connecting %s:%d !\n", m_strIP.c_str(), m_dwPort);
    IMP("start connecting %s:%d !", m_strIP.c_str(), m_dwPort);

    return TRUE;
Exit0:

    return FALSE;
}

void LPAPI LPConnector::Close()
{
    PROCESS_SUCCESS(GetState() >= eCommonState_Close);

    LOG_PROCESS_ERROR(GetState() == eCommonState_Inited);

    //不需关闭socket，统一由ILPSocker的Close关闭
    SetReconnect(FALSE);
    SetState(eCommonState_Close);

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
    LOG_PROCESS_ERROR(nullptr == GetSocker() && nullptr == m_pSocket);

    nResult = PostConnect();
    LOG_PROCESS_ERROR(nResult);

    return TRUE;
Exit0:
    return FALSE;
}

void LPAPI LPConnector::OnConnectionClose()
{
    LPINT32 nResult = 0;

    m_pSocket = nullptr;
    SetSocker(nullptr);

    if(IsReconnect())
    {
        nResult = Reconnect();
        LOG_PROCESS_ERROR(nResult);
    }

Exit0:
    return;
}

LPUINT16 LPAPI LPConnector::GetPort()
{
    return m_dwPort;;
}

std::string& LPAPI LPConnector::GetIp()
{
    return m_strIP;
}

void LPAPI LPConnector::SetSocker(lp_shared_ptr<ILPSockerImpl> pSocker)
{
    m_pSocker = pSocker;
}

lp_shared_ptr<ILPSockerImpl> LPAPI LPConnector::GetSocker()
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

void LPAPI LPConnector::CloseConnectSockWhileStateError()
{
    system::error_code err;
    if(nullptr != m_pSocket)
    {
        m_pSocket->close(err);
        LOG_CHECK_ERROR(system::errc::success == err);
        m_pSocket = nullptr;
    }
}

BOOL LPAPI LPConnector::PostConnect()
{
    system::error_code err;
    ip::tcp::endpoint ep(ip::address::from_string(m_strIP), m_dwPort);

    LOG_PROCESS_ERROR(nullptr == m_pSocket);
    LOG_PROCESS_ERROR(nullptr == m_pSocker);

    m_pSocket = lp_make_shared<ip::tcp::socket>(m_pReactor->GetIoService(GetId()));
    LOG_PROCESS_ERROR(m_pSocket);

    m_pSocket->async_connect(ep, boost::bind(&LPConnector::HandleConnect, this, _1));

    return TRUE;
Exit0:
    return FALSE;
}

void LPAPI LPConnector::HandleConnect(const system::error_code& err)
{
    LPINT32 nResult = 0;
    system::error_code err2;
    lp_shared_ptr<ILPSockerImpl> pSocker = nullptr;
    LPINT32 idwRetLocal = 0;
    LPINT32 idwRetRemote = 0;
    const char cArg = 1;

    switch(GetState())
    {
    case eCommonState_Initing:
        {
            INF("connector is initing, discard connect", __FUNCTION__);
            CloseConnectSockWhileStateError();
            nResult = PostConnect();
            LOG_PROCESS_ERROR(nResult);
            PROCESS_ERROR(FALSE);
        }
        break;
    case eCommonState_Inited:
        {
            if(system::errc::success != err)
            {
                m_pNetImpl->GetEventMgr().PushConnectErrorEvent(lp_shared_from_this(), WSAGetLastError());
                CloseConnectSockWhileStateError();
                SetSocker(nullptr);
                nResult = PostConnect();
                LOG_PROCESS_ERROR(nResult);
            }
            else
            {
                pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, FALSE);
                if(nullptr == pSocker)
                {
                    LOG_CHECK_ERROR(FALSE);
                    CloseConnectSockWhileStateError();
                    nResult = PostConnect();
                    LOG_PROCESS_ERROR(nResult);
                    PROCESS_ERROR(FALSE);
                }

                IMP("connector create socker, socker_id=%d, hSock=%d !", pSocker->GetSockerId(), (LPUINT32)m_pSocket->native());

                //获取本地地址 <UNDONE> ...
                //

                //设置sock选项
                m_pSocket->set_option(ip::tcp::no_delay(true));

                //设置LPSocker对象
                pSocker->SetSocket(m_pSocket);
                pSocker->SetConnect(true);

                //设置地址 <UNDONE> ...
                //

                //设置本连接器关联的socker对象
                //注意顺序：需要在socker收发数据之前设置，否则reactor处理获取socker为NULL
                SetSocker(pSocker);

                //push连接建立事件
                m_pNetImpl->GetEventMgr().PushEstablishEvent(pSocker, FALSE);
            }
        }
        break;
    case eCommonState_NoInit:
    case eCommonState_UnIniting:
    case eCommonState_UnInited:
    case eCommonState_Close:
    default:
        CloseConnectSockWhileStateError();
        LOG_PROCESS_ERROR(FALSE);
        break;
    }

Exit0:
    return;
}



//end声明所处的名字空间
NS_LZPL_END