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



//begin�������������ֿռ�
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
// time_t       t          ��Ҫ��ʾת��Ϊtime_t�ٴ���
// zip blob     z
//****************************************************************************/

// Summary:
//		ԭʼstmt sqlģ�����Ľṹ
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
	//		��
	LPSqlStmt();

	// Summary:
	//		��
	~LPSqlStmt();

	// Summary:
	//		��
	BOOL LPAPI Init(LPSqlMgr* pSqlMgr, INT_32 nSqlStmtId);

	// Summary:
	//		����ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		��
	BOOL LPAPI PushParam(INT_8 cValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(INT_16 hValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(INT_32 dValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(INT_64 lValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(UINT_8 ucValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(UINT_16 uhValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(UINT_32 udValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(UINT_64 ulValue);

	// Summary:
	//		��
	BOOL LPAPI PushTime(time_t tValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(const char* pcszValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(UINT_8* pucValue, UINT_32 dwValueSize);

	// Summary:
	//		��
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











//end�������������ֿռ�
NS_LZPL_END

#endif