//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_IOCP_H_
#define _LP_IOCP_H_

#ifdef _WIN32


#include "lp_base.h"
#include "lpi_sockoption.h"
#include "lp_objpool.h"
#include "lpi_socker.h"
#include "lpi_eventhandler.h"
#include "lp_loopbuf.h"
#include "lp_lock.h"
#include "lpi_packetparser.h"
#include "lp_system.h"
#include "lp_lock.h"
#include "lp_thread.h"
#include "lpi_packetparser.h"
#include "lpi_connector.h"
#include "lpi_eventhandler.h"
#include <MSWSock.h>
#include "lpi_listener.h"
#include "lpi_connector.h"
#include "lpi_netmessagehandler.h"
#include "lpi_reactor.h"
#include "lpi_net.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN


#define POST_ACCEPTEX_COUNT            (128)



class LPSockerMgr;
class LPReactorImpl;
class LPNetImpl;


enum eCommonState
{
	eCommonState_NoInit,
	eCommonState_Initing,
	eCommonState_Inited,
	eCommonState_Close,
	eCommonState_UnIniting,
	eCommonState_UnInited,
};


// Summary:
//		每个io对应的绑定数据
struct PER_IO_DATA
{
	OVERLAPPED             stOverlapped;
	SOCKET                 hSock;
	e_IoOptType            eIoOptType;
	e_EventHandlerType     eHandlerType;
	WSABUF                 stWsaBuf;
	char                   szBuf[128];

	PER_IO_DATA()
	{
		memset(&stOverlapped, 0, sizeof(stOverlapped));
		hSock = INVALID_SOCKET;
		eIoOptType = eIoOptType_None;
		eHandlerType = eEventHandlerType_None;
		memset(&stWsaBuf, 0, sizeof(stWsaBuf));
		memset(szBuf, 0, sizeof(szBuf));
	}
};



//****************************************************************************/
//	网络选项接口类
//****************************************************************************/
// Summary:
//		网络选项类
class DECLARE LPNetOption : public ILPNetOption
{
public:

	// Summary:
	//		none
	LPNetOption();

	// Summary:
	//		none
	virtual ~LPNetOption();

};



// Summary:
//		底层socke对象t封装类
class DECLARE LPSocker : public ILPSocker, public ILPEventHandler
{
public:

	// Summary:
	//		无
	LPSocker();

	// Summary:
	//		无
	virtual ~LPSocker();

	// Summary:
	//		获取底层sock的句柄
	// Return:
	//		返回底层sock句柄
	virtual SOCKET LPAPI GetSock();

	// Summary:
	//		获取socker id
	// Return:
	//		返回socker id
	virtual LPUINT32 LPAPI GetSockerId();

	// Summary:
	//		获取对端连接的ip
	virtual LPUINT32 LPAPI GetRemoteIp();

	// Summary:
	//		获取对端连接的ip字符串
	virtual const char* LPAPI GetRemoteIpStr();

	// Summary:
	//		获取对端连接的port
	virtual LPUINT16 LPAPI GetRemotePort();

	// Summary:
	//		获取本地连接的ip
	virtual LPUINT32 LPAPI GetLocalIp();

	// Summary:
	//		获取本地连接的ip字符串
	virtual const char* LPAPI GetLocalIpStr();

	// Summary:
	//		获取本地连接的port
	virtual LPUINT16 LPAPI GetLocalPort();

	// Summary:
	//		判断当前的socket是否处于连接状态
	// Return:
	//		TRUE-连接，FALSE-断开
	virtual BOOL LPAPI IsConnect();

	// Summary:
	//		发送数据
	// Input:
	//		pData：数据
	//		dwLen：长度
	virtual BOOL LPAPI Send(const char* pData, LPUINT32 dwLen);

