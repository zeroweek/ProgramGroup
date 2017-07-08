#include "lp_sqlmgr.h"
#include "mysql.h"
#include "zlib.h"
#include "lp_string.h"
#include "lp_system.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define SQL_PREALLOCATE_STMT_COUNT          (8)
#define SQL_MAX_DB_QUERY_COUNT              (65535)
#define SQL_MAX_RETRY_COUNT                 (1)
#define SQL_MAX_PARAM_BUFFER_SIZE           (64 * 1024)



LPSqlMgr::LPSqlMgr()
{
    m_nSqlStmtCount             = 0;
    m_pSqlStmtList              = NULL;
    m_dwServerPort              = 0;
    m_nThreadCount              = 0;
    m_pThread                   = NULL;
    m_bStopThread               = FALSE;
    m_nThreadIndex              = 0;
    m_pSqlStmtPool              = NULL;
    m_nNewSqlStmtCount          = 0;
    m_nDelSqlStmtCount          = 0;
}

LPSqlMgr::~LPSqlMgr()
{
    UnInit();
}

BOOL LPAPI LPSqlMgr::Init(
    const char * pcszServerAddr,
    const char * pcszUserName,
    const char * pcszPwd,
    const char * pcszDbName,
    const char * pcszCharSet,
    LPUINT32 dwServerPort,
    SQL_STMT_DATA* pSqlStmtList,
    LPINT32 nSqlStmtCount,
    LPINT32 nThreadCount)
{

    LPINT32 nResult = 0;
    LPSqlStmt* pSqlStmt = NULL;

    LOG_PROCESS_ERROR(pcszServerAddr);
    LOG_PROCESS_ERROR(pcszUserName);
    LOG_PROCESS_ERROR(pcszPwd);
    LOG_PROCESS_ERROR(pcszDbName);
    LOG_PROCESS_ERROR(pcszCharSet);
    LOG_PROCESS_ERROR(pSqlStmtList);
    LOG_PROCESS_ERROR(nThreadCount < SQL_MAX_DB_WORKER_THREAD_COUNT);

    nResult = mysql_library_init(0, NULL, NULL);
    LOG_PROCESS_ERROR(0 == nResult);

    m_pSqlStmtList = new SQL_STMT_DATA[nSqlStmtCount];
    LOG_PROCESS_ERROR(m_pSqlStmtList);

    memcpy(m_pSqlStmtList, pSqlStmtList, sizeof(SQL_STMT_DATA) * nSqlStmtCount);
    m_nSqlStmtCount = nSqlStmtCount;
    m_nThreadIndex = 0;

    m_pSqlStmtPool = new LPBaseList[m_nSqlStmtCount];
    LOG_PROCESS_ERROR(m_pSqlStmtPool);

    for(LPINT32 nIndex = 0; nIndex < m_nSqlStmtCount; ++nIndex)
    {
        LOG_PROCESS_ERROR(nIndex == m_pSqlStmtList[nIndex].nSqlStmtId);

        nResult = _ParseSqlStmt(m_pSqlStmtList + nIndex);
        LOG_PROCESS_ERROR(nResult);
    }

    for(LPINT32 nSqlStmtId = 0; nSqlStmtId < m_nSqlStmtCount; ++nSqlStmtId)
    {
        m_pSqlStmtPool[nSqlStmtId].Clear();
        for(LPINT32 nIndex = 0; nIndex < SQL_PREALLOCATE_STMT_COUNT; ++nIndex)
        {
            pSqlStmt = new LPSqlStmt();
            LOG_PROCESS_ERROR(pSqlStmt);

            nResult = pSqlStmt->Init(this, nSqlStmtId);
            LOG_PROCESS_ERROR(nResult);

            nResult = m_pSqlStmtPool[nSqlStmtId].PushRear(pSqlStmt);
            LOG_PROCESS_ERROR(nResult);

            pSqlStmt = NULL;
        }
    }

    lpStrCpyN(m_szServerAddr, pcszServerAddr, COMMON_NAME_LEN);
    lpStrCpyN(m_szUserName, pcszUserName, COMMON_NAME_LEN);
    lpStrCpyN(m_szPwd, pcszPwd, COMMON_NAME_LEN);
    lpStrCpyN(m_szDbName, pcszDbName, COMMON_NAME_LEN);
    lpStrCpyN(m_szCharSet, pcszCharSet, COMMON_NAME_LEN);

    m_dwServerPort = dwServerPort;
    m_bStopThread = FALSE;
    m_nThreadCount = nThreadCount;
    m_pThread = new std::thread*[m_nThreadCount];
    for(LPINT32 nIndex = 0; nIndex < m_nThreadCount; ++nIndex)
    {
        nResult = m_PushList[nIndex].Init(SQL_MAX_DB_QUERY_COUNT);
        LOG_PROCESS_ERROR(nResult);

        nResult = m_PopList[nIndex].Init(SQL_MAX_DB_QUERY_COUNT);
        LOG_PROCESS_ERROR(nResult);

        SQL_THREAD_PARAM* pParam = new SQL_THREAD_PARAM;
        pParam->pSqlMgr = this;
        pParam->nIndex = nIndex;

        m_pThread[nIndex] = new std::thread(_WorkerThreadFunc, pParam);
    }

    return TRUE;

Exit0:

    SAFE_DELETE(pSqlStmt);
    UnInit();

    return FALSE;
}

