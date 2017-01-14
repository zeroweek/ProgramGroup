//****************************************************************************/
//    author: caihy
//    date: June 1, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_OBJ_POOL_H_
#define _LP_OBJ_POOL_H_

#include "lp_base.h"
#include "lp_system.h"
#include "lp_lock.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		对象池相关错误码
enum e_ObjPoolErr
{
	ObjPoolErr_NoError = 0,
	ObjPoolErr_NotEnoughMemory = 1,
	ObjPoolErr_FreeNullPointer = 2,
	ObjPoolErr_FreeErrorPointer = 3,
	ObjPoolErr_InputSizeZero = 4,
	ObjPoolErr_Max
};



// Summary:
//		对象池类模板，静态内存池
template <typename T>
class LPObjPool
{
public:

	struct POOL_NODE
	{
		char*    buf_start;
		char*    buf_end;
	};

	typedef std::list<char*> LIST;

	struct MEM_NODE
	{
		struct MEM_NODE* next;
	};

	LPObjPool(void) : m_dwElemCount(0), m_dwNextCount(100), m_dwUseCount(0), 
		m_pstFreeNode(NULL), m_poLock(NULL), m_bInit(FALSE)
	{
	}

	LPObjPool(LPUINT32 dwNextCount, BOOL bMutex) : m_dwElemCount(0), m_dwNextCount(dwNextCount), 
		m_dwUseCount(0), m_pstFreeNode(NULL), m_poLock(NULL), m_bInit(FALSE)
	{
		LPINT32 nResult = 0;

		nResult = _Init();
		LOG_CHECK_ERROR(nResult);
	
		if (bMutex)
		{
			nResult = SetMutex();
			LOG_CHECK_ERROR(nResult);
		}

		m_bInit = TRUE;
	}

	BOOL LPAPI Init(LPUINT32 dwNextCount, BOOL bMutex)
	{
		LPINT32 nResult = 0;

		PROCESS_SUCCESS(m_bInit);

		m_dwNextCount = dwNextCount;

		nResult = _Init();
		LOG_PROCESS_ERROR(nResult);

		if (bMutex)
		{
			nResult = SetMutex();
			LOG_PROCESS_ERROR(nResult);
		}

		m_bInit = TRUE;

	Exit1:
		return TRUE;
	Exit0:
		return FALSE;
	}

	~LPObjPool()
	{
		POOL_NODE* pNode = NULL;
		if (NULL != m_poLock) m_poLock->Lock();
		LIST::iterator it, end = m_list.end();
		for (it = m_list.begin(); it != end; ++it)
		{
			pNode = (POOL_NODE*)(*it);
			LOG_CHECK_ERROR(pNode);
			if (NULL == pNode) continue;

			::free(pNode->buf_start);
			::free(pNode);
		}
		m_list.clear();
		if (NULL != m_poLock) m_poLock->UnLock();

		if (NULL != m_poLock) SAFE_DELETE(m_poLock);
	}

	BOOL LPAPI SetMutex()
	{
		LOG_PROCESS_ERROR(NULL == m_poLock);
		m_poLock = new LPLock();
		LOG_PROCESS_ERROR(m_poLock);

		return TRUE;
	Exit0:
		return FALSE;
	}

	T* LPAPI Malloc()
	{
		T* p = _MutexMalloc();
		if (NULL == p)
		{
			return p;
		}
		new(p)T;
		return p;
	}

	template <typename T1>
	T* LPAPI Malloc(T1 t1)
	{
		T* p = _MutexMalloc();
		if (NULL == p)
		{
			return p;
		}
		new(p)T(t1);
		return p;
	}

	template <typename T1, typename T2>
	T* LPAPI Malloc(T1 t1, T2 t2)
	{
		T* p = _MutexMalloc();
		if (NULL == p)
		{
			return p;
		}
		new(p)T(t1, t2);
		return p;
	}