	// Summary:
	//		关闭链接，不管是主动关闭还是被动关闭，都统一调用此接口关闭已经建立的socker
	virtual void LPAPI Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose);

	// Summary:
	//		获取句柄
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		获取事件处理器类型
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		设置sock句柄
	// Input:
	//		sock：底层socket句柄值
	void LPAPI SetSock(SOCKET sock);

	// Summary:
	//		设置连接状态标记值
	// Input:
	//		bConnect：true-连接，false-断开
	void LPAPI SetConnect(bool bConnect);

	// Summary:
	//		绑定解析对象
	// Input:
	//		pPacketParser：解析对象
	void LPAPI AttachPacketParser(ILPPacketParser* pPacketParser);

	// Summary:
	//		解除绑定解析对象
	void LPAPI DetachPacketParser();

	// Summary:
	//		重置socker对象
	void LPAPI Reset();

	// Summary:
	//		post异步接收数据操作
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI PostRecv();

	// Summary:
	//		post异步发送数据操作
	// Return:
	//		TRUE-有发送数据，FALSE-没发送数据
	BOOL LPAPI PostSend();

	// Summary:
	//		收到消息回调
	// Input:
	//		dwBytes：接收到的字节数
	void LPAPI OnRecv(DWORD dwBytes);

	// Summary:
	//		发送消息回调
	// Input:
	//		dwBytes：接收到的字节数
	void LPAPI OnSend(DWORD dwBytes);

	// Summary:
	//		设置接收缓冲区
	// Input:
	//		pBuf：缓冲区
	void LPAPI AttachRecvBuf(LPLoopBuf* pLoopBuf);

	// Summary:
	//		分离接收缓冲区
	// Return:
	//		接收缓冲区
	LPLoopBuf* LPAPI DetachRecvBuf();

	// Summary:
	//		设置发送缓冲区
	// Input:
	//		pBuf：缓冲区
	void LPAPI AttachSendBuf(LPLoopBuf* pLoopBuf);

	// Summary:
	//		分离发送缓冲区
	// Return:
	//		发送缓冲区
	LPLoopBuf* LPAPI DetachSendBuf();

	// Summary:
	//		设置远端连接ip
	// Input:
	//		dwIp：ip
	void LPAPI SetRemoteIp(LPUINT32 dwIp);

	// Summary:
	//		设置远端连接端口
	// Input:
	//		wPort：端口
	void LPAPI SetRemotePort(LPUINT16 wPort);

	// Summary:
	//		设置本地连接ip
	// Input:
	//		dwIp：ip
	void LPAPI SetLocalIp(LPUINT32 dwIp);

	// Summary:
	//		设置本地连接端口
	// Input:
	//		wPort：端口
	void LPAPI SetLocalPort(LPUINT16 wPort);

	// Summary:
	//		设置开始延迟关闭tick
	void LPAPI SetDelayCloseBeginTick(LPUINT64 qwTick);

	// Summary:
	//		获取开始延迟关闭tick
	LPUINT64 LPAPI GetDelayCloseBeginTick();

	// Summary:
	//		设置延迟关闭持续时间
	void LPAPI SetDelayCloseDuration(LPUINT64 qwDuration);

	// Summary:
	//		获取延迟关闭持续时间
	LPUINT64 LPAPI GetDelayCloseDuration();

	// Summary:
	//		设置开始延迟释放tick
	void LPAPI SetDelayReleaseBeginTick(LPUINT64 qwTick);

	// Summary:
	//		获取开始延迟释放tick
	LPUINT64 LPAPI GetDelayReleaseBeginTick();

	// Summary:
	//		设置延迟释放持续时间
	void LPAPI SetDelayReleaseDuration(LPUINT64 qwDuration);

	// Summary:
	//		获取延迟释放持续时间
	LPUINT64 LPAPI GetDelayReleaseDuration();

	// Summary:
	//		设置socker id
	// Input:
	//		dwSockerId：socker id
	void LPAPI SetSockerId(LPUINT32 dwSockerId);

	// Summary:
	//		异步关闭回调
	void LPAPI OnClose();

	// Summary:
	//		设置父级对象的id（连接器或监听器的id）
	// Input:
	//		dwParentId：id
	void LPAPI SetParentId(LPUINT32 dwParentId);

	// Summary:
	//		获取父级对象的id（连接器或监听器的id）
	// Return:
	//		id
	LPUINT32 LPAPI GetParentId();

	// Summary:
	//		设置是否是accept创建的标记，accept还是connect创建
	void LPAPI SetAcceptCreate(BOOL bAcceptCreate);

	// Summary:
	//		判断是否是accept创建
	//	Return:
	//		TRUE-是，FALSE-不是
	BOOL LPAPI IsAcceptCreate();

	// Summary:
	//		设置是否被动关闭
	void LPAPI SetPassiveClose(BOOL bPassiveClose);

	// Summary:
	//		判断是否被动关闭
	//	Return:
	//		TRUE-是，FALSE-不是
	virtual BOOL LPAPI IsPassiveClose();

	// Summary:
	//		无
	void LPAPI SetNetImpl(LPNetImpl* pNetImpl);

