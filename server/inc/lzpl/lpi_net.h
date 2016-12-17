//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 网络组件接口定义
//
//****************************************************************************/
#ifndef _LPI_NET_H_
#define _LPI_NET_H_

#include "lpi_listener.h"
#include "lpi_connector.h"
#include "lpi_logger.h"
#include "lpi_packetparser.h"
#include "lpi_netmessagehandler.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		net模块名称定义
const char LPNET_MODULENAME[] = "LPNet";



// Summary:
//		net模块版本号定义
const MODULE_VERSION LPNET_MODULEVERSION = { 0, 1, 0, 1 };



// Summary:
//		io类型枚举
enum e_IoType
{
	eIoType_None = 0,
	eIoType_CompletionPort = 1,
};



// tolua_begin
#pragma pack(push, 8)

// Summary:
//		
struct NET_CONFIG
{
	LPUINT32                       dwRecvBufSize;               // 接收缓冲区的大小
	LPUINT32                       dwSendBufSize;               // 发送缓冲区的大小
	LPUINT32                       dwConnectCount;              // 允许连接的数量
	LPUINT32                       dwNetEventPoolSize;          // 网络事件池大小
	LPUINT32                       dwNetEventListCount;         // 网络事件处理列表个数
	LPUINT32                       dwNetRecvEventBufSize;       // 网络接收事件缓冲区大小

};

#pragma pack(pop)
// tolua_end



// Summary:
//		网络组件接口类
class DECLARE ILPNet : public ILPBase
{
public:

	// Summary:
	//		无
	virtual ~ILPNet(){}

	// Summary:
	//		创建ILPListener监听控制器对象
	//Input:
	//		eIoType：网络IO类型
	//		pPacketParser：解析对象
	//Return:
	//		监听器对象
	virtual ILPListener* LPAPI CreateListenerCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		创建ILPConnector连接器对象
	//Input:
	//		eIoType：网络IO类型
	//		pPacketParser：解析对象
	//Return:
	//		连接器对象
	virtual ILPConnector* LPAPI CreateConnectorCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		处理网络包函数
	//Input:
	//		nCount-每次检测处理网络包的最大数量，-1表示处理所有
	//Return:
	//		TRUE-所有网络包都已被处理，FALSE-有剩余未处理网络包
	virtual BOOL LPAPI Run(LPINT32 nCount = -1) = 0;

};



// Summary:
//		获取网络组件对象
// Return:
//		成功返回组件对象，失败返回NULL
DECLARE ILPNet* LPAPI lpCreateNetModule(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig);








//end声明所处的名字空间
NS_LZPL_END

#endif