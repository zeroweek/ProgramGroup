#include "guardserver.h"
#include "lp_processerror.h"



SINGLETON_IMPLEMENT(CGuardServer)

CGuardServer::CGuardServer()
{
	m_pNet          = NULL;
	m_dwServerState = eServerState_Invalid;
}

CGuardServer::~CGuardServer()
{
	UnInit();
}

BOOL LPAPI CGuardServer::Init(void)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(FALSE);

	return TRUE;

Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL LPAPI CGuardServer::UnInit(void)
{
	LPINT32 nResult = 0;

	SetServerState(eServerState_UnIniting);

	if (NULL != m_pNet)
	{
		m_pNet->Release();
		m_pNet = NULL;
	}

	SetServerState(eServerState_UnInited);

	return TRUE;
}

BOOL LPAPI CGuardServer::MainLoop(void)
{
	lpSleep(1);

	return TRUE;
}

LPUINT32 LPAPI CGuardServer::GetServerState(void)
{
	return m_dwServerState;
}

void CGuardServer::SetServerState(LPUINT32 dwServerState)
{
	m_dwServerState = dwServerState;
}

void CGuardServer::Close(void)
{
	SetServerState(eServerState_Closed);
}



int main(int argc, char* argv[])
{
	LPINT32 nResult = 0;

	RedefinedConsole();

	nResult = CGuardServer::Instance().Init();
	LOG_PROCESS_ERROR(nResult);

	LPPRINTF("guardserver start success !\n");
	IMP("guardserver start success !");

	while (CGuardServer::Instance().GetServerState() != eServerState_Closing)
	{
		nResult = CGuardServer::Instance().MainLoop();
		LOG_CHECK_ERROR(nResult);
	}

	CGuardServer::Instance().Close();

Exit0:

	CGuardServer::Instance().UnInit();

	LPPRINTF("\nEnter any key to exit !\n");
	getchar();

	return 0;
}
