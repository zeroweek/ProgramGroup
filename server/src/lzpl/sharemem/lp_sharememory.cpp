#include "lp_sharememory.h"
#include "lp_processerror.h"
#include "lp_string.h"
#include "lp_system.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



BOOL LPShareMemory::IsExisting(const char* pcszName)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;

	LOG_PROCESS_ERROR(pcszName);

#   ifdef _WIN32
	{
		hMap = ::OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, pcszName);
		PROCESS_ERROR(hMap);

		lpCloseHandle(hMap);
	}
#   endif

	return TRUE;
Exit0:

	if (hMap != INVALID_HANDLE_VALUE)
	{
		lpCloseHandle(hMap);
	}
	return FALSE;
}

LPShareMemory::LPShareMemory()
{
	memset(m_szName, 0, sizeof(m_szName));
	m_bNew = TRUE;
	m_qwSize = 0;
	m_pszShareMem = NULL;
	m_hHandle = 0;
}

LPShareMemory::~LPShareMemory()
{
}

BOOL LPAPI LPShareMemory::Init(const char* pcszName, LPUINT64 qwSize)
{
	LPINT32 nResult = 0;
	LPUINT32 dwHighSize = 0;
	LPUINT32 dwLowSize = 0;
	SHARE_MEM_HEADER* pHeader = NULL;
	LPUINT32 dwLastError = 0;

	LOG_PROCESS_ERROR(pcszName);

#   ifdef _WIN32
	{
		m_qwSize = qwSize;
		lpStrCpyN(m_szName, pcszName, COMMON_NAME_LEN);

		dwHighSize = (LPUINT32)(qwSize >> 32);
		dwLowSize = (LPUINT32)(qwSize & 0x00000000ffffffff);
		m_hHandle = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			dwHighSize,
			dwLowSize,
			pcszName
		);
		LOG_PROCESS_ERROR(m_hHandle);

		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			m_bNew = FALSE;

			//防止第一、二次创建同步问题
			lpSleep(500);
		}
		else
		{
			m_bNew = TRUE;
		}

		m_pszShareMem = (char*)::MapViewOfFile(m_hHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		LOG_PROCESS_ERROR(m_pszShareMem);

		pHeader = (SHARE_MEM_HEADER*)m_pszShareMem;

		if (pHeader->nFlag != SHARE_MEMORY_FLAG)
		{
			pHeader->nFlag = SHARE_MEMORY_FLAG;
			pHeader->qwSize = qwSize - sizeof(SHARE_MEM_HEADER);
		}
		else
		{
			LOG_PROCESS_ERROR(pHeader->qwSize + sizeof(SHARE_MEM_HEADER) == qwSize);
		}
	}
#   else
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
	}
#   endif

	return TRUE;
Exit0:
	
	dwLastError = lpGetLastError();
	FTL("share memory initialize failed, error: %d", dwLastError);

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL LPAPI LPShareMemory::UnInit(void)
{
	if (m_pszShareMem)
	{
#       ifdef _WIN32
		{
			UnmapViewOfFile(m_pszShareMem);
		}
#       endif
		m_pszShareMem = NULL;
	}

	if (m_hHandle)
	{
		lpCloseHandle(m_hHandle);
		m_hHandle = 0;
	}
	return TRUE;
}

const char *LPAPI LPShareMemory::Name(void)
{
	return m_szName;
}

BOOL LPAPI LPShareMemory::IsNew(void)
{
	return m_bNew;
}

LPUINT64 LPAPI LPShareMemory::Size(void)
{
	return m_qwSize - sizeof(SHARE_MEM_HEADER);
}

char *LPAPI LPShareMemory::Mem(void)
{
	LOG_PROCESS_ERROR(m_pszShareMem);

	return (char*)m_pszShareMem + sizeof(SHARE_MEM_HEADER);
Exit0:
	return NULL;
}

HANDLE LPAPI LPShareMemory::Handle(void)
{
	return m_hHandle;
}



//end声明所处的名字空间
NS_LZPL_END
