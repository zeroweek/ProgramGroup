#include "lp_filelogger.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



DECLARE ILPFileLogger* LPAPI lpCreateFileLogger()
{
	ILPFileLogger* pFileLogger = NULL;

	pFileLogger	= new LPFileLogger();
	PRINTF_PROCESS_ERROR(pFileLogger);

Exit0:
	return pFileLogger;
}

LPFileLogger::LPFileLogger()
{
	m_bRun                   = FALSE;
	m_bInit                  = FALSE;
	m_bErrorLog              = false;
	m_dwRef                  = 0;
	m_eLogMode               = eLogMode_None;
	m_dwOutputMask           = eOutputType_Off;

	lpFastZeroCharArray(m_szDir);
	lpFastZeroCharArray(m_szFileName);
	lpFastZeroCharArray(m_szFilePath);

	m_fpFile                 = NULL;
	m_bDirty                 = FALSE;
	m_pLoopBuf               = NULL;
	m_dwSubFile              = 0;

	m_dwMsgId                = 0;
	
}

LPFileLogger::~LPFileLogger()
{
	UnInit();
}

void LPAPI LPFileLogger::LogText(const char *pszLog, LPUINT32 dwLen)
{
	LPINT32 nResult = FALSE;
	LPINT32 nSleepCount = 0;

	PRINTF_PROCESS_ERROR(pszLog);
	PRINTF_PROCESS_ERROR(m_pLoopBuf);
	PRINTF_PROCESS_ERROR(!m_bErrorLog);

	//检测日志缓冲区大小是否足够大，超过3秒钟都没有足够大的缓冲区，则丢掉信息
	if (m_pLoopBuf->GetTotalWritableLen() < dwLen + sizeof(dwLen))
	{
		PRINTF_CHECK_ERROR(FALSE);
		while (m_pLoopBuf->GetTotalWritableLen() < dwLen + sizeof(dwLen))
		{
			lpSleep(1);
			nSleepCount++;
			if (nSleepCount > 3000)
			{
				PRINTF_PROCESS_ERROR(FALSE);
			}
		}
	}

	nResult = m_pLoopBuf->Write((const char*)(&dwLen), sizeof(dwLen));
	PRINTF_PROCESS_ERROR(nResult);

	nResult = m_pLoopBuf->Write(pszLog, dwLen);
	PRINTF_PROCESS_ERROR(nResult);

	return;

Exit0:
	
	m_bErrorLog = true;
	LPPRINTF("Log fail: %s\n", pszLog);

	return ;
}

LPUINT32 LPAPI LPFileLogger::CreateMsgId()
{
	return ++m_dwMsgId;
}

BOOL LPAPI LPFileLogger::Init(LOG_CONFIG& stLogConfig)
{
	LPINT32 nResult = 0;

	PRINTF_PROCESS_ERROR(m_bInit == FALSE);
	PRINTF_PROCESS_ERROR(strcmp(stLogConfig.szLogDir, "") != 0);
	PRINTF_PROCESS_ERROR(strcmp(stLogConfig.szFileName, "") != 0);

	m_stLogConfig = stLogConfig;

	//日志缓冲区初始化 
	m_pLoopBuf = new LPLoopBuf();
	PRINTF_PROCESS_ERROR(m_pLoopBuf);

	nResult = m_pLoopBuf->Init(stLogConfig.dwMaxLogLoopBufSize);
	PRINTF_PROCESS_ERROR(nResult);

	//日志记录模式判断
	PRINTF_PROCESS_ERROR(stLogConfig.dwLogMode > eLogMode_None);
	PRINTF_PROCESS_ERROR(stLogConfig.dwLogMode < eLogMode_Max);
	
	m_eLogMode = (e_LogMode)stLogConfig.dwLogMode;
	m_dwOutputMask = stLogConfig.dwOutputMask;

	//日志路径及文件名处理
	lpStrCpyN(m_szDir, stLogConfig.szLogDir, MAX_PATH);
	lpStrCpyN(m_szFileName, stLogConfig.szFileName, MAX_FILE_NAME);
	lpPathFilter(m_szDir, sizeof(m_szDir));

	//创建目录
	nResult = lpCreateDirectory(m_szDir);
	PRINTF_PROCESS_ERROR(nResult);

	//打开文件
	_UpdateFilePointer();
	PRINTF_PROCESS_ERROR(m_fpFile);

	nResult = m_oThread.Start(_ThreadProc, this);
	PRINTF_PROCESS_ERROR(nResult);

	m_bInit = TRUE;
	m_bRun = TRUE;

	return TRUE;

Exit0:

	UnInit();

	return FALSE;
}