BOOL LPAPI LPSqlMgr::UnInit(void)
{
    LPINT32 nResult = 0;

    m_bStopThread = TRUE;
    for(LPINT32 nIndex = 0; nIndex < m_nThreadCount; ++nIndex)
    {
        m_pThread[nIndex]->join();
        SAFE_DELETE(m_pThread[nIndex]);
    }
    SAFE_DELETE_SZ(m_pThread);

    for(LPINT32 nIndex = 0; nIndex < m_nSqlStmtCount; ++nIndex)
    {
        BASE_LIST_FOR_BEGIN(m_pSqlStmtPool[nIndex])
        {
            LPSqlStmt* pSqlStmt = (LPSqlStmt*)ptNode;
            LOG_CHECK_ERROR(pSqlStmt);
            IF_NULL_CONTINUE(pSqlStmt);

            nResult = pSqlStmt->UnInit();
            LOG_CHECK_ERROR(nResult);

            SAFE_DELETE(pSqlStmt);
        }
        BASE_LIST_FOR_END

        m_pSqlStmtPool[nIndex].Clear();
    }
    m_nSqlStmtCount = 0;

    SAFE_DELETE_SZ(m_pSqlStmtPool);
    SAFE_DELETE_SZ(m_pSqlStmtList);

    mysql_library_end();

    return TRUE;
}

BOOL LPAPI LPSqlMgr::MainLoop(void)
{
    LPINT32 nResult = 0;

    for(LPINT32 nIndex = 0; nIndex < m_nThreadCount; ++nIndex)
    {
        while(m_PopList[nIndex].CanPop())
        {
            LPSqlStmt* pSqlStmt = (LPSqlStmt*)m_PopList->Pop();
            LOG_PROCESS_ERROR(pSqlStmt);

            if(pSqlStmt->m_pfnCallback && pSqlStmt->m_nExecuteCount <= SQL_MAX_RETRY_COUNT)
            {
                nResult = pSqlStmt->m_pfnCallback(pSqlStmt->m_lstResult, pSqlStmt->m_pUserData);
                LOG_CHECK_ERROR(nResult);
            }

            nResult = _DelSqlStmt(pSqlStmt);
            LOG_CHECK_ERROR(nResult);
        }
    }

Exit0:
    return TRUE;
}

BOOL LPAPI LPSqlMgr::AsyncExecSqlStmt(LPINT32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void * pUserData, const char * pcszFormat, ...)
{
    va_list valist;
    LPSqlStmt* pSqlStmt = NULL;

    va_start(valist, pcszFormat);
    pSqlStmt = _ExecSqlStmt(nSqlStmtId, pfnCallback, pUserData, pcszFormat, valist);
    va_end(valist);
    LOG_PROCESS_ERROR(nullptr != pSqlStmt);

    return TRUE;
Exit0:
    return FALSE;
}

BOOL LPAPI LPSqlMgr::SyncExecSqlStmt(LPINT32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void * pUserData, const char * pcszFormat, ...)
{
    LPINT32 nResult = 0;
    va_list valist;
    LPSqlStmt* pSqlStmt = NULL;
    BOOL bCompleted = FALSE;

    va_start(valist, pcszFormat);
    pSqlStmt = _ExecSqlStmt(nSqlStmtId, pfnCallback, pUserData, pcszFormat, valist);
    va_end(valist);
    LOG_PROCESS_ERROR(nullptr != pSqlStmt);

    while(!bCompleted)
    {
        for(LPINT32 nIndex = 0; nIndex < m_nThreadCount; ++nIndex)
        {
            while(m_PopList[nIndex].CanPop())
            {
                LPSqlStmt* pTempSqlStmt = (LPSqlStmt*)m_PopList[nIndex].Pop();
                LOG_PROCESS_ERROR(pTempSqlStmt);

                if(pTempSqlStmt->m_pfnCallback)
                {
                    nResult = pTempSqlStmt->m_pfnCallback(pTempSqlStmt->m_lstResult, pTempSqlStmt->m_pUserData);
                    LOG_PROCESS_ERROR(nResult);
                }

                if(pSqlStmt == pTempSqlStmt)
                {
                    bCompleted = TRUE;
                }

                nResult = _DelSqlStmt(pTempSqlStmt);
                LOG_CHECK_ERROR(nResult);
            }
        }
    }

    return TRUE;
Exit0:
    return FALSE;
}

