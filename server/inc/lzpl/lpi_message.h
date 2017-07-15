//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: ��Ϣ�ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_MESSAGE_H_
#define _LPI_MESSAGE_H_

#include "lpi_messageserializer.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//   ��Ϣ��ͷ����
enum e_MsgHeadType
{
    eMsgHeadType_ss                       = 1, //�ڲ�server to server ��Ϣ��ͷ
};



// Summary:
//   ��Ϣ��ͷ����
enum e_MsgHeadLen
{
    eMsgHeadLen_ss                        = 8, //�ڲ�server to server ��Ϣ��ͷ����
};



#pragma pack(push, 1)

// Summary:
//   ��Ϣͷ�ӿڽṹ
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
//      ��Ϣ�ӿ���
class DECLARE ILPMessage
{
public:

    // Summary:
    //      ��
    virtual ~ILPMessage() {}

public:

    virtual BOOL LPAPI Serialize(ILPMessageSerializer* pMessageSerializer) = 0;
    virtual BOOL LPAPI UnSerialize(ILPMessageSerializer* pMessageSerializer) = 0;

};







//end�������������ֿռ�
NS_LZPL_END

#endif