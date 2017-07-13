#include "lp_loopbuf.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



lp_shared_ptr<LZPL::ILPLoopBuf> ILPLoopBuf::CreateBuf(LPUINT32 dwSize)
{
    LPINT32 nResult = FALSE;
    lp_shared_ptr<ILPLoopBuf> pLoopBuf = nullptr;

    pLoopBuf = lp_make_shared<LPLoopBuf>();
    LOG_PROCESS_ERROR(pLoopBuf);

    nResult = ((LPLoopBuf*)pLoopBuf.get())->Init(dwSize);
    LOG_PROCESS_ERROR(nResult);

    return pLoopBuf;

Exit0:

    ReleaseBuf(pLoopBuf);
    return nullptr;
}

void ILPLoopBuf::ReleaseBuf(lp_shared_ptr<ILPLoopBuf>& pLoopBuf)
{
    pLoopBuf = nullptr;
}

LPLoopBuf::LPLoopBuf()
{
    m_pBuf            = NULL;
    m_pRead           = NULL;
    m_pWrite          = NULL;
    m_dwDataLen       = 0;
    m_dwBufSize       = 0;
    m_dwPoolId        = BUF_INVALID_POOL_ID;
}

LPLoopBuf::~LPLoopBuf()
{
    UnInit();
}

BOOL LPAPI LPLoopBuf::Init(LPUINT32 dwSize, LPUINT32 dwPoolId)
{
    m_dwBufSize       = dwSize;
    m_dwPoolId        = dwPoolId;

    LOG_PROCESS_ERROR(NULL == m_pBuf);
    m_pBuf = new char[m_dwBufSize];
    LOG_PROCESS_ERROR(m_pBuf);

    m_pRead           = m_pBuf;
    m_pWrite          = m_pBuf;

    m_dwDataLen       = 0;

    return TRUE;
Exit0:

    UnInit();

    return FALSE;
}

BOOL LPAPI LPLoopBuf::UnInit()
{
    SAFE_DELETE(m_pBuf);

    m_pRead = NULL;
    m_pWrite = NULL;
    m_dwDataLen = 0;
    m_dwBufSize = 0;
    m_dwPoolId = BUF_INVALID_POOL_ID;

    return TRUE;
}

void LPAPI LPLoopBuf::Reset()
{
    m_pRead = m_pBuf;
    m_pWrite = m_pBuf;

    m_dwDataLen = 0;
}

LPUINT32 LPAPI LPLoopBuf::GetPoolId()
{
    return m_dwPoolId;
}

LPUINT32 LPAPI LPLoopBuf::GetTotalReadableLen()
{
    return m_dwDataLen;
}

LPUINT32 LPAPI LPLoopBuf::GetOnceReadableLen()
{
    //线性可读大小
    LPUINT32 dwLineReadableSize = m_dwBufSize - (LPUINT32)(m_pRead - m_pBuf);
    LPUINT32 dwTotalReadableLen = GetTotalReadableLen();

    if(dwLineReadableSize > dwTotalReadableLen)
    {
        dwLineReadableSize = dwTotalReadableLen;
    }

    return dwLineReadableSize;
}

LPUINT32 LPAPI LPLoopBuf::GetTotalWritableLen()
{
    return m_dwBufSize - m_dwDataLen;
}

LPUINT32 LPAPI LPLoopBuf::GetOnceWritableLen()
{
    //线性可写大小
    LPUINT32 dwLineWritableSize = m_dwBufSize - (LPUINT32)(m_pWrite - m_pBuf);
    LPUINT32 dwTotalWritableLen = GetTotalWritableLen();

    if(dwLineWritableSize > dwTotalWritableLen)
    {
        dwLineWritableSize = dwTotalWritableLen;
    }

    return dwLineWritableSize;
}

