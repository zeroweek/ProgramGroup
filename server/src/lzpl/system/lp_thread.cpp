#include "lp_thread.h"
#include "lp_system.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPThread::LPThread()
{
    m_bStart = FALSE;
    m_tId = 0;
    m_hHandle = INVALID_THREAD_HANDLE;
}

LPThread::~LPThread()
{
    if(m_hHandle != INVALID_THREAD_HANDLE)
    {
        Terminate();
        m_hHandle = INVALID_THREAD_HANDLE;
    }
}

static THREAD_HANDLE _CreateThread(pfunThrdProc pfThrdProc, void* pParam, THREAD_ID* ptId, BOOL bSuspend)
{
#   ifdef _WIN32
    {
        int nThrdFlag = bSuspend ? CREATE_SUSPENDED : 0;
        return (THREAD_HANDLE)_beginthreadex(NULL, 0, pfThrdProc, pParam, nThrdFlag, ptId);
    }
#   else
    {
        int nRet = 0;
        pthread_t tTID;
        nRet = pthread_create(&tTID, nullptr, pfThrdProc, pParam);
        if(nRet != 0)
        {
            return INVALID_THREAD_HANDLE;
        }
        else
        {
            *ptId = (THREAD_ID)tTID;
            return tTID;
        }
    }
#   endif
}

static LPINT32 _WaitThread(THREAD_HANDLE hHandle, LPUINT32 dwWaitTime)
{
#   ifdef _WIN32
    {
        return ::WaitForSingleObject(hHandle, dwWaitTime);
    }
#   else
    {
        return pthread_join(hHandle, nullptr);
    }
#   endif
}

static void _TerminateThread(THREAD_HANDLE hHandle)
{
#   ifdef _WIN32
    {
        TerminateThread(hHandle, 0);
        lpCloseHandle(hHandle);
    }
#   else
    {
        pthread_cancel(hHandle);
    }
#   endif
}

BOOL LPAPI LPThread::Start(void)
{
    if(!m_bStart)
    {
        m_hHandle = _CreateThread(DefaultThrdProc, this, &m_tId, FALSE);
        LOG_PROCESS_ERROR(INVALID_THREAD_HANDLE != m_hHandle);

        m_bStart = TRUE;
    }

Exit0:
    return m_bStart;
}

BOOL LPAPI LPThread::Start(pfunThrdProc pfThrdProc, void * pParam)
{
    if(!m_bStart)
    {
        m_hHandle = _CreateThread(pfThrdProc, pParam, &m_tId, FALSE);
        LOG_PROCESS_ERROR(INVALID_THREAD_HANDLE != m_hHandle);

        m_bStart = TRUE;
    }

Exit0:
    return m_bStart;
}

void LPAPI LPThread::Wait(LPUINT32 dwWaitTime)
{
    if(m_bStart)
    {
        m_bStart = FALSE;
        _WaitThread(m_hHandle, dwWaitTime);
    }
}

void LPAPI LPThread::Terminate(void)
{
    if(m_bStart)
    {
        _TerminateThread(m_hHandle);
        m_bStart = FALSE;
    }
}

THREAD_ID LPAPI LPThread::GetId(void)
{
    return m_tId;
}

THREAD_FUNC_DECLARE(LPThread::DefaultThrdProc)(void * pParam)
{
    LPThread* pThread = static_cast<LPThread*>(pParam);
    if(NULL != pThread)
    {
        pThread->ThrdProc();
    }

    return 0;
}



//end声明所处的名字空间
NS_LZPL_END