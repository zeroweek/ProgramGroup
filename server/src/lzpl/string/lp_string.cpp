#include "lp_string.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN


std::string NULL_STR = "";

DECLARE void LPAPI lpPathFilter(char * pszPath, UINT_32 dwMaxLen)
{
	char* p = pszPath;
	PROCESS_ERROR(pszPath);

	while (*p && dwMaxLen > 0)
	{
#       ifdef _WIN32
		{
			if ('/' == *p)
			{
				*p = '\\';
			}
		}
#       else
		{
			if ('\\' == *p)
			{
				*p = '/';
			}
		}
#       endif

		++p;
		--dwMaxLen;
	}

Exit0:
	return;
}

DECLARE size_t LPAPI lpStrCpyN(char * dst, const char * src, size_t size)
{
	const char* d = dst;

	if (dst == NULL || src == NULL || size == 0)
	{
		return 0;
	}

	while (--size > 0)
	{
		if ((*dst++ = *src++) == 0)
		{
			break;
		}
	}

	if (size == 0)
	{
		*dst++ = 0;
	}

	return dst - d - 1;
}

DECLARE size_t LPAPI lpStrCatN(char * dst, const char * src, size_t size)
{
	const char* d = dst;

	if (dst == NULL || src == NULL || size == 0)
	{
		return 0;
	}

	while (size && *dst)
	{
		size--;
		dst++;
	}

	while (--size > 0)
	{
		if ((*dst++ = *src++) == 0)
		{
			break;
		}
	}

	if (size == 0)
	{
		*dst++ = 0;
	}

	return dst - d - 1;
}

DECLARE BOOL LPAPI lpStrCharCheck(const char* pcszSrc, e_CharCheckType eCharCheckType, INT_32 nMaxLen, BOOL bIsEmptyMeet)
{
	PROCESS_ERROR(pcszSrc != NULL);
	PROCESS_ERROR(eCharCheckType_None < eCharCheckType && eCharCheckType < eCharCheckType_Max);

	if (strcmp(pcszSrc, "") == 0 || nMaxLen == 0)
	{
		return bIsEmptyMeet;
	}

	for (INT_32 i = nMaxLen; i != 0; --i)
	{
		PROCESS_SUCCESS('\0' == *pcszSrc);

		switch (eCharCheckType)
		{
		case LZPL::eCharCheckType_Digit:
			{
				PROCESS_ERROR('0' <= *pcszSrc && *pcszSrc <= '9');
			}
			break;
		case LZPL::eCharCheckType_Alpha:
			{
				PROCESS_ERROR('A' <= *pcszSrc && *pcszSrc <= 'Z' || 'a' <= *pcszSrc && *pcszSrc <= 'z');
			}
			break;
		case LZPL::eCharCheckType_Alnum:
			{
				PROCESS_ERROR('0' <= *pcszSrc && *pcszSrc <= '9' || 'A' <= *pcszSrc && *pcszSrc <= 'Z' || 'a' <= *pcszSrc && *pcszSrc <= 'z');
			}
			break;
		case LZPL::eCharCheckType_Integer:
			{
				PROCESS_ERROR('0' <= *pcszSrc && *pcszSrc <= '9' || *pcszSrc == '-');
				if (*pcszSrc == '-')
				{
					PROCESS_ERROR(i == nMaxLen && (i - 1) != 0 && '0' <= *(pcszSrc + 1) && *(pcszSrc + 1) <= '9');
				}
			}
			break;
		case LZPL::eCharCheckType_None:
		case LZPL::eCharCheckType_Max:
		default:
			PROCESS_ERROR(FALSE);
		}

		++pcszSrc;
	}

Exit1:
	return TRUE;
Exit0:
	return FALSE;
}

LPString::LPString(UINT_32 dwSize, bool bAutoExtend)
{
	INT_32 nResult = FALSE;

	nResult = Init(dwSize, bAutoExtend);
	LOG_CHECK_ERROR(nResult);
	if (!nResult)
	{
		throw bad_alloc();
	}
}

