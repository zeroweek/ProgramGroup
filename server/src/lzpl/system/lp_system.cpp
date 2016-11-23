#include "lp_system.h"
#include "lp_processerror.h"
#include "lp_string.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN


#define EPOCHFILETIME           (116444736000000000i64)


DECLARE void LPAPI lpGetTimeEx(TIME_VALUE& stTime)
{
#   ifdef _WIN32
	{
		FILETIME        fileTime;
		LARGE_INTEGER   li;
		__int64         t;

		GetSystemTimeAsFileTime(&fileTime);
		li.LowPart = fileTime.dwLowDateTime;
		li.HighPart = fileTime.dwHighDateTime;
		t = li.QuadPart;
		t -= EPOCHFILETIME;
		t /= 10;
		stTime.m_qwSec = (long)(t / 1000000);
		stTime.m_qwUsec = (long)(t % 1000000);
	}
#   else
	{

	}
#   endif
}

DECLARE void LPAPI lpSleep(UINT_32 dwMicSeconds)
{
#   ifdef _WIN32
	{
		Sleep(dwMicSeconds);
	}
#   else
	{

	}
#   endif
}

DECLARE INT_32 LPAPI lpShutDown(SOCKET s, INT_32 nHowTo)
{
#   ifdef _WIN32
	{
		return shutdown(s, nHowTo);
	}
#   else
	{

	}
#   endif
}

DECLARE INT_32 LPAPI lpCloseSocket(SOCKET s)
{
#   ifdef _WIN32
	{
		return closesocket(s);
	}
#   else
	{

	}
#   endif
}

DECLARE INT_32 LPAPI lpCloseHandle(HANDLE h)
{
#   ifdef _WIN32
	{
		return CloseHandle(h);
	}
#   else
	{

	}
#   endif
}

DECLARE void LPAPI lpCancelIoEx(SOCKET s)
{
#   ifdef _WIN32
	{
		CancelIoEx((HANDLE)s, NULL);
	}
#   else
	{

	}
#   endif
}

DECLARE BOOL LPAPI lpCreateDirectory(const char* pszDir)
{
#   ifdef _WIN32
	{
		INT_32 nResult = 0;
		char szPath[MAX_PATH];
		char* p1 = szPath;

		LOG_PROCESS_ERROR(pszDir);

		lpStrCpyN(szPath, pszDir, MAX_PATH);

		while (*p1)
		{
			if ('/' == *p1)
			{
				*p1 = '\\';
			}
			++p1;
		}

		if ((szPath[0] & 0x80) && (':' == szPath[1]))
		{
			p1 = szPath + 2;

			if ('\0' == *p1)
			{
				PROCESS_SUCCESS(TRUE);
			}

			if ('\\' == *p1)
			{
				++p1;
				if ('\0' == *p1)
				{
					PROCESS_SUCCESS(TRUE);
				}
			}
		}
		else
		{
			p1 = szPath;
		}

		do 
		{
			p1 = strchr(p1, '\\');
			if (NULL != p1)
			{
				*p1 = '\0';
			}

			nResult = CreateDirectory(szPath, NULL);
			if (!nResult)
			{
				if (ERROR_ALREADY_EXISTS != GetLastError())
				{
					LOG_PROCESS_ERROR(FALSE);
				}
			}

			if (NULL != p1)
			{
				*p1++ = '\\';
			}
		} while (NULL != p1);

	Exit1:
		return TRUE;
	Exit0:
		return FALSE;
	}
#   else
	{

	}
#   endif
}

DECLARE const char* LPAPI lpGetExePath()
{
#   ifdef _WIN32
	{
		static char szPath[MAX_PATH];
		static BOOL bFirstTime = TRUE;

		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
		}

		return szPath;
	}
#   else
	{

	}
#   endif
}

DECLARE const char *LPAPI lpGetExeDir()
{
#   ifdef _WIN32
	{
		static char* p = NULL;
		static char szPath[MAX_PATH];
		static BOOL bFirstTime = TRUE;

		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
			p = strrchr(szPath, '\\');
			*p = '\0';
		}

		return szPath;
	}
#   else
	{

	}
#   endif
}

DECLARE const char *LPAPI lpGetExeFileName()
{
#   ifdef _WIN32
	{
		static char* p = NULL;
		static char szPath[MAX_PATH];
		static BOOL bFirstTime = TRUE;

		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
			p = strrchr(szPath, '\\');
			++p;
		}

		return p;
	}
#   else
	{

	}
#   endif
}

DECLARE const char *LPAPI lpGetExeProcessName()
{
#   ifdef _WIN32
	{
		static char* p = NULL;
		static char szPath[MAX_PATH];
		static BOOL bFirstTime = TRUE;

		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
			p = strrchr(szPath, '.');
			*p = '\0';
			p = strrchr(szPath, '\\');
			++p;
		}

		return p;
	}
#   else
	{

	}
#   endif
}

DECLARE const char *LPAPI lpGetWorkingDir()
{
#   ifdef _WIN32
	{
		static char szPath[MAX_PATH];
		static BOOL bFirstTime = TRUE;

		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetCurrentDirectory(MAX_PATH, szPath);
		}

		return szPath;
	}
#   else
	{

	}
#   endif
}

DECLARE UINT_64 LPAPI lpGetTickCountEx()
{
#   ifdef _WIN32
	{
		return GetTickCount64();
	}
#   else
	{

	}
#   endif
}

DECLARE UINT_64 LPAPI lpRdtsc(void)
{
	UINT_64 qwTsc = 0;

#   ifdef _WIN32
	{
		qwTsc = __rdtsc();
	}
#   else
	{
		LPASSERT(FALSE);
	}
#   endif

	return qwTsc;
}

DECLARE void LPAPI lpINT3(void)
{
#   ifdef _WIN32
	{
		return __debugbreak();
	}
#   else
	{

	}
#   endif
}

DECLARE UINT_32 LPAPI lpGetCurrentProcessId()
{
#   ifdef _WIN32
	{
		return GetCurrentProcessId();
	}
#   else
	{

	}
#   endif
}

DECLARE UINT_32 LPAPI lpGetCurrentThreadId()
{
#   ifdef _WIN32
	{
		return GetCurrentThreadId();
	}
#   else
	{

	}
#   endif
}

DECLARE INT_32 LPAPI lpGetLastError()
{
#   ifdef _WIN32
	{
		return GetLastError();
	}
#   else
	{

	}
#   endif
}




//end声明所处的名字空间
NS_LZPL_END
