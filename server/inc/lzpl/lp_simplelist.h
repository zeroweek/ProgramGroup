//****************************************************************************/
//    author: caihy
//    date: January 18, 2016
//    description: 简单的单向列表实现
//
//****************************************************************************/
#ifndef _LP_SIMPLE_LIST_H_
#define _LP_SIMPLE_LIST_H_

#include "lp_base.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



struct SIMPLE_LIST_NODE
{
	SIMPLE_LIST_NODE*    pstNext;
};


// Summary:
//		简单列表类
class DECLARE LPSimpleList
{
public:

	LPSimpleList()
	{
		Clear();
	}

	virtual ~LPSimpleList()
	{
		LOG_CHECK_ERROR(m_stTop.pstNext == NULL);
	}

	inline void LPAPI Clear(void);
	inline BOOL LPAPI IsEmpty(void);
	inline UINT_32 LPAPI Size(void);

	inline BOOL LPAPI Push(SIMPLE_LIST_NODE* pstNode);
	inline SIMPLE_LIST_NODE* LPAPI Pop(void);
	inline SIMPLE_LIST_NODE* LPAPI Top(void);

	inline BOOL LPAPI InsertAfter(SIMPLE_LIST_NODE* pNewNode, SIMPLE_LIST_NODE* pTargetNode);
	inline BOOL LPAPI InsertBefore(SIMPLE_LIST_NODE* pNewNode, SIMPLE_LIST_NODE* pTargetNode);

	inline BOOL LPAPI Remove(SIMPLE_LIST_NODE* pstNode);

private:

	UINT_32               m_dwSize;
	SIMPLE_LIST_NODE      m_stTop;
};


inline void LPAPI LPSimpleList::Clear(void)
{
	m_dwSize = 0;
	m_stTop.pstNext = NULL;
}

inline BOOL LPAPI LPSimpleList::IsEmpty(void)
{
	return m_stTop.pstNext == NULL;
}

inline UINT_32 LPAPI LPSimpleList::Size(void)
{
	return m_dwSize;
}

inline BOOL LPAPI LPSimpleList::Push(SIMPLE_LIST_NODE * pstNode)
{
	LOG_PROCESS_ERROR(pstNode);
	LOG_PROCESS_ERROR(pstNode->pstNext == NULL);

	pstNode->pstNext = m_stTop.pstNext;
	m_stTop.pstNext = pstNode;
	++m_dwSize;

	return TRUE;
Exit0:
	return FALSE;
}

inline SIMPLE_LIST_NODE *LPAPI LPSimpleList::Pop(void)
{
	SIMPLE_LIST_NODE* pNode = NULL;

	PROCESS_ERROR(!IsEmpty());

	pNode = m_stTop.pstNext;
	m_stTop.pstNext = pNode->pstNext;
	--m_dwSize;

	pNode->pstNext = NULL;

	return pNode;
Exit0:
	return NULL;
}

inline SIMPLE_LIST_NODE *LPAPI LPSimpleList::Top(void)
{
	PROCESS_ERROR(!IsEmpty());

	return m_stTop.pstNext;

Exit0:
	return NULL;
}

inline BOOL LPAPI LPSimpleList::InsertAfter(SIMPLE_LIST_NODE * pNewNode, SIMPLE_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);
	LOG_PROCESS_ERROR(pNewNode);
	LOG_PROCESS_ERROR(pNewNode->pstNext == NULL);

	pNewNode->pstNext = pTargetNode->pstNext;
	pTargetNode->pstNext = pNewNode;

	++m_dwSize;

	return TRUE;
Exit0:
	return FALSE;
}

inline BOOL LPAPI LPSimpleList::InsertBefore(SIMPLE_LIST_NODE * pNewNode, SIMPLE_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);
	LOG_PROCESS_ERROR(pNewNode);
	LOG_PROCESS_ERROR(pNewNode->pstNext == NULL);

	for (SIMPLE_LIST_NODE* pNode = m_stTop.pstNext; pNode != NULL; pNode = pNode->pstNext)
	{
		if (pNode->pstNext != pTargetNode)
		{
			continue;
		}

		return InsertAfter(pNewNode, pTargetNode);
	}

	LOG_PROCESS_ERROR(FALSE);

Exit0:
	return FALSE;
}

inline BOOL LPAPI LPSimpleList::Remove(SIMPLE_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);

	for (SIMPLE_LIST_NODE* pNode = &m_stTop; pNode != NULL; pNode = pNode->pstNext)
	{
		if (pNode->pstNext != pTargetNode)
		{
			continue;
		}

		pNode->pstNext = pTargetNode->pstNext;
		pTargetNode->pstNext = NULL;

		return TRUE;
	}

Exit0:
	return FALSE;
}









//end声明所处的名字空间
NS_LZPL_END

#endif