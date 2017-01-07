//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_SOCKER_H_
#define _LP_SOCKER_H_

#include "lpi_socker.h"
#include "lp_objpool.h"
#include "lpi_eventhandler.h"
#include "lp_lock.h"
#include "lp_thread.h"
#include "lpi_packetparser.h"
#include "lp_loopbuf.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;



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
	//		获取网络事件处理器类型
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		网络事件处理
	virtual void LPAPI OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData);

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
	virtual BOOL LPAPI PostRecv();

	// Summary:
	//		post异步发送数据操作
	// Return:
	//		TRUE-有发送数据，FALSE-没发送数据
	BOOL LPAPI PostSend();

	// Summary:
	//		收到消息回调
	// Input:
	//		dwBytes：接收到的字节数
	void LPAPI OnRecv(LPUINT32 dwBytes);

	// Summary:
	//		发送消息回调
	// Input:
	//		dwBytes：接收到的字节数
	void LPAPI OnSend(LPUINT32 dwBytes);

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
	virtual void LPAPI OnClose();

	// Summary:
	//		设置父级对象的id（连接器或监听器的id）
	// Input:
	//		dwParentId：id
	void LPAPI SetParentId(LPUINT32 dwParentId);

	// Summary:
	//		获取父级对象的id（连接器或监听器的id）
	// Return:
	//		id
	virtual LPUINT32 LPAPI GetParentId();

	// Summary:
	//		设置是否是accept创建的标记，accept还是connect创建
	void LPAPI SetAcceptCreate(BOOL bAcceptCreate);

	// Summary:
	//		判断是否是accept创建
	//	Return:
	//		TRUE-是，FALSE-不是
	virtual BOOL LPAPI IsAcceptCreate();

	// Summary:
	//		设置是否被动关闭
	virtual void LPAPI SetPassiveClose(BOOL bPassiveClose);

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

	LPUINT32                   m_dwMaxSockId;        // 最大sock id
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



//end声明所处的名字空间
NS_LZPL_END

#endif