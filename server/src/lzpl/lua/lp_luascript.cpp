#include "lp_luascript.h"
#include "lp_processerror.h"
#include "lpi_file.h"
#include "zlib.h"
#include "tolua++.h"
#include "lp_string.h"
#include "lp_luascriptmgr.h"
#include "lp_codeconvert.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define LUA_SCRIPT_THIS            "LuaScriptThis"
#define LUA_GLOBAL_BUFFER          "LuaGlobalBuffer"



LPLuaScript::LPLuaScript()
{
	m_szScriptName[0] = 0;
	m_pLuaState = NULL;
	m_dwCRC = 0;
}

LPLuaScript::~LPLuaScript()
{
	UnInit();
}

BOOL LPAPI LPLuaScript::Init(void)
{
	LOG_PROCESS_ERROR(m_pLuaState == NULL);

	m_pLuaState = lua_open();
	LOG_PROCESS_ERROR(m_pLuaState);

	luaL_openlibs(m_pLuaState);

	lua_register(m_pLuaState, "print", _Print);
	lua_register(m_pLuaState, "include", _Include);

	lua_pushlightuserdata(m_pLuaState, this);
	lua_setglobal(m_pLuaState, LUA_SCRIPT_THIS);

	m_dwCRC = 0;

	return TRUE;

Exit0:

	if (m_pLuaState)
	{
		lua_close(m_pLuaState);
		m_pLuaState = NULL;
	}

	return FALSE;
}

BOOL LPAPI LPLuaScript::UnInit(void)
{
	if (m_pLuaState)
	{
		lua_close(m_pLuaState);
		m_pLuaState = NULL;
	}

	return TRUE;
}

BOOL LPAPI LPLuaScript::LoadFromFile(const char * pcszFileName)
{
	char* pszBuf      = NULL;
	ILPFile* pFile    = NULL;

	LPINT32 nResult    = 0;
	LPUINT32 dwSize    = 0;
	LPUINT32 dwOldCRC  = m_dwCRC;

	LOG_PROCESS_ERROR(pcszFileName);

	m_dwCRC = 0;
	nResult = _GetCRC(pcszFileName, 0);
	LOG_PROCESS_ERROR(nResult);

	if (dwOldCRC != m_dwCRC)
	{
		pFile = ILPFile::OpenFile(pcszFileName, "rb");
		LOG_PROCESS_ERROR(pFile);

		dwSize = pFile->Size();
		pszBuf = new char[dwSize];
		LOG_PROCESS_ERROR(pszBuf);

		nResult = pFile->Read(pszBuf, dwSize);
		LOG_PROCESS_ERROR(nResult == dwSize);

		if ((*(LPUINT32*)pszBuf & 0x00FFFFFF) == UTF8_BOM)
		{
			nResult = LoadFromBuffer(pcszFileName, pszBuf + 3, dwSize - 3);
			LOG_PROCESS_ERROR(nResult);
		}
		else
		{
			nResult = LoadFromBuffer(pcszFileName, pszBuf, dwSize);
			LOG_PROCESS_ERROR(nResult);
		}

		SAFE_DELETE_SZ(pszBuf);
		pszBuf = NULL;

		pFile->Close();
		pFile = NULL;

		IMP("load lua script %s successfully !", pcszFileName);
	}
	else
	{

	}

	return TRUE;

Exit0:

	if (pcszFileName)
	{
		WRN("fail to load lua script %s", pcszFileName);
	}

	SAFE_DELETE_SZ(pszBuf);
	pszBuf = NULL;

	if (pFile)
	{
		pFile->Close();
		pFile = NULL;
	}

	return FALSE;
}

BOOL LPAPI LPLuaScript::LoadFromBuffer(const char * pcszScriptName, const char * pcszBuf, LPUINT32 dwSize)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pcszBuf);

	if (pcszScriptName)
	{
		lpStrCpyN(m_szScriptName, pcszScriptName, MAX_FILE_NAME);
	}

	nResult = luaL_loadbuffer(m_pLuaState, pcszBuf, dwSize, pcszScriptName);
	LOG_PROCESS_ERROR(0 == nResult);

	lua_setglobal(m_pLuaState, LUA_GLOBAL_BUFFER);
	lua_getglobal(m_pLuaState, LUA_GLOBAL_BUFFER);

	nResult = lua_pcall(m_pLuaState, 0, 0, 0);
	LOG_PROCESS_ERROR(0 == nResult);

	return TRUE;

