#include "gs_internal_packet_parser.h"
#include "lp_processerror.h"
#include "LPDefine.pb.h"



CGSInternalPacketParser::CGSInternalPacketParser()
{
    m_pMessageHead = MessageHead::CreateMsgHead(eMsgHeadType_ss);
    LOG_PROCESS_ERROR(m_pMessageHead);

    m_pMessageSerializer = lp_make_shared<LPMessageSerializer>();
    LOG_PROCESS_ERROR(m_pMessageSerializer);

Exit0:
    return;
}

CGSInternalPacketParser::~CGSInternalPacketParser()
{
    MessageHead::DeleteMsgHead(m_pMessageHead);
    m_pMessageSerializer = nullptr;
}

LPINT32 LPAPI CGSInternalPacketParser::Parse(lp_shared_ptr<ILPLoopBuf> pLoopBuf)
{
    LPINT32 nResult = 0;
    LPINT32 nParseCount = 0;
    char* pszMsgHead = nullptr;

    LOG_PROCESS_ERROR(pLoopBuf);
    LOG_PROCESS_ERROR(m_pMessageHead);

    if(pLoopBuf->GetTotalReadableLen() < m_pMessageHead->GetHeadLength())
    {
        PROCESS_SUCCESS(TRUE);
    }

    pszMsgHead = new char[m_pMessageHead->GetHeadLength()];
    LOG_PROCESS_ERROR(pszMsgHead);

    nResult = pLoopBuf->Read(pszMsgHead, m_pMessageHead->GetHeadLength(), FALSE, FALSE);
    LOG_PROCESS_ERROR(nResult);

    nResult = m_pMessageSerializer->Init(nullptr, 0, pszMsgHead, m_pMessageHead->GetHeadLength());
    LOG_PROCESS_ERROR(nResult);

    nResult = m_pMessageHead->UnSerialize(m_pMessageSerializer);
    LOG_PROCESS_ERROR(nResult);

    LOG_PROCESS_ERROR(LPDefine::msg_begin < m_pMessageHead->GetMessageID() && m_pMessageHead->GetMessageID() < LPDefine::msg_end);

    if(pLoopBuf->GetTotalReadableLen() >= m_pMessageHead->GetMessageLength())
    {
        nParseCount = m_pMessageHead->GetMessageLength();
    }

    PROCESS_SUCCESS(TRUE);

Exit0:
    nParseCount = -1;

Exit1:

    SAFE_DELETE_SZ(pszMsgHead);

    return nParseCount;
}
