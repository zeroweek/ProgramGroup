#include "lp_filelogger.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_processerror.h"



//begin�������������ֿռ�
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

	//�����־��������С�Ƿ��㹻�󣬳���3���Ӷ�û���㹻��Ļ��������򶪵���Ϣ
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

	//��־��������ʼ�� 
	m_pLoopBuf = new LPLoopBuf();
	PRINTF_PROCESS_ERROR(m_pLoopBuf);

	nResult = m_pLoopBuf->Init(stLogConfig.dwMaxLogLoopBufSize);
	PRINTF_PROCESS_ERROR(nResult);

	//��־��¼ģʽ�ж�
	PRINTF_PROCESS_ERROR(stLogConfig.dwLogMode > eLogMode_None);
	PRINTF_PROCESS_ERROR(stLogConfig.dwLogMode < eLogMode_Max);
	
	m_eLogMode = (e_LogMode)stLogConfig.dwLogMode;
	m_dwOutputMask = stLogConfig.dwOutputMask;

	//��־·�����ļ�������
	lpStrCpyN(m_szDir, stLogConfig.szLogDir, MAX_PATH);
	lpStrCpyN(m_szFileName, stLogConfig.szFileName, MAX_FILE_NAME);
	lpPathFilter(m_szDir, sizeof(m_szDir));

	//����Ŀ¼
	nResult = lpCreateDirectory(m_szDir);
	PRINTF_PROCESS_ERROR(nResult);

	//���ļ�
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
	//��һ�δ�����־�ļ�
	if (NULL == m_fpFile)
	{
		m_oCurTime = LPTime::GetNowTime();
		m_oLastTime = m_oCurTime;

		//ʹ��fopen����뱨��windows����ʹ��fopen_s�ļ����ܹ���
		m_fpFile = _OpenOrCreateLogFile(m_oCurTime);
		PRINTF_PROCESS_ERROR(m_fpFile);
		PROCESS_SUCCESS(TRUE);
	}

	m_oCurTime = LPTime::GetNowTime();

	//����Ƿ���Ҫ�������ļ�
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

	//�����־�ļ����������ļ���С�ⴴ�����ļ����մ򿪵��ļ�Ҳ��Ҫ�жϣ�
	//�����жϵ����ļ���С���ܵ���0����ֹ���ô����������̺�һֱ�������ļ�
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

	//�����ݵ��ļ��մ򿪣�ftell����0�������Ҫ����ָ��λ��
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
	LPUINT32  dwSleepMicSec = 10;                      // ÿ��˯�ߺ�����
	LPUINT32  dwFlushIntervalSec = 0;                  // flush��������룩
	LPUINT32  dwLastFlushTimeMicSecInc = 0;            // �ϴ�flush���ʱ�����������룩
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

			//�����Read��ִ��memcpy������������Ҫ�Ż�
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
				// m_pLoopBuf��writeʱ��д��һ��null��ֹ��

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



//end�������������ֿռ�
NS_LZPL_END