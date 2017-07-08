//****************************************************************************/
//    author: caihy
//    date: October 14, 2015
//    description: lua�ű�
//
//****************************************************************************/
#ifndef _LP_LUA_SCRIPT_H_
#define _LP_LUA_SCRIPT_H_

#include "lp_base.h"
#include "lua.hpp"



//begin�������������ֿռ�
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
    //      ��
    LPLuaScript();

    // Summary:
    //      ��
    ~LPLuaScript();

    // Summary:
    //
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    BOOL LPAPI Init(void);

    // Summary:
    //      ����ʼ��
    BOOL LPAPI UnInit(void);

    // Summary:
    //      ��
    BOOL LPAPI LoadFromFile(const char* pcszFileName);

    // Summary:
    //      ��
    BOOL LPAPI LoadFromBuffer(const char* pcszScriptName, const char* pcszBuf, LPUINT32 dwSize);

    // Summary:
    //      ��
    BOOL LPAPI RegisterFunc(LUA_FUNC astFuncList[], LPINT32 nFuncCount);

    // Summary:
    //      ��
    BOOL LPAPI CallFunction(const char* pcszFuncName, const char* pcszFormat, ...);

    // Summary:
    //      ��
    BOOL LPAPI Execute(void);

    // Summary:
    //      ��
    BOOL LPAPI IsFuncExist(const char* pcszFuncName);

    // Summary:
    //      ��
    BOOL LPAPI IsTableExist(const char* pcszTableName);

    // Summary:
    //      ��
    const char* LPAPI GetScriptName(void);

    // Summary:
    //      ��
    lua_State* LPAPI GetLuaState(void);

    // Summary:
    //      ��
    BOOL LPAPI CollectGarbage(LPINT32 nGCSize);

    // Summary:
    //      ��
    LPUINT32 LPAPI GetCRC(void);

private:

    // Summary:
    //      ��
    BOOL LPAPI _CallFunction(const char* pcszFuncName, const char* pcszFormat, va_list valist);

    // Summary:
    //      ��
    BOOL LPAPI _GetCRC(const char* pcszFileName, LPINT32 nLevel);

public:

    // Summary:
    //      ��
    static LPLuaScript* LPAPI GetScriptFromState(lua_State* L);

private:

    // Summary:
    //      ��
    static LPINT32 _Include(lua_State* L);

    // Summary:
    //      ��
    static LPINT32 _LuaErrFunc(lua_State* L);

    // Summary:
    //      ��
    static void LPAPI _EnumTable(lua_State* L, int nTableIndex, int nTableDepth);

    // Summary:
    //      ��
    static LPINT32 _Print(lua_State* L);

private:

    char                     m_szScriptName[MAX_FILE_NAME];
    lua_State*               m_pLuaState;
    LPUINT32                  m_dwCRC;
};



//end�������������ֿռ�
NS_LZPL_END

#endif