void LPAPI LPFileLogger::AddRef(void)
{
	++m_dwRef;
}

LPUINT32 LPAPI LPFileLogger::QueryRef(void)
{
	return m_dwRef;
}

void LPAPI LPFileLogger::Release()
{
	PRINTF_CHECK_ERROR(m_dwRef > 0);

	if (m_dwRef > 0)
	{
		--m_dwRef;
	}

	if (0 == m_dwRef)
	{
		delete this;
	}
}

void LPAPI LPFileLogger::UnInit()
{
	m_bInit = FALSE;

	if (m_bRun)
	{
		m_bRun = FALSE;
		m_oThread.Wait(10000);
	}

	_CloseFile();

	SAFE_DELETE(m_pLoopBuf);
}

void LPAPI LPFileLogger::_Flush()
{
	if (NULL != m_fpFile && m_bDirty)
	{
		fflush(m_fpFile);
		m_bDirty = FALSE;
	}
}

void LPAPI LPFileLogger::_CloseFile()
{
	_Flush();
	if (NULL != m_fpFile)
	{
		fclose(m_fpFile);
		m_fpFile = NULL;
	}
}

void LPAPI LPFileLogger::_UpdateFilePointer()
{
	//第一次创建日志文件
	if (NULL == m_fpFile)
	{
		m_oCurTime = LPTime::GetNowTime();
		m_oLastTime = m_oCurTime;

		//使用fopen会编译报错（windows），使用fopen_s文件不能共享
		m_fpFile = _OpenOrCreateLogFile(m_oCurTime);
		PRINTF_PROCESS_ERROR(m_fpFile);
		PROCESS_SUCCESS(TRUE);
	}

	m_oCurTime = LPTime::GetNowTime();

	//检查是否需要创建新文件
	switch (m_eLogMode)
	{
	case eLogMode_SingleFile:
		break;
	case eLogMode_DayDivide:
		{
			if (m_oCurTime.GetYear() != m_oLastTime.GetYear()
				|| m_oCurTime.GetMon() != m_oLastTime.GetMon()
				|| m_oCurTime.GetMday() != m_oLastTime.GetMday())
			{
				m_dwSubFile = 0;

				_CloseFile();
				m_fpFile = _OpenOrCreateLogFile(m_oCurTime);
				PRINTF_PROCESS_ERROR(m_fpFile);
			}
		}
		break;
	case eLogMode_MonthDivide:
		{
			if (m_oCurTime.GetYear() != m_oLastTime.GetYear()
				|| m_oCurTime.GetMon() != m_oLastTime.GetMon())
			{
				m_dwSubFile = 0;

				_CloseFile();
				m_fpFile = _OpenOrCreateLogFile(m_oCurTime);
				PRINTF_PROCESS_ERROR(m_fpFile);
			}
		}
		break;
	case eLogMode_HourDivide:
		{
			if (m_oCurTime.GetYear() != m_oLastTime.GetYear()
				|| m_oCurTime.GetMon() != m_oLastTime.GetMon()
				|| m_oCurTime.GetMday() != m_oLastTime.GetMday()
				|| m_oCurTime.GetHour() != m_oLastTime.GetHour())
			{
				m_dwSubFile = 0;

				_CloseFile();
				m_fpFile = _OpenOrCreateLogFile(m_oCurTime);
				PRINTF_PROCESS_ERROR(m_fpFile);
			}
		}
		break;
	default:
		PRINTF_PROCESS_ERROR(FALSE);
		break;
	}

	m_oLastTime = m_oCurTime;

Exit1:

	//如果日志文件超过单个文件大小这创建子文件（刚打开的文件也需要判断）
	//这里判断单个文件大小不能等于0，防止配置错误，启动进程后一直创建空文件
	PRINTF_PROCESS_ERROR(m_fpFile);
	PRINTF_PROCESS_ERROR(m_stLogConfig.dwMaxLogSingleFileSize != 0);
	while (ftell(m_fpFile) >= m_stLogConfig.dwMaxLogSingleFileSize)
	{
		++m_dwSubFile;
		_CloseFile();
		m_fpFile = _OpenOrCreateLogFile(m_oCurTime);
		PRINTF_PROCESS_ERROR(m_fpFile);
	}

Exit0:
	return;
}

