#include "lp_loopbuf.h"
#include "lp_processerror.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN


LPLoopBuf::LPLoopBuf()
{
	m_pBuf            = NULL;
	m_pRead           = NULL;
	m_pWrite          = NULL;
	m_dwDataLen       = 0;
	m_dwBufSize       = 0;
	m_dwRefCount      = 0;
	m_dwPoolId        = BUF_INVALID_POOL_ID;
}

LPLoopBuf::~LPLoopBuf()
{
	UnInit();
}

BOOL LPAPI LPLoopBuf::Init(UINT_32 dwSize, UINT_32 dwPoolId)
{
	INT_32 nResult = 0;


	m_dwBufSize       = dwSize;
	m_dwPoolId        = dwPoolId;

	LOG_PROCESS_ERROR(NULL == m_pBuf);
	m_pBuf = new char[m_dwBufSize];
	LOG_PROCESS_ERROR(m_pBuf);

	m_pRead           = m_pBuf;
	m_pWrite          = m_pBuf;

	m_dwDataLen       = 0;
	m_dwRefCount      = 0;

	return TRUE;
Exit0:

	UnInit();

	return NULL;
}

BOOL LPAPI LPLoopBuf::UnInit()
{
	SAFE_DELETE(m_pBuf);

	m_pRead = NULL;
	m_pWrite = NULL;
	m_dwDataLen = 0;
	m_dwBufSize = 0;
	m_dwRefCount = 0;
	m_dwPoolId = BUF_INVALID_POOL_ID;

	return TRUE;
}

void LPAPI LPLoopBuf::Reset()
{
	m_pRead = m_pBuf;
	m_pWrite = m_pBuf;

	m_dwDataLen = 0;
	m_dwRefCount = 0;
}

UINT_32 LPAPI LPLoopBuf::GetPoolId()
{
	return m_dwPoolId;
}

UINT_32 LPAPI LPLoopBuf::QueryRef()
{
	return m_dwRefCount;
}

void LPAPI LPLoopBuf::AddRef()
{
	++m_dwRefCount;
}

void LPAPI LPLoopBuf::DeductRef()
{
	LOG_CHECK_ERROR(m_dwRefCount > 0);
	if (m_dwRefCount > 0)
	{
		--m_dwRefCount;
	}
}

UINT_32 LPAPI LPLoopBuf::GetTotalReadableLen()
{
	return m_dwDataLen;
}

UINT_32 LPAPI LPLoopBuf::GetOnceReadableLen()
{
	//线性可读大小
	UINT_32 dwLineReadableSize = m_dwBufSize - (UINT_32)(m_pRead - m_pBuf);
	UINT_32 dwTotalReadableLen = GetTotalReadableLen();

	if (dwLineReadableSize > dwTotalReadableLen)
	{
		dwLineReadableSize = dwTotalReadableLen;
	}

	return dwLineReadableSize;
}

UINT_32 LPAPI LPLoopBuf::GetTotalWritableLen()
{
	return m_dwBufSize - m_dwDataLen;
}

UINT_32 LPAPI LPLoopBuf::GetOnceWritableLen()
{
	//线性可写大小
	UINT_32 dwLineWritableSize = m_dwBufSize - (UINT_32)(m_pWrite - m_pBuf);
	UINT_32 dwTotalWritableLen = GetTotalWritableLen();

	if (dwLineWritableSize > dwTotalWritableLen)
	{
		dwLineWritableSize = dwTotalWritableLen;
	}

	return dwLineWritableSize;
}

