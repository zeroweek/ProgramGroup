#include "lp_errorstring.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



DECLARE const char* lpGetErrorString(LPUINT32 dwErrorString)
{
	switch (dwErrorString)
	{
	case LZPL::eErrorString_Unknow:
		return "*** eErrorString_Unknow ***";
	case LZPL::eErrorString_Fail:
		return "失败";
	case LZPL::eErrorString_Success:
		return "成功";
	case LZPL::eErrorString_Socket_AcceptFail:
		return "accept失败";
	case LZPL::eErrorString_Socket_ConnectFail:
		return "connect失败";
	case LZPL::eErrorString_Socket_SendBufOverFlow:
		return "发送缓冲区溢出";
	case LZPL::eErrorString_Socket_PostRecvFail:
		return "执行异步接收操作失败";
	case LZPL::eErrorString_Socket_PostSendFail:
		return "执行异步发送操作失败";
	case LZPL::eErrorString_Socket_ReactorErrorEvent:
		return "反应器错误事件";
	case LZPL::eErrorString_Socket_RecvError:
		return "接收数据错误";
	case LZPL::eErrorString_Total:
		return "eErrorString_Total";
	default:
		break;
	}

	return "*** invalid error string ***";
}



//end声明所处的名字空间

NS_LZPL_END
