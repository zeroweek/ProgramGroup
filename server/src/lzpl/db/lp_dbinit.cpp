#include "lp_dbinit.h"
#include "lp_processerror.h"
#include "mysql.h"
#include "lpi_file.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



DECLARE BOOL LPAPI lpDBExist(MYSQL * pMysql, const char * pcszDBName)
{
	LPINT32 nResult = 0;
	BOOL bFound = FALSE;
	MYSQL_RES* pMysqlRes = NULL;

	LOG_PROCESS_ERROR(pMysql);
	LOG_PROCESS_ERROR(pcszDBName);

	pMysqlRes = mysql_list_dbs(pMysql, pcszDBName);
	LOG_PROCESS_ERROR(pMysqlRes);
	
	while (mysql_fetch_row(pMysqlRes))
	{
		bFound = TRUE;
	}

Exit0:
	return bFound;
}

static BOOL _GetDBVersion(MYSQL* pMysql, LPINT32& nDBVersion)
{
	LPINT32 nResult = 0;
	BOOL bFound = FALSE;
	MYSQL_RES* pMysqlRes = NULL;
	MYSQL_ROW ppRow = NULL;

	LOG_PROCESS_ERROR(pMysql);

	pMysqlRes = mysql_list_tables(pMysql, "t_version");
	LOG_PROCESS_ERROR(pMysqlRes);

	while (mysql_fetch_row(pMysqlRes))
	{
		bFound = TRUE;
	}

	if (!bFound)
	{
		nDBVersion = -1;
		IMP("table t_version isn't exist");
		PROCESS_SUCCESS(TRUE);
	}

	nResult = mysql_query(pMysql, "SELECT `number` FROM `t_version` WHERE `id` = \"VERSION\";");
	LOG_PROCESS_ERROR(nResult == 0);

	pMysqlRes = mysql_store_result(pMysql);
	LOG_PROCESS_ERROR(pMysqlRes);

	ppRow = mysql_fetch_row(pMysqlRes);
	LOG_PROCESS_ERROR(ppRow);

	nDBVersion = atoi(ppRow[0]);

Exit1:

	if (pMysqlRes)
	{
		mysql_free_result(pMysqlRes);
	}

	return TRUE;

Exit0:

	FTL("*** %s failed to get db version", __FUNCTION__);
	if (mysql_errno(pMysql) != 0)
	{
		FTL("*** %s mysql error %d (%s)", __FUNCTION__, mysql_errno(pMysql), mysql_error(pMysql));
	}

	return FALSE;
}

static BOOL _ExecuteUpdateDBSqlFile(MYSQL* pMysql, const char* pcszDBPrefix, LPINT32 nVersion)
{
	LPINT32 nResult = 0;
	MYSQL_RES* pMysqlRes = NULL;

	LPUINT32 dwFileSize = 0;
	ILPFile* pFile = NULL;
	char szFileName[MAX_FILE_NAME];
	char* pszFileContent = NULL;

	LOG_PROCESS_ERROR(pMysql);
	LOG_PROCESS_ERROR(pcszDBPrefix);

	sprintf_s(szFileName, sizeof(szFileName) - 1, "config/sql/%s%d.sql", pcszDBPrefix, nVersion);

	pFile = ILPFile::OpenFile(szFileName, "rt");
	LOG_PROCESS_ERROR(pFile);

	dwFileSize = pFile->Size();
	LOG_PROCESS_ERROR(dwFileSize > 0);

	pszFileContent = new char[dwFileSize + 32];
	LOG_PROCESS_ERROR(pszFileContent);
	memset(pszFileContent, 0, dwFileSize);

	nResult = pFile->Read(pszFileContent, dwFileSize);
	LOG_PROCESS_ERROR(nResult > 0);

	nResult = mysql_query(pMysql, pszFileContent);
	LOG_PROCESS_ERROR(nResult == 0);

	do 
	{
		pMysqlRes = mysql_store_result(pMysql);
		if (NULL == pMysqlRes)
		{
			LOG_PROCESS_ERROR(mysql_field_count(pMysql) == 0);
		}
		else
		{
			mysql_free_result(pMysqlRes);
		}
	} while (!mysql_next_result(pMysql));

	pFile->Close();
	pFile = NULL;

	SAFE_DELETE_SZ(pszFileContent);

	IMP("execute sql file %s%d.sql successfully", pcszDBPrefix, nVersion);

	return TRUE;
Exit0:

	FTL("*** %s fail to execute sql file %s%d.sql", __FUNCTION__, pcszDBPrefix, nVersion);
	if (mysql_errno(pMysql) != 0)
	{
		FTL("*** %s mysql error %d (%s)", __FUNCTION__, mysql_errno(pMysql), mysql_error(pMysql));
	}

	if (pFile)
	{
		pFile->Close();
		pFile = NULL;
	}

	if (pszFileContent)
	{
		SAFE_DELETE_SZ(pszFileContent);
	}

	return FALSE;
}

