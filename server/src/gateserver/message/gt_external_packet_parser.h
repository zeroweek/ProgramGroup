//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _GT_EXTERNAL_PACKET_PARSER_H_
#define _GT_EXTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "external_message_header.h"


using namespace EXTERNAL_MESSAGE;



// Summary:
//
class DECLARE CGTExternalPacketParser : public ILPPacketParser
{
public:

    // Summary:
    //      ��
    CGTExternalPacketParser();

    // Summary:
    //      ��
    ~CGTExternalPacketParser();

    // Summary:
    //      ���
    // Input:
    //      pLoopBuf�����������ݻ�����
    // Return:
    //      "<0"-��������"=0"-����������">0"-����������
    virtual LPINT32 LPAPI Parse(lp_shared_ptr<ILPLoopBuf> pLoopBuf);

private:

    EXTERNAL_MESSAGE_HEADER       m_stMessageHead;
};






#endif