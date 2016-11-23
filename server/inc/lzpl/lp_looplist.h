//****************************************************************************/
//    author: caihy
//    date: October 9, 2015
//    description: 循环列表实现，支持单线程pop、单线程push无锁操作
//
//****************************************************************************/
#ifndef _LP_LOOP_LIST_H_
#define _LP_LOOP_LIST_H_

#include "lp_base.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN





// Summary:
//		循环列表类，支持单线程pop、单线程push无锁操作
class DECLARE LPLoopList
{
public:

	// Summary:
	//		无
	LPLoopList();

	// Summary:
	//		无
	~LPLoopList();

	// Summary:
	//		初始化，开始内存分配操作
	// Return:
	//		TRUE-内存分配成功，FALSE-内存分配失败
	BOOL LPAPI Init(UINT_32 dwSize);

	// Summary:
	//		反初始化
	BOOL LPAPI UnInit(void);

	// Summary:
	//		无
	inline BOOL LPAPI CanPush(void);

	// Summary:
	//		无
	inline BOOL LPAPI Push(void* pData);

	// Summary:
	//		无
	inline BOOL LPAPI CanPop(void);

	// Summary:
	//		无
	inline void* LPAPI Pop(void);

	// Summary:
	//		无
	inline void* LPAPI Check(void);


private:

	UINT_32                  m_dwSize;
	UINT_32                  m_dwPop;
	UINT_32                  m_dwPush;
	void**                   m_ppData;
	volatile atomic_uint     m_dwValidCount;


};

inline BOOL LPAPI LPLoopList::CanPush(void)
{
	return m_dwValidCount < m_dwSize;
}

inline BOOL LPAPI LPLoopList::Push(void * pData)
{
	LOG_PROCESS_ERROR(pData);
	LOG_PROCESS_ERROR(CanPush());

	m_ppData[m_dwPush] = pData;
	m_dwPush = (m_dwPush + 1) % m_dwSize;
	m_dwValidCount++;

	return TRUE;

Exit0:

	return FALSE;
}

inline BOOL LPAPI LPLoopList::CanPop(void)
{
	return m_dwValidCount > 0;
}

inline void *LPAPI LPLoopList::Pop(void)
{
	void* pResult = NULL;

	LOG_PROCESS_ERROR(CanPop());

	pResult = m_ppData[m_dwPop];
	m_dwPop = (m_dwPop + 1) % m_dwSize;
	m_dwValidCount--;

Exit0:

	return pResult;
}

inline void *LPAPI LPLoopList::Check(void)
{
	PROCESS_ERROR(CanPop());

	return m_ppData[m_dwPop];
Exit0:
	return NULL;
}





//end声明所处的名字空间
NS_LZPL_END

#endif