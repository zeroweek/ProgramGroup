//****************************************************************************/
//    author: caihy
//    date: Jan 9, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_SOCKER_MGR_H_
#define _LP_SOCKER_MGR_H_

#include "lpi_sockerimpl.h"
#include "lp_objpool.h"
#include "lp_lock.h"
#include "lp_thread.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;



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

	typedef std::list<ILPSockerImpl*> LPListSocker;
	typedef std::map<LPUINT32, ILPSockerImpl*> LPMapSocker;

public:

	// Summary:
	//		初始化函数
	// Input:
	//		pNetImpl: 
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI Init(LPNetImpl* pNetImpl);

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
	ILPSockerImpl* LPAPI Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		释放socker对象
	// Input:
	//		pSocker：socker对象
	void LPAPI Release(ILPSockerImpl* pSocker);

	// Summary:
	//		延迟关闭socker对象
	// Input:
	//		pSocker：socker对象
	void LPAPI DelayClose(ILPSockerImpl* pSocker);

	// Summary:
	//		延迟释放socker对象，注意与Release调用位置的区分
	// Input:
	//		pSocker：socker对象
	void LPAPI DelayRelease(ILPSockerImpl* pSocker);

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
	ILPSockerImpl* LPAPI Find(LPUINT32 dwSockerId);

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
	ILPSockerImpl* LPAPI _Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		释放socker对象
	// Input:
	//		pSocker：socker对象
	void LPAPI _Release(ILPSockerImpl* pSocker);

	// Summary:
	//		创建sock id
	// Return:
	//		sock id
	LPUINT32 LPAPI _CreateSockId();

private:

	static THREAD_FUNC_DECLARE(_CheckDelayThreadProc)(void* pParam);
	static THREAD_FUNC_DECLARE(_PostSendThreadProc)(void* pParam);

private:

	LPUINT32                   m_dwMaxSockId;        // 最大sock id
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