//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: windowsƽ̨�߳�ʵ��
//
//****************************************************************************/
#ifndef _LP_THREAD_H_
#define _LP_THREAD_H_

#ifdef _WIN32


#include "lp_base.h"


//begin�������������ֿռ�
NS_LZPL_BEGIN


typedef UINT_32 THREAD_ID;
typedef HANDLE THREAD_HANDLE;
typedef unsigned int (LPAPI *pfunThrdProc)(void *);

#define THREAD_FUNC_DECLARE(f) unsigned int LPAPI f



// Summary:
//		��
class LPThread
{
public:

	// Summary:
	//		��
	LPThread();
	
	// Summary:
	//		��
	virtual ~LPThread();

	// Summary:
	//		ͨ���̳�ʵ�ֵ��̣߳����ñ���������
	BOOL LPAPI Start(void);

	// Summary:
	//		ͨ��ֱ�Ӵ����̴߳�������ʽ�����ñ���������
	BOOL LPAPI Start(pfunThrdProc pfThrdProc, void* pParam);

	// Summary:
	//		�ȴ��߳̽���
	// Input:
	//		dwWaitTime-�ȴ�ʱ�䣬��λ����
	void LPAPI Wait(UINT_32 dwWaitTime = INFINITE_VALUE);

	// Summary:
	//		��
	void LPAPI Terminate(void);

public:

	// Summary:
	//		��
	static THREAD_ID LPAPI GetThreadId(void);

protected:

	// Summary:
	//		ͨ���̳�ʵ�ֵ��̣߳���Ҫ��д������
	virtual void ThrdProc(void) { }

protected:

	// Summary:
	//		��
	static THREAD_FUNC_DECLARE(DefaultThrdProc)(void* pParam);

protected:

	// Summary:
	//		��ֹʵ��
	LPThread(const LPThread&);
	LPThread& operator = (const LPThread&);

protected:

	BOOL                     m_bStart;
	THREAD_ID                m_tId;
	THREAD_HANDLE            m_hHandle;

};










//end�������������ֿռ�
NS_LZPL_END



#endif /* END OF _WIN32*/

#endif