//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _GT_INTERNAL_PACKET_PARSER_H_
#define _GT_INTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "internal_message_header.h"



using namespace INTERNAL_MESSAGE;



// Summary:
//
class DECLARE CGTInternalPacketParser : public ILPPacketParser
{
public:

    // Summary:
    //      ��
    CGTInternalPacketParser();

    // Summary:
    //      ��
    ~CGTInternalPacketParser();

    // Summary:
    //      ���
    // Input:
    //      pLoopBuf�����������ݻ�����
    // Return:
    //      "<0"-��������"=0"-����������">0"-����������
    virtual LPINT32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

};

extern LPINT32  g_MessageSize[max_internal_message_count];






#endif