#include "lp_tabfile.h"
#include "lp_processerror.h"
#include "lp_string.h"
#include "zlib.h"
#include "lp_codeconvert.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN


LPTabFile::LPTabFile()
{
	m_pData = NULL;
	m_uSize = 0;
	m_nRowCount = 0;
	m_nColCount = 0;
	m_pOffsetTable = NULL;
	m_dwCRC32 = 0;
}

LPTabFile::~LPTabFile()
{
	UnInit();
}

BOOL LPAPI LPTabFile::Init(const char * pcszFileName, INT_32 nSkipLine, BOOL bZeroReplaceNull)
{
	INT_32 nResult = 0;
	INT_32 nIndex = 0;
	INT_32 nTempIndex = 0;
	INT_32 nStringIndex = 0;
	INT_32 nOffsetIndex = 0;
	INT_32 nTabCount = 0;

	BOOL bInQuote = FALSE;
	UINT_32 uBOM = 0;
	ILPFile* pFile = NULL;
	char* pszTmp = NULL;
	wchar_t* pwszData = NULL;
	const char* pcszValue = NULL;
	std::pair<MAP_NAME_2_COL::iterator, bool> InsRet;

	m_nSkipLine = nSkipLine;
	m_bZeroReplaceNull = bZeroReplaceNull;
	m_nColCount = 1;
	m_nRowCount = 0;

	LOG_PROCESS_ERROR(pcszFileName);

	pFile = lpFileOpen(pcszFileName, "rb");
	LOG_PROCESS_ERROR(pFile);

	m_uSize = pFile->Size();
	LOG_PROCESS_ERROR(m_uSize > 0);
	
	nResult = pFile->Read(&uBOM, sizeof(uBOM));
	LOG_PROCESS_ERROR(nResult == sizeof(uBOM));

	if ((uBOM & 0xFFFF) == UTF16_BOM)
	{
		m_uSize -= 2;
		nResult = pFile->Seek(2, SEEK_SET);
		LOG_PROCESS_ERROR(nResult);
	}
	else if((uBOM & 0xFFFFFF) == UTF8_BOM)
	{
		m_uSize -= 3;
		nResult = pFile->Seek(3, SEEK_SET);
		LOG_PROCESS_ERROR(nResult);
	}
	else
	{
		nResult = pFile->Seek(0, SEEK_SET);
		LOG_PROCESS_ERROR(nResult);
	}

	LOG_PROCESS_ERROR(m_pData == NULL);
	m_pData = new char[m_uSize];
	LOG_PROCESS_ERROR(m_pData);
	memset(m_pData, 0, m_uSize);

	nResult = pFile->Read(m_pData, (UINT_32)m_uSize);
	LOG_PROCESS_ERROR(nResult == m_uSize);

	pFile->Close();
	pFile = NULL;

	if ((uBOM & 0xFFFF) == UTF16_BOM)
	{
		pszTmp = new char[m_uSize * 3 / 2];
		memset(pszTmp, 0, m_uSize * 3 / 2);
		
		pwszData = (wchar_t*)m_pData;
		m_pData = new char[m_uSize * 3 / 2];
		LOG_PROCESS_ERROR(m_pData);

		m_uSize = lpUTF16ToUTF8(pwszData, m_uSize / 2, pszTmp, m_uSize * 3 / 2);
		LOG_PROCESS_ERROR(m_uSize > 0);

		memcpy(m_pData, pszTmp, m_uSize);
		SAFE_DELETE_SZ(pszTmp);
	}

	m_dwCRC32 = crc32(0, (const Bytef*)m_pData, (uInt)m_uSize);

	// 1
	nIndex = 0;
	m_nColCount = 1;
	m_nRowCount = 0;
	INT_32 n = m_pData[22];
	n = m_pData[23];
	n = m_pData[95];
	n = m_pData[96];
	n = m_pData[97];
	while (nIndex < m_uSize)
	{
		switch (m_pData[nIndex])
		{
		case '\t':
			{
				if (m_nRowCount == 0 && !bInQuote)
				{
					m_nColCount++;
				}
				nTabCount++;
			}
			break;
		case '\r':
			{
				if (!bInQuote)
				{
					m_pData[nIndex] = 0;
				}
			}
			break;
		case '\n':
			{
				if (!bInQuote)
				{
					if (nIndex - 1 >= 0 && m_pData[nIndex - 1] == '\r')
					{
						nIndex++;
						continue;
					}
					m_nRowCount++;
				}
			}
			break;
		case '\"':
			{
				if (bInQuote)
				{
					if (m_pData[nIndex + 1] != '\"')
					{
						bInQuote = FALSE;
					}
					else
					{
						nIndex++;
					}
				}
				else
				{
					bInQuote = TRUE;
				}
			}
			break;
		default:
			break;
		}

		nIndex++;
	}

	LOG_PROCESS_ERROR(m_pOffsetTable == NULL);
	m_pOffsetTable = new int[m_nColCount * m_nRowCount];
	LOG_PROCESS_ERROR(m_pOffsetTable);

	// 2
	nIndex = 0;
	bInQuote = FALSE;
	while (nIndex < m_uSize)
	{
		switch (m_pData[nIndex])
		{
		case '\t':
		case '\n':
			{
				if (!bInQuote)
				{
					m_pData[nIndex] = 0;
					while ((nStringIndex < nIndex) &&
						(m_pData[nStringIndex] == 0 || m_pData[nStringIndex] == '\r' || m_pData[nStringIndex] == '\n'))
					{
						nStringIndex++;
					}

					LOG_PROCESS_ERROR(nOffsetIndex < m_nColCount * m_nRowCount);
					m_pOffsetTable[nOffsetIndex] = nStringIndex;
					nOffsetIndex++;

					nStringIndex = nIndex + 1;
				}
			}
			break;
		case '\"':
			{
				if (bInQuote)
				{
					if (m_pData[nIndex + 1] == '\"')
					{
						nTempIndex = nIndex;
						while (m_pData[nTempIndex] != 0 && m_pData[nTempIndex + 1] != '\n' && m_pData[nTempIndex + 1] != '\t')
						{
							m_pData[nTempIndex] = m_pData[nTempIndex + 1];
							nTempIndex++;
						}
						m_pData[nTempIndex] = 0;
					}
					else
					{
						bInQuote = FALSE;
						m_pData[nIndex] = 0;
					}
				}
				else
				{
					bInQuote = TRUE;
					m_pData[nIndex] = 0;
				}
			}
		break;
		default:
			break;
		}

		nIndex++;
	}

	for (INT_32 i = 0; i < m_nColCount; ++i)
	{
		pcszValue = GetData(i, 0);
		LOG_PROCESS_ERROR(pcszValue);

		InsRet = m_mapName2Col.insert(std::make_pair(pcszValue, i));
		LOG_PROCESS_ERROR(InsRet.second);
	}

	SAFE_DELETE_SZ(pwszData);

	return TRUE;

Exit0:

	if (pFile)
	{
		pFile->Close();
		pFile = NULL;
	}

	SAFE_DELETE_SZ(pwszData);
	SAFE_DELETE_SZ(pszTmp);

	UnInit();

	return FALSE;
}

