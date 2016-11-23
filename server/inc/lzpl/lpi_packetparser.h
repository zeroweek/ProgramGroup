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
//		网络包解析接口类
class DECLARE ILPPacketParser : public ILPObject
{
public:

	// Summary:
	//		无
	virtual ~ILPPacketParser(){}

	// Summary:
	//		解包
	// Input:
	//		pLoopBuf：待解析数据缓冲区
	// Return:
	//		"<0"-解析错误，"=0"-不完整包，">0"-完整包长度
	virtual INT_32 LPAPI Parse(ILPLoopBuf* pLoopBuf) = 0;
};




//end声明所处的名字空间
NS_LZPL_END

#endif