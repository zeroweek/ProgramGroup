#include "global_config.h"
#include "server_config.h"
#include "guard_config.h"
#include "lp_processerror.h"
#include "lp_luascriptmgr.h"
#include "tolua++.h"
#include "../script/tolua_global_config.h"


using namespace LZPL;

GLOBAL_CONFIG g_GlobalConfig;

char g_szGlobalConfigFileName[MAX_PATH] = "config/globalconfig/global_config.lua";

BOOL g_LoadGlobalConfig(const char* pcszFileName)
{
	LPINT32 nResult = 0;
	LPINT32 nStackTop = 0;

	LPLuaScript oScript;
	char szRootPath[MAX_PATH] = { 0 };

	lpStrCpyN(szRootPath, LPLuaScriptMgr::Instance().GetRootPath(), MAX_PATH);
	LPLuaScriptMgr::Instance().SetRootPath(NULL);

	nResult = oScript.Init();
	LOG_PROCESS_ERROR(nResult);

	tolua_global_config_open(oScript.GetLuaState());

	nResult = oScript.LoadFromFile(pcszFileName);
	LOG_PROCESS_ERROR(nResult);
	
	nResult = oScript.CallFunction("load_file", "os", &g_GlobalConfig, "GLOBAL_CONFIG", "GlobalConfig");
	LOG_PROCESS_ERROR(nResult);

	nResult = oScript.UnInit();
	LOG_CHECK_ERROR(nResult);

	LPLuaScriptMgr::Instance().SetRootPath(szRootPath);
		
	return TRUE;

Exit0:

	oScript.UnInit();
	LPLuaScriptMgr::Instance().SetRootPath(szRootPath);

	return FALSE;
}
