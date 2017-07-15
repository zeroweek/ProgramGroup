//****************************************************************************/
//    author: caihy
//    date: July 15, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_MESSAGE_H_
#define _LP_MESSAGE_H_

#include "lpi_message.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#pragma pack(push, 1)

// Summary:
//   消息头接口结构
struct DECLARE SSMessageHead : public MessageHead
{
    LPUINT32 nMsgID;
    LPUINT32 nMsgLength;

    SSMessageHead() : nMsgID(0), nMsgLength(0) { }
    virtual ~SSMessageHead() { }

    virtual BOOL LPAPI Serialize(lp_shared_ptr<ILPMessageSerializer> pMessageSerializer);
    virtual BOOL LPAPI UnSerialize(lp_shared_ptr<ILPMessageSerializer> pMessageSerializer);

    virtual const LPUINT32 LPAPI GetHeadLength();

    virtual LPUINT32 LPAPI GetMessageID();
    virtual void LPAPI SetMessageID(LPUINT32 nMessageID);

    virtual const LPUINT32 LPAPI GetMessageLength();
    virtual void LPAPI SetMessageLength(LPUINT32 nMessageLength);
};

#pragma pack(pop)





//end声明所处的名字空间
NS_LZPL_END

#endif