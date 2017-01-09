//****************************************************************************/
//    author: caihy
//    date: Jan 5, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_NET_EVENT_H_
#define _LP_NET_EVENT_H_

#include "lpi_net.h"
#include "lp_lock.h"
#include "lp_loopbuf.h"
#include "lpi_sockerimpl.h"
#include "lpi_connectorimpl.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		事件类型
enum e_EventType
{
	eEventType_None = 0,
	eEventType_Recv = 1,
	eEventType_Terminate = 2,
	eEventType_Establish = 3,
	eEventType_ConnectError = 4,
	eEventType_Max
};



// Summary;
//		接收事件结构体
struct DECLARE RECV_EVENT
{
	ILPSockerImpl*           pSocker;
	LPUINT32                 dwLen;
};



// Summary;
//		断开事件结构体
struct DECLARE TERMINATE_EVENT
{
	ILPSockerImpl*           pSocker;
	LPUINT32                 dwSockerId;
};



// Summary;
//		连接建立事件结构体
struct DECLARE ESTABLISH_EVENT
{
	ILPSockerImpl*           pSocker;
	BOOL                     bAccept;
};



// Summary;
//		连接错误事件结构体
struct DECLARE CONNECT_ERROR_EVENT
{
	ILPConnectorImpl*        pConnector;
	LPUINT32                 dwErrorNo;
};



// Summary;
//		事件结构体
struct DECLARE NET_EVENT
{
	e_EventType              eEventType;
	LPUINT32                  dwFlag;      // 本字段目前仅用于分散到不同事件列表，平衡压力
	union
	{
		RECV_EVENT             stRecvEvent;
		TERMINATE_EVENT        stTerminateEvent;
		ESTABLISH_EVENT        stEstablishEvent;
		CONNECT_ERROR_EVENT    stConnectErrorEvent;
	} stUn;

	static LPINT32 ms_dwNetEventCount;
	static NET_EVENT* LPAPI NewNetEvent()
	{
		++ms_dwNetEventCount;
		return new NET_EVENT();
	}

	static void LPAPI DeleteNetEvent(NET_EVENT* & pNetEvent)
	{
		if (nullptr != pNetEvent)
		{
			--ms_dwNetEventCount;
		}
		SAFE_DELETE(pNetEvent);
	}
};



//类声明
class LPNetImpl;



// Summary:
//		事件管理类
class DECLARE LPEventMgr
{
public:

	// Summary：
	//     构造函数
	// Returns:
	//     无
	LPEventMgr();

	// Summary：
	//     析构函数
	// Returns:
	//     无
	~LPEventMgr();

public:

	typedef std::list<NET_EVENT*> LPListEvent;

public:

	// Summary:
	//		初始化函数
	// Input:
	//		nEventListCount：
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPNetMessageHandler* pNetMessageHandler, LPINT32 nEventListCount);

	// Summary:
	//		清除函数
	BOOL LPAPI UnInit();

	// Summary:
	//		push一个接收事件
	BOOL LPAPI PushRecvEvent(ILPSockerImpl* pSocker, LPUINT32 dwSockerId, ILPLoopBuf* pLoopBuf, LPUINT32 dwLen);

	// Summary:
	//		push一个断开事件
	void LPAPI PushTerminateEvent(ILPSockerImpl* pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose);

	// Summary:
	//		push一个连接建立事件
	void LPAPI PushEstablishEvent(ILPSockerImpl* pSocker, BOOL bAccept);

	// Summary:
	//		push一个连接错误事件
	void LPAPI PushConnectErrorEvent(std::shared_ptr<ILPConnectorImpl> pConnector, LPUINT32 dwErrorNo);

	// Summary:
	//		判断是否有事件待处理
	BOOL HaveEventForHandled();

	// Summary:
	//		处理一个事件，该函数会由主线程调用，将相应的一些事件都集中在此，
	//			保证同一个线程处理
	void HandleOneEvent();

private:

	// Summary:
	//		处理接收事件
	void LPAPI _ProcRecvEvent(RECV_EVENT* pstRecvEvent);

	// Summary:
	//		处理断开事件
	void LPAPI _ProcTerminateEvent(TERMINATE_EVENT* pstTerminateEvent);

	// Summary:
	//		处理连接建立事件
	void LPAPI _ProcEstablishEvent(ESTABLISH_EVENT* pstEstablishEvent);

	// Summary:
	//		处理连接错误事件
	void LPAPI _ProcConnectErrorEvent(CONNECT_ERROR_EVENT* pstConnectErrorEvent);

private:

	BOOL                        m_bInit;
	LPLoopBuf*                  m_pRecvLoopBuf;        // 接收事件数据缓冲区
	char*                       m_pPacketTempBuf;      // 数据包临时缓冲区
	LPINT32                     m_nEventListCount;     // 事件列表个数
	LPListEvent*                m_pEventList;          // 事件列表
	LPLock*                     m_pEventListLock;      // 事件列表锁
	ILPNetMessageHandler*       m_pNetMessageHandler;  // 
	LPNetImpl*                  m_pNetImpl;            //
};



//end声明所处的名字空间
NS_LZPL_END

#endif