//****************************************************************************/
//    author: caihy
//    date: October 20, 2014
//    description: 
//
//****************************************************************************/
#ifndef _LPI_SOCKER_H_
#define _LPI_SOCKER_H_

#include "lpi_sockoption.h"
#include "lp_processerror.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN



//****************************************************************************/
//	自定义socket错误码枚举
//
//		1. 连接建立前的错误事件：socket、connect、bind、listen、accept返回错误
//		2. 连接建立事件：connect、accept返回成功
//		3. 连接保持中的成功事件：read、write返回成功
//		3. 连接保持中的错误事件：read、write返回非网络断开错误
//		4. 连接断开事件：read、write返回网络断开
//
//****************************************************************************/
enum e_SockErrCode
{
	eSockErrCode_Fail                        = 0,    // 失败
	eSockErrCode_Success                     = 1,    // 成功
	eSockErrCode_AcceptFail                  = 2,    // 接受失败
	eSockErrCode_ConnectFail                 = 3,    // 连接失败
	eSockErrCode_SendBufOverflow             = 4,    // 发送缓冲区溢出
	eSockErrCode_PostRecvFail                = 5,    // post异步接收操作失败
	eSockErrCode_PostSendFail                = 6,    // post异步发送失败
	eSockErrCode_ReactorErrorEvent           = 7,    // 反应器错误事件
	eSockErrCode_RecvError                   = 8,    // 接收数据错误
	eSockErrCode_Max                                 // 无
};



// Summary:
//		sock错误码信息结构体
struct SOCK_ERR_CODE
{
	e_SockErrCode   eSockErrCode;  // 自定义错误码
	LPUINT32         dwParam;       // 附加参数，方便定位
	LPUINT32         dwErrno;       // 系统错误码（注意是否区分GetLastError()和WSAGetLastError()）

	SOCK_ERR_CODE(e_SockErrCode _eSockErrCode, LPUINT32 _dwParam, LPUINT32 _dwErrno)
	{
		eSockErrCode = _eSockErrCode;
		dwParam      = _dwParam;
		dwErrno      = _dwErrno;
	}

	const char* LPAPI GetDescription();
};



// Summary:
//		底层socket对象封装接口类
class DECLARE ILPSocker
{
public:

	// Summary:
	//		无
	virtual ~ILPSocker(){}

	// Summary:
	//		获取底层sock的句柄
	// Return:
	//		返回底层sock句柄
	virtual SOCKET LPAPI GetSock() = 0;

	// Summary:
	//		获取sock id
	// Return:
	//		返回sock id
	virtual LPUINT32 LPAPI GetSockerId() = 0;

	// Summary:
	//		获取对端连接的ip
	virtual LPUINT32 LPAPI GetRemoteIp() = 0;

	// Summary:
	//		获取对端连接的ip字符串
	virtual const char* LPAPI GetRemoteIpStr() = 0;

	// Summary:
	//		获取对端连接的port
	virtual LPUINT16 LPAPI GetRemotePort() = 0;

	// Summary:
	//		获取本地连接的ip
	virtual LPUINT32 LPAPI GetLocalIp() = 0;

	// Summary:
	//		获取本地连接的ip字符串
	virtual const char* LPAPI GetLocalIpStr() = 0;

	// Summary:
	//		获取本地连接的port
	virtual LPUINT16 LPAPI GetLocalPort() = 0;

	// Summary:
	//		判断当前的socket是否处于连接状态
	// Return:
	//		TRUE-连接，FALSE-断开
	virtual BOOL LPAPI IsConnect() = 0;

	// Summary:
	//		判断是否被动关闭
	// Return:
	//		TRUE-是，FALSE-否
	virtual BOOL LPAPI IsPassiveClose() = 0;

	// Summary:
	//		发送数据
	// Input:
	//		pData：数据
	//		dwLen：长度
	virtual BOOL LPAPI Send(const char* pData, LPUINT32 dwLen) = 0;

	// Summary:
	//		关闭链接，不管是主动关闭还是被动关闭，都统一调用此接口关闭已经建立的socker
	virtual void LPAPI Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose) = 0;
};




//end声明所处的名字空间
NS_LZPL_END

#endif