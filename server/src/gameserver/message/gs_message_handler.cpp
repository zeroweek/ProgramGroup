#include "gs_message_handler.h"
#include "lp_processerror.h"
#include "lpi_message.h"



CGSMessageHandler::CGSMessageHandler()
{
    memset(m_MessageCallbackList, 0, sizeof(m_MessageCallbackList));
}

CGSMessageHandler::~CGSMessageHandler()
{
    UnInit();
}

BOOL LPAPI CGSMessageHandler::Init(void)
{
    m_pRecvMsgHead = MessageHead::CreateMsgHead(eMsgHeadType_ss);
    LOG_PROCESS_ERROR(m_pRecvMsgHead);

    m_pMessageSerializer = lp_make_shared<LPMessageSerializer>();
    LOG_PROCESS_ERROR(m_pMessageSerializer);

    m_pRecvMessageSerializer = lp_make_shared<LPMessageSerializer>();
    LOG_PROCESS_ERROR(m_pRecvMessageSerializer);

    m_pSendMessageSerializer = lp_make_shared<LPMessageSerializer>();
    LOG_PROCESS_ERROR(m_pSendMessageSerializer);

    //协议回调函数注册
    m_MessageCallbackList[LPDefine::msg_ack_server_register] = &CGSMessageHandler::OnGTRegisterAck;

    return TRUE;
Exit0:
    return FALSE;
}

BOOL LPAPI CGSMessageHandler::UnInit(void)
{
    MessageHead::DeleteMsgHead(m_pRecvMsgHead);

    m_pMessageSerializer        = nullptr;
    m_pRecvMessageSerializer    = nullptr;
    m_pSendMessageSerializer    = nullptr;

    return TRUE;
}

