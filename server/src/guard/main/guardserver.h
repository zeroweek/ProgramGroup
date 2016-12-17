//****************************************************************************/
//    author: caihy
//    date: December 4, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_GUARD_SERVER_H_
#define _LP_GUARD_SERVER_H_

#include "lp_lzpl.h"
#include "common.h"
#include "server_def.h"


using namespace LZPL;



// Summary:
//		нч
class CGuardServer
{
	SINGLETON_DECLARE(CGuardServer)

public:

	BOOL LPAPI Init(void);
	BOOL LPAPI UnInit(void);

	BOOL LPAPI MainLoop(void);

	LPUINT32 LPAPI GetServerState(void);
	void SetServerState(LPUINT32 dwServerState);

	void Close(void);

private:

	// Summary:
	//		none
	CGuardServer();

	// Summary:
	//		none
	virtual ~CGuardServer();

private:

	ILPNet*                          m_pNet;

	LPUINT32                          m_dwServerState;

};















#endif