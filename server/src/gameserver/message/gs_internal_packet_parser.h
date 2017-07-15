//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _GS_INTERNAL_PACKET_PARSER_H_
#define _GS_INTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "lpi_message.h"
#include "lp_messageserializer.h"



// Summary:
//
class DECLARE CGSInternalPacketParser : public ILPPacketParser
{
public:

    // Summary:
    //      ��
    CGSInternalPacketParser();

    // Summary:
    //      ��
    ~CGSInternalPacketParser();

    // Summary:
    //      ���
    // Input:
    //      pLoopBuf�����������ݻ�����
    // Return:
    //      "<0"-��������"=0"-����������">0"-����������
    virtual LPINT32 LPAPI Parse(lp_shared_ptr<ILPLoopBuf> pLoopBuf);

private:

    lp_shared_ptr<MessageHead> m_pMessageHead;
    lp_shared_ptr<LPMessageSerializer> m_pMessageSerializer;
};



#endif