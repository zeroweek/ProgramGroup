//****************************************************************************/
//    author: caihy
//    date: October 17, 2014
//    description: 连接器接口类定义
//
//****************************************************************************/
#ifndef _LPI_CONNECTOR_H_
#define _LPI_CONNECTOR_H_

#include "lp_base.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		连接器接口类
class DECLARE ILPConnector
{
public:

	// Summary:
	//		无
	virtual ~ILPConnector(){}

	// Summary:
	//		开始连接
	// Input:
	//		pcszIp: ip地址字符串（格式：192.168.1.1）
	//		dwPort: 连接的端口
	//		bReconnect: 是否自动重连，如果没有设置自动重连，需要在断开回调手动调用重连接口
	virtual BOOL LPAPI Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReconnect) = 0;

	// Summary:
	//		停止连接
	// Return:
	//		TRUE-成功，FALSE-失败
	virtual void LPAPI Stop() = 0;

	// Summary:
	//		获取id
	virtual LPUINT32 LPAPI GetId() = 0;

	// Summary:
	//		重连
	virtual BOOL LPAPI Reconnect() = 0;

	// Summary:
	//		释放
	virtual void LPAPI Release() = 0;
};





//end声明所处的名字空间
NS_LZPL_END

#endif