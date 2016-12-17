//****************************************************************************/
//    author: caihy
//    date: October 9, 2015
//    description: �򵥵�˫���б�ʵ��
//
//****************************************************************************/
#ifndef _LP_BASE_LIST_H_
#define _LP_BASE_LIST_H_

#include "lp_base.h"
#include "lp_processerror.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



struct BASE_LIST_NODE
{
	BASE_LIST_NODE*    pstPrev;
	BASE_LIST_NODE*    pstNext;

	BASE_LIST_NODE()
	{
		pstPrev = nullptr;
		pstNext = nullptr;
	}
};


inline void LPAPI lpBaseListClearNode(BASE_LIST_NODE* pNode)
{
	pNode->pstPrev = NULL;
	pNode->pstNext = NULL;
}


// Summary:
//		���б���
class DECLARE LPBaseList
{
public:

	// Summary:
	//		��
	LPBaseList()
	{
		Clear();
	}

	// Summary:
	//		ͨ����������ʱ��Ҫ���µ���Clear������m_stHead��m_stRear��ָ�벻׼
	LPBaseList(const LPBaseList& l)
	{
		Clear();
	}

	// Summary:
	//		ͨ����ֵ����ʱ��Ҫ���µ���Clear������m_stHead��m_stRear��ָ�벻׼
	LPBaseList& operator=(const LPBaseList& l)
	{
		Clear();
	}

	// Summary:
	//		��
	virtual ~LPBaseList()
	{
		LOG_CHECK_ERROR(m_stHead.pstPrev == NULL);
		LOG_CHECK_ERROR(m_stHead.pstNext == &m_stRear);
		LOG_CHECK_ERROR(m_stRear.pstPrev == &m_stHead);
		LOG_CHECK_ERROR(m_stRear.pstNext == NULL);
	}

	// Summary:
	//		��
	inline void LPAPI Clear(void);

	// Summary:
	//		��
	inline BOOL LPAPI IsEmpty(void);

	// Summary:
	//		��
	inline BASE_LIST_NODE& LPAPI Head(void);

	// Summary:
	//		��
	inline BASE_LIST_NODE& LPAPI Rear(void);

	// Summary:
	//		��
	inline LPUINT32 LPAPI Size(void) const;

	// Summary:
	//		��
	inline BOOL LPAPI InsertAfter(BASE_LIST_NODE* pNewNode, BASE_LIST_NODE* pTargetNode);

	// Summary:
	//		��
	inline BOOL LPAPI InsertBefore(BASE_LIST_NODE* pNewNode, BASE_LIST_NODE* pTargetNode);

	// Summary:
	//		��
	inline BOOL LPAPI Remove(BASE_LIST_NODE* pstNode);

	// Summary:
	//		��
	inline BOOL LPAPI PushHead(BASE_LIST_NODE* pstNode);

	// Summary:
	//		��
	inline BASE_LIST_NODE* LPAPI PopHead(void);

	// Summary:
	//		��
	inline BOOL LPAPI PushRear(BASE_LIST_NODE* pstNode);

	// Summary:
	//		��
	inline BASE_LIST_NODE* LPAPI PopRear(void);

private:

	LPUINT32             m_dwSize;
	BASE_LIST_NODE      m_stHead;
	BASE_LIST_NODE      m_stRear;
};



// Summary:
//   ͨ����Ա�������ʶ��󣨳�Ա������ƫ������
#define BASE_NODE_CAST(_obj_class_ptr_, _member_, _node_ptr_)\
			((_obj_class_ptr_)((char*)(_node_ptr_) - (char*)&(((_obj_class_ptr_)0)->_member_)))

