#include "lp_lock.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPLock::LPLock()
{
#   ifdef _WIN32
    {
        INIT_CRITICAL_SECTION(&m_stCs);
    }
#   else
    {
        INIT_CRITICAL_SECTION(&m_stCs, NULL);
    }
#   endif
}

LPLock::~LPLock()
{
    DELETE_CRITICAL_SECTION(&m_stCs);
}

void LPAPI LPLock::Lock()
{
    ENTER_CRITICAL_SECTION(&m_stCs);
}

void LPAPI LPLock::UnLock()
{
    LEAVE_CRITICAL_SECTION(&m_stCs);
}

LPAutoLock::LPAutoLock()
{
    m_bNew = TRUE;
    m_poLock = new LPLock();
    LOG_CHECK_ERROR(m_poLock);
    if(NULL != m_poLock)
    {
        m_poLock->Lock();
    }
}

LPAutoLock::LPAutoLock(LPLock* pLock) : m_poLock(pLock)
{
    LOG_CHECK_ERROR(m_poLock);

    m_bNew = FALSE;
    if(NULL != m_poLock)
    {
        m_poLock->Lock();
    }
}

LPAutoLock::~LPAutoLock()
{
    if(NULL != m_poLock)
    {
        m_poLock->UnLock();
        if(m_bNew)
        {
            SAFE_DELETE(m_poLock);
        }
        else
        {
            m_poLock = NULL;
        }
    }
}



//end声明所处的名字空间
NS_LZPL_END