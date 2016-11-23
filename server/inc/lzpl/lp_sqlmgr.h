//****************************************************************************/
//    author: caihy
//    date: October 10, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_SQL_MGR_H_
#define _LP_SQL_MGR_H_

#include "lp_base.h"
#include "lp_looplist.h"
#include "lp_baselist.h"
#include "lp_sqlstmt.h"


typedef struct st_mysql MYSQL;
typedef struct st_mysql_stmt MYSQL_STMT;
typedef struct st_mysql_bind MYSQL_BIND;


//begin�������������ֿռ�
NS_LZPL_BEGIN


#define SQL_MAX_DB_WORKER_THREAD_COUNT        (8)



// Summary:
//		sql�����̲߳���
struct SQL_THREAD_PARAM
{
	LPSqlMgr         *pSqlMgr;
	INT_32           nIndex;
};



// Summary:
//		
class DECLARE LPSqlMgr
{
public:

	// Summary:
	//		��
	LPSqlMgr();

	// Summary:
	//		��
	~LPSqlMgr();

	// Summary:
	//		
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	BOOL LPAPI Init(
		const char* pcszServerAddr, 
		const char* pcszUserName, 
		const char* pcszPwd,
		const char* pcszDbName,
		const char* pcszCharSet,
		UINT_32 dwServerPort,
		SQL_STMT_DATA* pSqlStmtList,
		INT_32 nSqlStmtCount,
		INT_32 nThreadCount);

	// Summary:
	//		����ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		��
	BOOL LPAPI MainLoop(void);

	// Summary:
	//		��
	BOOL LPAPI AsyncExecSqlStmt(INT_32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void* pUserData, const char* pcszFormat, ...);

	// Summary:
	//		��
	BOOL LPAPI SyncExecSqlStmt(INT_32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void* pUserData, const char* pcszFormat, ...);

	// Summary:
	//		��
	const SQL_STMT_DATA* LPAPI GetSqlStmtData(INT_32 nSqlStmtId);

	// Summary:
	//		��
	inline BOOL LPAPI IsCompleted(void);

private:

	// Summary:
	//		��
	BOOL LPAPI _ParseSqlStmt(SQL_STMT_DATA* pSqlStmtData);

	// Summary:
	//		��
	LPSqlStmt* LPAPI _NewSqlStmt(INT_32 nSqlStmtId);

	// Summary:
	//		��
	BOOL LPAPI _DelSqlStmt(LPSqlStmt* pSqlStmt);

	// Summary:
	//		��
	LPSqlStmt* LPAPI _ExecSqlStmt(INT_32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void* pUserData, const char* pcszFormat, va_list valist);

	// Summary:
	//		��
	static void LPAPI _WorkerThreadFunc(SQL_THREAD_PARAM* pstSqlThreadParam);

private:

	INT_32                      m_nSqlStmtCount;
	SQL_STMT_DATA*           m_pSqlStmtList;

	char                        m_szServerAddr[COMMON_NAME_LEN];
	char                        m_szUserName[COMMON_NAME_LEN];
	char                        m_szPwd[COMMON_NAME_LEN];
	char                        m_szDbName[COMMON_NAME_LEN];
	char                        m_szCharSet[COMMON_NAME_LEN];
	UINT_32                     m_dwServerPort;

	INT_32                      m_nThreadCount;
	std::thread**               m_pThread;
	BOOL                        m_bStopThread;
	LPLoopList                  m_PushList[SQL_MAX_DB_WORKER_THREAD_COUNT];
	LPLoopList                  m_PopList[SQL_MAX_DB_WORKER_THREAD_COUNT];
	INT_32                      m_nThreadIndex;

	LPBaseList*                 m_pSqlStmtPool;
	INT_32                      m_nNewSqlStmtCount;
	INT_32                      m_nDelSqlStmtCount;

};


inline BOOL LPAPI LPSqlMgr::IsCompleted(void)
{
	return m_nNewSqlStmtCount == m_nDelSqlStmtCount;
}








//end�������������ֿռ�
NS_LZPL_END

#endif