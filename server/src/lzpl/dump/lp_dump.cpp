#include "lp_dump.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_string.h"

#ifdef _WIN32
#include <Psapi.h>
#include <DbgHelp.h>
#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define DUMP_MAX_WAIT_TIME             (100)



SINGLETON_IMPLEMENT(LPDumpMgr)

LPDumpMgr::LPDumpMgr()
{
	m_bDumpThreadRun = FALSE;
	m_lstDump.clear();
	memset(m_szDumpFloder, 0, sizeof(m_szDumpFloder));
	m_pDumpCheckFunc = NULL;
	m_nLastDumpIndex = 0;
}

LPDumpMgr::~LPDumpMgr()
{
	UnInit();
}

static void _CrtHandler(void)
{
	lpINT3();
}

static void _InvalidParamHandler(const wchar_t * pcszExpression, const wchar_t * pcszFunction, const wchar_t * pcszFile, unsigned int dwLine, uintptr_t reserved)
{
	lpINT3();
}

BOOL LPAPI LPDumpMgr::Init(pfunDumpCheckFunc pDumpCheckFunc, const char* pcszDumpFloder)
{
	LPINT32 nResult = 0;
	char szWorkingDir[MAX_PATH] = { 0 };

	LOG_PROCESS_ERROR(pcszDumpFloder);

	m_bDumpThreadRun = TRUE;
	m_pDumpCheckFunc = pDumpCheckFunc;
	lpStrCpyN(m_szDumpFloder, pcszDumpFloder, sizeof(m_szDumpFloder));
	m_nLastDumpIndex = 0;

#if defined(_WIN32)
	{
		SetErrorMode(SEM_NOGPFAULTERRORBOX);
		SetUnhandledExceptionFilter(lpDUMPExceptionFilter);
		_set_purecall_handler(_CrtHandler);
		set_terminate(_CrtHandler);
		set_unexpected(_CrtHandler);
		_set_invalid_parameter_handler(_InvalidParamHandler);
	}
#endif

	nResult = m_oCreateDumpThread.Start(_DumpThreadProc, this);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

BOOL LPAPI LPDumpMgr::UnInit(void)
{
	if (m_bDumpThreadRun)
	{
		m_bDumpThreadRun = FALSE;
		m_oCreateDumpThread.Wait();
	}

	return TRUE;
}

BOOL LPAPI LPDumpMgr::PushException(EXCEPTION_POINTERS * pExceptionPointers)
{
	LPINT32 nResult = 0;
	LPINT32 nWaitTime = 0;

	m_oDumpLock.Lock();
	m_lstDump.push_back(pExceptionPointers);
	m_oDumpLock.UnLock();

	//这里等待dump被处理才返回，防止栈信息丢失，dump信息不全或错误
	while (nWaitTime < DUMP_MAX_WAIT_TIME)
	{
		m_oDumpLock.Lock();
		nResult = m_lstDump.empty() ? TRUE : FALSE;
		m_oDumpLock.UnLock();

		PROCESS_SUCCESS(nResult);

		++nWaitTime;
		lpSleep(50);
	}

Exit1:
	return TRUE;
}

void LPAPI LPDumpMgr::_CreateDumpFile(EXCEPTION_POINTERS * pExceptionPointers, LPUINT32 dwProcessId, LPUINT32 dwThreadId)
{
	LPINT32 nResult = 0;
	char szFileName[MAX_PATH] = { 0 };
	char szExePath[MAX_PATH] = { 0 };
	char szWorkingDir[MAX_PATH] = { 0 };

	char* pszModuleName = NULL;
	char szModuleName[MAX_PATH] = { 0 };

	LPUINT32 dwBufSize = MAX_PATH;
	HANDLE hDumpFile = INVALID_HANDLE_VALUE;
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	BOOL bSmallDump = FALSE;

	LPTime oLocalTime = LPTime::GetNowTime();
	MINIDUMP_EXCEPTION_INFORMATION stExceptionInfo;

#if !defined(_DEBUG)
#ifdef _WIN32
	ULARGE_INTEGER FreeBytes;
	GetDiskFreeSpaceEx(szWorkingDir, &FreeBytes, NULL, NULL);
	if (FreeBytes.QuadPart < 5LL * 1024 * 1024 * 1024)
	{
		bSmallDump = TRUE;
	}
#else
	LOG_CHECK_ERROR(FALSE);
#endif
#endif

	LOG_PROCESS_ERROR(m_szDumpFloder[0] != '\0');
	nResult = lpCreateDirectory(m_szDumpFloder);
	LOG_PROCESS_ERROR(nResult);

	snprintf(szFileName, MAX_PATH, "%s/dump_%04d_%02d_%02d",
		m_szDumpFloder,
		oLocalTime.GetYear(),
		oLocalTime.GetMon(),
		oLocalTime.GetMday());
	nResult = lpCreateDirectory(szFileName);
	LOG_PROCESS_ERROR(nResult);

	lpStrCpyN(szExePath, lpGetExePath(), MAX_PATH);
	pszModuleName = szExePath + strlen(szExePath);
	while (*pszModuleName != '\\' && pszModuleName > szExePath)
	{
		if (*pszModuleName == '.')
		{
			*pszModuleName = 0;
		}
		--pszModuleName;
	}
	pszModuleName++;

	if (oLocalTime.SecEqualTo(m_oLastDumpTime))
	{
		m_nLastDumpIndex++;
	}
	else
	{
		m_oLastDumpTime = oLocalTime;
		m_nLastDumpIndex = 1;
	}
	snprintf(szFileName, MAX_PATH, "%s/dump_%04d_%02d_%02d/%s_%d_%04d_%02d_%02d_%02d_%02d_%02d_%03d.dmp",
		m_szDumpFloder,
		oLocalTime.GetYear(),
		oLocalTime.GetMon(),
		oLocalTime.GetMday(),
		pszModuleName,
		dwProcessId,
		oLocalTime.GetYear(),
		oLocalTime.GetMon(),
		oLocalTime.GetMday(),
		oLocalTime.GetHour(),
		oLocalTime.GetMin(),
		oLocalTime.GetSec(),
		m_nLastDumpIndex);

#   ifdef _WIN32
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessId);
		LOG_PROCESS_ERROR(hProcess);

		hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
		LOG_PROCESS_ERROR(INVALID_HANDLE_VALUE != hDumpFile);

		stExceptionInfo.ThreadId = dwThreadId;
		stExceptionInfo.ExceptionPointers = pExceptionPointers;
		stExceptionInfo.ClientPointers = NULL;

		if (NULL == pExceptionPointers)
		{
			nResult = MiniDumpWriteDump(hProcess, dwProcessId, hDumpFile,
				(MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithThreadInfo), NULL, NULL, NULL);
			LOG_PROCESS_ERROR(nResult);
		}
		else
		{
			if (bSmallDump)
			{
				nResult = MiniDumpWriteDump(hProcess, dwProcessId, hDumpFile,
					(MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithThreadInfo), &stExceptionInfo, NULL, NULL);
				LOG_PROCESS_ERROR(nResult);
			}
			else
			{
				nResult = MiniDumpWriteDump(hProcess, dwProcessId, hDumpFile,
					(MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithThreadInfo | MiniDumpWithDataSegs | MiniDumpWithFullMemory), &stExceptionInfo, NULL, NULL);
				LOG_PROCESS_ERROR(nResult);
			}
		}
	}