const SQL_STMT_DATA* LPAPI LPSqlMgr::GetSqlStmtData(LPINT32 nSqlStmtId)
{
    LOG_PROCESS_ERROR(nSqlStmtId >= 0 && nSqlStmtId < m_nSqlStmtCount);

    return m_pSqlStmtList + nSqlStmtId;

Exit0:
    return NULL;
}

BOOL LPAPI LPSqlMgr::_ParseSqlStmt(SQL_STMT_DATA* pSqlStmtData)
{
    LPINT32 nResult = 0;
    const char* pcszSql = NULL;
    BOOL bParamEscaped = FALSE;
    BOOL bResultEscaped = FALSE;

    LOG_PROCESS_ERROR(pSqlStmtData);
    LOG_PROCESS_ERROR(pSqlStmtData->pcszSql);
    pcszSql = pSqlStmtData->pcszSql;

    pSqlStmtData->nQuerySize = 0;
    pSqlStmtData->nParamCount = 0;
    pSqlStmtData->nParamBufSize = 0;
    pSqlStmtData->nResultCount = 0;
    pSqlStmtData->nResultBufSize = 0;
    pSqlStmtData->nBufSize = 0;

    while(*pcszSql)
    {
        switch(*pcszSql)
        {
        case '[':
            {
                LOG_PROCESS_ERROR(!bParamEscaped && !bResultEscaped);
                bParamEscaped = TRUE;
                pSqlStmtData->szQuery[pSqlStmtData->nQuerySize++] = '?';
            }
            break;
        case ']':
            {
                LOG_PROCESS_ERROR(bParamEscaped);
                bParamEscaped = FALSE;
            }
            break;
        case '{':
            {
                LOG_PROCESS_ERROR(!bParamEscaped && !bResultEscaped);
                bResultEscaped = TRUE;
            }
            break;
        case '}':
            {
                LOG_PROCESS_ERROR(bResultEscaped);
                bResultEscaped = FALSE;
            }
            break;
        case 'c':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_TINY;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = sizeof(LPINT8);
                    pSqlStmtData->nParamBufSize += pSqlStmtData->nParamSize[pSqlStmtData->nParamCount];
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_TINY;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = sizeof(LPINT8);
                    pSqlStmtData->nResultBufSize += pSqlStmtData->nResultSize[pSqlStmtData->nResultCount];
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 'h':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_SHORT;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = sizeof(LPINT16);
                    pSqlStmtData->nParamBufSize += pSqlStmtData->nParamSize[pSqlStmtData->nParamCount];
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_SHORT;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = sizeof(LPINT16);
                    pSqlStmtData->nResultBufSize += pSqlStmtData->nResultSize[pSqlStmtData->nResultCount];
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 'd':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_LONG;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = sizeof(LPINT32);
                    pSqlStmtData->nParamBufSize += pSqlStmtData->nParamSize[pSqlStmtData->nParamCount];
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_LONG;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = sizeof(LPINT32);
                    pSqlStmtData->nResultBufSize += pSqlStmtData->nResultSize[pSqlStmtData->nResultCount];
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 'l':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_LONGLONG;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = sizeof(LPINT64);
                    pSqlStmtData->nParamBufSize += pSqlStmtData->nParamSize[pSqlStmtData->nParamCount];
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_LONGLONG;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = sizeof(LPINT64);
                    pSqlStmtData->nResultBufSize += pSqlStmtData->nResultSize[pSqlStmtData->nResultCount];
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 's':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_STRING;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = atoi(pcszSql + 1) + 1;
                    LOG_PROCESS_ERROR(pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] > 0);
                    pSqlStmtData->nParamBufSize += (sizeof(LPUINT32) + pSqlStmtData->nParamSize[pSqlStmtData->nParamCount]);
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_STRING;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = atoi(pcszSql + 1) + 1;
                    LOG_PROCESS_ERROR(pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] > 0);
                    pSqlStmtData->nResultBufSize += pSqlStmtData->nResultSize[pSqlStmtData->nResultCount];
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 'b':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_BLOB;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = atoi(pcszSql + 1);
                    LOG_PROCESS_ERROR(pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] > 0);
                    pSqlStmtData->nParamBufSize += (sizeof(LPUINT32) + pSqlStmtData->nParamSize[pSqlStmtData->nParamCount]);
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_BLOB;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = atoi(pcszSql + 1);
                    LOG_PROCESS_ERROR(pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] > 0);
                    pSqlStmtData->nResultBufSize += (sizeof(LPUINT32) + pSqlStmtData->nResultSize[pSqlStmtData->nResultCount]);
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 'z':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_LONG_BLOB;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = atoi(pcszSql + 1);       // 还是使用原来buf的最大大小，超出概率小？
                    LOG_PROCESS_ERROR(pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] > 0);
                    pSqlStmtData->nParamBufSize += (sizeof(LPUINT32) + pSqlStmtData->nParamSize[pSqlStmtData->nParamCount]);
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_LONG_BLOB;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = atoi(pcszSql + 1);
                    LOG_PROCESS_ERROR(pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] > 0);
                    pSqlStmtData->nResultBufSize += (sizeof(LPUINT32) + pSqlStmtData->nResultSize[pSqlStmtData->nResultCount]);
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 't':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->nParamType[pSqlStmtData->nParamCount] = MYSQL_TYPE_DATETIME;
                    pSqlStmtData->nParamSize[pSqlStmtData->nParamCount] = sizeof(MYSQL_TIME);
                    pSqlStmtData->nParamBufSize += pSqlStmtData->nParamSize[pSqlStmtData->nParamCount];
                    pSqlStmtData->nParamCount++;
                }
                else if(bResultEscaped)
                {
                    pSqlStmtData->nResultType[pSqlStmtData->nResultCount] = MYSQL_TYPE_DATETIME;
                    pSqlStmtData->nResultSize[pSqlStmtData->nResultCount] = sizeof(MYSQL_TIME);
                    pSqlStmtData->nResultBufSize += pSqlStmtData->nResultSize[pSqlStmtData->nResultCount];
                    pSqlStmtData->nResultCount++;
                }
            }
            break;
        case 'u':
            {
                if(bParamEscaped)
                {
                    pSqlStmtData->bIsUnsigned[pSqlStmtData->nParamCount] = TRUE;
                }
                else if(bResultEscaped)
                {

                }
            }
            break;
        default:
            LOG_PROCESS_ERROR(isdigit(*pcszSql) || (!bParamEscaped && !bResultEscaped));
            break;
        }

        if(!bParamEscaped && !bResultEscaped && *pcszSql != ']' && *pcszSql != '}')
        {
            pSqlStmtData->szQuery[pSqlStmtData->nQuerySize++] = *pcszSql;
        }

        ++pcszSql;
    }

    LOG_PROCESS_ERROR(pSqlStmtData->nParamBufSize < SQL_MAX_PARAM_BUFFER_SIZE);
    LOG_PROCESS_ERROR(pSqlStmtData->nResultBufSize < SQL_MAX_PARAM_BUFFER_SIZE);
    pSqlStmtData->nBufSize = pSqlStmtData->nParamBufSize > pSqlStmtData->nResultBufSize ?
                             pSqlStmtData->nParamBufSize : pSqlStmtData->nResultBufSize;

    return TRUE;

