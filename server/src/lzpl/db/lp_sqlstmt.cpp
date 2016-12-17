#include "lp_sqlstmt.h"
#include "mysql.h"
#include "lp_sqlmgr.h"
#include "lp_string.h"
#include "zlib.h"



//begin声明所处的名字空间
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

BOOL LPAPI LPSqlStmt::Init(LPSqlMgr * pSqlMgr, LPINT32 nSqlStmtId)
{
	LOG_PROCESS_ERROR(pSqlMgr);

	m_pSqlMgr = pSqlMgr;

	m_pSqlStmtData = m_pSqlMgr->GetSqlStmtData(nSqlStmtId);
	LOG_PROCESS_ERROR(m_pSqlStmtData);

	m_pucParamBuf = new LPUINT8[m_pSqlStmtData->nParamBufSize];
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

IMPL_PUSH_PARAM_FUNC(LPINT8, MYSQL_TYPE_TINY, FALSE);
IMPL_PUSH_PARAM_FUNC(LPINT16, MYSQL_TYPE_SHORT, FALSE);
IMPL_PUSH_PARAM_FUNC(LPINT32, MYSQL_TYPE_LONG, FALSE);
IMPL_PUSH_PARAM_FUNC(LPINT64, MYSQL_TYPE_LONGLONG, FALSE);

IMPL_PUSH_PARAM_FUNC(LPUINT8, MYSQL_TYPE_TINY, TRUE);
IMPL_PUSH_PARAM_FUNC(LPUINT16, MYSQL_TYPE_SHORT, TRUE);
IMPL_PUSH_PARAM_FUNC(LPUINT32, MYSQL_TYPE_LONG, TRUE);
IMPL_PUSH_PARAM_FUNC(LPUINT64, MYSQL_TYPE_LONGLONG, TRUE);

BOOL LPAPI LPSqlStmt::PushTime(time_t tValue)
{
	LPINT32 nResult = 0;
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
	LPINT32 nStrLen = 0;

	LOG_PROCESS_ERROR(pcszValue);
	LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);
	LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == MYSQL_TYPE_STRING);

	nStrLen = (LPINT32)lpStrNLen(pcszValue, m_pSqlStmtData->nParamSize[m_nParamCount]);
	LOG_PROCESS_ERROR(nStrLen < m_pSqlStmtData->nParamSize[m_nParamCount]);

	*(LPINT32*)(m_pucParamBuf + m_nParamBufSize) = nStrLen;
	m_nParamBufSize += sizeof(LPINT32);

	memcpy(m_pucParamBuf + m_nParamBufSize, pcszValue, nStrLen + 1);
	m_nParamBufSize += m_pSqlStmtData->nParamSize[m_nParamCount];
	m_nParamCount++;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPSqlStmt::PushParam(LPUINT8 * pucValue, LPUINT32 dwValueSize)
{
	LOG_PROCESS_ERROR(pucValue);
	LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);
	LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == MYSQL_TYPE_BLOB);
	LOG_PROCESS_ERROR(dwValueSize < (LPUINT32)m_pSqlStmtData->nParamSize[m_nParamCount]);

	*(LPUINT32*)(m_pucParamBuf + m_nParamBufSize) = dwValueSize;
	m_nParamBufSize += sizeof(LPUINT32);

	memcpy(m_pucParamBuf + m_nParamBufSize, pucValue, dwValueSize);
	m_nParamBufSize += m_pSqlStmtData->nParamSize[m_nParamCount];
	m_nParamCount++;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPSqlStmt::PushParamZ(LPUINT8 * pucValue, LPUINT32 dwValueSize)
{
	LPINT32 nResult = 0;
	LPUINT32 *pdwSize = NULL;
	unsigned long ulBufLen = 0;

	LOG_PROCESS_ERROR(pucValue);
	LOG_PROCESS_ERROR(m_nParamCount < m_pSqlStmtData->nParamCount);
	LOG_PROCESS_ERROR(m_pSqlStmtData->nParamType[m_nParamCount] == MYSQL_TYPE_LONG_BLOB);
	LOG_PROCESS_ERROR(dwValueSize < (LPUINT32)m_pSqlStmtData->nParamSize[m_nParamCount]);

	pdwSize = (LPUINT32*)(m_pucParamBuf + m_nParamBufSize);
	m_nParamBufSize += sizeof(LPUINT32);
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








//end声明所处的名字空间

NS_LZPL_END
