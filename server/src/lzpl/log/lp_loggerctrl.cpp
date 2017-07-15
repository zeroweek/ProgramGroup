#include "lp_loggerctrl.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_processerror.h"
#include "tinyxml.h"
#include "tinystr.h"
#include <stdarg.h>



//begin声明所处的名字空间
NS_LZPL_BEGIN



const char* g_pLogLevelPrompt_Unknow    = "[UNK]";
const char* g_pLogLevelPrompt_Debug     = "[DBG]";
const char* g_pLogLevelPrompt_Info      = "[INF]";
const char* g_pLogLevelPrompt_Warn      = "[WRN]";
const char* g_pLogLevelPrompt_Important = "[IMP]";
const char* g_pLogLevelPrompt_Error     = "[ERR]";
const char* g_pLogLevelPrompt_Fatal     = "[FTL]";
const char* g_pLogLevelPrompt_Lua       = "[LUA]";


lp_shared_ptr<LZPL::ILPLoggerCtrl> ILPLoggerCtrl::m_pLzplLoggerCtrl = nullptr;

DECLARE void LPAPI lpSetConsoleColor(e_ConsoleTextColor textColor /*= CTC_WHITE*/, e_ConsoleBackGroundColor backgroundColor /*= CBC_BLACK*/)
{
#   ifdef _WIN32
    {
        //STD_ERROR_HANDLE;
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, textColor | backgroundColor);
    }
#   else
    {

    }
#   endif
}

BOOL LPAPI ILPLoggerCtrl::InitLzplLoggerCtrl(LOG_CONFIG& stLogConfig)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPFileLogger> pFileLogger = nullptr;

    m_pLzplLoggerCtrl = ILPLoggerCtrl::CreateLoggerCtrl();
    PRINTF_PROCESS_ERROR(m_pLzplLoggerCtrl);

    pFileLogger = ILPFileLogger::CreateFileLogger();
    PRINTF_PROCESS_ERROR(pFileLogger);

    nResult = pFileLogger->Init(stLogConfig);
    PRINTF_PROCESS_ERROR(nResult);

    nResult = m_pLzplLoggerCtrl->Init(pFileLogger, stLogConfig);
    PRINTF_PROCESS_ERROR(nResult);

    return TRUE;

Exit0:

    ILPFileLogger::DeleteFileLogger(pFileLogger);
    ILPLoggerCtrl::DeleteLoggerCtrl(m_pLzplLoggerCtrl);

    return FALSE;
}

BOOL LPAPI ILPLoggerCtrl::SetLzplLoggerCtrl(lp_shared_ptr<ILPLoggerCtrl> pLzplLoggerCtrl)
{
    m_pLzplLoggerCtrl = pLzplLoggerCtrl;
    LOG_PROCESS_ERROR(m_pLzplLoggerCtrl);

    return TRUE;
Exit0:
    return FALSE;
}

lp_shared_ptr<LZPL::ILPLoggerCtrl> LPAPI ILPLoggerCtrl::GetLzplLoggerCtrl()
{
    return m_pLzplLoggerCtrl;
}

lp_shared_ptr<LZPL::ILPLoggerCtrl> LPAPI ILPLoggerCtrl::CreateLoggerCtrl()
{
    return lp_make_shared<LPLoggerCtrl>();
}

void LPAPI ILPLoggerCtrl::DeleteLoggerCtrl(lp_shared_ptr<ILPLoggerCtrl>& pLoggerCtrl)
{
    pLoggerCtrl = nullptr;
}



LPLoggerCtrl::LPLoggerCtrl()
{
    m_pLogger                                = NULL;
    m_bLogLock                               = TRUE;
    m_dwLogLevelConfig                       = eLogLevel_All;
    m_szModulePrefix[0]                      = '\0';
}

LPLoggerCtrl::~LPLoggerCtrl()
{
    UnInit();
}

BOOL LPAPI LPLoggerCtrl::Init(lp_shared_ptr<ILPLogger> pLogger, LOG_CONFIG& stLogConfig)
{
    PRINTF_PROCESS_ERROR(pLogger);

    m_pLogger = pLogger;

    m_stLogConfig = stLogConfig;

    m_bLogLock = stLogConfig.bWrithLock;

    if(strcmp(stLogConfig.szModulePrefix, "") != 0)
    {
        lpStrCpyN(m_szModulePrefix, stLogConfig.szModulePrefix, COMMON_NAME_LEN);
    }

    m_dwLogLevelConfig = stLogConfig.dwLogLevelConfig;
    m_dwOutputMask = stLogConfig.dwOutputMask;

    return TRUE;

Exit0:

    UnInit();

    return FALSE;
}

