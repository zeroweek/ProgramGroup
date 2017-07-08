//****************************************************************************/
//    author: caihy
//    date: October 16, 2014
//    description: 网络包解析接口定义
//
//****************************************************************************/
#ifndef _LPI_PACKET_PARSER_H_
#define _LPI_PACKET_PARSER_H_

#include "lpi_buf.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      网络包解析接口类
class DECLARE ILPPacketParser
{
public:

    // Summary:
    //      无
    virtual ~ILPPacketParser() {}

    // Summary:
    //      引用计数加1
    virtual void LPAPI AddRef(void) = 0;

    // Summary:
    //      引用计数减1
    virtual LPUINT32 LPAPI QueryRef(void) = 0;

    // Summary:
    //      释放对象
    virtual void LPAPI Release(void) = 0;

    // Summary:
    //      解包
    // Input:
    //      pLoopBuf：待解析数据缓冲区
    // Return:
    //      "<0"-解析错误，"=0"-不完整包，">0"-完整包长度
    virtual LPINT32 LPAPI Parse(ILPLoopBuf* pLoopBuf) = 0;
};




//end声明所处的名字空间
NS_LZPL_END

#endif