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
    //      无
    CGSInternalPacketParser();

    // Summary:
    //      无
    ~CGSInternalPacketParser();

    // Summary:
    //      解包
    // Input:
    //      pLoopBuf：待解析数据缓冲区
    // Return:
    //      "<0"-解析错误，"=0"-不完整包，">0"-完整包长度
    virtual LPINT32 LPAPI Parse(lp_shared_ptr<ILPLoopBuf> pLoopBuf);

private:

    lp_shared_ptr<MessageHead> m_pMessageHead;
    lp_shared_ptr<LPMessageSerializer> m_pMessageSerializer;
};



#endif