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
    //      无
    CGTInternalPacketParser();

    // Summary:
    //      无
    ~CGTInternalPacketParser();

    // Summary:
    //      解包
    // Input:
    //      pLoopBuf：待解析数据缓冲区
    // Return:
    //      "<0"-解析错误，"=0"-不完整包，">0"-完整包长度
    virtual LPINT32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

};

extern LPINT32  g_MessageSize[max_internal_message_count];






#endif