BOOL LPAPI LPLoggerCtrl::UnInit(void)
{
    m_pLogger = nullptr;

    return TRUE;
}

void LPAPI LPLoggerCtrl::Lua(const char * format, ...)
{
    PRINTF_PROCESS_ERROR(format);

    if(eLogLevel_Lua & m_dwLogLevelConfig)
    {
        va_list args;
        va_start(args, format);
        _Log(eLogLevel_Lua, format, args);
        va_end(args);
    }

    return;
Exit0:
    return;
}

void LPAPI LPLoggerCtrl::Fatal(const char* format, ...)
{
    PRINTF_PROCESS_ERROR(format);

    if(eLogLevel_Fatal & m_dwLogLevelConfig)
    {
        va_list args;
        va_start(args, format);
        _Log(eLogLevel_Fatal, format, args);
        va_end(args);
    }

    return;
Exit0:
    return;
}

void LPAPI LPLoggerCtrl::Error(const char* format, ...)
{
    PRINTF_PROCESS_ERROR(format);

    if(eLogLevel_Error & m_dwLogLevelConfig)
    {
        va_list args;
        va_start(args, format);
        _Log(eLogLevel_Error, format, args);
        va_end(args);
    }

    return;
Exit0:
    return;
}

void LPAPI LPLoggerCtrl::Warn(const char* format, ...)
{
    PRINTF_PROCESS_ERROR(format);

    if(eLogLevel_Warn & m_dwLogLevelConfig)
    {
        va_list args;
        va_start(args, format);
        _Log(eLogLevel_Warn, format, args);
        va_end(args);
    }

    return;
Exit0:
    return;
}

void LPAPI LPLoggerCtrl::Important(const char* format, ...)
{
    PRINTF_PROCESS_ERROR(format);

    if(eLogLevel_Important & m_dwLogLevelConfig)
    {
        va_list args;
        va_start(args, format);
        _Log(eLogLevel_Important, format, args);
        va_end(args);
    }

    return;
Exit0:
    return;
}

void LPAPI LPLoggerCtrl::Info(const char* format, ...)
{
    PRINTF_PROCESS_ERROR(format);

    if(eLogLevel_Info & m_dwLogLevelConfig)
    {
        va_list args;
        va_start(args, format);
        _Log(eLogLevel_Info, format, args);
        va_end(args);
    }

    return;
Exit0:
    return;
}

void LPAPI LPLoggerCtrl::Debug(const char* format, ...)
{
    PRINTF_PROCESS_ERROR(format);

    if(eLogLevel_Debug & m_dwLogLevelConfig)
    {
        va_list args;
        va_start(args, format);
        _Log(eLogLevel_Debug, format, args);
        va_end(args);
    }

    return;
Exit0:
    return;
}

