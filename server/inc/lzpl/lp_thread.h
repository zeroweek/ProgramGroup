//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: windowsƽ̨�߳�ʵ��
//
//****************************************************************************/
#ifndef _LP_THREAD_H_
#define _LP_THREAD_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



#ifdef _WIN32
	typedef LPUINT32 THREAD_ID;
	typedef HANDLE THREAD_HANDLE;
	#define INVALID_THREAD_HANDLE            (0)
	typedef unsigned int (LPAPI *pfunThrdProc)(void *);
	#define THREAD_FUNC_DECLARE(f) unsigned int LPAPI f
#else
	typedef LPUINT32 THREAD_ID;
	typedef pthread_t THREAD_HANDLE;
	#define INVALID_THREAD_HANDLE            (-1)
	typedef void* (LPAPI *pfunThrdProc)(void *);
	#define THREAD_FUNC_DECLARE(f) void* LPAPI f
#endif




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
	void LPAPI Wait(LPUINT32 dwWaitTime = INFINITE_VALUE);

	// Summary:
	//		��
	void LPAPI Terminate(void);

	// Summary:
	//		��
	THREAD_ID LPAPI GetId(void);

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