BOOL LPAPI LPLoopBuf::Read(char* pDst, LPUINT32 dwReadLen, BOOL bDoRead, BOOL bNullTerminate)
{
    LPUINT32 dwLineSize = 0;

    LOG_PROCESS_ERROR(pDst);
    LOG_PROCESS_ERROR(GetTotalReadableLen() >= dwReadLen);

    dwLineSize = m_dwBufSize - (LPUINT32)(m_pRead - m_pBuf);
    if(dwLineSize >= dwReadLen)
    {
        memcpy(pDst, m_pRead, dwReadLen);
        if(bDoRead)
        {
            m_pRead = m_pRead + dwReadLen;
        }
    }
    else
    {
        memcpy(pDst, m_pRead, dwLineSize);
        memcpy(pDst + dwLineSize, m_pBuf, dwReadLen - dwLineSize);
        if(bDoRead)
        {
            m_pRead = m_pBuf + dwReadLen - dwLineSize;
        }
    }

    if(bNullTerminate)
    {
        *(pDst + dwReadLen) = '\0';
    }

    //原子类型
    if(bDoRead)
    {
        m_dwDataLen -= dwReadLen;
    }

    return TRUE;
Exit0:
    return FALSE;
}

BOOL LPAPI LPLoopBuf::Write(const char* pSrc, LPUINT32 dwWriteLen)
{
    LPUINT32 dwLineSize = 0;

    LOG_PROCESS_ERROR(pSrc);
    LOG_PROCESS_ERROR(GetTotalWritableLen() >= dwWriteLen);

    dwLineSize = m_dwBufSize - (LPUINT32)(m_pWrite - m_pBuf);
    if(dwLineSize >= dwWriteLen)
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

void LPAPI LPLoopBuf::FinishRead(LPUINT32 dwReadLen)
{
    LPUINT32 dwLineSize = 0;
    LPUINT32 dwTotalReadableLen = GetTotalReadableLen();

    LOG_CHECK_ERROR(dwReadLen <= dwTotalReadableLen);

    if(dwReadLen > dwTotalReadableLen)
    {
        dwReadLen = dwTotalReadableLen;
    }

    dwLineSize = m_dwBufSize - (LPUINT32)(m_pRead - m_pBuf);
    if(dwLineSize >= dwReadLen)
    {
        m_pRead = m_pRead + dwReadLen;
    }
    else
    {
        m_pRead = m_pBuf + dwReadLen - dwLineSize;
    }

    if(m_dwBufSize == m_pRead - m_pBuf)
    {
        m_pRead = m_pBuf;
    }

    //原子类型
    m_dwDataLen -= dwReadLen;
}

void LPAPI LPLoopBuf::FinishWrite(LPUINT32 dwWriteLen)
{
    LPUINT32 dwLineSize = 0;
    LPUINT32 dwTotalWritableLen = GetTotalWritableLen();

    LOG_CHECK_ERROR(dwWriteLen <= dwTotalWritableLen);

    if(dwWriteLen > dwTotalWritableLen)
    {
        dwWriteLen = dwTotalWritableLen;
    }

    dwLineSize = m_dwBufSize - (LPUINT32)(m_pWrite - m_pBuf);
    if(dwLineSize >= dwWriteLen)
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

lp_shared_ptr<LZPL::ILPLoopBufPool> ILPLoopBufPool::CreatePool()
{
    return lp_make_shared<LPLoopBufPool>();
}


void LZPL::ILPLoopBufPool::ReleasePool(lp_shared_ptr<ILPLoopBufPool>& pLoopBufPool)
{
    pLoopBufPool = nullptr;
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

BOOL LPAPI LPLoopBufPool::PreBatchCreate(LPUINT32 dwBatchCount)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPLoopBuf> pLoopBuf = nullptr;

    PROCESS_SUCCESS(!m_bUsePool);

    for(LPUINT32 i = 0; i < dwBatchCount; ++i)
    {
        pLoopBuf = lp_make_shared<LPLoopBuf>();
        LOG_PROCESS_ERROR(pLoopBuf);

        nResult = ((LPLoopBuf*)pLoopBuf.get())->Init(m_dwSizeBuf, m_dwPoolId);
        LOG_PROCESS_ERROR(nResult);

        m_oBufSet.insert(pLoopBuf);
        m_oFreeBufList.push_back(pLoopBuf);
    }

Exit1:
    return TRUE;
Exit0:
    return FALSE;
}

BOOL LPAPI LPLoopBufPool::Init(LPUINT32 dwBufSize, BOOL bUsePool, LPUINT32 dwPoolInitCount)
{
    LPINT32 nResult = 0;

    m_dwPoolId = CreatePoolId();
    m_dwSizeBuf = dwBufSize;
    m_bUsePool = bUsePool;
    dwPoolInitCount = dwPoolInitCount;

    nResult = PreBatchCreate(dwPoolInitCount);
    LOG_PROCESS_ERROR(nResult);

    return TRUE;
Exit0:

    UnInit();
    return FALSE;
}

BOOL LPAPI LPLoopBufPool::UnInit()
{
    PROCESS_SUCCESS(!m_bUsePool);

    m_oBufSet.clear();
    m_oFreeBufList.clear();

Exit1:
    return TRUE;
}

LPUINT32 LPAPI LPLoopBufPool::GetBufSize()
{
    return m_dwSizeBuf;
}

lp_shared_ptr<ILPLoopBuf> LPAPI LPLoopBufPool::Create()
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPLoopBuf> pLoopBuf = nullptr;

    if(m_bUsePool)
    {
        if(!m_oFreeBufList.empty())
        {
            pLoopBuf = m_oFreeBufList.front();
            m_oFreeBufList.pop_front();
            LOG_PROCESS_ERROR(pLoopBuf);
        }
        else
        {
            //每次创建10分之一初始化大小的个数
            nResult = PreBatchCreate(dwPoolInitCount / 10);
            LOG_PROCESS_ERROR(nResult);
            LOG_PROCESS_ERROR(!m_oFreeBufList.empty());

            pLoopBuf = m_oFreeBufList.front();
            m_oFreeBufList.pop_front();
            LOG_PROCESS_ERROR(pLoopBuf);
        }
    }
    else
    {
        pLoopBuf = lp_make_shared<LPLoopBuf>();
        LOG_PROCESS_ERROR(pLoopBuf);
        nResult = ((LPLoopBuf*)pLoopBuf.get())->Init(m_dwSizeBuf, m_dwPoolId);
        LOG_PROCESS_ERROR(nResult);
    }

    ((LPLoopBuf*)pLoopBuf.get())->Reset();

    return pLoopBuf;
Exit0:

    if(!m_bUsePool)
    {
        pLoopBuf = nullptr;
    }

    return nullptr;
}

void LPAPI LPLoopBufPool::Release(lp_shared_ptr<ILPLoopBuf>& pLoopBuf)
{
    LOG_PROCESS_ERROR(pLoopBuf);

    if(m_bUsePool)
    {
        //判断是否由本池创建的
        LOG_PROCESS_ERROR(m_dwPoolId == ((LPLoopBuf*)pLoopBuf.get())->GetPoolId());

        m_oFreeBufList.push_back(pLoopBuf);
        pLoopBuf = nullptr;
    }
    else
    {
        pLoopBuf = nullptr;
    }

Exit0:
    return;
}

LPUINT32 LPAPI LPLoopBufPool::CreatePoolId()
{
    LPUINT32 dwId = 0;

    LOG_PROCESS_ERROR(m_poLock);

    m_poLock->Lock();
    dwId = ++m_dwBaseId;
    m_poLock->UnLock();

Exit0:
    return dwId;
}

LPLock* LPLoopBufPool::m_poLock = new LPLock();
LPUINT32 LPLoopBufPool::m_dwBaseId = BUF_INVALID_POOL_ID;



//end声明所处的名字空间

NS_LZPL_END