#else
	LPASSERT(FALSE);
#endif

Exit0:

	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		lpCloseHandle(hDumpFile);
		hDumpFile = INVALID_HANDLE_VALUE;
	}

	if (hProcess != NULL)
	{
		lpCloseHandle(hDumpFile);
		hProcess = INVALID_HANDLE_VALUE;
	}

	return;
}

THREAD_FUNC_DECLARE(LPDumpMgr::_DumpThreadProc)(void * pParam)
{
	LPINT32 nResult = 0;
	BOOL bEmpty = TRUE;
	EXCEPTION_POINTERS* pExceptionPointers = NULL;

	LPDumpMgr* pDumpMgr = (LPDumpMgr*)pParam;
	LOG_PROCESS_ERROR(pDumpMgr);

	while (pDumpMgr->m_bDumpThreadRun)
	{
		lpSleep(15);

		if (pDumpMgr->m_pDumpCheckFunc && pDumpMgr->m_pDumpCheckFunc())
		{
			pDumpMgr->_CreateDumpFile(NULL, lpGetCurrentProcessId(), pDumpMgr->m_oCreateDumpThread.GetThreadId());
		}

		pDumpMgr->m_oDumpLock.Lock();
		if (pDumpMgr->m_lstDump.empty())
		{
			bEmpty = TRUE;
		}
		else
		{
			bEmpty = FALSE;
			pExceptionPointers = pDumpMgr->m_lstDump.front();
			pDumpMgr->m_lstDump.pop_front();
		}
		pDumpMgr->m_oDumpLock.UnLock();

		if (bEmpty)
		{
			continue;
		}

		pDumpMgr->_CreateDumpFile(pExceptionPointers, lpGetCurrentProcessId(), pDumpMgr->m_oCreateDumpThread.GetThreadId());
	}

Exit0:
	return 0;
}

DECLARE long LPAPI lpDUMPExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers)
{
	LPDumpMgr::Instance().PushException(pExceptionPointers);

	FTL("*** detected a new exception %p ***", pExceptionPointers);

#   ifdef _WIN32
	{
		if (IsDebuggerPresent())
		{
			//lpINT3();
		}
	}

#endif

	return EXCEPTION_EXECUTE_HANDLER;
}



//end声明所处的名字空间

NS_LZPL_END