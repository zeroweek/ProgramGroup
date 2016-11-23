//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: 消息接口定义
//
//****************************************************************************/
#ifndef _LPI_MESSAGE_H_
#define _LPI_MESSAGE_H_

#include "lpi_messageserializer.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		消息接口类
class DECLARE ILPMessage
{
public:

	// Summary:
	//		无
	virtual ~ILPMessage() {}

public:

	virtual BOOL LPAPI Serialize(ILPMessageSerializer* pMessageSerializer) = 0;
	virtual BOOL LPAPI UnSerialize(ILPMessageSerializer* pMessageSerializer) = 0;

};







//end声明所处的名字空间
NS_LZPL_END

#endif