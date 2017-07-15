#include "lp_lzpl.h"
#include "boost/asio/impl/src.hpp"
#include "lp_processerror.h"
#include "tinyxml.h"
#include "tinystr.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



BOOL LPAPI LP::InitLzpl(LOG_CONFIG& stLogConfig)
{
    LPINT32 nResult = 0;

    GlobalAssert();

    nResult = LPTime::GlobalInit(stLogConfig.nTimezone);
    PRINTF_PROCESS_ERROR(nResult);

    nResult = ILPLoggerCtrl::InitLzplLoggerCtrl(stLogConfig);
    PRINTF_PROCESS_ERROR(nResult);

    return TRUE;

Exit0:
    return FALSE;
}

void LPAPI LP::UnInitLzpl()
{

}

BOOL LPAPI LP::LoadLogConfig(const char* pszLogConfigDir, const char* pszLogConfigFileName, LOG_CONFIG& stLogConfig)
{
    bool bResult = false;
    BOOL nResult = FALSE;
    //LPINT32 nNodeValue = 0;
    char szPath[MAX_PATH];
    TiXmlDocument* poDocument = NULL;
    TiXmlElement* poRootElement = NULL;
    TiXmlNode* poNode = NULL;
    const char* pcszNodeValue = NULL;

    LPINT32 WrithLock = 0;
    LPINT32 LogMode = 0;
    LPINT32 LogLevel_DBG = 0;
    LPINT32 LogLevel_INF = 0;
    LPINT32 LogLevel_WRN = 0;
    LPINT32 LogLevel_IMP = 0;
    LPINT32 LogLevel_ERR = 0;
    LPINT32 LogLevel_FTL = 0;
    LPINT32 LogLevel_LUA = 0;
    LPINT32 OutputMask_File = 0;
    LPINT32 OutputMask_Console = 0;
    LPINT32 OutputMask_Debug = 0;
    LPINT32 MaxLogLoopBufSize = 0;
    LPINT32 MaxLogSingleFileSize = 0;
    LPINT32 MaxLogOneMsgSize = 0;
    LPINT32 LogFlushInterval = 0;
    LPINT32 Timezone = 0;

    //设置默认值
    memset(&stLogConfig, 0, sizeof(stLogConfig));

    //合成配置文件路径
    PRINTF_PROCESS_ERROR(NULL != pszLogConfigDir);
    PRINTF_PROCESS_ERROR(NULL != pszLogConfigFileName);
    memset(&szPath, 0, sizeof(szPath));
    lpStrCpyN(szPath, pszLogConfigDir, MAX_PATH);
    lpStrCatN(szPath, "\\", MAX_PATH);
    lpStrCatN(szPath, pszLogConfigFileName, MAX_PATH);
    lpPathFilter(szPath, MAX_PATH);

    //加载配置文件
    poDocument = new TiXmlDocument(szPath);
    PRINTF_PROCESS_ERROR(NULL != poDocument);

    bResult = poDocument->LoadFile();
    PRINTF_PROCESS_ERROR(bResult);

    poRootElement = poDocument->RootElement();
    PRINTF_PROCESS_ERROR(NULL != poRootElement);

    //读取参数：日志存储目录
    poNode = poRootElement->FirstChild("LogDir");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    PRINTF_PROCESS_ERROR_WITH_MSG(strcmp(pcszNodeValue, "") != 0, "LogDir is empty");
    lpStrCpyN(stLogConfig.szLogDir, lpGetWorkingDir(), MAX_PATH);
    lpStrCatN(stLogConfig.szLogDir, "\\", MAX_PATH);
    lpStrCatN(stLogConfig.szLogDir, pcszNodeValue, MAX_PATH);
    lpPathFilter(stLogConfig.szLogDir, MAX_PATH);

    //读取参数：日志文件名
    poNode = poRootElement->FirstChild("FileName");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    PRINTF_PROCESS_ERROR_WITH_MSG(strcmp(pcszNodeValue, "") != 0, "FileName is empty");
    lpStrCpyN(stLogConfig.szFileName, pcszNodeValue, MAX_PATH);

    ////读取参数：日志模块前缀
    //poNode = poRootElement->FirstChild("ModulePrefix");
    //PRINTF_PROCESS_ERROR(NULL != poNode);
    //pcszNodeValue = poNode->ToElement()->Attribute("value");
    //PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    //lpStrCpyN(stLogConfig.szModulePrefix, pcszNodeValue, MAX_PATH);

    //读取参数：是否写锁
    poNode = poRootElement->FirstChild("WrithLock");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    WrithLock = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(WrithLock == 0 || WrithLock == 1);
    if(WrithLock == 0)
    {
        stLogConfig.bWrithLock = FALSE;
    }
    else
    {
        stLogConfig.bWrithLock = TRUE;
    }

    //读取参数：日志模式
    poNode = poRootElement->FirstChild("LogMode");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogMode = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogMode > eLogMode_None);
    PRINTF_PROCESS_ERROR(LogMode < eLogMode_Max);
    stLogConfig.dwLogMode = LogMode;

    //读取参数：日志等级LogLevel_DBG
    poNode = poRootElement->FirstChild("LogLevel_DBG");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogLevel_DBG = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogLevel_DBG == 0 || LogLevel_DBG == 1);
    if(LogLevel_DBG == 1)
    {
        stLogConfig.dwLogLevelConfig |= eLogLevel_Debug;
    }

    //读取参数：日志等级LogLevel_INF
    poNode = poRootElement->FirstChild("LogLevel_INF");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogLevel_INF = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogLevel_INF == 0 || LogLevel_INF == 1);
    if(LogLevel_INF == 1)
    {
        stLogConfig.dwLogLevelConfig |= eLogLevel_Info;
    }

    //读取参数：日志等级LogLevel_WRN
    poNode = poRootElement->FirstChild("LogLevel_WRN");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogLevel_WRN = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogLevel_WRN == 0 || LogLevel_WRN == 1);
    if(LogLevel_WRN == 1)
    {
        stLogConfig.dwLogLevelConfig |= eLogLevel_Warn;
    }

    //读取参数：日志等级LogLevel_IMP
    poNode = poRootElement->FirstChild("LogLevel_IMP");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogLevel_IMP = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogLevel_IMP == 0 || LogLevel_IMP == 1);
    if(LogLevel_IMP == 1)
    {
        stLogConfig.dwLogLevelConfig |= eLogLevel_Important;
    }

    //读取参数：日志等级LogLevel_ERR
    poNode = poRootElement->FirstChild("LogLevel_ERR");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogLevel_ERR = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogLevel_ERR == 0 || LogLevel_ERR == 1);
    if(LogLevel_ERR == 1)
    {
        stLogConfig.dwLogLevelConfig |= eLogLevel_Error;
    }

    //读取参数：日志等级LogLevel_FTL
    poNode = poRootElement->FirstChild("LogLevel_FTL");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogLevel_FTL = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogLevel_FTL == 0 || LogLevel_FTL == 1);
    if(LogLevel_FTL == 1)
    {
        stLogConfig.dwLogLevelConfig |= eLogLevel_Fatal;
    }

    //读取参数：日志等级LogLevel_LUA
    poNode = poRootElement->FirstChild("LogLevel_LUA");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogLevel_LUA = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogLevel_LUA == 0 || LogLevel_LUA == 1);
    if(LogLevel_LUA == 1)
    {
        stLogConfig.dwLogLevelConfig |= eLogLevel_Lua;
    }

    //读取参数：输出掩码OutputMask_File
    poNode = poRootElement->FirstChild("OutputMask_File");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    OutputMask_File = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(OutputMask_File == 0 || OutputMask_File == 1);
    if(OutputMask_File == 1)
    {
        stLogConfig.dwOutputMask |= eOutputType_File;
    }

    //读取参数：输出掩码OutputMask_Console
    poNode = poRootElement->FirstChild("OutputMask_Console");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    OutputMask_Console = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(OutputMask_Console == 0 || OutputMask_Console == 1);
    if(OutputMask_Console == 1)
    {
        stLogConfig.dwOutputMask |= eOutputType_Console;
    }

    //读取参数：输出掩码OutputMask_Debug
    poNode = poRootElement->FirstChild("OutputMask_Debug");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    OutputMask_Debug = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(OutputMask_Debug == 0 || OutputMask_Debug == 1);
    if(OutputMask_Debug == 1)
    {
        stLogConfig.dwOutputMask |= eOutputType_Debug;
    }

    //读取参数：MaxLogLoopBufSize
    poNode = poRootElement->FirstChild("MaxLogLoopBufSize");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    MaxLogLoopBufSize = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(MaxLogLoopBufSize >= 0);
    PRINTF_CHECK_ERROR(MaxLogLoopBufSize == 0 || MaxLogLoopBufSize >= 10 * MB);
    if(MaxLogLoopBufSize == 0 || MaxLogLoopBufSize < 10 * MB)
    {
        stLogConfig.dwMaxLogLoopBufSize = 10 * MB;
    }
    else
    {
        stLogConfig.dwMaxLogLoopBufSize = MaxLogLoopBufSize;
    }

    //读取参数：MaxLogSingleFileSize
    poNode = poRootElement->FirstChild("MaxLogSingleFileSize");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    MaxLogSingleFileSize = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(MaxLogSingleFileSize >= 0);
    PRINTF_CHECK_ERROR(MaxLogSingleFileSize == 0 || MaxLogSingleFileSize >= 10 * MB);
    if(MaxLogSingleFileSize == 0 || MaxLogSingleFileSize < 10 * MB)
    {
        stLogConfig.dwMaxLogSingleFileSize = 10 * MB;
    }
    else
    {
        stLogConfig.dwMaxLogSingleFileSize = MaxLogSingleFileSize;
    }

    //读取参数：MaxLogOneMsgSize
    poNode = poRootElement->FirstChild("MaxLogOneMsgSize");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    MaxLogOneMsgSize = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(MaxLogOneMsgSize >= 0);
    PRINTF_CHECK_ERROR(MaxLogOneMsgSize == 0 || MaxLogOneMsgSize >= 1 * MB);
    if(MaxLogOneMsgSize == 0 || MaxLogOneMsgSize < 1 * MB)
    {
        stLogConfig.dwMaxLogOneMsgSize = 1 * MB;
    }
    else
    {
        stLogConfig.dwMaxLogOneMsgSize = MaxLogOneMsgSize;
    }

    //读取参数：LogFlushInterval
    poNode = poRootElement->FirstChild("LogFlushInterval");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Digit, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    LogFlushInterval = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(LogFlushInterval >= 0);
    PRINTF_CHECK_ERROR(LogFlushInterval == 0 || LogFlushInterval >= 3 * 1000);
    if(LogFlushInterval == 0 || LogFlushInterval < 3 * 1000)
    {
        stLogConfig.dwLogFlushInterval = 3 * 1000;
    }
    else
    {
        stLogConfig.dwLogFlushInterval = LogFlushInterval;
    }

    //读取参数：Timezone
    poNode = poRootElement->FirstChild("Timezone");
    PRINTF_PROCESS_ERROR(NULL != poNode);
    pcszNodeValue = poNode->ToElement()->Attribute("value");
    PRINTF_PROCESS_ERROR(NULL != pcszNodeValue);
    nResult = lpStrCharCheck(pcszNodeValue, eCharCheckType_Integer, -1, FALSE);
    PRINTF_PROCESS_ERROR(nResult);
    Timezone = atoi(pcszNodeValue);
    PRINTF_PROCESS_ERROR(TIME_ZONE_MIN <= Timezone  && Timezone <= TIME_ZONE_MAX);
    stLogConfig.nTimezone = Timezone;

    nResult = TRUE;

