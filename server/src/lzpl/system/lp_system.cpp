#include "lp_system.h"
#include "lp_processerror.h"
#include "lp_string.h"
#include "lp_global.h"

#ifndef _WIN32
#include <unistd.h>  
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#endif




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
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		stTime.m_qwSec = tv.tv_sec;
		stTime.m_qwUsec = tv.tv_usec;
	}
#   endif
}

DECLARE void LPAPI lpSleep(LPUINT32 dwMicSeconds)
{
#   ifdef _WIN32
	{
		Sleep(dwMicSeconds);
	}
#   else
	{
		usleep(dwMicSeconds * 1000);
	}
#   endif
}

DECLARE void LPAPI lpShutDown(SOCKET s, LPINT32 nHowTo)
{
	shutdown(s, nHowTo);
}

DECLARE void LPAPI lpCloseSocket(SOCKET s)
{
#   ifdef _WIN32
	{
		closesocket(s);
	}
#   else
	{
		close(s);
	}
#   endif
}

DECLARE void LPAPI lpCloseHandle(HANDLE h)
{
#   ifdef _WIN32
	{
		CloseHandle(h);
	}
#   else
	{
		close(h);
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
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
	}
#   endif
}

DECLARE BOOL LPAPI lpCreateDirectory(const char* pszDir)
{
	LPINT32 nResult = 0;
	char szPath[MAX_PATH];
	char* p1 = nullptr;

#	ifdef _WIN32
	char cSep = '\\';
#	else
	char cSep = '/';
#	endif

	LOG_PROCESS_ERROR(pszDir);

	lpStrCpyN(szPath, pszDir, MAX_PATH);
	lpPathFilter(szPath, MAX_PATH);

	if ((szPath[0] & 0x80) && (':' == szPath[1]))
	{
		p1 = szPath + 2;

		if ('\0' == *p1)
		{
			PROCESS_SUCCESS(TRUE);
		}

		if (cSep == *p1)
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
		p1 = strchr(p1, cSep);
		if (NULL != p1)
		{
			*p1 = '\0';
		}

#		ifdef _WIN32
		{
			nResult = CreateDirectory(szPath, NULL);
			if (!nResult)
			{
				if (ERROR_ALREADY_EXISTS != GetLastError())
				{
					LOG_PROCESS_ERROR_WITH_MSG(FALSE, "error=%d", GetLastError());
				}
			}
		}
#		else
		{
			if (p1 != &szPath[0])
			{
				nResult = mkdir(szPath, 0775);
				if (nResult != 0)
				{
					if (EEXIST != errno)
					{
						LOG_PROCESS_ERROR(FALSE);
						LOG_PROCESS_ERROR_WITH_MSG(FALSE, "errno=%d", errno);
					}
				}
			}
		}
#		endif

		if (NULL != p1)
		{
			*p1++ = cSep;
		}
	} while (NULL != p1);

Exit1:
	return TRUE;
Exit0:
	return FALSE;
}

DECLARE const char* LPAPI lpGetExePath()
{
	static char szPath[MAX_PATH];
	static BOOL bFirstTime = TRUE;

#   ifdef _WIN32
	{
		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
		}
	}
#   else
	{
		if (bFirstTime)
		{
			LPINT32 nSize = readlink("/proc/self/exe", szPath, MAX_PATH - 1);
			szPath[nSize] = '\0';
		}
	}
#   endif

	return szPath;
}

DECLARE const char *LPAPI lpGetExeDir()
{
	static char* p = nullptr;
	static char szPath[MAX_PATH];
	static BOOL bFirstTime = TRUE;

#   ifdef _WIN32
	{
		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
			p = strrchr(szPath, '\\');
			*p = '\0';
		}
	}
#   else
	{
        if (bFirstTime)
        {
            LPINT32 nSize = readlink("/proc/self/exe", szPath, MAX_PATH - 1);
            szPath[nSize] = '\0';
			p = strrchr(szPath, '/');
			*p = '\0';
        }
	}
#   endif

	return szPath;
}

DECLARE const char *LPAPI lpGetExeFileName()
{
	static char* p = nullptr;
	static char szPath[MAX_PATH];
	static BOOL bFirstTime = TRUE;

#   ifdef _WIN32
	{
		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
			p = strrchr(szPath, '\\');
			++p;
		}
	}
#   else
	{
		if (bFirstTime)
		{
			LPINT32 nSize = readlink("/proc/self/exe", szPath, MAX_PATH - 1);
			szPath[nSize] = '\0';
			p = strrchr(szPath, '/');
			++p;
		}
	}
#   endif

	return p;
}

DECLARE const char *LPAPI lpGetExeProcessName()
{
	static char* p = nullptr;
	static char szPath[MAX_PATH];
	static BOOL bFirstTime = TRUE;

#   ifdef _WIN32
	{
		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetModuleFileName(NULL, szPath, sizeof(szPath));
			p = strrchr(szPath, '.');
			*p = '\0';
			p = strrchr(szPath, '\\');
			++p;
		}
	}
#   else
	{
		if (bFirstTime)
		{
			LPINT32 nSize = readlink("/proc/self/exe", szPath, MAX_PATH - 1);
			szPath[nSize] = '\0';
			p = strrchr(szPath, '/');
			++p;
		}
	}
#   endif

	return p;
}

DECLARE const char *LPAPI lpGetWorkingDir()
{
	static char szPath[MAX_PATH];
	static BOOL bFirstTime = TRUE;

#   ifdef _WIN32
	{
		if (bFirstTime)
		{
			bFirstTime = FALSE;

			GetCurrentDirectory(MAX_PATH, szPath);
		}
	}
#   else
	{
		if (bFirstTime)
		{
			char* p = getcwd(szPath, MAX_PATH);
			if (nullptr == p)
			{
				return nullptr;
			}
		}
	}
#   endif

	return szPath;
}

DECLARE LPUINT64 LPAPI lpGetTickCountEx()
{
#   ifdef _WIN32
	{
		return GetTickCount64();
	}
#   else
	{
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
	}
#   endif
}

DECLARE LPUINT64 LPAPI lpRdtsc(void)
{
	LPUINT64 qwTsc = 0;

#   ifdef _WIN32
	{
		qwTsc = __rdtsc();
	}
#   else
	{
		unsigned hi, lo;
		__asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
		qwTsc = ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
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
		LOG_CHECK_ERROR(FALSE);
	}
#   endif
}

DECLARE LPUINT32 LPAPI lpGetCurrentProcessId()
{
#   ifdef _WIN32
	{
		return GetCurrentProcessId();
	}
#   else
	{
		return (LPUINT32)getpid();
	}
#   endif
}

DECLARE LPUINT32 LPAPI lpGetCurrentThreadId()
{
#   ifdef _WIN32
	{
		return GetCurrentThreadId();
	}
#   else
	{
		return (LPUINT32)pthread_self();
	}
#   endif
}

DECLARE LPINT32 LPAPI lpGetLastError()
{
#   ifdef _WIN32
	{
		return GetLastError();
	}
#   else
	{
		return GetLastError();
	}
#   endif
}



//end声明所处的名字空间
NS_LZPL_END