void LPAPI LPLoggerCtrl::_Log(e_LogLevel eLogLevel, const char* format, va_list argptr)
{
    static char s_szErrorFlag[] = "*** ";
    static char s_szNormalFlag[] = "";

    static LPINT32 s_nMsgBufSize = m_stLogConfig.dwMaxLogOneMsgSize;
    static char* s_pszMsg = new char[s_nMsgBufSize];

    LPINT32 nResult = 0;
    LPINT32 nCount = 0;
    LPINT32 nTotalCount = 0;
    char* pszFlag = s_szNormalFlag;
    LPTime oCurTime = LPTime::GetNowTime();

    PRINTF_PROCESS_ERROR(m_pLogger);
    PRINTF_PROCESS_ERROR(eLogLevel > eLogLevel_Off);
    PRINTF_PROCESS_ERROR(eLogLevel <= eLogLevel_All);

    if(m_bLogLock) m_oLock.Lock();

    //错误等级日志添加标记
    if(eLogLevel == eLogLevel_Error || eLogLevel == eLogLevel_Fatal)
    {
        pszFlag = s_szErrorFlag;
    }

    lpStrFastZero(s_pszMsg);
    if('\0' != m_szModulePrefix[0])
    {
        nCount = snprintf(s_pszMsg, s_nMsgBufSize, "[%4d-%02d-%02d %02d:%02d:%02d,%06d] [%08d] [%s] %s %s",
                          oCurTime.GetYear(),
                          oCurTime.GetMon(),
                          oCurTime.GetMday(),
                          oCurTime.GetHour(),
                          oCurTime.GetMin(),
                          oCurTime.GetSec(),
                          oCurTime.GetUsec(),
                          m_pLogger->CreateMsgId(),
                          m_szModulePrefix,
                          _GetLogLevelPrompt(eLogLevel),
                          pszFlag);
    }
    else
    {
        nCount = snprintf(s_pszMsg, s_nMsgBufSize, "[%4d-%02d-%02d %02d:%02d:%02d,%06d] [%08d] %s %s",
                          oCurTime.GetYear(),
                          oCurTime.GetMon(),
                          oCurTime.GetMday(),
                          oCurTime.GetHour(),
                          oCurTime.GetMin(),
                          oCurTime.GetSec(),
                          oCurTime.GetUsec(),
                          m_pLogger->CreateMsgId(),
                          _GetLogLevelPrompt(eLogLevel),
                          pszFlag);
    }
    PRINTF_PROCESS_ERROR(nCount >= 0);
    nTotalCount += nCount;

    PRINTF_PROCESS_ERROR(s_nMsgBufSize >= nCount);
#   ifdef _WIN32
    {
        //缓冲区大小m（包含终止符），最多输入n长度（会在后面加上终止符，但是n不包含终止符）
        nResult = vsnprintf_s(s_pszMsg + nCount, s_nMsgBufSize - nCount, s_nMsgBufSize - nCount, format, argptr);
        PRINTF_PROCESS_ERROR(nResult >= 0);
    }
#   else
    {
        //不指定缓冲区，最多输入n长度（会在后面加上终止符，n包含终止符）
        nResult = vsnprintf(s_pszMsg + nCount, s_nMsgBufSize - nCount, format, argptr);
        PRINTF_PROCESS_ERROR(nResult >= 0 && nResult < s_nMsgBufSize - nCount);
    }
#   endif
    nTotalCount += nResult;
    PRINTF_CHECK_ERROR(nTotalCount < s_nMsgBufSize);

    // 这里多加一个null终止符字节，用于写file线程省略memcpy操作
    m_pLogger->LogText(s_pszMsg, (LPUINT32)(nTotalCount + 1));

    if(m_dwOutputMask & eOutputType_Console)
    {
        switch(eLogLevel)
        {
        case LZPL::eLogLevel_Debug:
            {
                lpSetConsoleColor(CTC_GRAY);
                LPPRINTF("%s\n", s_pszMsg);
                lpSetConsoleColor();
            }
            break;
        case LZPL::eLogLevel_Warn:
            {
                lpSetConsoleColor(CTC_YELLOW);
                LPPRINTF("%s\n", s_pszMsg);
                lpSetConsoleColor();
            }
            break;
        case LZPL::eLogLevel_Important:
            {
                lpSetConsoleColor(CTC_GREEN);
                LPPRINTF("%s\n", s_pszMsg);
                lpSetConsoleColor();
            }
            break;
        case LZPL::eLogLevel_Error:
        case LZPL::eLogLevel_Fatal:
            {
                lpSetConsoleColor(CTC_RED);
                LPPRINTF("%s\n", s_pszMsg);
                lpSetConsoleColor();
            }
            break;
        default:
            {
                LPPRINTF("%s\n", s_pszMsg);
            }
            break;
        }
    }

    if(m_dwOutputMask & eOutputType_Debug)
    {
#       ifdef _WIN32
        {
            if(IsDebuggerPresent())
            {
                LP_OUTPUT_DEBUG_STRING(s_pszMsg);
                LP_OUTPUT_DEBUG_STRING("\n");
            }
        }
#       endif
    }

    if(m_bLogLock) m_oLock.UnLock();

Exit0:
    return;
}

const char *LPAPI LPLoggerCtrl::_GetLogLevelPrompt(e_LogLevel eLogLevel)
{
    switch(eLogLevel)
    {
    case LZPL::eLogLevel_Debug:
        return g_pLogLevelPrompt_Debug;
        break;
    case LZPL::eLogLevel_Info:
        return g_pLogLevelPrompt_Info;
        break;
    case LZPL::eLogLevel_Warn:
        return g_pLogLevelPrompt_Warn;
        break;
    case LZPL::eLogLevel_Important:
        return g_pLogLevelPrompt_Important;
        break;
    case LZPL::eLogLevel_Error:
        return g_pLogLevelPrompt_Error;
        break;
    case LZPL::eLogLevel_Fatal:
        return g_pLogLevelPrompt_Fatal;
        break;
    case LZPL::eLogLevel_Lua:
        return g_pLogLevelPrompt_Lua;
        break;
    default:
        PRINTF_PROCESS_ERROR(FALSE);
        break;
    }

Exit0:
    return g_pLogLevelPrompt_Unknow;
}



//end声明所处的名字空间
NS_LZPL_END
