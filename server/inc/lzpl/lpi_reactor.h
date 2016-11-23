//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description: 
//
//        反应器ILPReactor接口声明。
//
//****************************************************************************/
#ifndef _LPI_REACTOR_H_
#define _LPI_REACTOR_H_

#include "lpi_socker.h"
#include "lpi_eventhandler.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		io操作类型枚举
enum e_IoOptType
{
	eIoOptType_None = 0,
	eIoOptType_Recv,
	eIoOptType_Send,
	eIoOptType_Accept,
	eIoOptType_Connect,
	eIoOptType_Max,
};


// Summary:
//		无
DECLARE const char * LPAPI lpGetIoOptTypeName(e_IoOptType eType);



// Summary：
//     反应器，与ILPEventHandler交互，负责网络事件处理
class DECLARE ILPReactor
{
public:

	// Summary:
	//		无
	virtual ~ILPReactor(){}

	// Summary:
	//     注册ILPEventHandler
	// Returns:
	//     TRUE-成功，FALSE-失败
	virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler) = 0;

	// Summary：
	//     注销ILPEventHandler
	// Returns:
	//     TRUE-成功，FALSE-失败
	virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler) = 0;
};




//end声明所处的名字空间
NS_LZPL_END

#endif