LPString::~LPString()
{
	UnInit();
}

BOOL LPAPI LPString::Init(UINT_32 dwSize, bool bAutoExtend)
{
	if (dwSize < 1)
	{
		dwSize = 1;
	}

	m_pBuf = new char[dwSize];
	LOG_PROCESS_ERROR(m_pBuf != NULL);

	//多出1个字节，放终止符，方便读取
	m_bAutoExtend = bAutoExtend;
	m_dwCapacity = dwSize - 1;
	m_dwDataSize = 0;
	m_pBuf[0] = '\0';

	return TRUE;

Exit0:
	return FALSE;
}

BOOL LPAPI LPString::UnInit()
{
	SAFE_DELETE_SZ(m_pBuf);
	return TRUE;
}

void LPAPI LPString::Reset()
{
	m_dwDataSize = 0;
	LOG_CHECK_ERROR(NULL != m_pBuf);
	if (NULL != m_pBuf)
	{
		m_pBuf[0] = '\0';
	}
}

BOOL LPAPI LPString::CheckExtend(UINT_32 dwWillAddSize)
{
	PROCESS_SUCCESS(m_dwDataSize + dwWillAddSize <= m_dwCapacity);

	//多出1个字节，放终止符，方便读取
	//数据量大时，尽量保证申请的字符串大小是512的倍数
	UINT_32 dwMul = (UINT_32)(dwWillAddSize * 1.0 / m_dwCapacity + 1);
	UINT_32 dwAddSize = dwMul * (m_dwCapacity + 1);
	char* pBuf = new char[dwAddSize + m_dwCapacity + 1];
	LOG_PROCESS_ERROR(pBuf != NULL);

	memcpy(pBuf, m_pBuf, m_dwCapacity + 1);

	SAFE_DELETE_SZ(m_pBuf);
	m_pBuf = pBuf;
	m_dwCapacity += dwAddSize;

Exit1:
	return TRUE;

Exit0:

	SAFE_DELETE_SZ(pBuf);
	return FALSE;
}

UINT_32 LPAPI LPString::GetFreeSize()
{
	return m_dwCapacity - m_dwDataSize;
}