DECLARE BOOL LPAPI lpDBCreate(MYSQL * pMysql, const char * pcszDBName)
{
	LPINT32 nResult = 0;
	char szDBSql[1024];

	LOG_PROCESS_ERROR(pMysql);
	LOG_PROCESS_ERROR(pcszDBName);

	sprintf_s(szDBSql, sizeof(szDBSql) - 1, "CREATE DATABASE %s;", pcszDBName);

	nResult = mysql_query(pMysql, szDBSql);
	LOG_PROCESS_ERROR(nResult == 0);

	return TRUE;
Exit0:

	FTL("*** %s create db fail", __FUNCTION__);
	if (mysql_errno(pMysql) != 0)
	{
		FTL("*** %s mysql error %d (%s)", __FUNCTION__, mysql_errno(pMysql), mysql_error(pMysql));
	}
	return FALSE;
}

DECLARE BOOL LPAPI lpDBUpdate(MYSQL * pMysql, const char * pcszDBName, const char * pcszDBPrefix, LPINT32 nNewDBVersion)
{
	LPINT32 nResult = 0;
	LPINT32 nCurDBVersion = 0;
	char szDBSql[1024];

	LOG_PROCESS_ERROR(pMysql);
	LOG_PROCESS_ERROR(pcszDBName);
	LOG_PROCESS_ERROR(pcszDBPrefix);

	sprintf_s(szDBSql, sizeof(szDBSql) - 1, "USE %s;", pcszDBName);

	nResult = mysql_query(pMysql, szDBSql);
	LOG_PROCESS_ERROR(nResult == 0);

	nResult = _GetDBVersion(pMysql, nCurDBVersion);
	LOG_PROCESS_ERROR(nResult);

	if (nCurDBVersion > nNewDBVersion)
	{
		INF("current db version is %d, newer than required version %d", nCurDBVersion, nNewDBVersion);
		PROCESS_ERROR(FALSE);
	}

	if (nCurDBVersion == nNewDBVersion)
	{
		INF("current db version is %d, already up to new version", nCurDBVersion, nNewDBVersion);
		PROCESS_SUCCESS(TRUE);
	}

	++nCurDBVersion;
	for (; nCurDBVersion <= nNewDBVersion; ++nCurDBVersion)
	{
		nResult = _ExecuteUpdateDBSqlFile(pMysql, pcszDBPrefix, nCurDBVersion);
		if (nResult)
		{
			INF("update db version to %d successfully", nCurDBVersion);
		}
		else
		{
			INF("*** update db version to %d failed", nCurDBVersion);
			PROCESS_ERROR(FALSE);
		}
	}

Exit1:
	return TRUE;

Exit0:

	FTL("*** %s update db fail", __FUNCTION__);
	if (mysql_errno(pMysql) != 0)
	{
		FTL("*** %s mysql error %d (%s)", __FUNCTION__, mysql_errno(pMysql), mysql_error(pMysql));
	}

	return FALSE;
}



//end声明所处的名字空间

NS_LZPL_END