	template <typename T1, typename T2, typename T3>
	T* LPAPI Malloc(T1 t1, T2 t2, T3 t3)
	{
		T* p = _MutexMalloc();
		if (NULL == p)
		{
			return p;
		}
		new(p)T(t1, t2, t3);
		return p;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	T* LPAPI Malloc(T1 t1, T2 t2, T3 t3, T4 t4)
	{
		T* p = _MutexMalloc();
		if (NULL == p)
		{
			return p;
		}
		new(p)T(t1, t2, t3, t4);
		return p;
	}

	//要支持更多的构造函数参数，在这里添加
	//TODO:
	//...

	void LPAPI Free(T* pt)
	{
		LOG_PROCESS_ERROR(pt);

		pt->~T();

		if (NULL != m_poLock) m_poLock->Lock();
		_Free(pt);
		if (NULL != m_poLock) m_poLock->UnLock();

	Exit0:
		return;
	}

	BOOL LPAPI IsInThisPool(char* pt)
	{
		LPINT32 nResult = 0;

		if (NULL == m_poLock) m_poLock->Lock();
		nResult = _IsInThisPool(pt);
		if (NULL == m_poLock) m_poLock->UnLock();

		return nResult;
	}

	LPUINT32 LPAPI GetMemorySize() const
	{
		return m_dwElemCount * (max(sizeof(T), sizeof(T*)));
	}

	LPUINT32 LPAPI GetUseCount() const
	{
		return m_dwUseCount;
	}

private:

	T* LPAPI _MutexMalloc()
	{
		T* pT = NULL;

		if (NULL != m_poLock) m_poLock->Lock();
		pT = _Malloc();
		if (NULL != m_poLock) m_poLock->UnLock();

		return pT;
	}

	BOOL LPAPI _IsInThisPool(char* pt)
	{
		LPINT32 nResult = FALSE;
		const POOL_NODE* pNode = NULL;
		LIST::const_iterator cit, end = m_list.end();

		for (cit = m_list.begin(); cit != end; ++cit)
		{
			pNode = (POOL_NODE*)(*cit);
			if (pt >= pNode->buf_start && pt < pNode->buf_end)
			{
				nResult = TRUE;
				break;
			}
		}

		return nResult;
	}

	BOOL LPAPI _Init()
	{
		LPINT32 nResult = 0;
		POOL_NODE* pstPoolNode = NULL;
		MEM_NODE* pstMemNode = NULL;
		const LPUINT32 size_T = max(sizeof(T), sizeof(T*));
		const LPUINT32 size = size_T * m_dwNextCount;

		LOG_PROCESS_ERROR(0 != size);

		m_pstFreeNode = (MEM_NODE*)::malloc(size);
		LOG_PROCESS_ERROR(m_pstFreeNode);
		memset(m_pstFreeNode, 0, size);
	
		pstPoolNode = (POOL_NODE*)::malloc(sizeof(POOL_NODE));
		LOG_PROCESS_ERROR(pstPoolNode);

		pstPoolNode->buf_start = (char*)m_pstFreeNode;
		pstPoolNode->buf_end = pstPoolNode->buf_start + size;
		m_list.push_back((char*)pstPoolNode);

		m_pstFreeNode->next = NULL;
		pstMemNode = m_pstFreeNode;

		for (LPUINT32 i = 0; i < m_dwNextCount - 1; ++i)
		{
			pstMemNode->next = (MEM_NODE*)((char*)pstMemNode + sizeof(T));
			pstMemNode = pstMemNode->next;
			pstMemNode->next = NULL;
		}

		m_dwElemCount += m_dwNextCount;

		return TRUE;

	Exit0:

		if (m_pstFreeNode)
		{
			::free(m_pstFreeNode);
			m_pstFreeNode = NULL;
		}

		return FALSE;
	}

	T* LPAPI _Malloc()
	{
		LPINT32 nResult = 0;
		T* p = NULL;

		if (NULL == m_pstFreeNode)
		{
			nResult = _Init();
			LOG_PROCESS_ERROR(nResult);
		}

		p = (T*)m_pstFreeNode;
		m_pstFreeNode = m_pstFreeNode->next;
		++m_dwUseCount;

	Exit0:
		return p;
	}

	void LPAPI _Free(T* pt)
	{
		MEM_NODE* p = (MEM_NODE*)(pt);
		p->next = m_pstFreeNode;
		m_pstFreeNode = p;
		--m_dwUseCount;
	}

private:

	LPUINT32                        m_dwElemCount;
	LPUINT32                        m_dwNextCount;
	LPUINT32                        m_dwUseCount;
	LIST                            m_list;
	MEM_NODE*                       m_pstFreeNode;
	e_ObjPoolErr                    m_eLastErr;
	LPLock*                         m_poLock;
	BOOL                            m_bInit;
};



//end声明所处的名字空间
NS_LZPL_END

#endif