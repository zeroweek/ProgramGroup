#include "lp_sqlstmt.h"
#include "mysql.h"
#include "lp_sqlmgr.h"
#include "lp_string.h"
#include "zlib.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN


LPSqlStmt::LPSqlStmt()
{
	pstPrev              = NULL;
	pstNext              = NULL;
	m_pSqlStmtData       = NULL;
	m_nParamCount        = 0;
	m_pucParamBuf        = NULL;
	m_nParamBufSize      = 0;
	m_pfnCallback        = NULL;
	m_pUserData          = NULL;
	m_nExecuteCount      = 0;
	m_pSqlMgr            = NULL;
}

LPSqlStmt::~LPSqlStmt()
{
	UnInit();
}

BOOL LPAPI LPSqlStmt::Init(LPSqlMgr * pSqlMgr, INT_32 nSqlStmtId)
{
	LOG_PROCESS_ERROR(pSqlMgr);

	m_pSqlMgr = pSqlMgr;

	m_pSqlStmtData = m_pSqlMgr->GetSqlStmtData(nSqlStmtId);
	LOG_PROCESS_ERROR(m_pSqlStmtData);

	m_pucParamBuf = new UINT_8[m_pSqlStmtData->nParamBufSize];
	LOG_PROCESS_ERROR(m_pucParamBuf);

	m_nParamCount = 0;
	m_nParamBufSize = 0;
	m_pfnCallback = NULL;
	m_pUserData = NULL;
	m_nExecuteCount = 0;

	return TRUE;
Exit0:

	return FALSE;
}

BOOL LPAPI LPSqlStmt::UnInit(void)
{
	m_pSqlStmtData = NULL;
	SAFE_DELETE_SZ(m_pucParamBuf);

	for (std::list<void*>::iterator it = m_lstResult.begin(); it != m_lstResult.end(); ++it)
	{
		SAFE_DELETE_SZ(*it);
	}
	m_lstResult.clear();

	return TRUE;
}

#define IMPL_PUSH_PARAM_FUNC(_ctype_, _mysql_type_, _is_unsigned_)									\
	BOOL LPSqlStmt::PushParam(_ctype_ _param)														\
	{																								\
		LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);								\
		LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == (_mysql_type_));				\
		LOG_PROCESS_ERROR(m_pSqlStmtData->bIsUnsigned[m_nParamCount] == (_is_unsigned_));			\
																									\
		memcpy(m_pucParamBuf + m_nParamBufSize, &_param, m_pSqlStmtData->nParamSize[m_nParamCount]);\
		m_nParamBufSize += m_pSqlStmtData->nParamSize[m_nParamCount];								\
		m_nParamCount++;																			\
																									\
		return TRUE;																				\
	Exit0:																							\
		return FALSE;																				\
	}

IMPL_PUSH_PARAM_FUNC(INT_8, MYSQL_TYPE_TINY, FALSE);
IMPL_PUSH_PARAM_FUNC(INT_16, MYSQL_TYPE_SHORT, FALSE);
IMPL_PUSH_PARAM_FUNC(INT_32, MYSQL_TYPE_LONG, FALSE);
IMPL_PUSH_PARAM_FUNC(INT_64, MYSQL_TYPE_LONGLONG, FALSE);

IMPL_PUSH_PARAM_FUNC(UINT_8, MYSQL_TYPE_TINY, TRUE);
IMPL_PUSH_PARAM_FUNC(UINT_16, MYSQL_TYPE_SHORT, TRUE);
IMPL_PUSH_PARAM_FUNC(UINT_32, MYSQL_TYPE_LONG, TRUE);
IMPL_PUSH_PARAM_FUNC(UINT_64, MYSQL_TYPE_LONGLONG, TRUE);

BOOL LPAPI LPSqlStmt::PushTime(time_t tValue)
{
	INT_32 nResult = 0;
	MYSQL_TIME mysqlTime;
	struct tm tTm;

	LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);
	LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == MYSQL_TYPE_DATETIME);
	
	nResult  = localtime_s(&tTm, &tValue);
	LOG_PROCESS_ERROR(0 == nResult);

	mysqlTime.year            = tTm.tm_year + 1900;
	mysqlTime.month           = tTm.tm_mon + 1;
	mysqlTime.day             = tTm.tm_mday;
	mysqlTime.hour            = tTm.tm_hour;
	mysqlTime.minute          = tTm.tm_min;
	mysqlTime.second          = tTm.tm_sec;

	mysqlTime.neg             = FALSE;
	mysqlTime.second_part     = 0;

	memcpy(m_pucParamBuf + m_nParamBufSize, &mysqlTime, m_pSqlStmtData->nParamSize[m_nParamCount]);
	m_nParamBufSize += m_pSqlStmtData->nParamSize[m_nParamCount];
	m_nParamCount++;	
	
	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPSqlStmt::PushParam(const char * pcszValue)
{
	INT_32 nStrLen = 0;

	LOG_PROCESS_ERROR(pcszValue);
	LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);
	LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == MYSQL_TYPE_STRING);

	nStrLen = (INT_32)lpStrNLen(pcszValue, m_pSqlStmtData->nParamSize[m_nParamCount]);
	LOG_PROCESS_ERROR(nStrLen < m_pSqlStmtData->nParamSize[m_nParamCount]);

	*(INT_32*)(m_pucParamBuf + m_nParamBufSize) = nStrLen;
	m_nParamBufSize += sizeof(INT_32);

	memcpy(m_pucParamBuf + m_nParamBufSize, pcszValue, nStrLen + 1);
	m_nParamBufSize += m_pSqlStmtData->nParamSize[m_nParamCount];
	m_nParamCount++;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPSqlStmt::PushParam(UINT_8 * pucValue, UINT_32 dwValueSize)
{
	LOG_PROCESS_ERROR(pucValue);
	LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);
	LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == MYSQL_TYPE_BLOB);
	LOG_PROCESS_ERROR(dwValueSize < (UINT_32)m_pSqlStmtData->nParamSize[m_nParamCount]);

	*(UINT_32*)(m_pucParamBuf + m_nParamBufSize) = dwValueSize;
	m_nParamBufSize += sizeof(UINT_32);

	memcpy(m_pucParamBuf + m_nParamBufSize, pucValue, dwValueSize);
	m_nParamBufSize += m_pSqlStmtData->nParamSize[m_nParamCount];
	m_nParamCount++;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPSqlStmt::PushParamZ(UINT_8 * pucValue, UINT_32 dwValueSize)
{
	INT_32 nResult = 0;
	UINT_32 *pdwSize = NULL;
	unsigned long ulBufLen = 0;

	LOG_PROCESS_ERROR(pucValue);
	LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);
	LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == MYSQL_TYPE_LONG_BLOB);
	LOG_PROCESS_ERROR(dwValueSize < (UINT_32)m_pSqlStmtData->nParamSize[m_nParamCount]);

	pdwSize = (UINT_32*)(m_pucParamBuf + m_nParamBufSize);
	m_nParamBufSize += sizeof(UINT_32);
	ulBufLen = m_pSqlStmtData->nParamSize[m_nParamCount];

	nResult = compress((unsigned char*)(m_pucParamBuf + m_nParamBufSize), &ulBufLen, (unsigned char*)pucValue, dwValueSize);
	LOG_PROCESS_ERROR(Z_OK == nResult);

	*pdwSize = ulBufLen;
	m_nParamBufSize += m_pSqlStmtData->nParamSize[m_nParamCount];
	m_nParamCount++;

	return TRUE;
Exit0:
	return FALSE;
}








//end�������������ֿռ�

NS_LZPL_END