const char* LPAPI LPFileLogger::_GetFilePath(LPTime& oTime)
{
	LPINT32 nResult = FALSE;

	lpFastZeroCharArray(m_szFilePath);

	switch (m_eLogMode)
	{
	case eLogMode_SingleFile:
		{
			if (m_dwSubFile > 0)
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s.log.%02d", m_szDir, m_szFileName, m_dwSubFile);
				LOG_CHECK_ERROR(nResult >= 0);
			}
			else
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s.log", m_szDir, m_szFileName);
				LOG_CHECK_ERROR(nResult >= 0);
			}
		}
		break;
	case eLogMode_DayDivide:
		{
			if (m_dwSubFile > 0)
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s_%04d%02d%02d.log.%02d", m_szDir, m_szFileName,
					oTime.GetYear(), oTime.GetMon(), oTime.GetMday(), m_dwSubFile);
				LOG_CHECK_ERROR(nResult >= 0);
			}
			else
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s_%04d%02d%02d.log", m_szDir, m_szFileName,
					oTime.GetYear(), oTime.GetMon(), oTime.GetMday());
				LOG_CHECK_ERROR(nResult >= 0);
			}
		}
		break;
	case eLogMode_MonthDivide:
		{
			if (m_dwSubFile > 0)
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s_%04d%02d.log.%02d",
					m_szDir, m_szFileName, oTime.GetYear(), oTime.GetMon(), m_dwSubFile);
				LOG_CHECK_ERROR(nResult >= 0);
			}
			else
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s_%04d%02d.log",
					m_szDir, m_szFileName, oTime.GetYear(), oTime.GetMon());
				LOG_CHECK_ERROR(nResult >= 0);
			}
		}
		break;
	case eLogMode_HourDivide:
		{
			if (m_dwSubFile > 0)
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s_%04d%02d%02d%02d.log.%02d",
					m_szDir, m_szFileName, oTime.GetYear(), oTime.GetMon(), oTime.GetMday(), oTime.GetHour(), m_dwSubFile);
				LOG_CHECK_ERROR(nResult >= 0);
			}
			else
			{
				nResult = snprintf(m_szFilePath, sizeof(m_szFilePath), "%s/%s_%04d%02d%02d%02d.log",
					m_szDir, m_szFileName, oTime.GetYear(), oTime.GetMon(), oTime.GetMday(), oTime.GetHour());
				LOG_CHECK_ERROR(nResult >= 0);
			}
		}
		break;
	default:
		PRINTF_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:
	return m_szFilePath;
}

FILE* LPAPI LPFileLogger::_OpenOrCreateLogFile(LPTime& oTime)
{
	LPINT32 nResult = 0;
	FILE* fpFile = NULL;

	fpFile = _fsopen(_GetFilePath(oTime), "a", _SH_DENYNO);
	PRINTF_PROCESS_ERROR(fpFile);

	//有数据的文件刚打开，ftell返回0，因此需要调整指标位置
	nResult = fseek(fpFile, 0, SEEK_END);
	PRINTF_PROCESS_ERROR(nResult == 0);

Exit0:
	return fpFile;
}

