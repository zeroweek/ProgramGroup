//****************************************************************************/
//    author: caihy
//    date: Dec 2, 2016
//    description: 简单的栈列表实现（先进后出）
//
//****************************************************************************/
#ifndef _LP_STACK_LIST_H_
#define _LP_STACK_LIST_H_

#include "lp_base.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



struct STACK_LIST_NODE
{
	STACK_LIST_NODE*    pstNext;

	STACK_LIST_NODE()
	{
		pstNext = nullptr;
	}
};


// Summary:
//		简单栈列表类
class DECLARE LPStackList
{
public:

	LPStackList()
	{
		Clear();
	}

	virtual ~LPStackList()
	{
		LOG_CHECK_ERROR(m_stTop.pstNext == nullptr);
	}

	inline void LPAPI Clear(void);
	inline BOOL LPAPI IsEmpty(void);
	inline LPUINT32 LPAPI Size(void) const;

	inline BOOL LPAPI Push(STACK_LIST_NODE* pstNode);
	inline STACK_LIST_NODE* LPAPI Pop(void);
	inline STACK_LIST_NODE* LPAPI Top(void);

	inline BOOL LPAPI InsertAfter(STACK_LIST_NODE* pNewNode, STACK_LIST_NODE* pTargetNode);
	inline BOOL LPAPI InsertBefore(STACK_LIST_NODE* pNewNode, STACK_LIST_NODE* pTargetNode);

	inline BOOL LPAPI Remove(STACK_LIST_NODE* pstNode);

private:

	LPUINT32               m_dwSize;
	STACK_LIST_NODE       m_stTop;
};



// Summary:
//		通过成员变量访问对象（成员变量的偏移量）
#define STACK_NODE_CAST(_obj_class_ptr_, _member_, _node_ptr_)\
			((_obj_class_ptr_)((char*)(_node_ptr_) - (char*)&(((_obj_class_ptr_)0)->_member_)))

// Summary:
//   for循环开始宏
#define STACK_LIST_FOR_BEGIN(__base_list__) \
	for (STACK_LIST_NODE* ptNode = ##__base_list__.Top(); ptNode != nullptr;)\
	{\
		STACK_LIST_NODE* ptNextNode = ptNode->pstNext;

// Summary:
//   for循环结束宏
#define STACK_LIST_FOR_END \
		ptNode = ptNextNode;\
	}



inline void LPAPI LPStackList::Clear(void)
{
	m_dwSize = 0;
	m_stTop.pstNext = nullptr;
}

inline BOOL LPAPI LPStackList::IsEmpty(void)
{
	return m_stTop.pstNext == nullptr;
}

inline LPUINT32 LPAPI LPStackList::Size(void) const
{
	return m_dwSize;
}

inline BOOL LPAPI LPStackList::Push(STACK_LIST_NODE * pstNode)
{
	LOG_PROCESS_ERROR(pstNode);
	LOG_PROCESS_ERROR(pstNode->pstNext == nullptr);

	pstNode->pstNext = m_stTop.pstNext;
	m_stTop.pstNext = pstNode;
	++m_dwSize;

	return TRUE;
Exit0:
	return FALSE;
}

inline STACK_LIST_NODE *LPAPI LPStackList::Pop(void)
{
	STACK_LIST_NODE* pNode = nullptr;

	PROCESS_ERROR(!IsEmpty());

	pNode = m_stTop.pstNext;
	m_stTop.pstNext = pNode->pstNext;
	--m_dwSize;

	pNode->pstNext = nullptr;

	return pNode;
Exit0:
	return nullptr;
}

inline STACK_LIST_NODE *LPAPI LPStackList::Top(void)
{
	PROCESS_ERROR(!IsEmpty());

	return m_stTop.pstNext;

Exit0:
	return nullptr;
}

inline BOOL LPAPI LPStackList::InsertAfter(STACK_LIST_NODE * pNewNode, STACK_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);
	LOG_PROCESS_ERROR(pNewNode);
	LOG_PROCESS_ERROR(pNewNode->pstNext == nullptr);

	pNewNode->pstNext = pTargetNode->pstNext;
	pTargetNode->pstNext = pNewNode;

	++m_dwSize;

	return TRUE;
Exit0:
	return FALSE;
}

inline BOOL LPAPI LPStackList::InsertBefore(STACK_LIST_NODE * pNewNode, STACK_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);
	LOG_PROCESS_ERROR(pTargetNode != &m_stTop);
	LOG_PROCESS_ERROR(pNewNode);
	LOG_PROCESS_ERROR(pNewNode->pstNext == nullptr);

	for (STACK_LIST_NODE* pNode = &m_stTop; pNode != nullptr; pNode = pNode->pstNext)
	{
		if (pNode->pstNext != pTargetNode)
		{
			continue;
		}

		pNewNode->pstNext = pNode->pstNext;
		pNode->pstNext = pNewNode;

		++m_dwSize;
		break;
	}

	return TRUE;
Exit0:
	return FALSE;
}

inline BOOL LPAPI LPStackList::Remove(STACK_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);
	LOG_PROCESS_ERROR(pTargetNode != &m_stTop);

	for (STACK_LIST_NODE* pNode = &m_stTop; pNode != nullptr; pNode = pNode->pstNext)
	{
		if (pNode->pstNext != pTargetNode)
		{
			continue;
		}

		pNode->pstNext = pTargetNode->pstNext;
		pTargetNode->pstNext = nullptr;

		return TRUE;
	}

Exit0:
	return FALSE;
}









//end声明所处的名字空间
NS_LZPL_END

#endif