Exit0:

    FTL("Fail to parse sql stmt : %s", pSqlStmtData->pcszSql);

    return FALSE;
}

LPSqlStmt *LPAPI LPSqlMgr::_NewSqlStmt(LPINT32 nSqlStmtId)
{
    LPINT32 nResult = 0;
    LPSqlStmt* pSqlStmt = NULL;

    LOG_PROCESS_ERROR(nSqlStmtId >= 0 && nSqlStmtId < m_nSqlStmtCount);

    if(m_pSqlStmtPool[nSqlStmtId].IsEmpty())
    {
        pSqlStmt = new LPSqlStmt();
        nResult = pSqlStmt->Init(this, nSqlStmtId);
        LOG_PROCESS_ERROR(nResult);
    }
    else
    {
        pSqlStmt = (LPSqlStmt*)m_pSqlStmtPool[nSqlStmtId].PopHead();
    }
    LOG_PROCESS_ERROR(pSqlStmt);

    pSqlStmt->m_nExecuteCount = 0;
    pSqlStmt->m_nParamCount = 0;
    pSqlStmt->m_nParamBufSize = 0;

    m_nNewSqlStmtCount++;

    return pSqlStmt;

Exit0:

    if(pSqlStmt)
    {
        _DelSqlStmt(pSqlStmt);
    }

    return NULL;
}

BOOL LPAPI LPSqlMgr::_DelSqlStmt(LPSqlStmt * pSqlStmt)
{
    LPINT32 nResult = 0;

    LOG_PROCESS_ERROR(pSqlStmt);
    LOG_PROCESS_ERROR(pSqlStmt->m_pSqlStmtData);
    LOG_PROCESS_ERROR(pSqlStmt->m_pSqlStmtData->nSqlStmtId >= 0 && pSqlStmt->m_pSqlStmtData->nSqlStmtId < m_nSqlStmtCount);

    for(std::list<void*>::iterator it = pSqlStmt->m_lstResult.begin();
            it != pSqlStmt->m_lstResult.end(); ++it)
    {
        SAFE_DELETE_SZ(*it);
    }
    pSqlStmt->m_lstResult.clear();

    nResult = m_pSqlStmtPool[pSqlStmt->m_pSqlStmtData->nSqlStmtId].PushRear(pSqlStmt);
    LOG_PROCESS_ERROR(nResult);

    m_nDelSqlStmtCount++;

    return TRUE;
Exit0:
    return FALSE;
}

