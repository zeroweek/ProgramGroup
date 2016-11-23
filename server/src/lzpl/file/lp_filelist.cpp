#include "lp_filelist.h"
#include "lp_processerror.h"
#include "lp_string.h"

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#endif


//begin�������������ֿռ�
NS_LZPL_BEGIN



BOOL LPAPI FILE_LIST::ScanDirectory(const char * pcszDir, const char * pcszExt, BOOL bFilenameWithDir)
{
	INT_32 nResult = 0;
	char szPathWithExt[MAX_PATH];

#ifdef _WIN32
	_finddata_t dir;
	INT_64 lHandle;
#else
	dirent* current = 0;
	DIR* pDir = NULL;
	INT_32 nExtLen = 0;
	INT_32 nNameLen = 0;
#endif

	LOG_PROCESS_ERROR(pcszDir);
	LOG_PROCESS_ERROR(pcszExt);

	_CleanUp();

	lpStrCpyN(szPathWithExt, pcszDir, MAX_PATH);
	lpStrCatN(szPathWithExt, "/*", MAX_PATH);
	lpStrCatN(szPathWithExt, pcszExt, MAX_PATH);

#ifdef _WIN32
	lHandle = _findfirst(szPathWithExt, &dir);
	LOG_PROCESS_ERROR(lHandle != -1L);

	do
	{
		nResult = _FileListAdd(pcszDir, dir.name, bFilenameWithDir);
		LOG_CHECK_ERROR(nResult);

	} while (_findnext(lHandle, &dir) == 0);
	_findclose(lHandle);
#else
	pDir = opendir(pcszDir);
	LOG_PROCESS_ERROR(pDir);

	nExtLen = lpStrNLen(pcszExt, MAX_PATH);
	while ((current = readdir(pDir)) != 0)
	{
		if (nExtLen == 0)
		{
			_FileListAdd(pcszDir, current->d_name, bFilenameWithDir);
			continue;
		}

		nLen = lpStrNLen(current->d_name, MAX_PATH);
		if (nLen > nExtLen && strncmp(current->d_name + nLen - nExtLen, pcszExt, nExtLen) == 0)
		{
			_FileListAdd(pcszDir, current->d_name, bFilenameWithDir);
		}
	}
	closedir(pDir);
#endif

	if (dwSize > 1)
	{
		qsort(szFiles, dwSize, sizeof(char*), _Compare);
	}

	return TRUE;
Exit0:
	return FALSE;
}

void FILE_LIST::_CleanUp(void)
{
	for (INT_32 i = 0; i < dwSize; ++i)
	{
		SAFE_DELETE_SZ(szFiles[i]);
	}

	dwSize = 0;
}

BOOL FILE_LIST::_FileListAdd(const char * pcszDir, const char * pcszPath, BOOL bFilenameWithDir)
{
	INT_32 nResult = 0;
	UINT_32 dwLen = 0;

	LOG_PROCESS_ERROR(pcszPath);
	LOG_PROCESS_ERROR(dwSize < MAX_FILE_LIST_COUNT);

	if (bFilenameWithDir)
	{
		dwLen += (UINT_32)lpStrNLen(pcszDir, MAX_PATH - dwLen - 1);
		dwLen += (UINT_32)lpStrNLen("/", MAX_PATH - dwLen - 1);
	}
	dwLen += (UINT_32)lpStrNLen(pcszPath, MAX_PATH - dwLen - 1);

	szFiles[dwSize] = new char[dwLen + 1];

	dwLen = 0;
	if (bFilenameWithDir)
	{
		nResult = (INT_32)lpStrCpyN(szFiles[dwSize] + dwLen, pcszDir, MAX_PATH);
		dwLen += nResult;

		nResult = (INT_32)lpStrCpyN(szFiles[dwSize] + dwLen, "/", MAX_PATH);
		dwLen += nResult;
	}
	nResult = (INT_32)lpStrCpyN(szFiles[dwSize] + dwLen, pcszPath, MAX_PATH);
	dwLen += nResult;

	++dwSize;

	return TRUE;
Exit0:
	return FALSE;
}

int FILE_LIST::_Compare(const void * s1, const void * s2)
{
	return strcmp(*(const char**)s1, *(const char**)s2);
}






//end�������������ֿռ�
NS_LZPL_END