BOOL LPAPI LPString::Read(LPString& oDstStr, UINT_32 dwDstStartPos, UINT_32 dwSrcStartPos, UINT_32 dwReadLen)
{
	INT_32 nResult = FALSE;

	nResult = oDstStr.Insert(dwDstStartPos, *this, dwSrcStartPos, dwReadLen);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPString::Read(char* pszDst, UINT_32 dwDstStartPos, UINT_32 dwDstMaxLen, UINT_32 dwStartPos, UINT_32 dwReadLen, BOOL bNullTerminate)
{
	LOG_PROCESS_ERROR(dwStartPos + dwReadLen <= m_dwDataSize);

	LOG_PROCESS_ERROR(pszDst != NULL);
	LOG_PROCESS_ERROR(dwDstStartPos + dwReadLen <= dwDstMaxLen);

	if (bNullTerminate)
	{
		LOG_PROCESS_ERROR(dwDstStartPos + dwReadLen + 1 <= dwDstMaxLen);
	}

	memcpy(pszDst + dwDstStartPos, m_pBuf + dwStartPos, dwReadLen);

	if (bNullTerminate)
	{
		pszDst[dwDstStartPos + dwReadLen] = '\0';
	}

	return TRUE;

Exit0:
	return FALSE;
}

BOOL LPAPI LPString::Append(const char c)
{
	if (m_bAutoExtend)
	{
		CheckExtend(sizeof(c));
	}

	LOG_PROCESS_ERROR(1 + m_dwDataSize <= m_dwCapacity);

	m_pBuf[m_dwDataSize] = c;
	++m_dwDataSize;
	m_pBuf[m_dwDataSize] = '\0';

	return TRUE;

Exit0:
	return FALSE;
}

BOOL LPAPI LPString::Append(const char* pszSrc, UINT_32 dwStartPos, UINT_32 dwLen)
{
	if (m_bAutoExtend)
	{
		CheckExtend(dwLen);
	}

	LOG_PROCESS_ERROR(pszSrc != NULL);
	LOG_PROCESS_ERROR(dwLen + m_dwDataSize <= m_dwCapacity);

	memcpy(m_pBuf + m_dwDataSize, pszSrc + dwStartPos, dwLen);
	m_dwDataSize += dwLen;
	m_pBuf[m_dwDataSize] = '\0';

	return TRUE;

Exit0:
	return FALSE;
}

BOOL LPAPI LPString::Append(LPString& oStr, UINT_32 dwStartPos, UINT_32 dwLen)
{
	if (m_bAutoExtend)
	{
		CheckExtend(dwLen);
	}

	LOG_PROCESS_ERROR(dwLen + m_dwDataSize <= m_dwCapacity);

	LOG_PROCESS_ERROR(oStr.GetData() != NULL);
	LOG_PROCESS_ERROR(dwStartPos + dwLen <= oStr.GetSize());

	memcpy(m_pBuf + m_dwDataSize, oStr.GetData() + dwStartPos, dwLen);
	m_dwDataSize += dwLen;
	m_pBuf[m_dwDataSize] = '\0';

	return TRUE;

Exit0:
	return FALSE;
}

BOOL LPAPI LPString::Insert(UINT_32 dwDstStartPos, LPString& oSrcStr, UINT_32 dwSrcStartPos, UINT_32 dwLen)
{
	LOG_PROCESS_ERROR(dwDstStartPos <= m_dwDataSize);

	LOG_PROCESS_ERROR(oSrcStr.GetData() != NULL);
	LOG_PROCESS_ERROR(dwSrcStartPos + dwLen <= oSrcStr.GetSize());

	if (m_bAutoExtend)
	{
		CheckExtend(dwLen - dwDstStartPos);
	}

	LOG_PROCESS_ERROR(dwLen + dwDstStartPos <= m_dwCapacity);

	memcpy(m_pBuf + dwDstStartPos, oSrcStr.GetData() + dwSrcStartPos, dwLen);
	m_dwDataSize = dwDstStartPos + dwLen;
	m_pBuf[m_dwDataSize] = '\0';

	return TRUE;

Exit0:
	return FALSE;
}

BOOL LPAPI LPString::Insert(UINT_32 dwDstStartPos, const char* pszSrc, UINT_32 dwSrcStartPos, UINT_32 dwLen)
{
	LOG_PROCESS_ERROR(pszSrc != NULL);
	LOG_PROCESS_ERROR(dwDstStartPos <= m_dwDataSize);

	if (m_bAutoExtend)
	{
		CheckExtend(dwLen - dwDstStartPos);
	}

	LOG_PROCESS_ERROR(dwLen + dwDstStartPos <= m_dwCapacity);

	memcpy(m_pBuf + dwDstStartPos, pszSrc + dwSrcStartPos, dwLen);
	m_dwDataSize = dwDstStartPos + dwLen;
	m_pBuf[m_dwDataSize] = '\0';

	return TRUE;

Exit0:
	return FALSE;
}

char* LPAPI LPString::GetData()
{
	return m_pBuf;
}

DECLARE std::string LPAPI lpSerializeToString(UINT_32 nMaxLen, const char * format, ...)
{
	INT_32 nResult = 0;
	char* pszBuf = nullptr;

	if (nullptr == format || nMaxLen == 0)
	{
		return NULL_STR;
	}

	pszBuf = new char[nMaxLen + 1];

	va_list args;
	va_start(args, format);
	nResult = vsnprintf_s(pszBuf, nMaxLen + 1, nMaxLen, format, args);
	va_end(args);

	if (nResult < 0)
	{
		SAFE_DELETE_SZ(pszBuf);
		return NULL_STR;
	}

	std::string strResult = std::string(pszBuf, nResult);
	SAFE_DELETE_SZ(pszBuf);

	return strResult;
}







//end声明所处的名字空间
NS_LZPL_END
