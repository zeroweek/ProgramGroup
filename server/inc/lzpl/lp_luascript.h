//****************************************************************************/
//    author: caihy
//    date: October 14, 2015
//    description: lua脚本
//
//****************************************************************************/
#ifndef _LP_LUA_SCRIPT_H_
#define _LP_LUA_SCRIPT_H_

#include "lp_base.h"
#include "lua.hpp"



//begin声明所处的名字空间
NS_LZPL_BEGIN



struct LUA_FUNC
{
    const char*              pcszFuncname;
    lua_CFunction            pFunc;
};



// Summary:
//
class DECLARE LPLuaScript
{
public:

    // Summary:
    //      无
    LPLuaScript();

    // Summary:
    //      无
    ~LPLuaScript();

    // Summary:
    //
    // Return:
    //      TRUE-成功，FALSE-失败
    BOOL LPAPI Init(void);

    // Summary:
    //      反初始化
    BOOL LPAPI UnInit(void);

    // Summary:
    //      无
    BOOL LPAPI LoadFromFile(const char* pcszFileName);

    // Summary:
    //      无
    BOOL LPAPI LoadFromBuffer(const char* pcszScriptName, const char* pcszBuf, LPUINT32 dwSize);

    // Summary:
    //      无
    BOOL LPAPI RegisterFunc(LUA_FUNC astFuncList[], LPINT32 nFuncCount);

    // Summary:
    //      无
    BOOL LPAPI CallFunction(const char* pcszFuncName, const char* pcszFormat, ...);

    // Summary:
    //      无
    BOOL LPAPI Execute(void);

    // Summary:
    //      无
    BOOL LPAPI IsFuncExist(const char* pcszFuncName);

    // Summary:
    //      无
    BOOL LPAPI IsTableExist(const char* pcszTableName);

    // Summary:
    //      无
    const char* LPAPI GetScriptName(void);

    // Summary:
    //      无
    lua_State* LPAPI GetLuaState(void);

    // Summary:
    //      无
    BOOL LPAPI CollectGarbage(LPINT32 nGCSize);

    // Summary:
    //      无
    LPUINT32 LPAPI GetCRC(void);

private:

    // Summary:
    //      无
    BOOL LPAPI _CallFunction(const char* pcszFuncName, const char* pcszFormat, va_list valist);

    // Summary:
    //      无
    BOOL LPAPI _GetCRC(const char* pcszFileName, LPINT32 nLevel);

public:

    // Summary:
    //      无
    static LPLuaScript* LPAPI GetScriptFromState(lua_State* L);

private:

    // Summary:
    //      无
    static LPINT32 _Include(lua_State* L);

    // Summary:
    //      无
    static LPINT32 _LuaErrFunc(lua_State* L);

    // Summary:
    //      无
    static void LPAPI _EnumTable(lua_State* L, int nTableIndex, int nTableDepth);

    // Summary:
    //      无
    static LPINT32 _Print(lua_State* L);

private:

    char                     m_szScriptName[MAX_FILE_NAME];
    lua_State*               m_pLuaState;
    LPUINT32                  m_dwCRC;
};



//end声明所处的名字空间
NS_LZPL_END

#endif