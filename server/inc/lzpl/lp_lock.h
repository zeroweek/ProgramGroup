//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 锁实现
//
//****************************************************************************/
#ifndef _LP_LOCK_H_
#define _LP_LOCK_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//临界区相关函数定义
#if defined _WIN32
#    define INIT_CRITICAL_SECTION                           InitializeCriticalSection
#    define DELETE_CRITICAL_SECTION                         DeleteCriticalSection
#    define ENTER_CRITICAL_SECTION                          EnterCriticalSection
#    define LEAVE_CRITICAL_SECTION                          LeaveCriticalSection
#    define COM_CRITICAL_SECTION                            CRITICAL_SECTION
#else	    
#    define INIT_CRITICAL_SECTION                           pthread_mutex_init
#    define DELETE_CRITICAL_SECTION                         pthread_mutex_destroy
#    define ENTER_CRITICAL_SECTION                          pthread_mutex_lock
#    define LEAVE_CRITICAL_SECTION                          pthread_mutex_unlock
#    define COM_CRITICAL_SECTION                            pthread_mutex_t     
#endif



// Summary:
//		临界区锁
class DECLARE LPLock
{
public:

	// Summary:
	//		无
	LPLock();

	// Summary:
	//		无
	~LPLock();

	// Summary:
	//		锁
	void LPAPI Lock();

	// Summary:
	//		解锁
	void LPAPI UnLock();

protected:

	COM_CRITICAL_SECTION       m_stCs;
};



// Summary:
//		自释放线程互斥锁类
class DECLARE LPAutoLock
{
public:

	// Summary:
	//		none
	explicit LPAutoLock();

	// Summary:
	//		none
	explicit LPAutoLock(LPLock* pLock);

	// Summary:
	//		none
	~LPAutoLock();

private:

	// Summary:
	//		私有化，不实现，屏蔽调用
	LPAutoLock(const LPAutoLock& l);

	// Summary:
	//		私有化，不实现，屏蔽调用
	LPAutoLock& operator=(const LPAutoLock& l);

private:

	BOOL                     m_bNew;
	LPLock*                  m_poLock;
};



//end声明所处的名字空间
NS_LZPL_END

#endif