BOOL LPAPI LPTabFile::UnInit(void)
{
	SAFE_DELETE_SZ(m_pData);
	SAFE_DELETE_SZ(m_pOffsetTable);

	m_mapName2Col.clear();

	return TRUE;
}

INT_32 LPAPI LPTabFile::GetRowCount(void)
{
	if (m_nRowCount >= (m_nSkipLine + 1))
	{
		return m_nRowCount - m_nSkipLine - 1;
	}
	else
	{
		return 0;
	}
}

INT_32 LPAPI LPTabFile::GetColCount(void)
{
	return m_nColCount;
}

const char *LPAPI LPTabFile::GetColName(INT_32 nCol)
{
	return GetData(nCol, 0);
}

BOOL LPAPI LPTabFile::ReadData(INT_32 nCol, INT_32 nRow, INT_32 & rnValue)
{
	LOG_PROCESS_ERROR(nCol >= 0 && nCol < m_nColCount);

	nRow += (m_nSkipLine + 1);
	LOG_PROCESS_ERROR(nRow >= 0 && nRow < m_nRowCount);

	if ((m_pData + m_pOffsetTable[nRow * m_nColCount + nCol])[0])
	{
		rnValue = atoi(m_pData + m_pOffsetTable[nRow * m_nColCount + nCol]);
	}
	else
	{
		//rnValue = atoi(m_pData + m_pOffsetTable[2 * m_nColCount + nCol]);
		rnValue = 0;
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTabFile::ReadData(const char * pcszColName, int nRow, INT_32 & rnValue)
{
	MAP_NAME_2_COL::iterator it;

	it = m_mapName2Col.find(pcszColName);
	LOG_PROCESS_ERROR(it != m_mapName2Col.end());

	return ReadData(it->second, nRow, rnValue);

Exit0:
	return FALSE;
}

BOOL LPAPI LPTabFile::ReadData(INT_32 nCol, INT_32 nRow, char * pszValue, INT_32 nSize)
{
	LOG_PROCESS_ERROR(pszValue);
	LOG_PROCESS_ERROR(nCol >= 0 && nCol < m_nColCount);

	nRow += (m_nSkipLine + 1);
	LOG_PROCESS_ERROR(nRow >= 0 && nRow < m_nRowCount);

	if ((m_pData + m_pOffsetTable[nRow * m_nColCount + nCol])[0])
	{
		lpStrCpyN(pszValue, m_pData + m_pOffsetTable[nRow * m_nColCount + nCol], nSize);
	}
	else
	{
		//lpStrCpyN(pszValue, m_pData + m_pOffsetTable[2 * m_nColCount + nCol], nSize);
		if (m_bZeroReplaceNull)
		{
			lpStrCpyN(pszValue, "0", nSize);
		}
		else
		{
			lpStrCpyN(pszValue, "", nSize);
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTabFile::ReadData(const char * pcszColName, int nRow, char * pszValue, INT_32 nSize)
{
	MAP_NAME_2_COL::iterator it;

	it = m_mapName2Col.find(pcszColName);
	LOG_PROCESS_ERROR(it != m_mapName2Col.end());

	return ReadData(it->second, nRow, pszValue, nSize);

Exit0:
	return FALSE;
}

UINT_32 LPAPI LPTabFile::GetCRC(void)
{
	return m_dwCRC32;
}

const char *LPAPI LPTabFile::GetData(INT_32 nCol, INT_32 nRow)
{
	LOG_PROCESS_ERROR(nCol >= 0 && nCol < m_nColCount);
	LOG_PROCESS_ERROR(nRow >= 0 && nRow < m_nRowCount);

	return m_pData + m_pOffsetTable[nRow * m_nColCount + nCol];
	
Exit0:
	return NULL;
}

const char *LPAPI LPTabFile::GetData(const char * pcszColName, INT_32 nRow)
{
	MAP_NAME_2_COL::iterator it;

	it = m_mapName2Col.find(pcszColName);
	LOG_PROCESS_ERROR(it != m_mapName2Col.end());

	return GetData(it->second, nRow);

Exit0:
	return NULL;
}







//end声明所处的名字空间
NS_LZPL_END