// Summary:
//   forѭ����ʼ��
#define BASE_LIST_FOR_BEGIN(__base_list__) \
	for (BASE_LIST_NODE* ptNode = ##__base_list__.Head().pstNext;\
		ptNode != &##__base_list__.Rear();)\
	{\
		LOG_CHECK_ERROR(ptNode != nullptr);\
		if (ptNode == nullptr) { break; }\
		BASE_LIST_NODE* ptNextNode = ptNode->pstNext;

// Summary:
//   forѭ��������
#define BASE_LIST_FOR_END \
		ptNode = ptNextNode;\
	}



inline void LPAPI LPBaseList::Clear(void)
{
	m_dwSize = 0;
	m_stHead.pstPrev = NULL;
	m_stHead.pstNext = &m_stRear;
	m_stRear.pstPrev = &m_stHead;
	m_stRear.pstNext = NULL;
}

inline BOOL LPAPI LPBaseList::IsEmpty(void)
{
	if (m_stHead.pstNext == &m_stRear)
	{
		LOG_CHECK_ERROR(m_stRear.pstPrev == &m_stHead);
	}
	if (m_stRear.pstPrev == &m_stHead)
	{
		LOG_CHECK_ERROR(m_stHead.pstNext == &m_stRear);
	}

	return (m_stHead.pstNext == &m_stRear);
}

inline BASE_LIST_NODE &LPAPI LPBaseList::Head(void)
{
	return m_stHead;
}

inline BASE_LIST_NODE &LPAPI LPBaseList::Rear(void)
{
	return m_stRear;
}

inline LPUINT32 LPAPI LPBaseList::Size(void) const
{
	return m_dwSize;
}

inline BOOL LPAPI LPBaseList::InsertAfter(BASE_LIST_NODE* pNewNode, BASE_LIST_NODE* pTargetNode)
{
	LOG_PROCESS_ERROR(pNewNode && pTargetNode);
	LOG_PROCESS_ERROR(NULL == pNewNode->pstPrev && NULL == pNewNode->pstNext);
	LOG_PROCESS_ERROR(pTargetNode->pstNext);

	pNewNode->pstPrev = pTargetNode;
	pNewNode->pstNext = pTargetNode->pstNext;
	pTargetNode->pstNext->pstPrev = pNewNode;
	pTargetNode->pstNext = pNewNode;
	++m_dwSize;

	return TRUE;

Exit0:

	return FALSE;
}

inline BOOL LPAPI LPBaseList::InsertBefore(BASE_LIST_NODE* pNewNode, BASE_LIST_NODE* pTargetNode)
{
	LOG_PROCESS_ERROR(pNewNode && pTargetNode);
	LOG_PROCESS_ERROR(NULL == pNewNode->pstPrev && NULL == pNewNode->pstNext);
	LOG_PROCESS_ERROR(pTargetNode->pstPrev);

	pNewNode->pstNext = pTargetNode;
	pNewNode->pstPrev = pTargetNode->pstPrev;
	pTargetNode->pstPrev->pstNext = pNewNode;
	pTargetNode->pstPrev = pNewNode;
	++m_dwSize;

	return TRUE;

Exit0:

	return FALSE;
}

inline BOOL LPAPI LPBaseList::Remove(BASE_LIST_NODE* pstNode)
{
	LOG_PROCESS_ERROR(pstNode);
	PROCESS_ERROR(pstNode->pstPrev && pstNode->pstNext);

	pstNode->pstPrev->pstNext = pstNode->pstNext;
	pstNode->pstNext->pstPrev = pstNode->pstPrev;

	pstNode->pstPrev = NULL;
	pstNode->pstNext = NULL;
	--m_dwSize;

	return TRUE;

Exit0:

	return FALSE;
}

inline BOOL LPAPI LPBaseList::PushHead(BASE_LIST_NODE* pstNode)
{
	LOG_PROCESS_ERROR(NULL == pstNode->pstPrev && NULL == pstNode->pstNext);

	pstNode->pstPrev = &m_stHead;
	pstNode->pstNext = m_stHead.pstNext;

	pstNode->pstNext->pstPrev = pstNode;
	m_stHead.pstNext = pstNode;
	++m_dwSize;

	return TRUE;

Exit0:

	return FALSE;
}

inline BASE_LIST_NODE* LPAPI LPBaseList::PopHead(void)
{
	BASE_LIST_NODE* pstResult = NULL;

	PROCESS_ERROR(m_stHead.pstNext != &m_stRear);

	pstResult = m_stHead.pstNext;
	m_stHead.pstNext = pstResult->pstNext;
	pstResult->pstNext->pstPrev = &m_stHead;

	pstResult->pstPrev = NULL;
	pstResult->pstNext = NULL;
	--m_dwSize;

	return pstResult;

Exit0:

	return NULL;
}

inline BOOL LPAPI LPBaseList::PushRear(BASE_LIST_NODE* pstNode)
{
	LOG_PROCESS_ERROR(NULL == pstNode->pstPrev && NULL == pstNode->pstNext);

	pstNode->pstPrev = m_stRear.pstPrev;
	pstNode->pstNext = &m_stRear;

	m_stRear.pstPrev = pstNode;
	pstNode->pstPrev->pstNext = pstNode;
	++m_dwSize;

	return TRUE;

Exit0:

	return FALSE;
}

inline BASE_LIST_NODE* LPAPI LPBaseList::PopRear(void)
{
	BASE_LIST_NODE* pstResult = NULL;

	PROCESS_ERROR(m_stRear.pstPrev != &m_stHead);

	pstResult = m_stRear.pstPrev;
	m_stRear.pstPrev = pstResult->pstPrev;
	pstResult->pstPrev->pstNext = &m_stRear;

	pstResult->pstPrev = NULL;
	pstResult->pstNext = NULL;
	--m_dwSize;

	return pstResult;

Exit0:

	return NULL;
}










//end�������������ֿռ�
NS_LZPL_END

#endif