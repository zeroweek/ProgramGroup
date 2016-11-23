//****************************************************************************/
//    author: caihy
//    date: October 9, 2015
//    description: ѭ���б�ʵ�֣�֧�ֵ��߳�pop�����߳�push��������
//
//****************************************************************************/
#ifndef _LP_LOOP_LIST_H_
#define _LP_LOOP_LIST_H_

#include "lp_base.h"
#include "lp_processerror.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN





// Summary:
//		ѭ���б��֧࣬�ֵ��߳�pop�����߳�push��������
class DECLARE LPLoopList
{
public:

	// Summary:
	//		��
	LPLoopList();

	// Summary:
	//		��
	~LPLoopList();

	// Summary:
	//		��ʼ������ʼ�ڴ�������
	// Return:
	//		TRUE-�ڴ����ɹ���FALSE-�ڴ����ʧ��
	BOOL LPAPI Init(UINT_32 dwSize);

	// Summary:
	//		����ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		��
	inline BOOL LPAPI CanPush(void);

	// Summary:
	//		��
	inline BOOL LPAPI Push(void* pData);

	// Summary:
	//		��
	inline BOOL LPAPI CanPop(void);

	// Summary:
	//		��
	inline void* LPAPI Pop(void);

	// Summary:
	//		��
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





//end�������������ֿռ�
NS_LZPL_END

#endif