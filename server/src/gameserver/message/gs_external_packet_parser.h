//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _GS_EXTERNAL_PACKET_PARSER_H_
#define _GS_EXTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "external_message_header.h"


using namespace EXTERNAL_MESSAGE;



// Summary:
//
class DECLARE CGSExternalPacketParser : public ILPPacketParser
{
public:

    // Summary:
    //      无
    CGSExternalPacketParser();

    // Summary:
    //      无
    ~CGSExternalPacketParser();

    // Summary:
    //      解包
    // Input:
    //      pLoopBuf：待解析数据缓冲区
    // Return:
    //      "<0"-解析错误，"=0"-不完整包，">0"-完整包长度
    virtual LPINT32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

private:

    EXTERNAL_MESSAGE_HEADER       m_stMessageHead;
};






#endif