BOOL LPAPI LPLoopBuf::Read(char* pDst, UINT_32 dwReadLen, BOOL bDoRead, BOOL bNullTerminate)
{
	UINT_32 dwLineSize = 0;

	LOG_PROCESS_ERROR(pDst);
	LOG_PROCESS_ERROR(GetTotalReadableLen() >= dwReadLen);

	dwLineSize = m_dwBufSize - (UINT_32)(m_pRead - m_pBuf);
	if (dwLineSize >= dwReadLen)
	{
		memcpy(pDst, m_pRead, dwReadLen);
		if (bDoRead)
		{
			m_pRead = m_pRead + dwReadLen;
		}
	}
	else
	{
		memcpy(pDst, m_pRead, dwLineSize);
		memcpy(pDst + dwLineSize, m_pBuf, dwReadLen - dwLineSize);
		if (bDoRead)
		{
			m_pRead = m_pBuf + dwReadLen - dwLineSize;
		}
	}

	if (bNullTerminate)
	{
		*(pDst + dwReadLen) = '\0';
	}

	//原子类型
	if (bDoRead)
	{
		m_dwDataLen -= dwReadLen;
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPLoopBuf::Write(const char* pSrc, UINT_32 dwWriteLen)
{
	UINT_32 dwLineSize = 0;

	LOG_PROCESS_ERROR(pSrc);
	LOG_PROCESS_ERROR(GetTotalWritableLen() >= dwWriteLen);

	dwLineSize = m_dwBufSize - (UINT_32)(m_pWrite - m_pBuf);
	if (dwLineSize >= dwWriteLen)
	{
		memcpy(m_pWrite, pSrc, dwWriteLen);
		m_pWrite = m_pWrite + dwWriteLen;
	}
	else
	{
		memcpy(m_pWrite, pSrc, dwLineSize);
		memcpy(m_pBuf, pSrc + dwLineSize, dwWriteLen - dwLineSize);
		m_pWrite = m_pBuf + dwWriteLen - dwLineSize;
	}

	//原子类型
	m_dwDataLen += dwWriteLen;

	return TRUE;
Exit0:
	return FALSE;
}

char* LPAPI LPLoopBuf::ReadPtr()
{
	return m_pRead;
}

char* LPAPI LPLoopBuf::WritePtr()
{
	return m_pWrite;
}

void LPAPI LPLoopBuf::FinishRead(UINT_32 dwReadLen)
{
	UINT_32 dwLineSize = 0;
	UINT_32 dwTotalReadableLen = GetTotalReadableLen();

	LOG_CHECK_ERROR(dwReadLen <= dwTotalReadableLen);

	if (dwReadLen > dwTotalReadableLen)
	{
		dwReadLen = dwTotalReadableLen;
	}

	dwLineSize = m_dwBufSize - (UINT_32)(m_pRead - m_pBuf);
	if (dwLineSize >= dwReadLen)
	{
		m_pRead = m_pRead + dwReadLen;
	}
	else
	{
		m_pRead = m_pBuf + dwReadLen - dwLineSize;
	}
	
	if (m_dwBufSize == m_pRead - m_pBuf)
	{
		m_pRead = m_pBuf;
	}

	//原子类型
	m_dwDataLen -= dwReadLen;
}

void LPAPI LPLoopBuf::FinishWrite(UINT_32 dwWriteLen)
{
	UINT_32 dwLineSize = 0;
	UINT_32 dwTotalWritableLen = GetTotalWritableLen();

	LOG_CHECK_ERROR(dwWriteLen <= dwTotalWritableLen);

	if (dwWriteLen > dwTotalWritableLen)
	{
		dwWriteLen = dwTotalWritableLen;
	}

	dwLineSize = m_dwBufSize - (UINT_32)(m_pWrite - m_pBuf);
	if (dwLineSize >= dwWriteLen)
	{
		m_pWrite = m_pWrite + dwWriteLen;
	}
	else
	{
		m_pWrite = m_pBuf + dwWriteLen - dwLineSize;
	}

	//原子类型
	m_dwDataLen += dwWriteLen;
}

LPLoopBufPool::LPLoopBufPool()
{
	m_bUsePool      = FALSE;
	m_dwSizeBuf     = 0;
	dwPoolInitCount = 0;
	m_dwPoolId      = BUF_INVALID_POOL_ID;

	m_oFreeBufList.clear();
	m_oBufSet.clear();
}

LPLoopBufPool::~LPLoopBufPool()
{
	UnInit();
}

BOOL LPAPI LPLoopBufPool::_PreBatchCreate(UINT_32 dwBatchCount)
{
	INT_32 nResult = 0;
	LPLoopBuf* pLoopBuf = NULL;

	PROCESS_SUCCESS(!m_bUsePool);

	for (UINT_32 i = 0; i < dwBatchCount; ++i)
	{
		pLoopBuf = new LPLoopBuf();
		LOG_PROCESS_ERROR(pLoopBuf);

		nResult = pLoopBuf->Init(m_dwSizeBuf, m_dwPoolId);
		LOG_PROCESS_ERROR(nResult);

		m_oBufSet.insert(pLoopBuf);
		m_oFreeBufList.push_back(pLoopBuf);
	}

Exit1:
	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPLoopBufPool::Init(UINT_32 dwBufSize, BOOL bUsePool, UINT_32 dwPoolInitCount)
{
	INT_32 nResult = 0;

	m_dwPoolId = CreatePoolId();
	m_dwSizeBuf = dwBufSize;
	m_bUsePool = bUsePool;
	dwPoolInitCount = dwPoolInitCount;

	nResult = _PreBatchCreate(dwPoolInitCount);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	UnInit();
	return FALSE;
}

BOOL LPAPI LPLoopBufPool::UnInit()
{
	INT_32 nResult = 0;
	LPSetBuf::iterator it;
	LPLoopBuf* pLoopBuf = NULL;

	PROCESS_SUCCESS(!m_bUsePool);

	it = m_oBufSet.begin();
	for (; it != m_oBufSet.end(); ++it)
	{
		pLoopBuf = (*it);
		LOG_CHECK_ERROR(pLoopBuf);

		SAFE_DELETE(pLoopBuf);
	}
	m_oBufSet.clear();
	m_oFreeBufList.clear();

Exit1:
	return TRUE;
}

UINT_32 LPAPI LPLoopBufPool::GetBufSize()
{
	return m_dwSizeBuf;
}

LPLoopBuf* LPAPI LPLoopBufPool::Create()
{
	INT_32 nResult = 0;
	LPLoopBuf* pLoopBuf = NULL;

	if (m_bUsePool)
	{
		if (!m_oFreeBufList.empty())
		{
			pLoopBuf = m_oFreeBufList.front();
			m_oFreeBufList.pop_front();
			LOG_PROCESS_ERROR(pLoopBuf);
		}
		else
		{
			//每次创建10分之一初始化大小的个数
			nResult = _PreBatchCreate(dwPoolInitCount/10);
			LOG_PROCESS_ERROR(nResult);
			LOG_PROCESS_ERROR(!m_oFreeBufList.empty());

			pLoopBuf = m_oFreeBufList.front();
			m_oFreeBufList.pop_front();
			LOG_PROCESS_ERROR(pLoopBuf);
		}
	}
	else
	{
		pLoopBuf = new LPLoopBuf();
		LOG_PROCESS_ERROR(pLoopBuf);
		nResult = pLoopBuf->Init(m_dwSizeBuf, m_dwPoolId);
		LOG_PROCESS_ERROR(nResult);
	}

	pLoopBuf->Reset();
	if (m_bUsePool) pLoopBuf->AddRef();

	return pLoopBuf;
Exit0:

	if (!m_bUsePool)
	{
		SAFE_DELETE(pLoopBuf);
	}

	return NULL;
}

void LPAPI LPLoopBufPool::Release(LPLoopBuf* &pLoopBuf)
{
	LOG_PROCESS_ERROR(pLoopBuf);

	if (m_bUsePool)
	{
		//判断是否重复释放
		LOG_PROCESS_ERROR(0 != pLoopBuf->QueryRef());

		//判断是否由本池创建的
		LOG_PROCESS_ERROR(m_dwPoolId == pLoopBuf->GetPoolId());

		pLoopBuf->DeductRef();
		m_oFreeBufList.push_back(pLoopBuf);
		pLoopBuf = NULL;
	}
	else
	{
		SAFE_DELETE(pLoopBuf);
	}

Exit0:
	return;
}

UINT_32 LPAPI LPLoopBufPool::CreatePoolId()
{
	UINT_32 dwId = 0;

	LOG_PROCESS_ERROR(m_poLock);

	m_poLock->Lock();
	dwId = ++m_dwBaseId;
	m_poLock->UnLock();

Exit0:
	return dwId;
}

LPLock* LPLoopBufPool::m_poLock = new LPLock();
UINT_32 LPLoopBufPool::m_dwBaseId = BUF_INVALID_POOL_ID;






//end声明所处的名字空间

NS_LZPL_END
