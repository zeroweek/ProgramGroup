//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _GS_INTERNAL_PACKET_PARSER_H_
#define _GS_INTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "internal_message_header.h"



using namespace INTERNAL_MESSAGE;



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
};

extern LPINT32  g_MessageSize[max_internal_message_count];






#endif