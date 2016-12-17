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

	SIMPLE_LIST_NODE()
	{
		pstNext = nullptr;
	}
};


// Summary:
//		简单单向列表类
class DECLARE LPSimpleList
{
public:

	LPSimpleList()
	{
		Clear();
	}

	virtual ~LPSimpleList()
	{
		LOG_CHECK_ERROR(m_stTop.pstNext == nullptr);
	}

	inline void LPAPI Clear(void);
	inline BOOL LPAPI IsEmpty(void) const;
	inline LPUINT32 LPAPI Size(void) const;

	inline BOOL LPAPI Append(SIMPLE_LIST_NODE* pstNode);
	inline SIMPLE_LIST_NODE* LPAPI Head(void) const;
	inline SIMPLE_LIST_NODE* LPAPI Pop(void);

	inline BOOL LPAPI InsertAfter(SIMPLE_LIST_NODE* pNewNode, SIMPLE_LIST_NODE* pTargetNode);
	inline BOOL LPAPI InsertBefore(SIMPLE_LIST_NODE* pNewNode, SIMPLE_LIST_NODE* pTargetNode);

	inline BOOL LPAPI Remove(SIMPLE_LIST_NODE* pstNode);

private:

	LPUINT32               m_dwSize;
	SIMPLE_LIST_NODE      m_stTop;
	SIMPLE_LIST_NODE*     m_ptRear;
};



// Summary:
//		通过成员变量访问对象（成员变量的偏移量）
#define SIMPLE_NODE_CAST(_obj_class_ptr_, _member_, _node_ptr_)\
			((_obj_class_ptr_)((char*)(_node_ptr_) - (char*)&(((_obj_class_ptr_)0)->_member_)))

// Summary:
//   for循环开始宏
#define SIMPLE_LIST_FOR_BEGIN(__base_list__) \
	for (SIMPLE_LIST_NODE* ptNode = ##__base_list__.Head(); ptNode != nullptr;)\
	{\
		SIMPLE_LIST_NODE* ptNextNode = ptNode->pstNext;

// Summary:
//   for循环结束宏
#define SIMPLE_LIST_FOR_END \
		ptNode = ptNextNode;\
	}



inline void LPAPI LPSimpleList::Clear(void)
{
	m_dwSize = 0;
	m_stTop.pstNext = nullptr;
	m_ptRear = nullptr;
}

inline BOOL LPAPI LPSimpleList::IsEmpty(void) const
{
	return m_stTop.pstNext == nullptr;
}

inline LPUINT32 LPAPI LPSimpleList::Size(void) const
{
	return m_dwSize;
}

inline BOOL LPAPI LPSimpleList::Append(SIMPLE_LIST_NODE * pstNode)
{
	LOG_PROCESS_ERROR(pstNode);
	LOG_PROCESS_ERROR(pstNode->pstNext == nullptr);

	if (m_ptRear == nullptr)
	{
		m_ptRear = pstNode;
		m_stTop.pstNext = pstNode;
	}
	else
	{
		m_ptRear->pstNext = pstNode;
		m_ptRear = pstNode;
	}

	++m_dwSize;

	return TRUE;
Exit0:
	return FALSE;
}

inline SIMPLE_LIST_NODE *LPAPI LPSimpleList::Pop(void)
{
	SIMPLE_LIST_NODE* pNode = nullptr;

	PROCESS_ERROR(!IsEmpty());

	if (m_stTop.pstNext == m_ptRear && m_ptRear != nullptr)
	{
		m_ptRear = m_stTop.pstNext->pstNext;
	}

	pNode = m_stTop.pstNext;
	m_stTop.pstNext = pNode->pstNext;
	--m_dwSize;

	pNode->pstNext = nullptr;

	return pNode;
Exit0:
	return nullptr;
}

inline SIMPLE_LIST_NODE *LPAPI LPSimpleList::Head(void) const
{
	PROCESS_ERROR(!IsEmpty());

	return m_stTop.pstNext;

Exit0:
	return nullptr;
}

inline BOOL LPAPI LPSimpleList::InsertAfter(SIMPLE_LIST_NODE * pNewNode, SIMPLE_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);
	LOG_PROCESS_ERROR(pNewNode);
	LOG_PROCESS_ERROR(pNewNode->pstNext == nullptr);

	if (m_ptRear == nullptr || m_ptRear == pTargetNode)
	{
		m_ptRear = pNewNode;
	}

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
	LOG_PROCESS_ERROR(pTargetNode != &m_stTop);
	LOG_PROCESS_ERROR(pNewNode);
	LOG_PROCESS_ERROR(pNewNode->pstNext == nullptr);

	for (SIMPLE_LIST_NODE* pNode = &m_stTop; pNode != nullptr; pNode = pNode->pstNext)
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

inline BOOL LPAPI LPSimpleList::Remove(SIMPLE_LIST_NODE * pTargetNode)
{
	LOG_PROCESS_ERROR(pTargetNode);
	LOG_PROCESS_ERROR(pTargetNode != &m_stTop);

	for (SIMPLE_LIST_NODE* pNode = &m_stTop; pNode != nullptr; pNode = pNode->pstNext)
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