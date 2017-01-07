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



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define SQL_MAX_DB_WORKER_THREAD_COUNT        (8)



// Summary:
//		sql工作线程参数
struct SQL_THREAD_PARAM
{
	LPSqlMgr         *pSqlMgr;
	LPINT32           nIndex;
};



// Summary:
//		
class DECLARE LPSqlMgr
{
public:

	// Summary:
	//		无
	LPSqlMgr();

	// Summary:
	//		无
	~LPSqlMgr();

	// Summary:
	//		
	// Return:
	//		TRUE-成功，FALSE-失败
	BOOL LPAPI Init(
		const char* pcszServerAddr, 
		const char* pcszUserName, 
		const char* pcszPwd,
		const char* pcszDbName,
		const char* pcszCharSet,
		LPUINT32 dwServerPort,
		SQL_STMT_DATA* pSqlStmtList,
		LPINT32 nSqlStmtCount,
		LPINT32 nThreadCount);

	// Summary:
	//		反初始化
	BOOL LPAPI UnInit(void);

	// Summary:
	//		无
	BOOL LPAPI MainLoop(void);

	// Summary:
	//		无
	BOOL LPAPI AsyncExecSqlStmt(LPINT32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void* pUserData, const char* pcszFormat, ...);

	// Summary:
	//		无
	BOOL LPAPI SyncExecSqlStmt(LPINT32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void* pUserData, const char* pcszFormat, ...);

	// Summary:
	//		无
	const SQL_STMT_DATA* LPAPI GetSqlStmtData(LPINT32 nSqlStmtId);

	// Summary:
	//		无
	inline BOOL LPAPI IsCompleted(void);

private:

	// Summary:
	//		无
	BOOL LPAPI _ParseSqlStmt(SQL_STMT_DATA* pSqlStmtData);

	// Summary:
	//		无
	LPSqlStmt* LPAPI _NewSqlStmt(LPINT32 nSqlStmtId);

	// Summary:
	//		无
	BOOL LPAPI _DelSqlStmt(LPSqlStmt* pSqlStmt);

	// Summary:
	//		无
	LPSqlStmt* LPAPI _ExecSqlStmt(LPINT32 nSqlStmtId, pfunSqlStmtCallback pfnCallback, void* pUserData, const char* pcszFormat, va_list valist);

	// Summary:
	//		无
	static void LPAPI _WorkerThreadFunc(SQL_THREAD_PARAM* pstSqlThreadParam);

private:

	LPINT32                      m_nSqlStmtCount;
	SQL_STMT_DATA*           m_pSqlStmtList;

	char                        m_szServerAddr[COMMON_NAME_LEN];
	char                        m_szUserName[COMMON_NAME_LEN];
	char                        m_szPwd[COMMON_NAME_LEN];
	char                        m_szDbName[COMMON_NAME_LEN];
	char                        m_szCharSet[COMMON_NAME_LEN];
	LPUINT32                     m_dwServerPort;

	LPINT32                      m_nThreadCount;
	std::thread**               m_pThread;
	BOOL                        m_bStopThread;
	LPLoopList                  m_PushList[SQL_MAX_DB_WORKER_THREAD_COUNT];
	LPLoopList                  m_PopList[SQL_MAX_DB_WORKER_THREAD_COUNT];
	LPINT32                      m_nThreadIndex;

	LPBaseList*                 m_pSqlStmtPool;
	LPINT32                      m_nNewSqlStmtCount;
	LPINT32                      m_nDelSqlStmtCount;

};

inline BOOL LPAPI LPSqlMgr::IsCompleted(void)
{
	return m_nNewSqlStmtCount == m_nDelSqlStmtCount;
}



//end声明所处的名字空间
NS_LZPL_END

#endif