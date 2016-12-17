#include "gamelogic.h"
#include "lp_processerror.h"
#include "../script/script_func.h"
#include "string_def.h"
#include "tab_test_data.h"
#include "global_config.h"
#include "bt_manager.h"
#include "ai_data.h"
#include "ai_manager.h"



extern int tolua_common_open(lua_State* tolua_S);
extern int tolua_ai_open(lua_State* tolua_S);



SINGLETON_IMPLEMENT(CGameLogic)

CGameLogic::CGameLogic()
{
}

CGameLogic::~CGameLogic()
{
	//这里不能调用，该函数内部会调用其他模块的UnInit函数，而
	//其他模块可能在本模块释放前已经释放，再调用UnInit会出错
	//UnInit();
}

BOOL LPAPI CGameLogic::Init(void)
{
	LPINT32 nResult = 0;

	lpStrCpyN(m_szConfigPath, g_GlobalConfig.Server.Gl.szConfigPath, MAX_PATH);
	
	nResult = _InitObjPool();
	LOG_PROCESS_ERROR(nResult);

	nResult = _InitScript();
	LOG_PROCESS_ERROR(nResult);

	nResult = _InitSettingData();
	LOG_PROCESS_ERROR(nResult);

	nResult = CBTManager::Instance().Init();
	LOG_PROCESS_ERROR(nResult);

	nResult = CAIManager::Instance().Init();
	LOG_PROCESS_ERROR(nResult);

	return TRUE;

Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL LPAPI CGameLogic::UnInit(void)
{
	LPINT32 nResult = 0;

	nResult = CAIManager::Instance().UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = CBTManager::Instance().UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = _UnInitSettingData();
	LOG_CHECK_ERROR(nResult);

	nResult = _UnInitScript();
	LOG_CHECK_ERROR(nResult);

	return TRUE;
}

BOOL LPAPI CGameLogic::MainLoop(void)
{
	LPINT32 nResult = 0;

	lpSleep(1);

	return TRUE;
}

BOOL LPAPI CGameLogic::_InitObjPool(void)
{
	LPINT32 nResult = 0;

	nResult = m_oBTNodePool.Init(1000, TRUE);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI CGameLogic::_InitScript(void)
{
	LPINT32 nResult = 0;

	nResult = LPLuaScriptMgr::Instance().Init(g_GlobalConfig.Server.Gl.szConfigPath);
	LOG_PROCESS_ERROR(nResult);

	nResult = LPLuaScriptMgr::Instance().RegisterPackage("common.pkg", tolua_common_open);
	LOG_PROCESS_ERROR(nResult);

	nResult = LPLuaScriptMgr::Instance().RegisterPackage("ai.pkg", tolua_ai_open);
	LOG_PROCESS_ERROR(nResult);

	nResult = LPLuaScriptMgr::Instance().RegisterFuncList(g_LuaScriptFuncTable);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI CGameLogic::_UnInitScript(void)
{
	LPINT32 nResult = 0;

	nResult = LPLuaScriptMgr::Instance().UnInit();
	LOG_CHECK_ERROR(nResult);

	return TRUE;
}

BOOL LPAPI CGameLogic::_InitSettingData(void)
{
	LPINT32 nResult = 0;
	char szFilePath[MAX_PATH];

	sprintf_s(szFilePath, MAX_PATH, "%s/%s", GetConfigPath(), DATA_FLODER_NAME);

	nResult = LPDataManger<TAB_TEST_DATA>::Instance().Init(DATA_TAB_TEST_FILE_NAME, szFilePath, NULL);
	LOG_PROCESS_ERROR(nResult);
	//LPDataManger<TAB_TEST_DATA>::Instance().Print();

	nResult = LPDataManger<AI_DATA>::Instance().Init(DATA_AI_FILE_NAME, szFilePath, NULL);
	LOG_PROCESS_ERROR(nResult);
	//LPDataManger<AI_DATA>::Instance().Print();

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI CGameLogic::_UnInitSettingData(void)
{
	LPINT32 nResult = 0;

	nResult = LPDataManger<TAB_TEST_DATA>::Instance().UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = LPDataManger<AI_DATA>::Instance().UnInit();
	LOG_CHECK_ERROR(nResult);

	return TRUE;
}
