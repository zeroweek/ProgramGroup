//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_CONNECTOR_H_
#define _LP_CONNECTOR_H_

#include "lpi_connector.h"
#include "lpi_eventhandler.h"
#include "lpi_packetparser.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;
class LPSocker;



// Summary:
//		连接器类
class DECLARE LPConnector : public ILPConnector, public ILPEventHandler
{
public:

	// Summary:
	//		无
	LPConnector();

	// Summary:
	//		无
	virtual ~LPConnector();

	// Summary：
	//		无
	// Input:
	//		pNetImpl: 
	//		pPacketParser: 消息包解析对象
	//		dwId: 连接器id
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);
	// Summary：
	//		无     
	BOOL LPAPI UnInit();

	// Summary:
	//		开始连接
	// Input:
	//		strIP: ip地址字符串（格式：192.168.1.1）
	//		dwPort: 连接的端口
	//		bAutoReconnect: 是否自动重连接
	// Return:
	//		TRUE-成功，FALSE-失败
	virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect);

	// Summary:
	//		停止连接
	virtual void LPAPI Stop();

	// Summary:
	//		获取id
	virtual LPUINT32 LPAPI GetId();

	// Summary:
	//		重连
	virtual BOOL LPAPI Reconnect();

	// Summary:
	//		获取句柄
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		获取网络事件处理器类型
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		网络事件处理
	virtual void LPAPI OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData);

	// Summary:
	//		链接返回结果回调
	void LPAPI OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

	// Summary:
	//		链接关闭回调
	void LPAPI OnClose();

	// Summary:
	//		设置是否自动重连
	void LPAPI SetReconnect(BOOL bReconnect);

	// Summary:
	//		是否自动重连
	BOOL LPAPI IsReconnect();

	// Summary:
	//		设置关联的socker对象
	void LPAPI SetSocker(LPSocker* pSocker);

	// Summary:
	//		获取关联的socker对象
	LPSocker* LPAPI GetSocker();

	// Summary:
	//		设置连接器当前状态
	void LPAPI SetState(LPUINT32 dwState);

	//Summary:
	//		获取连接器当前状态
	LPUINT32 LPAPI GetState();

protected:

	// Summary:
	//		获取ConnectEx函数指针，并且调用异步connect操作
	BOOL LPAPI _InitConnectEx();

	// Summary:
	//		post异步connect操作
	// Input:
	//		pstPerIoData：与链接建立的sock绑定的io数据
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI _PostConnectEx(PER_IO_DATA* pstPerIoData);

private:

	LPUINT32                     m_dwId;
	volatile atomic_uint        m_dwState;

	std::string                 m_strIP;
	LPUINT32                    m_dwPort;
	BOOL                        m_bReconnect;
	SOCKET                      m_hConnectSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;
	LPSocker*                   m_pSocker;

	LPFN_CONNECTEX              m_lpfnConnectEx;
	PER_IO_DATA*                m_pstPerIoData;
};

typedef std::map<LPUINT32, std::shared_ptr<LPConnector>> MAP_CONNECTOR;



//end声明所处的名字空间
NS_LZPL_END

#endif