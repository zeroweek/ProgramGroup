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
//		io类型枚举
enum e_IoType
{
	eIoType_None = 0,
	eIoType_CompletionPort = 1,
	eIoType_Epoll = 2,
	eIoType_Total
};



// tolua_begin
#pragma pack(push, 8)

// Summary:
//		
struct NET_CONFIG
{
	LPUINT32                       dwIoType;                    // io类型
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
class DECLARE ILPNet
{
public:

	// Summary:
	//		无
	virtual ~ILPNet(){}

	// Summary:
	//		创建ILPListener监听控制器对象
	//Input:
	//		pPacketParser：解析对象
	//Return:
	//		监听器对象
	virtual std::shared_ptr<ILPListener> LPAPI CreateListenerCtrl(ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		创建ILPConnector连接器对象
	//Input:
	//		pPacketParser：解析对象
	//Return:
	//		连接器对象
	virtual std::shared_ptr<ILPConnector> LPAPI CreateConnectorCtrl(ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		处理网络包函数
	//Input:
	//		nCount-每次检测处理网络包的最大数量，-1表示处理所有
	//Return:
	//		TRUE-所有网络包都已被处理，FALSE-有剩余未处理网络包
	virtual BOOL LPAPI Run(LPINT32 nCount = -1) = 0;

public:

	// Summary:
	//  网络初始化接口，使用网络模块之前必须调用
	static BOOL LPAPI GlobalInit();

	// Summary:
	//  网络反初始化接口，不再使用网络模块时必须调用
	static void LPAPI GlobalUnInit();

	// Summary:
	//  创建网络组件对象
	static std::shared_ptr<ILPNet> LPAPI CreateNetModule(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig);



	// Summary:
	//  删除网络组件对象
	static void LPAPI DeleteNetModule(std::shared_ptr<ILPNet>& poNet);
};



//end声明所处的名字空间
NS_LZPL_END

#endif