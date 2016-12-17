#include "lp_loggerctrl.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_processerror.h"
#include "tinyxml.h"
#include "tinystr.h"



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



//LZPL组件内部日志控制器对象
ILPLoggerCtrl* g_poLzplLoggerCtrl = NULL;



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

DECLARE BOOL LPAPI lpLoadLogConfig(const char* pszLogConfigDir, const char* pszLogConfigFileName, LOG_CONFIG& stLogConfig)
{
	bool bResult = false;
	BOOL nResult = FALSE;
	//LPINT32 nNodeValue = 0;
	char szPath[MAX_PATH];
	TiXmlDocument* poDocument = NULL;
	TiXmlElement* poRootElement = NULL;
	TiXmlNode* poNode = NULL;
	const char* pcszNodeValue = NULL;

	LPINT32 nDefaultValue = 0;
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
	if (WrithLock == 0)
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
	if (LogLevel_DBG == 1)
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
	if (LogLevel_INF == 1)
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
	if (LogLevel_WRN == 1)
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
	if (LogLevel_IMP == 1)
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
	if (LogLevel_ERR == 1)
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
	if (LogLevel_FTL == 1)
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
	if (LogLevel_LUA == 1)
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
	if (OutputMask_File == 1)
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
	if (OutputMask_Console == 1)
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
	if (OutputMask_Debug == 1)
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
	if (MaxLogLoopBufSize == 0 || MaxLogLoopBufSize < 10 * MB)
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
	if (MaxLogSingleFileSize == 0 || MaxLogSingleFileSize < 10 * MB)
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
	if (MaxLogOneMsgSize == 0 || MaxLogOneMsgSize < 1 * MB)
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
	if (LogFlushInterval == 0 || LogFlushInterval < 3 * 1000)
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
	
	if (NULL != poDocument)
	{
		SAFE_DELETE(poDocument);
	}

	return nResult;
}

DECLARE BOOL LPAPI lpInitLzplLoggerCtrl(LOG_CONFIG& stLogConfig)
{
	LPINT32 nResult = 0;
	ILPFileLogger* pFileLogger = NULL;

	g_poLzplLoggerCtrl = lpCreateLoggerCtrl();
	PRINTF_PROCESS_ERROR(g_poLzplLoggerCtrl);

	pFileLogger = lpCreateFileLogger();
	PRINTF_PROCESS_ERROR(pFileLogger);

	nResult = pFileLogger->Init(stLogConfig);
	PRINTF_PROCESS_ERROR(nResult);

	nResult = g_poLzplLoggerCtrl->Init(pFileLogger, stLogConfig);
	PRINTF_PROCESS_ERROR(nResult);

	return TRUE;

Exit0:

	if (pFileLogger) pFileLogger->Release();
	if (g_poLzplLoggerCtrl) g_poLzplLoggerCtrl->Release();

	return FALSE;
}

DECLARE BOOL LPAPI lpSetLzplLoggerCtrl(ILPLoggerCtrl * poLzplLoggerCtrl)
{
	g_poLzplLoggerCtrl = poLzplLoggerCtrl;
	LOG_PROCESS_ERROR(g_poLzplLoggerCtrl);

	return TRUE;
Exit0:
	return FALSE;
}

DECLARE ILPLoggerCtrl* LPAPI lpGetLzplLoggerCtrl()
{
	return g_poLzplLoggerCtrl;
}

DECLARE ILPLoggerCtrl* LPAPI lpCreateLoggerCtrl()
{
	return (new LPLoggerCtrl());
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

BOOL LPAPI LPLoggerCtrl::Init(ILPLogger* pLogger, LOG_CONFIG& stLogConfig)
{
	PRINTF_PROCESS_ERROR(pLogger);

	m_pLogger = pLogger;
	m_pLogger->AddRef();

	m_stLogConfig = stLogConfig;

	m_bLogLock = stLogConfig.bWrithLock;

	if (strcmp(stLogConfig.szModulePrefix, "") != 0)
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
	if (NULL != m_pLogger)
	{
		m_pLogger->Release();
		m_pLogger = NULL;
	}

	return TRUE;
}

void LPAPI LPLoggerCtrl::Lua(const char * format, ...)
{
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(format);
	
	if (eLogLevel_Lua & m_dwLogLevelConfig)
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
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(format);

	if (eLogLevel_Fatal & m_dwLogLevelConfig)
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
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(format);

	if (eLogLevel_Error & m_dwLogLevelConfig)
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
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(format);

	if (eLogLevel_Warn & m_dwLogLevelConfig)
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
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(format);

	if (eLogLevel_Important & m_dwLogLevelConfig)
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
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(format);

	if (eLogLevel_Info & m_dwLogLevelConfig)
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
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(format);

	if (eLogLevel_Debug & m_dwLogLevelConfig)
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

void LPAPI LPLoggerCtrl::Release()
{
	delete this;
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

	if (m_bLogLock) m_oLock.Lock();

	//错误等级日志添加标记
	if (eLogLevel == eLogLevel_Error || eLogLevel == eLogLevel_Fatal)
	{
		pszFlag = s_szErrorFlag;
	}

	lpStrFastZero(s_pszMsg);
	if ('\0' != m_szModulePrefix[0])
	{
		nCount = snprintf(s_pszMsg, s_nMsgBufSize, "%4d-%02d-%02d %02d:%02d:%02d,%06d [%08d] [%s] %s %s", 
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
		nCount = snprintf(s_pszMsg, s_nMsgBufSize, "%4d-%02d-%02d %02d:%02d:%02d,%06d [%08d] %s %s",
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
	nResult = vsnprintf_s(s_pszMsg + nCount, s_nMsgBufSize - nCount, s_nMsgBufSize - nCount, format, argptr);
	PRINTF_PROCESS_ERROR(nResult >= 0);
	nTotalCount += nResult;
	PRINTF_CHECK_ERROR(nTotalCount < s_nMsgBufSize);

	// 这里多加一个null终止符字节，用于写file线程省略memcpy操作
	m_pLogger->LogText(s_pszMsg, (LPUINT32)(nTotalCount + 1));

	if (m_dwOutputMask & eOutputType_Console)
	{
		switch (eLogLevel)
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

	if (m_dwOutputMask & eOutputType_Debug)
	{
#       ifdef _WIN32
		{
			if (IsDebuggerPresent())
			{
				LP_OUTPUT_DEBUG_STRING(s_pszMsg);
				LP_OUTPUT_DEBUG_STRING("\n");
			}
		}
#       endif
	}

	if (m_bLogLock) m_oLock.UnLock();

Exit0:
	return;
}

const char *LPAPI LPLoggerCtrl::_GetLogLevelPrompt(e_LogLevel eLogLevel)
{
	switch (eLogLevel)
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
