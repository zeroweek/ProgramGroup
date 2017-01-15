//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_CONNECTOR_H_
#define _LP_CONNECTOR_H_

#include "lpi_connectorimpl.h"
#include "lpi_sockerimpl.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		连接器类
class DECLARE LPConnector : public ILPConnectorImpl
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
	virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);
	// Summary：
	//		无     
	virtual BOOL LPAPI UnInit();

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
	virtual void LPAPI OnClose();

	// Summary:
	//		设置是否自动重连
	virtual void LPAPI SetReconnect(BOOL bReconnect);

	// Summary:
	//		是否自动重连
	virtual BOOL LPAPI IsReconnect();

	// Summary:
	//		设置关联的socker对象
	virtual void LPAPI SetSocker(ILPSockerImpl* pSocker);

	// Summary:
	//		获取关联的socker对象
	virtual ILPSockerImpl* LPAPI GetSocker();

	// Summary:
	//		设置连接器当前状态
	virtual void LPAPI SetState(LPUINT32 dwState);

	//Summary:
	//		获取连接器当前状态
	virtual LPUINT32 LPAPI GetState();

protected:

	// Summary:
	//		获取ConnectEx函数指针，并且调用异步connect操作
	virtual BOOL LPAPI _InitConnectEx();

	// Summary:
	//		post异步connect操作
	// Input:
	//		pstPerIoData：与链接建立的sock绑定的io数据
	// Return:
	//		TRUE-成功，FALSE-失败
	virtual BOOL LPAPI _PostConnectEx(PER_IO_DATA* pstPerIoData);

protected:

	LPUINT32                    m_dwId;
	volatile atomic_uint        m_dwState;

	std::string                 m_strIP;
	LPUINT32                    m_dwPort;
	BOOL                        m_bReconnect;
	SOCKET                      m_hConnectSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;
	ILPSockerImpl*              m_pSocker;

	PER_IO_DATA*                m_pstPerIoData;
};



// Summary:
//		windows网络通讯方式的connector实现类
class DECLARE LPWinNetConnector : public LPConnector
{
public:

	// Summary:
	//		无
	LPWinNetConnector();

	// Summary:
	//		无
	virtual ~LPWinNetConnector();

	// Summary:
	//		网络事件处理
	virtual void LPAPI OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData);

protected:

	// Summary:
	//		获取ConnectEx函数指针，并且调用异步connect操作
	virtual BOOL LPAPI _InitConnectEx();

	// Summary:
	//		post异步connect操作
	// Input:
	//		pstPerIoData：与链接建立的sock绑定的io数据
	// Return:
	//		TRUE-成功，FALSE-失败
	virtual BOOL LPAPI _PostConnectEx(PER_IO_DATA* pstPerIoData);

protected:
	LPFN_CONNECTEX              m_lpfnConnectEx;
};



//end声明所处的名字空间
NS_LZPL_END

#endif