protected:

	volatile atomic_bool      m_bConnect;                 // 连接状态标记
	volatile atomic_bool      m_bSending;                 // 数据是否正在发送标记

	BOOL                      m_bAcceptCreate;            // 是否是accept创建
	BOOL                      m_bPassiveClose;            // 是否被动关闭
	SOCKET                    m_hSock;                    // sock句柄
	LPUINT32                   m_dwSockerId;               // socker id
	LPUINT32                   m_dwParentId;               // 父级对象的id（连接器或监听器的id）
	ILPPacketParser*          m_pPacketParser;            // 解析对象
	LPNetImpl*                m_pNetImpl;                 //

	LPUINT32                   m_dwRemoteIp;               // 远端ip
	LPUINT16                   m_wRemotePort;              // 远端端口
	char                      m_szRemoteIpStr[IP_LEN];    // 远端ip字符串
	LPUINT32                   m_dwLocalIp;                // 本地ip
	LPUINT16                   m_wLocalPort;               // 本地端口
	char                      m_szLocalIpStr[IP_LEN];     // 本地ip字符串

	LPLoopBuf*                m_pRecvLoopBuf;             // 接收缓冲区（只有PostRecv和OnRecv有操作，无需写锁，因为没有PostRecv是不会收到OnRecv）
	LPLoopBuf*                m_pSendLoopBuf;             // 发送缓冲区（单线程写不用锁，读需要锁）

	LPUINT64                   m_qwDelayCloseBeginTick;    // 延迟关闭开始tick
	LPUINT64                   m_qwDelayCloseDuration;     // 延迟关闭持续时间
	LPUINT64                   m_qwDelayReleaseBeginTick;  // 延迟释放开始tick
	LPUINT64                   m_qwDelayReleaseDuration;   // 延迟释放持续时间

	PER_IO_DATA               m_stRecvPerIoData;          // io接收绑定的数据 
	PER_IO_DATA               m_stSendPerIoData;          // io发送绑定的数据

	static LPLock             m_oSendBufLock;             // 发送缓冲区锁（静态还是非静态？？？考虑锁消耗与sock效率平衡）
};



// Summary:
//		socker对象管理类
class DECLARE LPSockerMgr
{
public:

	// Summary：
	//     构造函数
	// Returns:
	//     无
	LPSockerMgr();

	// Summary：
	//     析构函数
	// Returns:
	//     无
	~LPSockerMgr();

public:

	typedef std::list<LPSocker*> LPListSocker;
	typedef std::map<LPUINT32, LPSocker*> LPMapSocker;

public:

	// Summary:
	//		初始化函数
	// Input:
	//		pNetImpl: 
	//		dwSize：socker对象池初始化大小
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI Init(LPNetImpl* pNetImpl, LPUINT32 dwSize);

	// Summary:
	//		清除函数
	BOOL LPAPI UnInit();

	// Summary:
	//		创建socker对象
	// Input:
	//		pPacketParser：解析对象
	//		dwParentId：创建对象（连接器或控制器）的id
	//		bAcceptCreate：是否accept创建
	// Return:
	//		socker对象
	LPSocker* LPAPI Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		释放socker对象
	// Input:
	//		pSocker：socker对象
	void LPAPI Release(LPSocker* pSocker);

	// Summary:
	//		延迟关闭socker对象
	// Input:
	//		pSocker：socker对象
	void LPAPI DelayClose(LPSocker* pSocker);

	// Summary:
	//		延迟释放socker对象，注意与Release调用位置的区分
	// Input:
	//		pSocker：socker对象
	void LPAPI DelayRelease(LPSocker* pSocker);

	// Summary:
	//		检测延迟释放的对象
	void LPAPI CheckDelay();

	// Summary:
	//		发送数据
	// Return:
	//		有数据发送的LPSocker对象个数
	LPINT32 LPAPI PostSend();

	// Summary:
	//		查找socker
	LPSocker* LPAPI Find(LPUINT32 dwSockerId);

	// Summary:
	//		获取当前有效的连接个数

	LPUINT32 LPAPI GetCurValidConnectCount();

private:

	// Summary:
	//		创建socker对象
	// Input:
	//		pPacketParser：解析对象
	//		dwParentId：创建对象（连接器或控制器）的id
	//		bAcceptCreate：是否accept创建
	// Return:
	//		socker对象
	LPSocker* LPAPI _Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		释放socker对象
	// Input:
	//		pSocker：socker对象
	void LPAPI _Release(LPSocker* pSocker);

