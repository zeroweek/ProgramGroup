//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_NET_DEF_H_
#define _LP_NET_DEF_H_

#include "lp_base.h"


#if defined _WIN32
#include <MSWSock.h>
#else

#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		事件处理器类型
enum e_EventHandlerType
{
	eEventHandlerType_None = 0,
	eEventHandlerType_Connector = 1,
	eEventHandlerType_Listener = 2,
	eEventHandlerType_Socker = 3,
	eEventHandlerType_Max
};



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
//		每个io对应的绑定数据
struct PER_IO_DATA
{
	OVERLAPPED             stOverlapped;
	SOCKET                 hSock;
	e_IoOptType            eIoOptType;
	e_EventHandlerType     eHandlerType;
	LPUINT64               qwByteTransferred;
	WSABUF                 stWsaBuf;
	char                   szBuf[128];

	PER_IO_DATA()
	{
		memset(&stOverlapped, 0, sizeof(stOverlapped));
		hSock = INVALID_SOCKET;
		eIoOptType = eIoOptType_None;
		eHandlerType = eEventHandlerType_None;
		qwByteTransferred = 0;
		memset(&stWsaBuf, 0, sizeof(stWsaBuf));
		memset(szBuf, 0, sizeof(szBuf));
	}
};









//end声明所处的名字空间
NS_LZPL_END

#endif