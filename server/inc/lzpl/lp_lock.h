//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: ��ʵ��
//
//****************************************************************************/
#ifndef _LP_LOCK_H_
#define _LP_LOCK_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//�ٽ�����غ�������
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
//		�ٽ�����
class DECLARE LPLock
{
public:

	// Summary:
	//		��
	LPLock();

	// Summary:
	//		��
	~LPLock();

	// Summary:
	//		��
	void LPAPI Lock();

	// Summary:
	//		����
	void LPAPI UnLock();

protected:

	COM_CRITICAL_SECTION       m_stCs;
};



// Summary:
//		���ͷ��̻߳�������
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
	//		˽�л�����ʵ�֣����ε���
	LPAutoLock(const LPAutoLock& l);

	// Summary:
	//		˽�л�����ʵ�֣����ε���
	LPAutoLock& operator=(const LPAutoLock& l);

private:

	BOOL                     m_bNew;
	LPLock*                  m_poLock;
};



//end�������������ֿռ�
NS_LZPL_END

#endif