	// Summary:
	//		创建sock id
	// Return:
	//		sock id
	LPUINT32 LPAPI _CreateSockId();

	// Summary:
	//		Malloc
	LPSocker* LPAPI _MallocSocker();

	// Summary:
	//		Free
	void LPAPI _FreeSocker(LPSocker* pSocker);

private:

	static THREAD_FUNC_DECLARE(_CheckDelayThreadProc)(void* pParam);
	static THREAD_FUNC_DECLARE(_PostSendThreadProc)(void* pParam);

private:

	LPUINT32                    m_dwMaxSockId;        // 最大sock id
	LPObjPool<LPSocker>        m_oSockerPool;        // socker对象池
	LPListSocker               m_oValidList;         // 当前连接有效的socker列表
	LPMapSocker                m_oValidMap;          // 当前连接有效的socker map
	LPListSocker               m_oDelayCloseList;    // 延迟关闭连接列表
	LPListSocker               m_oDelayReleaseList;  // 延迟释放的socker列表
	LPLock                     m_oLock;              // 锁
	LPLock                     m_oDelayLock;         // 延迟释放列表锁
	BOOL                       m_bCheckDelayRun;     // check delay线程是否停止
	BOOL                       m_bPostSendRun;       // post send线程是否停止
	LPThread                   m_oCheckDelayThread;  // check delay线程
	LPThread                   m_oPostSendThread;    // post send线程

	LPNetImpl*                 m_pNetImpl;           // 
	LPLoopBufPool*             m_pRecvLoopBufPool;   // 接受缓冲区池（若要支持多个size，可使用map pool）
	LPLoopBufPool*             m_pSendLoopBufPool;   // 发送缓冲区池（若要支持多个size，可使用map pool）

	time_t                     m_tLastCheckTime;
};



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
	//		pcszIp: ip地址字符串（格式：192.168.1.1）
	//		dwPort: 连接的端口
	//		bAutoReconnect: 是否自动重连接
	// Return:
	//		TRUE-成功，FALSE-失败
	virtual BOOL LPAPI Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReconnect);

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
	//		释放
	virtual void LPAPI Release();

	// Summary:
	//		获取句柄
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		获取事件处理器类型
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

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

	char                        m_szIp[IP_LEN];
	LPUINT32                     m_dwPort;
	BOOL                        m_bReconnect;
	SOCKET                      m_hConnectSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;
	LPSocker*                   m_pSocker;

	LPFN_CONNECTEX              m_lpfnConnectEx;
	PER_IO_DATA*                m_pstPerIoData;
};

typedef std::map<LPUINT32, LPConnector*> MAP_CONNECTOR;



// Summary:
//		监听器类
class DECLARE LPListener : public ILPListener, public ILPEventHandler
{
public:

	// Summary:
	//		无
	LPListener();

	// Summary:
	//		无
	virtual ~LPListener();

	// Summary：
	//		无
	// Input:
	//		pNetImpl: 
	//		pPacketParser: 消息包解析对象
	//		dwId: 监听器id
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);
	// Summary：
	//		无     
	BOOL LPAPI UnInit();

	// Summary:
	//		开始监听
	// Input:
	//		pcszIp: ip地址字符串（格式：192.168.1.1）
	//		dwPort: 监听端口
	//		bReUseAddr: 是否重复利用地址
	// Return:
	//		TRUE-成功，FALSE-失败
	virtual BOOL LPAPI Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReUseAddr);

	// Summary:
	//		停止监听
	virtual void LPAPI Stop();

	// Summary:
	//		获取id
	virtual LPUINT32 LPAPI GetId();

	// Summary:
	//		释放
	virtual void LPAPI Release();

	// Summary:
	//		获取句柄
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		获取事件处理器类型
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		接受链接回调
	void LPAPI OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

protected:

	//Summary:
	//		获取监听器状态
	LPUINT32 LPAPI _GetState();

	// Summary:
	//		设置监听器状态
	void LPAPI _SetState(LPUINT32 dwState);

	// Summary:
	//		获取AcceptEx和GetAcceptExSockaddrs函数指针，并且post异步accept操作
	BOOL LPAPI _InitAcceptEx();

	// Summary:
	//		post异步accept操作
	// Input:
	//		pstPerIoData：与接受上来的sock绑定的io数据
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI _PostAcceptEx(PER_IO_DATA* pstPerIoData);

