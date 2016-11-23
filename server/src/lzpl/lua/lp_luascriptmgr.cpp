#include "lp_luascriptmgr.h"
#include "lp_processerror.h"
#include "lpi_file.h"
#include "lp_string.h"
#include "lstate.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN

SINGLETON_IMPLEMENT(LPLuaScriptMgr)

LPLuaScriptMgr::LPLuaScriptMgr()
{
	m_bReload = FALSE;
	lpFastZeroCharArray(m_szRootPath);
}

LPLuaScriptMgr::~LPLuaScriptMgr()
{
	UnInit();
}

BOOL LPAPI LPLuaScriptMgr::Init(const char * pcszRootPath)
{
	INT_32 nResult = 0;

	LOG_PROCESS_ERROR(pcszRootPath);

	m_bReload = FALSE;
	lpStrCpyN(m_szRootPath, pcszRootPath, MAX_PATH);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPLuaScriptMgr::UnInit(void)
{
	for (MAP_NAME_2_PACKAGE::iterator it = m_mapPackage.begin();
		it != m_mapPackage.end(); ++it)
	{
		const char* pcszPackageName = it->first;
		SAFE_DELETE_SZ(pcszPackageName);
	}
	m_mapPackage.clear();

	for (MAP_NAME_2_SCRIPT::iterator it = m_mapScript.begin();
		it != m_mapScript.end(); ++it)
	{
		it->second->UnInit();
		SAFE_DELETE(it->second);
	}
	m_mapScript.clear();

	m_lstLuaFunc.clear();
	lpFastZeroCharArray(m_szRootPath);

	return TRUE;
}

BOOL LPAPI LPLuaScriptMgr::Reload(INT_32 & nErrorCount)
{
	INT_32 nResult = 0;

	m_bReload = FALSE;
	nErrorCount = 0;

	for (MAP_NAME_2_SCRIPT::iterator it = m_mapScript.begin();
	it != m_mapScript.end(); ++it)
	{
		LPLuaScript* poScript = it->second;
		nResult = poScript->LoadFromFile(it->first);
		LOG_CHECK_ERROR(nResult);

		if (!nResult)
		{
			nErrorCount++;
		}
	}

	return TRUE;
}

BOOL LPAPI LPLuaScriptMgr::Reload(const char * pcszSciptName)
{
	INT_32 nResult = 0;
	LPLuaScript* poScript = NULL;
	MAP_NAME_2_SCRIPT::iterator it;

	LOG_PROCESS_ERROR(pcszSciptName);

	it = m_mapScript.find(pcszSciptName);
	LOG_PROCESS_ERROR(it != m_mapScript.end());

	poScript = it->second;

	nResult = poScript->LoadFromFile(it->first);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

LPLuaScript *LPAPI LPLuaScriptMgr::NewScript(const char * pcszSciptName)
{
	INT_32 nResult = FALSE;
	char szFileName[MAX_FILE_NAME];
	LPLuaScript* poScript = NULL;
	std::pair<MAP_NAME_2_SCRIPT::iterator, bool> InsRet;

	LOG_PROCESS_ERROR(pcszSciptName);

	poScript = new LPLuaScript();
	LOG_PROCESS_ERROR(poScript);

	nResult = poScript->Init();
	LOG_PROCESS_ERROR(nResult);

	lua_register(poScript->GetLuaState(), "load_package", _LoadPackage);

	for (LST_LUA_FUNC::iterator it = m_lstLuaFunc.begin();
		it != m_lstLuaFunc.end(); ++it)
	{
		nResult = poScript->RegisterFunc(*it, 0);
		LOG_PROCESS_ERROR(nResult);
	}

	sprintf_s(szFileName, MAX_FILE_NAME, "%s/%s", m_szRootPath, pcszSciptName);
	nResult = poScript->LoadFromFile(szFileName);
	LOG_PROCESS_ERROR(nResult);

	InsRet = m_mapScript.insert(std::make_pair(poScript->GetScriptName(), poScript));
	LOG_PROCESS_ERROR(InsRet.second);

	return poScript;

Exit0:

	if (poScript)
	{
		poScript->UnInit();
		SAFE_DELETE(poScript);
	}

	return NULL;
}

BOOL LPAPI LPLuaScriptMgr::DelScript(LPLuaScript * poScript)
{
	MAP_NAME_2_SCRIPT::iterator it;

	PROCESS_ERROR(poScript);

	it = m_mapScript.find(poScript->GetScriptName());
	LOG_PROCESS_ERROR(it != m_mapScript.end());
	SAFE_DELETE(it->second);

	m_mapScript.erase(it);

	return TRUE;
Exit0:
	return FALSE;
}

LPLuaScript *LPAPI LPLuaScriptMgr::FindScript(const char * pcszSciptName)
{
	MAP_NAME_2_SCRIPT::iterator it;
	char szFileName[MAX_FILE_NAME];

	sprintf_s(szFileName, MAX_FILE_NAME, "%s/%s", m_szRootPath, pcszSciptName);
	it = m_mapScript.find(szFileName);
	PROCESS_ERROR(it != m_mapScript.end());

	return it->second;
Exit0:
	return NULL;
}

BOOL LPAPI LPLuaScriptMgr::RegisterPackage(const char * pcszPackageName, lua_CFunction pLoagPackage)
{
	char* pszPkgName = NULL;
	std::pair<MAP_NAME_2_PACKAGE::iterator, bool> InsRet;

	LOG_PROCESS_ERROR(pcszPackageName);
	LOG_PROCESS_ERROR(pLoagPackage);

	pszPkgName = new char[MAX_FILE_NAME];
	LOG_PROCESS_ERROR(pszPkgName);

	lpStrCpyN(pszPkgName, pcszPackageName, MAX_FILE_NAME);

	InsRet = m_mapPackage.insert(std::make_pair(pszPkgName, pLoagPackage));
	LOG_PROCESS_ERROR(InsRet.second);

	return TRUE;

Exit0:

	SAFE_DELETE_SZ(pszPkgName);

	return FALSE;
}

lua_CFunction LPAPI LPLuaScriptMgr::FindPackage(const char * pcszPackageName)
{
	MAP_NAME_2_PACKAGE::iterator it;

	it = m_mapPackage.find(pcszPackageName);
	LOG_PROCESS_ERROR(it != m_mapPackage.end());

	return it->second;
Exit0:
	return NULL;
}

BOOL LPAPI LPLuaScriptMgr::RegisterFuncList(LUA_FUNC* pstFuncList)
{
	LOG_PROCESS_ERROR(pstFuncList);

	m_lstLuaFunc.push_back(pstFuncList);

	return TRUE;
Exit0:
	return FALSE;
}

void LPAPI LPLuaScriptMgr::SetRootPath(const char * pcszRootPath)
{
	if (pcszRootPath == NULL)
	{
		lpFastZeroCharArray(m_szRootPath);
	}
	else
	{
		lpStrCpyN(m_szRootPath, pcszRootPath, MAX_PATH);
	}
}

const char *LPAPI LPLuaScriptMgr::GetRootPath(void)
{
	return m_szRootPath;
}

INT_64 LPAPI LPLuaScriptMgr::PrintMemoryStat(void)
{
	INT_64 qwTotalSize = 0;

	for (MAP_NAME_2_SCRIPT::iterator it = m_mapScript.begin();
		it != m_mapScript.end(); ++it)
	{
		lua_State* L = it->second->GetLuaState();
		INF("script %s allocated %.2fMB use %.2f", it->first, L->l_G->totalbytes / (1024 * 1024.0f),
			L->l_G->estimate / (1024 * 1024.0f));

		qwTotalSize += (L->l_G->totalbytes + sizeof(LPLuaScript));
	}

	INF("all scripts allocated %.2fMB", qwTotalSize / (1024 * 1024.0f));
	
	return qwTotalSize;
}

INT_32 LPLuaScriptMgr::_LoadPackage(lua_State * L)
{
	const char* pcszPackageName = NULL;
	lua_CFunction pLoadPackage = NULL;

	LOG_PROCESS_ERROR(lua_gettop(L) > 0);

	pcszPackageName = lua_tostring(L, 1);
	LOG_PROCESS_ERROR(pcszPackageName);

	pLoadPackage = LPLuaScriptMgr::Instance().FindPackage(pcszPackageName);
	LOG_PROCESS_ERROR(pLoadPackage);

	return pLoadPackage(L);

Exit0:

	if (pcszPackageName)
	{
		FTL("fail to load package %s", pcszPackageName);
	}

	return 0;
}








//end声明所处的名字空间

NS_LZPL_END
