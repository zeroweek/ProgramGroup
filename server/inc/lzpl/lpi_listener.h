//****************************************************************************/
//    author: caihy
//    date: May 29, 2015
//    description: 监听器接口定义
//
//****************************************************************************/
#ifndef _LPI_LISTENER_H_
#define _LPI_LISTENER_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		监听器接口类
class DECLARE ILPListener
{
public:

	// Summary:
	//		无
	virtual ~ILPListener(){}

	// Summary:
	//		开始监听
	// Input:
	//		pcszIp: ip地址字符串（格式：192.168.1.1）
	//		dwPort: 监听端口
	//		bReUseAddr: 是否重复利用地址
	// Return:
	//		TRUE-成功，FALSE-失败
	virtual BOOL LPAPI Start(const char* pcszIp, UINT_32 dwPort, BOOL bReUseAddr) = 0;

	// Summary:
	//		停止监听
	virtual void LPAPI Stop() = 0;

	// Summary:
	//		获取id
	virtual UINT_32 LPAPI GetId() = 0;

	// Summary:
	//		释放
	virtual void LPAPI Release() = 0;
};





//end声明所处的名字空间
NS_LZPL_END

#endif