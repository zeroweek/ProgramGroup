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
// LPINT8        c  
// LPUINT8       uc
// LPINT16       h   
// LPUINT16      uh
// LPINT32       d
// LPUINT32      ud
// LPINT64       l
// LPUINT64      ul
// char*        s
// time_t       t          ��Ҫ��ʾת��Ϊtime_t�ٴ���
// zip blob     z
//****************************************************************************/

// Summary:
//		ԭʼstmt sqlģ�����Ľṹ
struct SQL_STMT_DATA
{
	LPINT32           nThreadIndex;
	LPINT32           nSqlStmtId;

	const char*      pcszSql;
	char             szQuery[SQL_MAX_QUERY_LEN];
	LPINT32           nQuerySize;

	LPINT32           nParamCount;
	LPINT32           nParamType[SQL_MAX_STMT_PARAM_COUNT];
	LPINT32           nParamSize[SQL_MAX_STMT_PARAM_COUNT];
	bool             bIsUnsigned[SQL_MAX_STMT_PARAM_COUNT];
	LPINT32           nParamBufSize;

	LPINT32           nResultCount;
	LPINT32           nResultType[SQL_MAX_STMT_PARAM_COUNT];
	LPINT32           nResultSize[SQL_MAX_STMT_PARAM_COUNT];
	LPINT32           nResultBufSize;

	LPINT32           nBufSize;
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
	BOOL LPAPI Init(LPSqlMgr* pSqlMgr, LPINT32 nSqlStmtId);

	// Summary:
	//		����ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPINT8 cValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPINT16 hValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPINT32 dValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPINT64 lValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPUINT8 ucValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPUINT16 uhValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPUINT32 udValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPUINT64 ulValue);

	// Summary:
	//		��
	BOOL LPAPI PushTime(time_t tValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(const char* pcszValue);

	// Summary:
	//		��
	BOOL LPAPI PushParam(LPUINT8* pucValue, LPUINT32 dwValueSize);

	// Summary:
	//		��
	BOOL LPAPI PushParamZ(LPUINT8* pucValue, LPUINT32 dwValueSize);

private:

	const SQL_STMT_DATA*       m_pSqlStmtData;
	LPINT32                        m_nParamCount;
	LPUINT8*                       m_pucParamBuf;
	LPINT32                        m_nParamBufSize;
	std::list<void*>              m_lstResult;
	pfunSqlStmtCallback           m_pfnCallback;
	void*                         m_pUserData;
	LPINT32                        m_nExecuteCount;

	friend class LPSqlMgr;
	LPSqlMgr*                     m_pSqlMgr;

};



//end�������������ֿռ�
NS_LZPL_END

#endif