LPSqlStmt *LPAPI LPSqlMgr::_ExecSqlStmt(LPINT32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void * pUserData, const char * pcszFormat, va_list valist)
{
    LPINT32 nResult = 0;
    LPSqlStmt* pSqlStmt = NULL;
    BOOL bUnsigned = FALSE;

    LOG_PROCESS_ERROR(pcszFormat);

    pSqlStmt = _NewSqlStmt(nSqlStmtId);
    LOG_PROCESS_ERROR(pSqlStmt);

    pSqlStmt->m_pfnCallback = pfnCallback;
    pSqlStmt->m_pUserData = pUserData;

    while(*pcszFormat)
    {
        switch(*pcszFormat)
        {
        case 'c':
            {
                if(bUnsigned)
                {
                    LPUINT8 ucValue = va_arg(valist, LPINT32);
                    nResult = pSqlStmt->PushParam(ucValue);
                    LOG_PROCESS_ERROR(nResult);
                }
                else
                {
                    LPINT8 cValue = va_arg(valist, LPINT32);
                    nResult = pSqlStmt->PushParam(cValue);
                    LOG_PROCESS_ERROR(nResult);
                }

                bUnsigned = FALSE;
            }
            break;
        case 'h':
            {
                if(bUnsigned)
                {
                    LPUINT16 uhValue = va_arg(valist, LPINT32);
                    nResult = pSqlStmt->PushParam(uhValue);
                    LOG_PROCESS_ERROR(nResult);
                }
                else
                {
                    LPINT16 hValue = va_arg(valist, LPINT32);
                    nResult = pSqlStmt->PushParam(hValue);
                    LOG_PROCESS_ERROR(nResult);
                }

                bUnsigned = FALSE;
            }
            break;
        case 'd':
            {
                if(bUnsigned)
                {
                    LPUINT32 udValue = va_arg(valist, LPUINT32);
                    nResult = pSqlStmt->PushParam(udValue);
                    LOG_PROCESS_ERROR(nResult);
                }
                else
                {
                    LPINT32 dValue = va_arg(valist, LPINT32);
                    nResult = pSqlStmt->PushParam(dValue);
                    LOG_PROCESS_ERROR(nResult);
                }

                bUnsigned = FALSE;
            }
            break;
        case 'l':
            {
                if(bUnsigned)
                {
                    LPUINT64 ulValue = va_arg(valist, LPUINT64);
                    nResult = pSqlStmt->PushParam(ulValue);
                    LOG_PROCESS_ERROR(nResult);
                }
                else
                {
                    LPINT64 lValue = va_arg(valist, LPINT64);
                    nResult = pSqlStmt->PushParam(lValue);
                    LOG_PROCESS_ERROR(nResult);
                }

                bUnsigned = FALSE;
            }
            break;
        case 't':
            {
                LOG_PROCESS_ERROR(bUnsigned == FALSE);
                time_t tValue = va_arg(valist, time_t);
                nResult = pSqlStmt->PushTime(tValue);
                LOG_PROCESS_ERROR(nResult);
            }
            break;
        case 's':
            {
                LOG_PROCESS_ERROR(bUnsigned == FALSE);
                const char* pcszValue = va_arg(valist, const char*);
                nResult = pSqlStmt->PushParam(pcszValue);
                LOG_PROCESS_ERROR(nResult);
            }
            break;
        case 'b':
            {
                LPUINT8* pucValue = va_arg(valist, LPUINT8*);
                LPUINT32 dwValueSize = va_arg(valist, LPUINT32);
                nResult = pSqlStmt->PushParam(pucValue, dwValueSize);
                LOG_PROCESS_ERROR(nResult);
            }
            break;
        case 'z':
            {
                LPUINT8* pucValue = va_arg(valist, LPUINT8*);
                LPUINT32 dwValueSize = va_arg(valist, LPUINT32);
                nResult = pSqlStmt->PushParamZ(pucValue, dwValueSize);
                LOG_PROCESS_ERROR(nResult);
            }
            break;
        case 'u':
            {
                bUnsigned = TRUE;
            }
            break;
        default:
            LOG_PROCESS_ERROR(FALSE);
            break;
        }
        pcszFormat++;
    }

    if(pSqlStmt->m_pSqlStmtData && pSqlStmt->m_pSqlStmtData->nThreadIndex >= 0)
    {
        nResult = m_PushList[pSqlStmt->m_pSqlStmtData->nThreadIndex % m_nThreadCount].Push(pSqlStmt);
        LOG_PROCESS_ERROR(nResult);
    }
    else
    {
        nResult = m_PushList[m_nThreadIndex].Push(pSqlStmt);
        LOG_PROCESS_ERROR(nResult);

        m_nThreadIndex = (m_nThreadIndex + 1) % m_nThreadIndex;
    }

    return pSqlStmt;

Exit0:

    FTL("Fail to execute sql stmt %d", nSqlStmtId);
    if(pSqlStmt)
    {
        _DelSqlStmt(pSqlStmt);
    }

    return NULL;
}

