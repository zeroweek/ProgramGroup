#include "lp_urlcode.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



DECLARE BOOL lpUrlEncode(const char * pcszInStr, LPUINT32 dwInSize, char * pszOutStr, LPUINT32 & dwOutSize, eUrlCodeType eType)
{
	LPUINT32 nIndex = 0;
	LPUINT32 dwOutputCount = 0;

	LOG_PROCESS_ERROR(pcszInStr);
	LOG_PROCESS_ERROR(pszOutStr);

	for (; nIndex < dwInSize; ++nIndex)
	{
		LOG_PROCESS_ERROR(dwOutputCount < dwOutSize);

		if (pcszInStr[nIndex] == ' ')
		{
			pszOutStr[dwOutputCount++] = '+';
		}
		else if ((pcszInStr[nIndex] >= '0' && pcszInStr[nIndex] <= '9') ||
			(pcszInStr[nIndex] >= 'A' && pcszInStr[nIndex] <= 'Z') ||
			(pcszInStr[nIndex] >= 'a' && pcszInStr[nIndex] <= 'z') ||
			pcszInStr[nIndex] == '-' || pcszInStr[nIndex] == '_' ||
			pcszInStr[nIndex] == '.' || pcszInStr[nIndex] == '~')
		{
			pszOutStr[dwOutputCount++] = pcszInStr[nIndex];
		}
		else
		{
			LOG_PROCESS_ERROR(dwOutputCount + 2 < dwOutSize);
			pszOutStr[dwOutputCount++] = '%';
			pszOutStr[dwOutputCount++] = lpNumToHex(pcszInStr[nIndex] >> 4, TRUE);
			pszOutStr[dwOutputCount++] = lpNumToHex(pcszInStr[nIndex] % 16, TRUE);
		}
	}

	dwOutSize = dwOutputCount;

	return TRUE;
Exit0:
	return FALSE;
}

DECLARE BOOL LPAPI lpUrlDecode(const char * pcszInStr, LPUINT32 dwInSize, char * pszOutStr, LPUINT32 & dwOutSize, eUrlCodeType eType)
{
	LPINT32 nResult = 0;
	LPUINT32 nIndex = 0;
	LPUINT32 dwOutputCount = 0;

	LPUINT8 byLow = 0;
	LPUINT8 byHigh = 0;

	LOG_PROCESS_ERROR(pcszInStr);
	LOG_PROCESS_ERROR(pszOutStr);

	for (; nIndex < dwInSize; ++nIndex)
	{
		LOG_PROCESS_ERROR(dwOutputCount < dwOutSize);

		if (pcszInStr[nIndex] == '+')
		{
			pszOutStr[dwOutputCount++] = ' ';
		}
		else if (pcszInStr[nIndex] == '%')
		{
			LOG_PROCESS_ERROR(nIndex + 2 < dwInSize);
			++nIndex;
			byHigh = lpHexToNum(pcszInStr[nIndex++]) * 16;
			byLow = lpHexToNum(pcszInStr[nIndex]);
			pszOutStr[dwOutputCount++] = byHigh + byLow;
		}
		else
		{
			pszOutStr[dwOutputCount++] = pcszInStr[nIndex];
		}
	}

	dwOutSize = dwOutputCount;

	return TRUE;
Exit0:
	return FALSE;
}



//end声明所处的名字空间

NS_LZPL_END