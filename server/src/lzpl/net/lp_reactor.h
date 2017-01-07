//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_REACTOR_H_
#define _LP_REACTOR_H_

#include "lpi_reactor.h"
#include "lp_net.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		sql工作线程参数
struct REACTOR_THREAD_PARAM
{
	ILPReactor*       pReactorImpl;
	LPINT32           nCompletionPortIndex;
};



// Summary：
//     windows平台反应器。
class DECLARE LPReactorIocpImpl : public ILPReactor
{
public:

	// Summary：
	//     构造函数
	// Returns:
	//     无
	LPReactorIocpImpl();

	// Summary：
	//     析构函数
	// Returns:
	//     无
	~LPReactorIocpImpl();

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

protected:

	BOOL	                m_bInit;
	volatile atomic_uint    m_dwState;

	LPINT32                  m_nCompletionPortCount;
	HANDLE*                  m_pCompletionPort;
	LPINT32                  m_nWorkerCountPerCompIo;
	HANDLE**                 m_ppWorkerArray;
	LPNetImpl*               m_pNetImpl;
};



//end声明所处的名字空间
NS_LZPL_END

#endif