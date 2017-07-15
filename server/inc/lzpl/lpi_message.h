//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: 消息接口定义
//
//****************************************************************************/
#ifndef _LPI_MESSAGE_H_
#define _LPI_MESSAGE_H_

#include "lpi_messageserializer.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//   消息包头类型
enum e_MsgHeadType
{
    eMsgHeadType_ss                       = 1, //内部server to server 消息包头
};



// Summary:
//   消息包头长度
enum e_MsgHeadLen
{
    eMsgHeadLen_ss                        = 8, //内部server to server 消息包头长度
};



#pragma pack(push, 1)

// Summary:
//   消息头接口结构
struct DECLARE MessageHead
{
    virtual ~MessageHead() { }

    virtual BOOL LPAPI Serialize(lp_shared_ptr<ILPMessageSerializer> pMessageSerializer) = 0;
    virtual BOOL LPAPI UnSerialize(lp_shared_ptr<ILPMessageSerializer> pMessageSerializer) = 0;

    virtual const LPUINT32 LPAPI GetHeadLength() = 0;

    virtual LPUINT32 LPAPI GetMessageID() = 0;
    virtual void LPAPI SetMessageID(LPUINT32 _nMessageID) = 0;

    virtual const LPUINT32 LPAPI GetMessageLength() = 0;
    virtual void LPAPI SetMessageLength(LPUINT32 _nMessageLength) = 0;

    static lp_shared_ptr<MessageHead> CreateMsgHead(e_MsgHeadType eType);
    static void DeleteMsgHead(lp_shared_ptr<MessageHead>& pMsgHead);
};

#pragma pack(pop)



// Summary:
//      消息接口类
class DECLARE ILPMessage
{
public:

    // Summary:
    //      无
    virtual ~ILPMessage() {}

public:

    virtual BOOL LPAPI Serialize(ILPMessageSerializer* pMessageSerializer) = 0;
    virtual BOOL LPAPI UnSerialize(ILPMessageSerializer* pMessageSerializer) = 0;

};







//end声明所处的名字空间
NS_LZPL_END

#endif