#include "gt_internal_packet_parser.h"
#include "lp_processerror.h"
#include "LPDefine.pb.h"



CGTInternalPacketParser::CGTInternalPacketParser()
{
    m_pMessageHead = MessageHead::CreateMsgHead(eMsgHeadType_ss);
    LOG_PROCESS_ERROR(m_pMessageHead);

    m_pszMsgHead = new char[m_pMessageHead->GetHeadLength()];
    LOG_PROCESS_ERROR(m_pszMsgHead);

    m_pMessageSerializer = lp_make_shared<LPMessageSerializer>();
    LOG_PROCESS_ERROR(m_pMessageSerializer);

    return;
Exit0:

    SAFE_DELETE_SZ(m_pszMsgHead);
    return;
}

CGTInternalPacketParser::~CGTInternalPacketParser()
{
    SAFE_DELETE_SZ(m_pszMsgHead);
    MessageHead::DeleteMsgHead(m_pMessageHead);
    m_pMessageSerializer = nullptr;
}

LPINT32 LPAPI CGTInternalPacketParser::Parse(lp_shared_ptr<ILPLoopBuf> pLoopBuf)
{
    LPINT32 nResult = 0;
    LPINT32 nParseCount = 0;

    LOG_PROCESS_ERROR(pLoopBuf);
    LOG_PROCESS_ERROR(m_pMessageHead);
    LOG_PROCESS_ERROR(m_pszMsgHead);

    if(pLoopBuf->GetTotalReadableLen() < m_pMessageHead->GetHeadLength())
    {
        PROCESS_SUCCESS(TRUE);
    }

    nResult = pLoopBuf->Read(m_pszMsgHead, m_pMessageHead->GetHeadLength(), FALSE, FALSE);
    LOG_PROCESS_ERROR(nResult);

    nResult = m_pMessageSerializer->Init(nullptr, 0, m_pszMsgHead, m_pMessageHead->GetHeadLength());
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

    return nParseCount;
}
