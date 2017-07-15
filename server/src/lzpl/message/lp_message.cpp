#include "lp_message.h"
#include "lp_string.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



lp_shared_ptr<LZPL::MessageHead> MessageHead::CreateMsgHead(e_MsgHeadType eType)
{
    switch(eType)
    {
    case LZPL::eMsgHeadType_ss:
        return lp_make_shared<SSMessageHead>();
    default:
        LOG_PROCESS_ERROR(FALSE);
        break;
    }

Exit0:
    return nullptr;
}

void LZPL::MessageHead::DeleteMsgHead(lp_shared_ptr<MessageHead>& pMsgHead)
{
    pMsgHead = nullptr;
}

BOOL LPAPI SSMessageHead::Serialize(lp_shared_ptr<ILPMessageSerializer> pMessageSerializer)
{
    LPINT32 nResult = FALSE;

    LOG_PROCESS_ERROR(pMessageSerializer);

    nResult = pMessageSerializer->WriteUint32(nMsgID);
    LOG_PROCESS_ERROR(nResult);

    nResult = pMessageSerializer->WriteUint32(nMsgLength);
    LOG_PROCESS_ERROR(nResult);

    return TRUE;
Exit0:
    return FALSE;
}

BOOL LPAPI SSMessageHead::UnSerialize(lp_shared_ptr<ILPMessageSerializer> pMessageSerializer)
{
    LPINT32 nResult = FALSE;

    LOG_PROCESS_ERROR(pMessageSerializer);

    nResult = pMessageSerializer->ReadUint32(&nMsgID);
    LOG_PROCESS_ERROR(nResult);

    nResult = pMessageSerializer->ReadUint32(&nMsgLength);
    LOG_PROCESS_ERROR(nResult);

    return TRUE;
Exit0:
    return FALSE;
}

const LPUINT32 LPAPI SSMessageHead::GetHeadLength()
{
    return (LPUINT32)eMsgHeadLen_ss;
}

LPUINT32 LPAPI SSMessageHead::GetMessageID()
{
    return nMsgID;
}

void LPAPI SSMessageHead::SetMessageID(LPUINT32 nMessageID)
{
    nMsgID = nMessageID;
}

const LPUINT32 LPAPI SSMessageHead::GetMessageLength()
{
    return nMsgLength;
}

void LPAPI SSMessageHead::SetMessageLength(LPUINT32 nMessageLength)
{
    nMsgLength = nMessageLength;
}



//end声明所处的名字空间

NS_LZPL_END