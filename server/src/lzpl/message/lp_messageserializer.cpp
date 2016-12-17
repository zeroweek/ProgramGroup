#include "lp_messageserializer.h"
#include "lp_string.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPExternalMessageSerializer::LPExternalMessageSerializer()
{
	lpFastZeroCharArray(m_szSerializeBuf);
	lpFastZeroCharArray(m_szUnSerializeBuf);

	m_dwSerializeSize        = 0;
	m_dwUnSerializeSize      = 0;
	m_dwSerializeBufSize     = 0;
	m_dwUnSerializeBufSize   = 0;
	m_pSerializeBuf          = NULL;
	m_pUnSerializeBuf        = NULL;
}

LPExternalMessageSerializer::~LPExternalMessageSerializer()
{
	UnInit();
}

BOOL LPAPI LPExternalMessageSerializer::Init(char * pszSerializeBuf, LPUINT32 dwSerializeBufSize, const char * pszUnSerializeBuf, LPUINT32 dwUnSerializeBufSize)
{
	LOG_PROCESS_ERROR(NULL == pszSerializeBuf && 0 == dwSerializeBufSize 
		|| NULL != pszSerializeBuf && 0 != dwSerializeBufSize);

	LOG_PROCESS_ERROR(NULL == pszUnSerializeBuf && 0 == dwUnSerializeBufSize 
		|| NULL != pszUnSerializeBuf && 0 != dwUnSerializeBufSize);

	m_dwSerializeSize = 0;
	m_pSerializeBuf = pszSerializeBuf;
	m_dwSerializeBufSize = NULL != m_pSerializeBuf ? dwSerializeBufSize : MESSAGE_MAX_LEN;

	m_dwUnSerializeSize = 0;
	m_pUnSerializeBuf = pszUnSerializeBuf;
	m_dwUnSerializeBufSize = NULL != m_pUnSerializeBuf ? dwUnSerializeBufSize : MESSAGE_MAX_LEN;
	
	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::UnInit(void)
{
	lpFastZeroCharArray(m_szSerializeBuf);
	lpFastZeroCharArray(m_szUnSerializeBuf);

	m_dwSerializeSize = 0;
	m_pSerializeBuf = NULL;
	m_dwSerializeBufSize = 0;

	m_dwUnSerializeSize = 0;
	m_pUnSerializeBuf = NULL;
	m_dwUnSerializeBufSize = 0;

	return TRUE;
}

BOOL LPAPI LPExternalMessageSerializer::ReadUint8(LPUINT8 * pbyValue)
{
	const char* pBuf = NULL;
	LOG_PROCESS_ERROR(pbyValue);
	LOG_PROCESS_ERROR(sizeof(LPUINT8) == 1);
	LOG_PROCESS_ERROR(m_dwUnSerializeSize + 1 <= m_dwUnSerializeBufSize);

	pBuf = m_pUnSerializeBuf != NULL ? m_pUnSerializeBuf : m_szUnSerializeBuf;

	(*pbyValue) = *(LPUINT8*)(pBuf + m_dwUnSerializeSize);

	m_dwUnSerializeSize += 1;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::WriteUint8(LPUINT8 byValue)
{
	char* pBuf = NULL;
	LOG_PROCESS_ERROR(sizeof(LPUINT8) == 1);
	LOG_PROCESS_ERROR(m_dwSerializeSize + 1 <= m_dwSerializeBufSize);

	pBuf = m_pSerializeBuf != NULL ? m_pSerializeBuf : m_szSerializeBuf;
	*((LPUINT8*)(pBuf + m_dwSerializeSize)) = byValue;

	m_dwSerializeSize += 1;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::ReadUint16(LPUINT16 * pwValue)
{
	char* pBuf = NULL;
	LPINT32 nResult = 0;
	LPUINT8 byData1 = 0;
	LPUINT8 byData2 = 0;

	LOG_PROCESS_ERROR(pwValue);
	LOG_PROCESS_ERROR(sizeof(LPUINT16) == 2);

	nResult = ReadUint8(&byData1);
	LOG_PROCESS_ERROR(nResult);

	nResult = ReadUint8(&byData2);
	LOG_PROCESS_ERROR(nResult);

	(*pwValue) = ((LPUINT16)byData1 << 8) | (LPUINT16)byData2;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::WriteUint16(LPUINT16 wValue)
{
	LPINT32 nResult = 0;
	LPUINT8 byData1 = 0;
	LPUINT8 byData2 = 0;

	LOG_PROCESS_ERROR(sizeof(LPUINT16) == 2);

	byData1 = wValue >> 8;
	nResult = WriteUint8(byData1);
	LOG_PROCESS_ERROR(nResult);

	byData2 = (wValue << 8) >> 8;
	nResult = WriteUint8(byData2);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::ReadUint32(LPUINT32 * pdwValue)
{
	char* pBuf = NULL;
	LPINT32 nResult = 0;
	LPUINT16 wData1 = 0;
	LPUINT16 wData2 = 0;

	LOG_PROCESS_ERROR(pdwValue);
	LOG_PROCESS_ERROR(sizeof(LPUINT32) == 4);

	nResult = ReadUint16(&wData1);
	LOG_PROCESS_ERROR(nResult);

	nResult = ReadUint16(&wData2);
	LOG_PROCESS_ERROR(nResult);

	(*pdwValue) = ((LPUINT32)wData1 << 16) | (LPUINT32)wData2;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::WriteUint32(LPUINT32 dwValue)
{
	LPINT32 nResult = 0;
	LPUINT16 wData1 = 0;
	LPUINT16 wData2 = 0;

	LOG_PROCESS_ERROR(sizeof(LPUINT32) == 4);

	wData1 = dwValue >> 16;
	nResult = WriteUint16(wData1);
	LOG_PROCESS_ERROR(nResult);

	wData2 = (dwValue << 16) >> 16;
	nResult = WriteUint16(wData2);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::ReadUint64(LPUINT64 * pqwValue)
{
	char* pBuf = NULL;
	LPINT32 nResult = 0;
	LPUINT32 dwData1 = 0;
	LPUINT32 dwData2 = 0;

	LOG_PROCESS_ERROR(pqwValue);
	LOG_PROCESS_ERROR(sizeof(LPUINT64) == 8);

	nResult = ReadUint32(&dwData1);
	LOG_PROCESS_ERROR(nResult);

	nResult = ReadUint32(&dwData2);
	LOG_PROCESS_ERROR(nResult);

	(*pqwValue) = ((LPUINT64)dwData1 << 32) | (LPUINT64)dwData2;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::WriteUint64(LPUINT64 qwValue)
{
	LPINT32 nResult = 0;
	LPUINT32 dwData1 = 0;
	LPUINT32 dwData2 = 0;

	LOG_PROCESS_ERROR(sizeof(LPUINT64) == 8);

	dwData1 = qwValue >> 32;
	nResult = WriteUint32(dwData1);
	LOG_PROCESS_ERROR(nResult);

	dwData2 = (qwValue << 32) >> 32;
	nResult = WriteUint32(dwData2);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::ReadString(char * pcszData)
{
	LPINT32 nResult = 0;
	LPUINT32 dwLen = 0;
	const char* pBuf = NULL;

	LOG_PROCESS_ERROR(pcszData);

	nResult = ReadUint32(&dwLen);
	LOG_PROCESS_ERROR(nResult);

	LOG_PROCESS_ERROR(m_dwUnSerializeSize + dwLen <= m_dwUnSerializeBufSize);

	pBuf = m_pUnSerializeBuf != NULL ? m_pUnSerializeBuf : m_szUnSerializeBuf;

	memcpy(pcszData, pBuf + m_dwUnSerializeSize, dwLen);

	m_dwUnSerializeSize += dwLen;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPExternalMessageSerializer::WriteString(const char * pcszData, LPUINT32 dwLen, LPUINT32 dwMaxLen)
{
	LPINT32 nResult = 0;
	char* pBuf = NULL;

	LOG_PROCESS_ERROR(pcszData);

	if (dwLen > dwMaxLen)
	{
		dwLen = dwMaxLen;
	}

	nResult = WriteUint32(dwLen);
	LOG_PROCESS_ERROR(nResult);

	LOG_PROCESS_ERROR(m_dwSerializeSize + dwLen <= m_dwSerializeBufSize);
	pBuf = m_pSerializeBuf != NULL ? m_pSerializeBuf : m_szSerializeBuf;
	memcpy(pBuf + m_dwSerializeSize, pcszData, dwLen);

	m_dwSerializeSize += dwLen;

	return TRUE;
Exit0:
	return FALSE;
}

char *LPAPI LPExternalMessageSerializer::GetSerializeBuf(void)
{
	return NULL != m_pSerializeBuf ? m_pSerializeBuf : m_szSerializeBuf;
}

LPUINT32 LPAPI LPExternalMessageSerializer::GetSerializeSize(void)
{
	return m_dwSerializeSize;
}










//end声明所处的名字空间
NS_LZPL_END