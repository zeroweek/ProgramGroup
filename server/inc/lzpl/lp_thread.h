//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: windows平台线程实现
//
//****************************************************************************/
#ifndef _LP_THREAD_H_
#define _LP_THREAD_H_

#ifdef _WIN32


#include "lp_base.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN


typedef UINT_32 THREAD_ID;
typedef HANDLE THREAD_HANDLE;
typedef unsigned int (LPAPI *pfunThrdProc)(void *);

#define THREAD_FUNC_DECLARE(f) unsigned int LPAPI f



// Summary:
//		无
class LPThread
{
public:

	// Summary:
	//		无
	LPThread();
	
	// Summary:
	//		无
	virtual ~LPThread();

	// Summary:
	//		通过继承实现的线程，调用本函数开启
	BOOL LPAPI Start(void);

	// Summary:
	//		通过直接传入线程处理函数方式，调用本函数开启
	BOOL LPAPI Start(pfunThrdProc pfThrdProc, void* pParam);

	// Summary:
	//		等待线程结束
	// Input:
	//		dwWaitTime-等待时间，单位毫秒
	void LPAPI Wait(UINT_32 dwWaitTime = INFINITE_VALUE);

	// Summary:
	//		无
	void LPAPI Terminate(void);

public:

	// Summary:
	//		无
	static THREAD_ID LPAPI GetThreadId(void);

protected:

	// Summary:
	//		通过继承实现的线程，需要重写本函数
	virtual void ThrdProc(void) { }

protected:

	// Summary:
	//		无
	static THREAD_FUNC_DECLARE(DefaultThrdProc)(void* pParam);

protected:

	// Summary:
	//		禁止实现
	LPThread(const LPThread&);
	LPThread& operator = (const LPThread&);

protected:

	BOOL                     m_bStart;
	THREAD_ID                m_tId;
	THREAD_HANDLE            m_hHandle;

};










//end声明所处的名字空间
NS_LZPL_END



#endif /* END OF _WIN32*/

#endif