private:

	LPUINT32                     m_dwId;
	volatile atomic_uint        m_dwState;

	char                        m_szIp[IP_LEN];
	LPUINT32                     m_dwPort;
	BOOL                        m_bReUseAddr;
	SOCKET                      m_hListenSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;

	LPFN_ACCEPTEX               m_lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
	PER_IO_DATA*                m_pstPerIoDataArray;

};

typedef std::map<LPUINT32, LPListener*> MAP_LISTENER;



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
struct RECV_EVENT
{
	LPSocker*                pSocker;
	LPUINT32                  dwLen;
};



// Summary;
//		断开事件结构体
struct TERMINATE_EVENT
{
	LPSocker*                pSocker;
	LPUINT32                  dwSockerId;
};



// Summary;
//		连接建立事件结构体
struct ESTABLISH_EVENT
{
	LPSocker*                pSocker;
	BOOL                     bAccept;
};



// Summary;
//		连接错误事件结构体
struct CONNECT_ERROR_EVENT
{
	LPConnector*             pConnector;
	LPUINT32                  dwErrorNo;
};



// Summary;
//		事件结构体
struct NET_EVENT
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
};



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
	//		dwSize：批量创建的事件数量
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPNetMessageHandler* pNetMessageHandler, LPUINT32 dwSize, LPINT32 nEventListCount);

	// Summary:
	//		清除函数
	BOOL LPAPI UnInit();

	// Summary:
	//		push一个接收事件
	BOOL LPAPI PushRecvEvent(LPSocker* pSocker, LPUINT32 dwSockerId, ILPLoopBuf* pLoopBuf, LPUINT32 dwLen);

	// Summary:
	//		push一个断开事件
	void LPAPI PushTerminateEvent(LPSocker* pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose);

	// Summary:
	//		push一个连接建立事件
	void LPAPI PushEstablishEvent(LPSocker* pSocker, BOOL bAccept);

	// Summary:
	//		push一个连接错误事件
	void LPAPI PushConnectErrorEvent(LPConnector* pConnector, LPUINT32 dwErrorNo);

	// Summary:
	//		判断是否有事件待处理
	BOOL HaveEventForHandled();

	// Summary:
	//		处理一个事件，该函数会由主线程调用，将相应的一些事件都集中在此，
	//			保证同一个线程处理
	void HandleOneEvent();

private:

	// Summary:
	//		创建事件
	NET_EVENT* LPAPI _MallocEvent();

	// Summary:
	//		释放事件
	void LPAPI _FreeEvent(NET_EVENT* pstEvent);

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
	LPObjPool<NET_EVENT>         m_oEventPool;          // 事件对象池
	LPLoopBuf*                  m_pRecvLoopBuf;        // 接收事件数据缓冲区
	char*                       m_pPacketTempBuf;      // 数据包临时缓冲区
	LPINT32                      m_nEventListCount;     // 事件列表个数
	LPListEvent*                m_pEventList;          // 事件列表
	LPLock*                     m_pEventListLock;      // 事件列表锁
	ILPNetMessageHandler*       m_pNetMessageHandler;  // 
	LPNetImpl*                  m_pNetImpl;            //
};



// Summary:
//		sql工作线程参数
class LPReactorImpl;
struct REACTOR_THREAD_PARAM
{
	LPReactorImpl   *pReactorImpl;
	LPINT32           nCompletionPortIndex;
};



// Summary：
//     windows平台反应器。
class DECLARE LPReactorImpl : public ILPReactor
{
public:

	// Summary：
	//     构造函数
	// Returns:
	//     无
	LPReactorImpl();

	// Summary：
	//     析构函数
	// Returns:
	//     无
	~LPReactorImpl();

public:

	// Summary:
	//     注册ILPEventHandler
	// Returns:
	//     TRUE-成功，FALSE-失败
	virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler);

	// Summary：
	//     注销ILPEventHandler
	// Returns:
	//     TRUE-成功，FALSE-失败
	virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler);

public:

	// Summary：
	//     完成端口线程处理函数
	static unsigned LPAPI ThreadFunc(LPVOID pParam);

	// Summary：
	//		无
	// Input:
	//		pNetImpl: 
	//		bOneCompletionPortOneThread: 是否单个完成端口模式
	//			TRUE-一个完成端口对应多个线程，FALSE-多个完成端口，每个完成端口对应一个线程
	BOOL LPAPI Init(LPNetImpl* pNetImpl, BOOL bOneCompletionPortOneThread);
	// Summary：
	//		无     
	BOOL LPAPI UnInit();

	// Summary：
	//     线程处理函数的主逻辑
	void LPAPI OnExecute(LPINT32 nCompletionPortIndex);

