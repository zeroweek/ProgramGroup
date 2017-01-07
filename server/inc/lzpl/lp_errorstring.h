//****************************************************************************/
//    author: caihy
//    date: Jan 7, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_ERROR_STRING_H_
#define _LP_ERROR_STRING_H_

#include "lp_base.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary
//   错误string枚举
enum e_ErrorString
{
	eErrorString_Unknow = -1,
	eErrorString_Fail = 0,
	eErrorString_Success = 1,
	eErrorString_Socket_AcceptFail = 2,
	eErrorString_Socket_ConnectFail = 3,
	eErrorString_Socket_SendBufOverFlow = 4,
	eErrorString_Socket_PostRecvFail = 5,
	eErrorString_Socket_PostSendFail = 6,
	eErrorString_Socket_ReactorErrorEvent = 7,
	eErrorString_Socket_RecvError = 8,
	eErrorString_Total
};



// Summary:
//   获取错误码string
DECLARE const char* lpGetErrorString(LPUINT32 dwErrorString);



//end声明所处的名字空间
NS_LZPL_END
#endif