void LPAPI LPSqlMgr::_WorkerThreadFunc(SQL_THREAD_PARAM* pstSqlThreadParam)
{
    struct SQL_THREAD_DATA
    {
        LPUINT8*     pucBuf;
        MYSQL_STMT* pStmt;
        MYSQL_BIND* pParamBind;
        MYSQL_BIND* pResultBind;

    };

    LPINT32                   nResult = 0;
    LPINT32                   nThreadIndex = 0;
    BOOL                     bInited = FALSE;
    MYSQL*                   pMysql = NULL;
    LPSqlMgr*                pSqlMgr = NULL;
    SQL_THREAD_PARAM         stThreadParam;
    SQL_THREAD_DATA*         pstThreadData = NULL;
    char                     szCharList[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    stThreadParam.pSqlMgr = pstSqlThreadParam->pSqlMgr;
    stThreadParam.nIndex = pstSqlThreadParam->nIndex;
    SAFE_DELETE(pstSqlThreadParam);

    pSqlMgr = stThreadParam.pSqlMgr;
    nThreadIndex = stThreadParam.nIndex;
    LOG_PROCESS_ERROR(pSqlMgr);

    while(!pSqlMgr->m_bStopThread)
    {
        if(!bInited)
        {
            pMysql = mysql_init(NULL);
            LOG_PROCESS_ERROR(pMysql);

            pMysql = mysql_real_connect(
                         pMysql,
                         pSqlMgr->m_szServerAddr,
                         pSqlMgr->m_szUserName,
                         pSqlMgr->m_szPwd,
                         pSqlMgr->m_szDbName,
                         pSqlMgr->m_dwServerPort,
                         NULL,
                         0);
            LOG_PROCESS_ERROR(pMysql);

            if(pSqlMgr->m_szCharSet[0])
            {
                nResult = mysql_set_character_set(pMysql, pSqlMgr->m_szCharSet);
                LOG_PROCESS_ERROR(0 == nResult);
            }

            pstThreadData = new SQL_THREAD_DATA[pSqlMgr->m_nSqlStmtCount];
            LOG_PROCESS_ERROR(pstThreadData);
            memset(pstThreadData, 0, sizeof(SQL_THREAD_DATA) * pSqlMgr->m_nSqlStmtCount);

            for(LPINT32 nStmtIndex = 0; nStmtIndex < pSqlMgr->m_nSqlStmtCount; ++nStmtIndex)
            {
                LPINT32 nBufSize = 0;
                SQL_STMT_DATA* pstSqlStmtData = pSqlMgr->m_pSqlStmtList + nStmtIndex;

                pstThreadData[nStmtIndex].pStmt = mysql_stmt_init(pMysql);
                LOG_PROCESS_ERROR(pstThreadData[nStmtIndex].pStmt);

                nResult = mysql_stmt_prepare(pstThreadData[nStmtIndex].pStmt, pstSqlStmtData->szQuery, pstSqlStmtData->nQuerySize);
                LOG_PROCESS_ERROR(0 == nResult);

                pstThreadData[nStmtIndex].pucBuf = new LPUINT8[pstSqlStmtData->nBufSize];
                LOG_PROCESS_ERROR(pstThreadData[nStmtIndex].pucBuf);

                nResult = mysql_stmt_param_count(pstThreadData[nStmtIndex].pStmt);
                LOG_PROCESS_ERROR(nResult == pstSqlStmtData->nParamCount);

                if(pstSqlStmtData->nParamCount > 0)
                {
                    pstThreadData[nStmtIndex].pParamBind = new MYSQL_BIND[pstSqlStmtData->nParamCount];
                    LOG_PROCESS_ERROR(pstThreadData[nStmtIndex].pParamBind);
                    memset(pstThreadData[nStmtIndex].pParamBind, 0, sizeof(MYSQL_BIND) * pstSqlStmtData->nParamCount);

                    for(LPINT32 nParamIndex = 0; nParamIndex < pstSqlStmtData->nParamCount; ++ nParamIndex)
                    {
                        pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_type = (enum_field_types)pstSqlStmtData->nParamType[nParamIndex];

                        if(MYSQL_TYPE_LONG_BLOB == pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_type)
                        {
                            pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_type = MYSQL_TYPE_BLOB;
                        }

                        if(MYSQL_TYPE_STRING == pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_type)
                        {
                            pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_length = pstSqlStmtData->nParamSize[nParamIndex] - 1;
                        }
                        else
                        {
                            pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_length = pstSqlStmtData->nParamSize[nParamIndex];
                        }

                        if(MYSQL_TYPE_STRING == pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_type ||
                                MYSQL_TYPE_BLOB == pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer_type)
                        {
                            pstThreadData[nStmtIndex].pParamBind[nParamIndex].length = (unsigned long*)(pstThreadData[nStmtIndex].pucBuf + nBufSize);
                            nBufSize += sizeof(unsigned long);
                        }

                        pstThreadData[nStmtIndex].pParamBind[nParamIndex].buffer = pstThreadData[nStmtIndex].pucBuf + nBufSize;
                        nBufSize += pstSqlStmtData->nParamSize[nParamIndex];
                    }

                    nResult = mysql_stmt_bind_param(pstThreadData[nStmtIndex].pStmt, pstThreadData[nStmtIndex].pParamBind);
                    LOG_PROCESS_ERROR(0 == nResult);
                }

                if(pstSqlStmtData->nResultCount > 0)
                {
                    pstThreadData[nStmtIndex].pResultBind = new MYSQL_BIND[pstSqlStmtData->nResultCount];
                    LOG_PROCESS_ERROR(pstThreadData[nStmtIndex].pResultBind);
                    memset(pstThreadData[nStmtIndex].pResultBind, 0, sizeof(MYSQL_BIND) * pstSqlStmtData->nResultCount);

                    //<UNDONE>...
                    nBufSize = 0;
                    for(LPINT32 nResultIndex = 0; nResultIndex < pstSqlStmtData->nResultCount; ++nResultIndex)
                    {
                        pstThreadData[nStmtIndex].pResultBind[nResultIndex].buffer_type = (enum_field_types)pstSqlStmtData->nResultType[nResultIndex];

                        if(MYSQL_TYPE_LONG_BLOB == pstThreadData[nStmtIndex].pResultBind[nResultIndex].buffer_type)
                        {
                            pstThreadData[nStmtIndex].pResultBind[nResultIndex].buffer_type = MYSQL_TYPE_BLOB;
                        }

                        pstThreadData[nStmtIndex].pResultBind[nResultIndex].buffer_length = pstSqlStmtData->nResultSize[nResultIndex];

                        if(MYSQL_TYPE_BLOB == pstThreadData[nStmtIndex].pResultBind[nResultIndex].buffer_type)
                        {
                            pstThreadData[nStmtIndex].pResultBind[nResultIndex].length = (unsigned long *)(pstThreadData[nStmtIndex].pucBuf + nBufSize);
                            nBufSize += sizeof(unsigned long);
                        }

                        pstThreadData[nStmtIndex].pResultBind[nResultIndex].buffer = pstThreadData[nStmtIndex].pucBuf + nBufSize;
                        nBufSize += pstSqlStmtData->nResultSize[nResultIndex];
                    }

                    nResult = mysql_stmt_bind_result(pstThreadData[nStmtIndex].pStmt, pstThreadData[nStmtIndex].pResultBind);
                    LOG_PROCESS_ERROR(0 == nResult);
                }
            }

            INF("db worker thread %d initialized successfully !", nThreadIndex);
            bInited = TRUE;
        }

        if(pSqlMgr->m_PushList[nThreadIndex].Check() && bInited)
        {
            LPINT32 nSqlStmtId = 0;
            LPSqlStmt* pSqlStmt = (LPSqlStmt*)pSqlMgr->m_PushList[nThreadIndex].Check();
            LOG_PROCESS_ERROR(pSqlStmt);
            LOG_PROCESS_ERROR(pSqlStmt->m_pSqlStmtData);

            nSqlStmtId = pSqlStmt->m_pSqlStmtData->nSqlStmtId;

            if(pSqlStmt->m_nExecuteCount > SQL_MAX_RETRY_COUNT)
            {
                char szDumpData[SQL_MAX_PARAM_BUFFER_SIZE * 2];
                for(LPINT32 nIndex = 0; nIndex < pSqlStmt->m_nParamBufSize; ++nIndex)
                {
                    if(nIndex % 2 == 0)
                    {
                        szDumpData[nIndex] = szCharList[(pSqlStmt->m_pucParamBuf[nIndex / 2] >> 4) & 0xF];
                    }
                    else
                    {
                        szDumpData[nIndex] = szCharList[pSqlStmt->m_pucParamBuf[nIndex / 2] & 0xF];
                    }
                }

                szDumpData[pSqlStmt->m_nParamBufSize] = 0;
                FTL("Retry count of sql stmt %d exceeds limit. data dump: %s", nSqlStmtId, szDumpData);
                pSqlMgr->m_PushList[nThreadIndex].Pop();
                pSqlMgr->m_PopList[nThreadIndex].Push(pSqlStmt);
                continue;
            }
            pSqlStmt->m_nExecuteCount++;

            memcpy(pstThreadData[nSqlStmtId].pucBuf, pSqlStmt->m_pucParamBuf, pSqlStmt->m_pSqlStmtData->nParamBufSize);

            nResult = mysql_stmt_execute(pstThreadData[nSqlStmtId].pStmt);
            if(0 != nResult)
            {
                FTL("Fail to execute mysql stmt %d (%s)", nSqlStmtId, pSqlStmt->m_pSqlStmtData->pcszSql);
                goto Exit0;
            }

            nResult = mysql_stmt_fetch(pstThreadData[nSqlStmtId].pStmt);
            while(0 == nResult || MYSQL_DATA_TRUNCATED == nResult)
            {
                LOG_CHECK_ERROR(MYSQL_DATA_TRUNCATED != nResult);

                void* pData = new LPUINT8[pSqlStmt->m_pSqlStmtData->nResultBufSize];
                LOG_PROCESS_ERROR(pData);

                bool bHasZBlob = false;
                for(LPINT32 nResultIndex = 0; nResultIndex < pSqlStmt->m_pSqlStmtData->nResultCount; ++nResultIndex)
                {
                    if((enum_field_types)pSqlStmt->m_pSqlStmtData->nResultType[nResultIndex] == MYSQL_TYPE_LONG_BLOB)
                    {
                        bHasZBlob = true;
                        break;
                    }
                }

                if(bHasZBlob)
                {
                    LPINT32 nBufSize = 0;
                    unsigned long ulActualZipLength = 0;
                    unsigned long* pZBlobLength = 0;

                    for(LPINT32 nResultIndex = 0; nResultIndex < pSqlStmt->m_pSqlStmtData->nResultCount; ++nResultIndex)
                    {
                        if(MYSQL_TYPE_BLOB == pstThreadData[nSqlStmtId].pResultBind[nResultIndex].buffer_type)
                        {
                            pZBlobLength = pstThreadData[nSqlStmtId].pResultBind[nResultIndex].length;
                            ulActualZipLength = *(pstThreadData[nSqlStmtId].pResultBind[nResultIndex].length);
                            nBufSize += sizeof(unsigned long);
                        }

                        if(MYSQL_TYPE_LONG_BLOB == pstThreadData[nSqlStmtId].pResultBind[nResultIndex].buffer_type)
                        {
                            LPINT32 nRet = 0;
                            unsigned long ulLength = pSqlStmt->m_pSqlStmtData->nResultSize[nResultIndex];
                            nRet = uncompress((unsigned char*)pData + nBufSize, &ulLength,
                                              (unsigned char*)pstThreadData[nSqlStmtId].pResultBind[nResultIndex].buffer, ulActualZipLength);
                            if(Z_OK != nRet)
                            {
                                FTL("unzip failed, zipped data len:%u, zipped data crc:%X", pSqlStmt->m_pSqlStmtData->nResultSize[nResultIndex],
                                    crc32(0, (const Bytef*)pstThreadData[nSqlStmtId].pResultBind[nResultIndex].buffer, ulActualZipLength));
                            }

                            *pZBlobLength = ulLength;
                        }
                        else
                        {
                            memcpy((char*)pData + nBufSize, pstThreadData[nSqlStmtId].pResultBind[nResultIndex].buffer, pSqlStmt->m_pSqlStmtData->nResultSize[nResultIndex]);
                        }

                        nBufSize += pSqlStmt->m_pSqlStmtData->nResultSize[nResultIndex];
                    }
                }
                else
                {
                    memcpy(pData, pstThreadData[nSqlStmtId].pucBuf, pSqlStmt->m_pSqlStmtData->nResultBufSize);
                }

                pSqlStmt->m_lstResult.push_back(pData);

                nResult = mysql_stmt_fetch(pstThreadData[nSqlStmtId].pStmt);
            }

            nResult = mysql_stmt_free_result(pstThreadData[nSqlStmtId].pStmt);
            LOG_PROCESS_ERROR(0 == nResult);

            pSqlMgr->m_PushList[nThreadIndex].Pop();
            pSqlMgr->m_PopList[nThreadIndex].Push(pSqlStmt);
        }
        else
        {
            lpSleep(1);
        }

        //_try
        //{

        //}
        //__except (...)
        //{
        //  goto Exit0;
        //}

        continue;
    }



Exit0:

    FTL("*** %s excute sql ", __FUNCTION__);
    if(mysql_errno(pMysql) != 0)
    {
        FTL("*** %s mysql error %d (%s)", __FUNCTION__, mysql_errno(pMysql), mysql_error(pMysql));
    }

    if(pstThreadData && pSqlMgr)
    {
        for(LPINT32 nIndex = 0; nIndex < pSqlMgr->m_nSqlStmtCount; ++nIndex)
        {
            if(pstThreadData[nIndex].pStmt)
            {
                mysql_stmt_close(pstThreadData[nIndex].pStmt);
            }
        }

        for(LPINT32 nStmtIndex = 0; nStmtIndex < pSqlMgr->m_nSqlStmtCount; ++nStmtIndex)
        {
            SAFE_DELETE_SZ(pstThreadData[nStmtIndex].pucBuf);
            SAFE_DELETE_SZ(pstThreadData[nStmtIndex].pParamBind);
            SAFE_DELETE_SZ(pstThreadData[nStmtIndex].pResultBind);
        }

        SAFE_DELETE_SZ(pstThreadData);

        if(pMysql)
        {
            mysql_close(pMysql);
            pMysql = NULL;
        }

        bInited = FALSE;
        lpSleep(8 * 1000);
    }

    return ;
}



//end声明所处的名字空间

NS_LZPL_END