private:

	// Summary：
	//		获取当前状态
	LPUINT32 LPAPI _GetState();

	// Summary：
	//		设置当前状态
	void LPAPI _SetState(LPUINT32 dwState);

	void LPAPI _OnConnect(BOOL bOperateRet, ILPEventHandler* pEventHandler, PER_IO_DATA* pstPerIoData);
	void LPAPI _OnAccept(BOOL bOperateRet, ILPEventHandler* pEventHandler, PER_IO_DATA* pstPerIoData);
	void LPAPI _OnSendRecv(BOOL bOperateRet, ILPEventHandler* pEventHandler, PER_IO_DATA* pstPerIoData, LPUINT32 dwByteTransferred);


protected:

	BOOL	               m_bInit;
	volatile atomic_uint   m_dwState;

	LPINT32                 m_nCompletionPortCount;
	HANDLE*                m_pCompletionPort;
	LPINT32                 m_nWorkerCountPerCompIo;
	HANDLE**               m_ppWorkerArray;
	LPNetImpl*             m_pNetImpl;
};




//Summary:
//		windows平台的net组件实现类
class DECLARE LPNetImpl : public ILPNet
{
public:

	// Summary：
	//     无
	LPNetImpl();

	// Summary：
	//     无
	~LPNetImpl();

public:

	// Summary:
	//		引用计数加1
	virtual void LPAPI AddRef(void);

	// Summary:
	//		引用计数减1
	virtual LPUINT32 LPAPI QueryRef(void);

	// Summary:
	//		释放对象
	virtual void LPAPI Release(void);

	// Summary:
	//		获取版本信息
	virtual MODULE_VERSION LPAPI GetVersion(void);

	// Summary:
	//		获取组件名称，每个组件都将重载此函数，已提供该组件的名称
	virtual const char* LPAPI GetModuleName(void);

	// Summary:
	//		创建ILPListener监听控制器对象
	//Input:
	//		eIoType：网络IO类型
	//		pPacketParser：解析对象
	//Return:
	//		监听器对象
	virtual ILPListener* LPAPI CreateListenerCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser);

	// Summary:
	//		创建ILPConnector连接器对象
	//Input
	//		eIoType：网络IO类型
	//		pPacketParser：解析对象
	//Return:
	//		连接器对象
	virtual ILPConnector* LPAPI CreateConnectorCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser);

	// Summary:
	//		处理网络包函数
	//Input:
	//		nCount-每次检测处理网络包的最大数量，-1表示处理所有
	//Return:
	//		TRUE-所有网络包都已被处理，FALSE-有剩余未处理网络包
	virtual BOOL LPAPI Run(LPINT32 nCount = -1);

	// Summary:
	//		初始化函数
	//Return:
	//		成功返回TRUE，失败返回FALSE
	BOOL LPAPI Init(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig);

	// Summary:
	//		反初始化函数
	//Return:
	//		成功返回TRUE，失败返回FALSE
	void LPAPI LPNetImpl::UnInit(void);

	// Summary:
	//		查找监听器对象
	LPListener* LPAPI FindListener(LPUINT32 dwId);

	// Summary:
	//		查找连接器对象
	LPConnector* LPAPI FindConnector(LPUINT32 dwId);

	// Summary:
	//		无
	LPSockerMgr& LPAPI GetSockerMgr(void);

	// Summary:
	//		无
	LPEventMgr& LPAPI GetEventMgr(void);

	// Summary:
	//		无
	LPReactorImpl& LPAPI GetReactorImpl(void);

	// Summary:
	//		无
	NET_CONFIG&  LPAPI GetNetConfig(void);

protected:

	// Summary:
	//		创建id
	LPUINT32 LPAPI _CreateId();

protected:

	LPUINT32                  m_dwRef;
	LPUINT32                  m_dwMaxCreateId;
	LPLock                   m_oLock;
	ILPNetMessageHandler*    m_pNetMessageHandler;
	NET_CONFIG               m_oNetConfig;
	MODULE_VERSION        m_stModuleVersion;

	LPSockerMgr              m_oSockerMgr;
	LPEventMgr               m_oEventMgr;
	LPReactorImpl            m_oReactorImpl;

	MAP_LISTENER             m_mapListener;
	MAP_CONNECTOR            m_mapConnector;
};
















//end声明所处的名字空间
NS_LZPL_END

#endif /* END OF _WIN32*/

#endif