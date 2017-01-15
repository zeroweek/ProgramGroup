//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_REACTOR_H_
#define _LP_REACTOR_H_

#include "lpi_reactor.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;



// Summary:
//		sql工作线程参数
struct REACTOR_THREAD_PARAM
{
	ILPReactor*       pReactorImpl;
	LPINT32           nCompletionPortIndex;
};



// Summary：
//     windows平台反应器。
class DECLARE LPReactor : public ILPReactor
{
public:

	// Summary：
	//     构造函数
	// Returns:
	//     无
	LPReactor();

	// Summary：
	//     析构函数
	// Returns:
	//     无
	~LPReactor();

public:

	// Summary：
	//		无
	// Input:
	//		bOneCompletionPortOneThread: 是否单个完成端口模式
	//			TRUE-一个完成端口对应多个线程，FALSE-多个完成端口，每个完成端口对应一个线程
	virtual BOOL LPAPI Init(BOOL bOneCompletionPortOneThread);
	// Summary：
	//		无     
	virtual BOOL LPAPI UnInit();

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

	// Summary：
	//     线程处理函数的主逻辑
	virtual void LPAPI OnExecute(LPINT32 nCompletionPortIndex);

public:

	// Summary：
	//     完成端口线程处理函数
	static unsigned LPAPI ThreadFunc(void* pParam);

protected:

	// Summary：
	//		获取当前状态
	virtual LPUINT32 LPAPI _GetState();

	// Summary：
	//		设置当前状态
	virtual void LPAPI _SetState(LPUINT32 dwState);

protected:

	volatile atomic_uint    m_dwState;
};



// Summary：
//     windows IOCP反应器实现类
class DECLARE LPIocpReactor : public LPReactor
{
public:

	// Summary：
	//     构造函数
	// Returns:
	//     无
	LPIocpReactor();

	// Summary：
	//     析构函数
	// Returns:
	//     无
	~LPIocpReactor();

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
	//		无
	// Input:
	//		bOneCompletionPortOneThread: 是否单个完成端口模式
	//			TRUE-一个完成端口对应多个线程，FALSE-多个完成端口，每个完成端口对应一个线程
	virtual BOOL LPAPI Init(BOOL bOneCompletionPortOneThread);
	// Summary：
	//		无     
	virtual BOOL LPAPI UnInit();

	// Summary：
	//     线程处理函数的主逻辑
	virtual void LPAPI OnExecute(LPINT32 nCompletionPortIndex);

protected:

	LPINT32                  m_nCompletionPortCount;
	HANDLE*                  m_pCompletionPort;
	LPINT32                  m_nWorkerCountPerCompIo;
	HANDLE**                 m_ppWorkerArray;
};



//end声明所处的名字空间
NS_LZPL_END

#endif