Exit0:

    if(NULL != poDocument)
    {
        SAFE_DELETE(poDocument);
    }

    return nResult;
}

void LPAPI LP::GlobalAssert()
{
    LPASSERT(sizeof(LPINT8) == 1);
    PRINTF_CHECK_ERROR(sizeof(LPINT8) == 1);
    LPASSERT(sizeof(LPINT16) == 2);
    PRINTF_CHECK_ERROR(sizeof(LPINT16) == 2);
    LPASSERT(sizeof(LPINT32) == 4);
    PRINTF_CHECK_ERROR(sizeof(LPINT32) == 4);
    LPASSERT(sizeof(LPINT64) == 8);
    PRINTF_CHECK_ERROR(sizeof(LPINT64) == 8);
    LPASSERT(sizeof(LPUINT8) == 1);
    PRINTF_CHECK_ERROR(sizeof(LPUINT8) == 1);
    LPASSERT(sizeof(LPUINT16) == 2);
    PRINTF_CHECK_ERROR(sizeof(LPUINT16) == 2);
    LPASSERT(sizeof(LPUINT32) == 4);
    PRINTF_CHECK_ERROR(sizeof(LPUINT32) == 4);
    LPASSERT(sizeof(LPUINT64) == 8);
    PRINTF_CHECK_ERROR(sizeof(LPUINT64) == 8);
}



//end声明所处的名字空间

NS_LZPL_END
