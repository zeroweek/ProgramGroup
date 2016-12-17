//****************************************************************************/
//    author: caihy
//    date: October 9, 2015
//    description: 简单的双向列表实现
//
//****************************************************************************/
#ifndef _LP_BASE_LIST_H_
#define _LP_BASE_LIST_H_

#include "lp_base.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
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
//		简单列表类
class DECLARE LPBaseList
{
public:

	// Summary:
	//		无
	LPBaseList()
	{
		Clear();
	}

	// Summary:
	//		通过拷贝创建时需要重新调用Clear，否则m_stHead和m_stRear的指针不准
	LPBaseList(const LPBaseList& l)
	{
		Clear();
	}

	// Summary:
	//		通过赋值创建时需要重新调用Clear，否则m_stHead和m_stRear的指针不准
	LPBaseList& operator=(const LPBaseList& l)
	{
		Clear();
	}

	// Summary:
	//		无
	virtual ~LPBaseList()
	{
		LOG_CHECK_ERROR(m_stHead.pstPrev == NULL);
		LOG_CHECK_ERROR(m_stHead.pstNext == &m_stRear);
		LOG_CHECK_ERROR(m_stRear.pstPrev == &m_stHead);
		LOG_CHECK_ERROR(m_stRear.pstNext == NULL);
	}

	// Summary:
	//		无
	inline void LPAPI Clear(void);

	// Summary:
	//		无
	inline BOOL LPAPI IsEmpty(void);

	// Summary:
	//		无
	inline BASE_LIST_NODE& LPAPI Head(void);

	// Summary:
	//		无
	inline BASE_LIST_NODE& LPAPI Rear(void);

	// Summary:
	//		无
	inline LPUINT32 LPAPI Size(void) const;

	// Summary:
	//		无
	inline BOOL LPAPI InsertAfter(BASE_LIST_NODE* pNewNode, BASE_LIST_NODE* pTargetNode);

	// Summary:
	//		无
	inline BOOL LPAPI InsertBefore(BASE_LIST_NODE* pNewNode, BASE_LIST_NODE* pTargetNode);

	// Summary:
	//		无
	inline BOOL LPAPI Remove(BASE_LIST_NODE* pstNode);

	// Summary:
	//		无
	inline BOOL LPAPI PushHead(BASE_LIST_NODE* pstNode);

	// Summary:
	//		无
	inline BASE_LIST_NODE* LPAPI PopHead(void);

	// Summary:
	//		无
	inline BOOL LPAPI PushRear(BASE_LIST_NODE* pstNode);

	// Summary:
	//		无
	inline BASE_LIST_NODE* LPAPI PopRear(void);

private:

	LPUINT32             m_dwSize;
	BASE_LIST_NODE      m_stHead;
	BASE_LIST_NODE      m_stRear;
};



// Summary:
//   通过成员变量访问对象（成员变量的偏移量）
#define BASE_NODE_CAST(_obj_class_ptr_, _member_, _node_ptr_)\
			((_obj_class_ptr_)((char*)(_node_ptr_) - (char*)&(((_obj_class_ptr_)0)->_member_)))

// Summary:
//   for循环开始宏
#define BASE_LIST_FOR_BEGIN(__base_list__) \
	for (BASE_LIST_NODE* ptNode = ##__base_list__.Head().pstNext;\
		ptNode != &##__base_list__.Rear();)\
	{\
		LOG_CHECK_ERROR(ptNode != nullptr);\
		if (ptNode == nullptr) { break; }\
		BASE_LIST_NODE* ptNextNode = ptNode->pstNext;

// Summary:
//   for循环结束宏
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










//end声明所处的名字空间
NS_LZPL_END

#endif