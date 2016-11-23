//****************************************************************************/
//    author: caihy
//    date: October 10, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_SQL_STMT_H_
#define _LP_SQL_STMT_H_

#include "lp_base.h"
#include "lp_baselist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define SQL_MAX_STMT_PARAM_COUNT              (256)
#define SQL_MAX_STMT_RESULT_COUNT             (32)
#define SQL_MAX_QUERY_LEN                     (1024 * 8)



//****************************************************************************/
// INT_8        c  
// UINT_8       uc
// INT_16       h   
// UINT_16      uh
// INT_32       d
// UINT_32      ud
// INT_64       l
// UINT_64      ul
// char*        s
// time_t       t          需要显示转换为time_t再传入
// zip blob     z
//****************************************************************************/

// Summary:
//		原始stmt sql模板语句的结构
struct SQL_STMT_DATA
{
	INT_32           nThreadIndex;
	INT_32           nSqlStmtId;

	const char*      pcszSql;
	char             szQuery[SQL_MAX_QUERY_LEN];
	INT_32           nQuerySize;

	INT_32           nParamCount;
	INT_32           nParamType[SQL_MAX_STMT_PARAM_COUNT];
	INT_32           nParamSize[SQL_MAX_STMT_PARAM_COUNT];
	bool             bIsUnsigned[SQL_MAX_STMT_PARAM_COUNT];
	INT_32           nParamBufSize;

	INT_32           nResultCount;
	INT_32           nResultType[SQL_MAX_STMT_PARAM_COUNT];
	INT_32           nResultSize[SQL_MAX_STMT_PARAM_COUNT];
	INT_32           nResultBufSize;

	INT_32           nBufSize;
};



class LPSqlMgr;

typedef BOOL(*pfunSqlStmtCallback)(std::list<void*>& lstResult, void* pUserData);



// Summary:
//		
class DECLARE LPSqlStmt : public BASE_LIST_NODE
{
public:

	// Summary:
	//		无
	LPSqlStmt();

	// Summary:
	//		无
	~LPSqlStmt();

	// Summary:
	//		无
	BOOL LPAPI Init(LPSqlMgr* pSqlMgr, INT_32 nSqlStmtId);

	// Summary:
	//		反初始化
	BOOL LPAPI UnInit(void);

	// Summary:
	//		无
	BOOL LPAPI PushParam(INT_8 cValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(INT_16 hValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(INT_32 dValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(INT_64 lValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(UINT_8 ucValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(UINT_16 uhValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(UINT_32 udValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(UINT_64 ulValue);

	// Summary:
	//		无
	BOOL LPAPI PushTime(time_t tValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(const char* pcszValue);

	// Summary:
	//		无
	BOOL LPAPI PushParam(UINT_8* pucValue, UINT_32 dwValueSize);

	// Summary:
	//		无
	BOOL LPAPI PushParamZ(UINT_8* pucValue, UINT_32 dwValueSize);

private:

	const SQL_STMT_DATA*       m_pSqlStmtData;
	INT_32                        m_nParamCount;
	UINT_8*                       m_pucParamBuf;
	INT_32                        m_nParamBufSize;
	std::list<void*>              m_lstResult;
	pfunSqlStmtCallback           m_pfnCallback;
	void*                         m_pUserData;
	INT_32                        m_nExecuteCount;

	friend class LPSqlMgr;
	LPSqlMgr*                     m_pSqlMgr;

};











//end声明所处的名字空间
NS_LZPL_END

#endif