THREAD_FUNC_DECLARE(LPFileLogger::_ThreadProc)(void * pParam)
{
	char*    pszBuf = NULL;
	LPINT32   nBufSize = 0;

	LPINT32   nResult = 0;
	LPUINT32  dwLen = 0;
	LPUINT32  dwSleepMicSec = 10;                      // 每次睡眠毫秒数
	LPUINT32  dwFlushIntervalSec = 0;                  // flush间隔（毫秒）
	LPUINT32  dwLastFlushTimeMicSecInc = 0;            // 上次flush后的时间增量（毫秒）
	LPFileLogger* pFileLogger = NULL;

	PRINTF_PROCESS_ERROR(pParam);
	pFileLogger = (LPFileLogger*)pParam;
	PRINTF_PROCESS_ERROR(pFileLogger);

	nBufSize = pFileLogger->m_stLogConfig.dwMaxLogOneMsgSize * 2;
	pszBuf = new char[nBufSize];
	PRINTF_PROCESS_ERROR(pszBuf != NULL);

	dwFlushIntervalSec = pFileLogger->m_stLogConfig.dwLogFlushInterval;

	while (pFileLogger->m_bRun)
	{
		while (pFileLogger->m_pLoopBuf->GetTotalReadableLen() > sizeof(dwLen))
		{
			nResult = pFileLogger->m_pLoopBuf->Read((char*)(&dwLen), sizeof(dwLen), FALSE, FALSE);
			PRINTF_CHECK_ERROR(nResult);

			if (pFileLogger->m_pLoopBuf->GetTotalReadableLen() < dwLen + sizeof(dwLen))
			{
				break;
			}
			
			pFileLogger->m_pLoopBuf->FinishRead(sizeof(dwLen));

			pFileLogger->_UpdateFilePointer();

			//这里的Read会执行memcpy拷贝操作，需要优化
			if (pFileLogger->m_pLoopBuf->GetOnceReadableLen() < dwLen)
			{
				if (dwLen >= nBufSize)
				{
					pFileLogger->m_pLoopBuf->FinishRead(dwLen);
					PRINTF_CHECK_ERROR(FALSE);
					continue;
				}

				lpStrFastZero(pszBuf);
				nResult = pFileLogger->m_pLoopBuf->Read(pszBuf, dwLen, TRUE, TRUE);
				PRINTF_CHECK_ERROR(nResult);

				if (pFileLogger->m_dwOutputMask & eOutputType_File)
				{
					fprintf(pFileLogger->m_fpFile, "%s\n", pszBuf);
				}
			}
			else
			{
				// m_pLoopBuf在write时多写了一个null终止符

				if (pFileLogger->m_dwOutputMask & eOutputType_File)
				{
					fprintf(pFileLogger->m_fpFile, "%s\n", pFileLogger->m_pLoopBuf->ReadPtr());
				}

				pFileLogger->m_pLoopBuf->FinishRead(dwLen);
			}

			pFileLogger->m_bDirty = TRUE;
		}

		if (pFileLogger->m_bErrorLog)
		{
			pFileLogger->m_pLoopBuf->FinishRead(pFileLogger->m_pLoopBuf->GetTotalReadableLen());
			pFileLogger->m_bErrorLog = false;
		}

		dwLastFlushTimeMicSecInc += dwSleepMicSec;
		if (dwLastFlushTimeMicSecInc > dwFlushIntervalSec)
		{
			dwLastFlushTimeMicSecInc = 0;
			pFileLogger->_Flush();
		}

		if (pFileLogger->m_pLoopBuf->GetTotalReadableLen() < sizeof(dwLen))
		{
			lpSleep(dwSleepMicSec);
		}
	}

Exit0:
	
	SAFE_DELETE_SZ(pszBuf);

	pFileLogger->_Flush();

	return 0;
}



//end声明所处的名字空间
NS_LZPL_END