void LPAPI CGSMessageHandler::OnAccepted(lp_shared_ptr<ILPSocker> pSocker)
{
    std::pair<MAP_SOCKER::iterator, bool> InsRet;

    LOG_PROCESS_ERROR(pSocker);

    IMP("%s new socker : (%d:%d) (%s:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock(), pSocker->GetRemoteIp().c_str(), pSocker->GetRemotePort());

    InsRet = m_mapSocker.insert(std::make_pair(pSocker->GetSockerId(), pSocker));
    LOG_PROCESS_ERROR(InsRet.second);

    return;

Exit0:

    ERR("%s new socker insert fail : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

    return;
}

void LPAPI CGSMessageHandler::OnConnected(lp_shared_ptr<ILPSocker> pSocker)
{
    std::pair<MAP_SOCKER::iterator, bool> InsRet;

    LOG_PROCESS_ERROR(pSocker);

    IMP("%s new socker : (%d:%d) (%s:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock(), pSocker->GetRemoteIp().c_str(), pSocker->GetRemotePort());

    InsRet = m_mapSocker.insert(std::make_pair(pSocker->GetSockerId(), pSocker));
    LOG_PROCESS_ERROR(InsRet.second);

    DoGTRegister(pSocker);

    return;

Exit0:

    ERR("%s new socker insert fail : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

    return;
}

void LPAPI CGSMessageHandler::OnConnectError(lp_shared_ptr<ILPConnector> pConnector, LPUINT32 dwErrorNo)
{
    LOG_PROCESS_ERROR(pConnector != nullptr);

    ERR("connect to gt %s:%d fail, connector id %d, errno %d", pConnector->GetIp().c_str(), pConnector->GetPort(), pConnector->GetId(), dwErrorNo);

Exit0:
    return;
}

void LPAPI CGSMessageHandler::OnMessage(lp_shared_ptr<ILPSocker> pSocker, const char * pcszBuf, LPUINT32 dwSize)
{
    LPINT32 nResult = 0;
    LPUINT16 wMsgId = 0;

    LOG_PROCESS_ERROR(m_pRecvMsgHead);
    LOG_PROCESS_ERROR(pSocker);
    LOG_PROCESS_ERROR(pcszBuf);
    LOG_PROCESS_ERROR(dwSize >= m_pRecvMsgHead->GetHeadLength());

    nResult = m_pRecvMessageSerializer->Init(nullptr, 0, pcszBuf, m_pRecvMsgHead->GetHeadLength());
    LOG_PROCESS_ERROR(nResult);

    nResult = m_pRecvMsgHead->UnSerialize(m_pRecvMessageSerializer);
    LOG_PROCESS_ERROR(nResult);

    LOG_PROCESS_ERROR(LPDefine::msg_begin < m_pRecvMsgHead->GetMessageID() && m_pRecvMsgHead->GetMessageID() < LPDefine::msg_end);

    LOG_PROCESS_ERROR(m_MessageCallbackList[m_pRecvMsgHead->GetMessageID()]);

    (this->*m_MessageCallbackList[m_pRecvMsgHead->GetMessageID()])(pSocker, pcszBuf + m_pRecvMsgHead->GetHeadLength(), dwSize - m_pRecvMsgHead->GetHeadLength());

Exit0:
    return;
}

void LPAPI CGSMessageHandler::OnDisconnected(lp_shared_ptr<ILPSocker> pSocker)
{
    LOG_PROCESS_ERROR(pSocker);

    if(pSocker->IsPassiveClose())
    {
        IMP("%s peer close the socker : (%d:%d) (%s:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock(), pSocker->GetRemoteIp().c_str(), pSocker->GetRemotePort());
    }
    else
    {
        IMP("%s local close the socker : (%d:%d) (%s:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock(), pSocker->GetRemoteIp().c_str(), pSocker->GetRemotePort());
    }
    m_mapSocker.erase(pSocker->GetSockerId());

    return;

Exit0:

    return;
}

void LPAPI CGSMessageHandler::OnConnectDisconnected(lp_shared_ptr<ILPSocker> pSocker, lp_shared_ptr<ILPConnector> pConnector)
{
    LOG_PROCESS_ERROR(pSocker);
    LOG_PROCESS_ERROR(pConnector != nullptr);

    if(pSocker->IsPassiveClose())
    {
        IMP("%s peer close the connector socker : (%d:%d) (%s:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock(), pConnector->GetIp().c_str(), pConnector->GetPort());
    }
    else
    {
        IMP("%s local close the connector socker : (%d:%d) (%s:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock(), pConnector->GetIp().c_str(), pConnector->GetPort());
    }
    m_mapSocker.erase(pSocker->GetSockerId());

    return;

Exit0:

    return;
}

BOOL CGSMessageHandler::DoGTRegister(lp_shared_ptr<ILPSocker> pSocker)
{
    LPINT32 nResult = 0;
    LPMsg::ReqRegister xMsg;
    std::string strMsgData = nullstr;

    LOG_PROCESS_ERROR(pSocker);

    xMsg.set_server_id(1);
    xMsg.set_server_name("gameserver");
    xMsg.set_message("I'm gameserver, i want to registering !");

    LOG_PROCESS_ERROR(xMsg.SerializeToString(&strMsgData));

    nResult = SendMessage(pSocker, LPDefine::msg_req_server_register, strMsgData);
    LOG_PROCESS_ERROR(nResult);

    return TRUE;
Exit0:
    return FALSE;
}

void CGSMessageHandler::OnGTRegisterAck(lp_shared_ptr<ILPSocker> pSocker, const char * pcszBuf, LPUINT32 dwSize)
{
    LPINT32 nResult = 0;
    LPMsg::AckRegister xMsg;
    std::string strMsgData = nullstr;

    LOG_PROCESS_ERROR(pSocker);
    LOG_PROCESS_ERROR(pcszBuf);

    strMsgData = std::string(pcszBuf, dwSize);
    LOG_PROCESS_ERROR(xMsg.ParseFromString(strMsgData));

    IMP("recv gateserver regist ack, (%d : %s : %s)", xMsg.server_id(), xMsg.server_name().c_str(), xMsg.message().c_str());

Exit0:
    return;
}

void CGSMessageHandler::CloseAllSocker(void)
{
    lp_shared_ptr<ILPSocker> pSocker = nullptr;

    for(m_iterSocker = m_mapSocker.begin(); m_iterSocker != m_mapSocker.end(); ++m_iterSocker)
    {
        pSocker = m_iterSocker->second;
        LOG_CHECK_ERROR(pSocker);
        IF_NULL_CONTINUE(pSocker);

        pSocker->Close(SOCK_ERR_CODE(eSockErrCode_Success, 1, 0), FALSE);
    }
}

LPUINT32 CGSMessageHandler::GetSockerCount(void)
{
    return (LPUINT32)m_mapSocker.size();
}

BOOL CGSMessageHandler::SendMessage(lp_shared_ptr<ILPSocker> pSocker, const LPUINT32 nMsgID, const std::string& strMsgData)
{
    LPINT32 nResult = FALSE;
    lp_shared_ptr<MessageHead> pMsgHead = MessageHead::CreateMsgHead(eMsgHeadType_ss);

    LOG_PROCESS_ERROR(pSocker);
    LOG_PROCESS_ERROR(pMsgHead);
    LOG_PROCESS_ERROR(m_pSendMessageSerializer);

    pMsgHead->SetMessageID(nMsgID);
    pMsgHead->SetMessageLength(pMsgHead->GetHeadLength() + (LPUINT32)strMsgData.length());

    nResult = m_pSendMessageSerializer->Init(nullptr, 0, nullptr, 0);
    LOG_PROCESS_ERROR(nResult);

    nResult = pMsgHead->Serialize(m_pSendMessageSerializer);
    LOG_PROCESS_ERROR(nResult);

    nResult = pSocker->Send(m_pSendMessageSerializer->GetSerializeBuf(), m_pSendMessageSerializer->GetSerializeSize());
    LOG_PROCESS_ERROR(nResult);

    nResult = pSocker->Send(strMsgData.c_str(), (LPUINT32)strMsgData.length());
    LOG_PROCESS_ERROR(nResult);

    LOG_CHECK_ERROR(m_pSendMessageSerializer->UnInit());

    return TRUE;
Exit0:
    return FALSE;
}

