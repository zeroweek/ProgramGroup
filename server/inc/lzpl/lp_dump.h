//****************************************************************************/
//    author: caihy
//    date: December 24, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_DUMP_H_
#define _LP_DUMP_H_

#include "lp_base.h"
#include "lpi_socker.h"
#include "lp_thread.h"
#include "lp_lock.h"
#include "lp_time.h"


//begin�������������ֿռ�
NS_LZPL_BEGIN



#ifndef _WIN32
	typedef struct _EXCEPTION_POINTERS {
	} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
	#define EXCEPTION_EXECUTE_HANDLER      1
	#define EXCEPTION_CONTINUE_SEARCH      0
	#define EXCEPTION_CONTINUE_EXECUTION (-1)
#endif


// Summary:
//		dump��������
class DECLARE LPDumpMgr : public LPThread
{
	SINGLETON_DECLARE(LPDumpMgr)

public:

	typedef BOOL(*pfunDumpCheckFunc)(void);

public:

	LPDumpMgr();
	~LPDumpMgr();

	BOOL LPAPI Init(pfunDumpCheckFunc pDumpCheckFunc, const char* pcszDumpFloder);
	BOOL LPAPI UnInit(void);

	BOOL LPAPI PushException(EXCEPTION_POINTERS* pExceptionPointers);

private:

	void LPAPI _CreateDumpFile(EXCEPTION_POINTERS* pExceptionPointers, UINT_32 dwProcessId, UINT_32 dwThreadId);

private:

	static THREAD_FUNC_DECLARE(_DumpThreadProc)(void* pParam);

private:

	BOOL                m_bDumpThreadRun = TRUE;
	LPLock              m_oDumpLock;
	LPThread            m_oCreateDumpThread;

	typedef std::list<EXCEPTION_POINTERS*> LPListException;
	LPListException     m_lstDump;

	char                m_szDumpFloder[MAX_PATH];
	pfunDumpCheckFunc   m_pDumpCheckFunc = NULL;

	INT_32              m_nLastDumpIndex;
	LPTime              m_oLastDumpTime;

};


DECLARE long LPAPI lpDUMPExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers);









//end�������������ֿռ�
NS_LZPL_END

#endif