Exit0:

	const char* pcszErrorMsg = lua_tostring(m_pLuaState, -1);
	if (pcszErrorMsg)
	{
		FTL("[LUA] LoadFromBuffer %s : %s", pcszScriptName, pcszErrorMsg);
	}
	else
	{
		FTL("[LUA] LoadFromBuffer %s", pcszScriptName);
	}

	return FALSE;
}

BOOL LPAPI LPLuaScript::RegisterFunc(LUA_FUNC astFuncList[], LPINT32 nFuncCount)
{
	LOG_PROCESS_ERROR(m_pLuaState);

	if (nFuncCount == 0)
	{
		for (LPINT32 nIndex = 0; astFuncList[nIndex].pcszFuncname != NULL; ++nIndex)
		{
			lua_register(m_pLuaState, astFuncList[nIndex].pcszFuncname, astFuncList[nIndex].pFunc);
		}
	}
	else
	{
		for (LPINT32 nIndex = 0; nIndex < nFuncCount; ++nIndex)
		{
			lua_register(m_pLuaState, astFuncList[nIndex].pcszFuncname, astFuncList[nIndex].pFunc);
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPLuaScript::CallFunction(const char * pcszFuncName, const char * pcszFormat, ...)
{
	BOOL bResult = FALSE;

	va_list valist;
	va_start(valist, pcszFormat);
	bResult = _CallFunction(pcszFuncName, pcszFormat, valist);
	va_end(valist);

	return bResult;
}

BOOL LPAPI LPLuaScript::Execute(void)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(m_pLuaState);

	lua_getglobal(m_pLuaState, LUA_GLOBAL_BUFFER);
	nResult = lua_pcall(m_pLuaState, 0, 0, 0);
	LOG_PROCESS_ERROR(0 == nResult);

	return TRUE;

Exit0:

	if (m_pLuaState)
	{
		const char* pcszErrorMsg = lua_tostring(m_pLuaState, -1);
		if (pcszErrorMsg)
		{
			FTL("[LUA] fail to execute script %s : %s", m_szScriptName, pcszErrorMsg);
		}
		else
		{
			FTL("[LUA] fail to execute script %s", m_szScriptName);
		}
	}

	return FALSE;
}

BOOL LPAPI LPLuaScript::IsFuncExist(const char * pcszFuncName)
{
	BOOL bResult = FALSE;

	LOG_PROCESS_ERROR(pcszFuncName);

	lua_getglobal(m_pLuaState, pcszFuncName);
	bResult = lua_isfunction(m_pLuaState, lua_gettop(m_pLuaState));
	lua_pop(m_pLuaState, 1);

Exit0:
	return bResult;
}

BOOL LPAPI LPLuaScript::IsTableExist(const char * pcszTableName)
{
	BOOL bResult = FALSE;

	LOG_PROCESS_ERROR(pcszTableName);

	lua_getglobal(m_pLuaState, pcszTableName);
	bResult = lua_istable(m_pLuaState, lua_gettop(m_pLuaState));
	lua_pop(m_pLuaState, 1);

Exit0:
	return bResult;
}

const char *LPAPI LPLuaScript::GetScriptName(void)
{
	return m_szScriptName;
}

lua_State *LPAPI LPLuaScript::GetLuaState(void)
{
	return m_pLuaState;
}

BOOL LPAPI LPLuaScript::CollectGarbage(LPINT32 nGCSize)
{
	LPINT32 nMemoryAmount = lua_gc(m_pLuaState, LUA_GCCOUNT, 0);

	if (nMemoryAmount > nGCSize)
	{
		lua_gc(m_pLuaState, LUA_GCCOLLECT, 0);
		FTL("[LUA] collect garbage in script %s (%d kb)", m_szScriptName, nMemoryAmount);
	}

	return TRUE;
}

LPUINT32 LPAPI LPLuaScript::GetCRC(void)
{
	LPINT32 nResult = 0;

	if (m_dwCRC == 0)
	{
		nResult = _GetCRC(m_szScriptName, 0);
		LOG_PROCESS_ERROR(nResult);
	}

Exit0:
	return m_dwCRC;
}

BOOL LPAPI LPLuaScript::_CallFunction(const char * pcszFuncName, const char * pcszFormat, va_list valist)
{
	LPINT32 nRetCode      = 0;
	LPINT32 nResult       = 0;
	LPINT32 nStackTop     = -1;
	LPINT32 nIndex        = 0;
	LPINT32 nArgNum       = 0;
	LPINT32 nParam        = 0;
	LPINT32 nErrFunc      = 0;
	LPINT32 nStackIndex   = 0;

	LOG_PROCESS_ERROR(m_pLuaState);
	nStackTop = nParam = lua_gettop(m_pLuaState);

	lua_pushcfunction(m_pLuaState, _LuaErrFunc);
	nErrFunc = lua_gettop(m_pLuaState);

	lua_getglobal(m_pLuaState, pcszFuncName);
	LOG_PROCESS_ERROR(lua_isfunction(m_pLuaState, -1));

	while (pcszFormat[nIndex] != '\0' && pcszFormat[nIndex] != ':')
	{
		switch (pcszFormat[nIndex])
		{
		case 'f':
			{
				double fNumber = va_arg(valist, double);
				lua_pushnumber(m_pLuaState, fNumber);
				nArgNum++;
			}
			break;
		case 'd':
			{
				double fNumber = (double)va_arg(valist, LPINT32);
				lua_pushnumber(m_pLuaState, fNumber);
				nArgNum++;
			}
			break;
		case 's':
			{
				char* pszString = va_arg(valist, char*);
				lua_pushstring(m_pLuaState, pszString);
				nArgNum++;
			}
			break;
		case 'N':
			{
				lua_pushnil(m_pLuaState);
				nArgNum++;
			}
			break;
		case 'F':
			{
				lua_CFunction pLuaCFunc = va_arg(valist, lua_CFunction);
				lua_pushcfunction(m_pLuaState, pLuaCFunc);
				nArgNum++;
			}
			break;
		case 't':
			{
				lua_pushvalue(m_pLuaState, nParam);
				lua_remove(m_pLuaState, nParam);
				nArgNum++;
				nParam--;
			}
			break;
		case 'u':
			{
				lua_pushvalue(m_pLuaState, nParam);
				lua_remove(m_pLuaState, nParam);
				nArgNum++;
				nParam--;
			}
			break;
		case 'b':
			{
				BOOL bValue = va_arg(valist, BOOL);
				lua_pushboolean(m_pLuaState, bValue);
				nArgNum++;
			}
			break;
		case 'o':
			{
				void* pObj = va_arg(valist, void*);
				char* pszClassName = va_arg(valist, char*);
				tolua_pushusertype(m_pLuaState, pObj, pszClassName);
				nArgNum++;
			}
			break;
		default:
			LOG_PROCESS_ERROR(FALSE);
			break;
		}

		nIndex++;
	}

	if (pcszFormat[nIndex] == ':')
	{
		nIndex++;
	}

	nResult = (LPINT32)strlen(pcszFormat + nIndex);
	nRetCode = lua_pcall(m_pLuaState, nArgNum, nResult, nErrFunc);
	LOG_PROCESS_ERROR(0 == nRetCode);

	nStackIndex = lua_gettop(m_pLuaState) - nResult + 1;
	LOG_PROCESS_ERROR(nStackIndex > 0);

	while (pcszFormat[nIndex] != '\0')
	{
		switch (pcszFormat[nIndex])
		{
		case 'f':
			{
				float* pfNumber = va_arg(valist, float*);
				LOG_PROCESS_ERROR(pfNumber);
				LOG_PROCESS_ERROR(lua_isnumber(m_pLuaState, nStackIndex));

				*pfNumber = (float)lua_tonumber(m_pLuaState, nStackIndex++);
			}
			break;
		case 'D':
			{
				double* pfNumber = va_arg(valist, double*);
				LOG_PROCESS_ERROR(pfNumber);
				LOG_PROCESS_ERROR(lua_isnumber(m_pLuaState, nStackIndex));

				*pfNumber = lua_tonumber(m_pLuaState, nStackIndex++);
			}
			break;
		case 'd':
			{
				LPINT32* pnInteger = va_arg(valist, LPINT32*);
				LOG_PROCESS_ERROR(pnInteger);
				LOG_PROCESS_ERROR(lua_isnumber(m_pLuaState, nStackIndex));

				*pnInteger = (LPINT32)(m_pLuaState, nStackIndex++);
			}
			break;
		case 's':
			{
				char* pszString = va_arg(valist, char*);
				LOG_PROCESS_ERROR(pszString);
				LOG_PROCESS_ERROR(lua_isstring(m_pLuaState, nStackIndex));
				LPINT32 nStrLen = va_arg(valist, LPINT32);

				lpStrCpyN(pszString, lua_tostring(m_pLuaState, nStackIndex++), nStrLen);
			}
			break;
		case 'b':
			{
				BOOL* pbBoolean = va_arg(valist, BOOL*);
				LOG_PROCESS_ERROR(pbBoolean);
				LOG_PROCESS_ERROR(lua_isboolean(m_pLuaState, nStackIndex));

				*pbBoolean = lua_toboolean(m_pLuaState, nStackIndex++);
			}
			break;
		case 'o':
			{
				void** ppObj = va_arg(valist, void**);
				LOG_PROCESS_ERROR(ppObj);
				LOG_PROCESS_ERROR(lua_isuserdata(m_pLuaState, nStackIndex) || lua_isnil(m_pLuaState, nStackIndex));

				*ppObj = lua_touserdata(m_pLuaState, nStackIndex++);
			}
			break;
		default:
			break;
		}

		nIndex++;
	}

	lua_settop(m_pLuaState, nStackTop);

	return TRUE;

Exit0:

	const char* pcszErrorMsg = lua_tostring(m_pLuaState, -1);
	if (pcszErrorMsg)
	{
		FTL("[LUA] fail to call lua function %s(%s), %s", pcszFuncName, pcszFormat, pcszErrorMsg);
	}
	else
	{
		FTL("[LUA] fail to call lua function %s(%s)", pcszFuncName, pcszFormat);
	}

	if (nStackTop != -1)
	{
		lua_settop(m_pLuaState, nStackTop);
	}

	return FALSE;
}

BOOL LPAPI LPLuaScript::_GetCRC(const char * pcszFileName, LPINT32 nLevel)
{
	LPINT32 nResult = 0;
	LPUINT32 dwPointer = 0;
	LPUINT32 dwSize = 0;

	ILPFile* pFile = NULL;
	BOOL bInString = FALSE;

	char* pszFileData = NULL;
	char cStartChar = 0;
	char szIncludeFileName[MAX_PATH];
	char szTab[MAX_PATH] = { 0 };

	pFile = ILPFile::OpenFile(pcszFileName, "rb");
	LOG_PROCESS_ERROR(pFile);

	dwSize = pFile->Size();
	PROCESS_ERROR(dwSize > 0);

	pszFileData = new char[dwSize + 1];
	PROCESS_ERROR(pszFileData);

	nResult = pFile->Read(pszFileData, dwSize);
	LOG_PROCESS_ERROR(nResult == dwSize);

	pFile->Close();
	pFile = NULL;

	m_dwCRC = crc32(m_dwCRC, (const Bytef*)pszFileData, dwSize);

	while (dwPointer < dwSize)
	{
		switch (pszFileData[dwPointer])
		{
		case '\'':
			{
				if (!bInString)
				{
					bInString = TRUE;
					cStartChar = '\'';
				}
				else if (bInString && cStartChar == '\'')
				{
					bInString = FALSE;
				}
			}
			break;
		case '\"':
			{
				if (!bInString)
				{
					bInString = TRUE;
					cStartChar = '\"';
				}
				else if (bInString && cStartChar == '\"')
				{
					bInString = FALSE;
				}
			}
			break;
		case '[':
			{
				if (!bInString && (dwPointer + 1 < dwSize) && (pszFileData[dwPointer + 1] == '['))
				{
					bInString = TRUE;
					cStartChar = '[';
					dwPointer++;
				}
			}
			break;
		case ']':
			{
				if (!bInString && cStartChar == '[' && (dwPointer + 1 < dwSize) && (pszFileData[dwPointer + 1] == ']'))
				{
					bInString = FALSE;
				}
			}
			break;
		case '\\':
			{
				if (bInString)
				{
					dwPointer++;
				}
			}
			break;
		case '-':
			{
				if (!bInString && (dwPointer + 1 < dwSize) && (pszFileData[dwPointer + 1] == '-'))
				{
					dwPointer += 2;
					if ((dwPointer + 1 < dwSize) && (pszFileData[dwPointer + 1] == '['))
					{
						dwPointer += 3;
						while (dwPointer < dwSize && pszFileData[dwPointer - 2] != ']' && pszFileData[dwPointer - 1] != '-' && pszFileData[dwPointer] != '-')
						{
							dwPointer++;
						}
					}
					else
					{
						while (dwPointer < dwSize && pszFileData[dwPointer] != '\n')
						{
							dwPointer++;
						}
					}
				}
			}
			break;
		default:
			if (!bInString && (dwPointer + 8 < dwSize) &&
				*(LPUINT32*)(pszFileData + dwPointer) == 'lcnI' &&
				*(LPUINT32*)(pszFileData + dwPointer + 4) == '(edu')
			{
				dwPointer += 8;
				while (dwPointer < dwSize && pszFileData[dwPointer] != '\"')
				{
					dwPointer++;
				}
				dwPointer++;

				char* pszFileName = pszFileData + dwPointer;

				while (dwPointer < dwSize && pszFileData[dwPointer] != '\"')
				{
					dwPointer++;
				}
				pszFileData[dwPointer] = 0;

				if (LPLuaScriptMgr::Instance().GetRootPath()[0])
				{
					snprintf(szIncludeFileName, MAX_PATH, "%s/%s", LPLuaScriptMgr::Instance().GetRootPath(), pszFileName);
				}
				else
				{
					snprintf(szIncludeFileName, MAX_PATH, "%s", pszFileName);
				}

				nResult = _GetCRC(szIncludeFileName, nLevel + 1);
				LOG_PROCESS_ERROR(nResult);
			}
			break;
		}

		dwPointer++;
	}

	return TRUE;

Exit0:

	if (pFile)
	{
		pFile->Close();
		pFile = NULL;
	}

	SAFE_DELETE_SZ(pszFileData);

	if (pcszFileName)
	{
		FTL("[LUA] fail to get crc from %s", pcszFileName);
	}
	else
	{
		FTL("[LUA] fail to get crc from null pcszFileName");
	}

	return FALSE;
}

LPLuaScript *LPAPI LPLuaScript::GetScriptFromState(lua_State * L)
{
	LPINT32 nTop = 0;
	LPLuaScript* pLuaScript = NULL;
	
	LOG_PROCESS_ERROR(L);

	nTop = lua_gettop(L);
	lua_getglobal(L, LUA_SCRIPT_THIS);

	pLuaScript = (LPLuaScript*)lua_touserdata(L, -1);
	LOG_PROCESS_ERROR(pLuaScript);

Exit0:
	if (L)
	{
		lua_settop(L, nTop);
	}
	return pLuaScript;
}

LPINT32 LPLuaScript::_Include(lua_State * L)
{
	LPINT32 nResult = 0;
	LPINT32 nErrFunc = 0;
	LPUINT32 dwSize = 0;
	ILPFile* pFile = NULL;
	char* pszBuf = NULL;
	const char* pcszFileName = NULL;
	char szFileName[MAX_FILE_NAME];

	pcszFileName = lua_tostring(L, 1);
	LOG_PROCESS_ERROR(pcszFileName);

	lua_pushcfunction(L, _LuaErrFunc);
	nErrFunc = lua_gettop(L);

	if (LPLuaScriptMgr::Instance().GetRootPath()[0])
	{
		snprintf(szFileName, MAX_FILE_NAME, "%s/%s", LPLuaScriptMgr::Instance().GetRootPath(), pcszFileName);
	}
	else
	{
		snprintf(szFileName, MAX_FILE_NAME, "%s", pcszFileName);
	}
	pFile = ILPFile::OpenFile(szFileName, "rb");
	LOG_PROCESS_ERROR(pFile);

	dwSize = pFile->Size();
	LOG_PROCESS_ERROR(dwSize > 0);

	pszBuf = new char[dwSize];
	LOG_PROCESS_ERROR(pszBuf);

	nResult = pFile->Read(pszBuf, dwSize);
	LOG_PROCESS_ERROR(nResult == dwSize);

	if ((*(LPUINT32*)pszBuf & 0x00FFFFFF) == UTF8_BOM)
	{
		nResult = luaL_loadbuffer(L, pszBuf + 3, dwSize - 3, pcszFileName);
		LOG_PROCESS_ERROR(0 == nResult);
	}
	else
	{
		nResult = luaL_loadbuffer(L, pszBuf, dwSize, pcszFileName);
		LOG_PROCESS_ERROR(0 == nResult);
	}

	nResult = lua_pcall(L, 0, 0, nErrFunc);
	LOG_PROCESS_ERROR(0 == nResult);

	SAFE_DELETE_SZ(pszBuf);
	pFile->Close();
	pFile = NULL;

	return 0;

Exit0:

	if (pcszFileName)
	{
		if (lua_isstring(L, -1))
		{
			FTL("[LUA] Include() : %s, %s", pcszFileName, lua_tostring(L, -1));
		}
		else
		{
			FTL("[LUA] Include() : %s", pcszFileName);
		}
	}
	else
	{
		FTL("[LUA] Include() : nil file name");
	}

	if (pFile)
	{
		pFile->Close();
		pFile = NULL;
	}

	return 0;
}

LPINT32 LPLuaScript::_LuaErrFunc(lua_State * L)
{
	lua_Debug ar;
	LPINT32 nLevel = 1;
	static BOOL s_bInErrFunc = FALSE;

	if (s_bInErrFunc)
	{
		return 1;
	}

	s_bInErrFunc = TRUE;

	while (lua_getstack(L, nLevel, &ar))
	{
		if (lua_getinfo(L, "Snlu", &ar))
		{
			char szMsg[1024] = { 0 };
			if (ar.namewhat[0] != 0)
			{
				snprintf(szMsg, 1024, "[%d]%s:%d in function %s", nLevel, ar.short_src, ar.currentline, ar.name);
			}
			else if (*ar.what == 'm')
			{
				snprintf(szMsg, 1024, "[%d]%s:%d in main chunk", nLevel, ar.short_src, ar.currentline);
			}
			else if (*ar.what == 'C' || *ar.what == 't')
			{
				snprintf(szMsg, 1024, "[%d]%s:?", nLevel, ar.short_src);
			}
			else
			{
				snprintf(szMsg, 1024, "[%d]%s:%d in function <%s:%d>", nLevel, ar.short_src, ar.currentline, ar.short_src, ar.linedefined);
			}

			LUA(szMsg);
		}

		int nLocalIndex = 1;
		const char* pcszVarName = lua_getlocal(L, &ar, nLocalIndex);
		while (pcszVarName)
		{
			char szMsg[1024] = { 0 };
			switch (lua_type(L, -1))
			{
			case LUA_TNONE:
				{
					snprintf(szMsg, 1024, "\t%s : LUA_TNONE", pcszVarName);
				}
				break;
			case LUA_TUSERDATA:
				{
					snprintf(szMsg, 1024, "\t%s : LUA_TUSERDATA", pcszVarName);
				}
				break;
			case LUA_TNIL:
				{
					snprintf(szMsg, 1024, "\t%s : LUA_TNIL", pcszVarName);
				}
				break;
			case LUA_TNUMBER:
				{
					snprintf(szMsg, 1024, "\t%s : LUA_TNUMBER %f", pcszVarName, lua_tonumber(L, -1));
				}
				break;
			case LUA_TSTRING:
				{
					const char* pszValue = lua_tostring(L, -1);
					if (pszValue)
					{
						snprintf(szMsg, 1024, "\t%s : LUA_TSTRING %s", pcszVarName, pszValue);
					}
					else
					{
						snprintf(szMsg, 1024, "\t%s : LUA_TSTRING NULL", pcszVarName);
					}
				}
				break;
			case LUA_TTABLE:
				{
					snprintf(szMsg, 1024, "\t%s : LUA_TTABLE", pcszVarName);
				}
				break;
			case LUA_TFUNCTION:
				{
					snprintf(szMsg, 1024, "\t%s : LUA_TFUNCTION", pcszVarName);
				}
				break;
			default:
				break;
			}

			LUA(szMsg);

			if (lua_type(L, -1) == LUA_TTABLE)
			{
				_EnumTable(L, -1, 2);
			}

			lua_pop(L, 1);
			nLocalIndex++;
			pcszVarName = lua_getlocal(L, &ar, nLocalIndex);
		}

		nLevel++;
	}

	s_bInErrFunc = FALSE;

	return 1;
}

void LPAPI LPLuaScript::_EnumTable(lua_State * L, int nTableIndex, int nTableDepth)
{
	int nTop = lua_gettop(L);

	nTableIndex--;
	lua_pushnil(L);

	while (lua_next(L, nTableIndex) != 0)
	{
		char szMsg[MAX_FILE_NAME] = { 0 };
		char szTab[MAX_FILE_NAME] = { 0 };
		const char* pcszName = NULL;
		char szKeyName[256];

		switch (lua_type(L, -2))
		{
		case LUA_TNUMBER:
			{
#				ifdef _WIN32
				{
					_itoa_s((int)lua_tonumber(L, -2), szKeyName, 10);
				}
#				else
				{
					lpSerializeToCString(szKeyName, 10, "%d", (int)lua_tonumber(L, -2));
				}
#				endif
				pcszName = szKeyName;
			}
			break;
		case LUA_TSTRING:
			{
				pcszName = lua_tostring(L, -2);
			}
			break;
		case LUA_TNIL:
		case LUA_TBOOLEAN:
		case LUA_TLIGHTUSERDATA:
		case LUA_TTABLE:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
		default:
			break;
		}

		if (pcszName == NULL)
		{
			LUA("NULL key name !");
			continue;
		}

		for (int i = 0; i < nTableDepth; ++i)
		{
#			ifdef _WIN32
			{
				strcat_s(szTab, "\t");
			}
#			else
			{
				strncat(szTab, "\t", 1);
			}
#			endif
		}

		switch (lua_type(L, -1))
		{
		case LUA_TNIL:
			{
				snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TNIL", szTab, pcszName);
			}
			break;
		case LUA_TBOOLEAN:
			{
				if (lua_toboolean(L, -1))
				{
					snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TBOOLEAN true", szTab, pcszName);
				}
				else
				{
					snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TBOOLEAN false", szTab, pcszName);
				}
			}
			break;
		case LUA_TLIGHTUSERDATA:
			{
				snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TLIGHTUSERDATA %p", szTab, pcszName, lua_touserdata(L, -1));
			}
			break;
		case LUA_TNUMBER:
			{
				snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TNUMBER %f", szTab, pcszName, lua_tonumber(L, -1));
			}
			break;
		case LUA_TSTRING:
			{
				const char* pcszStr = lua_tostring(L, -1);
				if (pcszStr)
				{
					snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TSTRING %s", szTab, pcszName, pcszStr);
				}
				else
				{
					snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TSTRING NULL", szTab, pcszName);
				}
			}
			break;
		case LUA_TTABLE:
			{
				snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TTABLE", szTab, pcszName);
			}
			break;
		case LUA_TFUNCTION:
			{
				snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TFUNCTION", szTab, pcszName);
			}
			break;
		case LUA_TUSERDATA:
			{
				snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TUSERDATA %p", szTab, pcszName, lua_touserdata(L, -1));
			}
			break;
		case LUA_TTHREAD:
			{
				snprintf(szMsg, MAX_FILE_NAME, "%s%s : LUA_TTHREAD %p", szTab, pcszName, lua_tothread(L, -1));
			}
			break;
		default:
			break;
		}

		LUA(szMsg);

		if (lua_type(L, -1) == LUA_TTABLE && pcszName[0] && nTableDepth < 2)
		{
			_EnumTable(L, -1, nTableDepth + 1);
		}

		lua_pop(L, 1);
	}

	lua_settop(L, nTop);

	return;
}

LPINT32 LPLuaScript::_Print(lua_State * L)
{
	const char* pcszMsg = NULL;

	LOG_PROCESS_ERROR(lua_gettop(L) > 0);

	if (lua_istable(L, 1))
	{
		_EnumTable(L, -1, 0);
	}
	else
	{
		pcszMsg = lua_tostring(L, 1);
		LOG_PROCESS_ERROR(pcszMsg);

		LUA(pcszMsg);
	}

Exit0:
	return 0;
}



//end声明所处的名字空间

NS_LZPL_END