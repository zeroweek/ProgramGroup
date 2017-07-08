//****************************************************************************/
//    author: caihy
//    date: October 14, 2015
//    description: lua脚本管理
//
//****************************************************************************/
#ifndef _LP_LUA_SCRIPT_MGR_H_
#define _LP_LUA_SCRIPT_MGR_H_

#include "lp_base.h"
#include "lp_luascript.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//
class DECLARE LPLuaScriptMgr
{
    SINGLETON_DECLARE(LPLuaScriptMgr)

public:

    // Summary:
    //
    // Return:
    //      TRUE-成功，FALSE-失败
    BOOL LPAPI Init(const char* pcszRootPath);

    // Summary:
    //      反初始化
    BOOL LPAPI UnInit(void);

    // Summary:
    //      无
    BOOL LPAPI Reload(LPINT32& nErrorCount);

    // Summary:
    //      无
    BOOL LPAPI Reload(const char* pcszSciptName);

    // Summary:
    //      无
    LPLuaScript* LPAPI NewScript(const char* pcszSciptName);

    // Summary:
    //      无
    BOOL LPAPI DelScript(LPLuaScript* poScript);

    // Summary:
    //      无
    LPLuaScript* LPAPI FindScript(const char* pcszSciptName);

    // Summary:
    //      无
    BOOL LPAPI RegisterPackage(const char* pcszPackageName, lua_CFunction pLoagPackage);

    // Summary:
    //      无
    lua_CFunction LPAPI FindPackage(const char* pcszPackageName);

    // Summary:
    //      无
    BOOL LPAPI RegisterFuncList(LUA_FUNC* pstFuncList);

    // Summary:
    //      无
    void LPAPI SetRootPath(const char* pcszRootPath);

    // Summary:
    //      无
    const char* LPAPI GetRootPath(void);

    // Summary:
    //      无
    LPINT64 LPAPI PrintMemoryStat(void);

private:

    // Summary:
    //      无
    static LPINT32 _LoadPackage(lua_State* L);

private:

    // Summary:
    //      none
    LPLuaScriptMgr();

    // Summary:
    //      none
    virtual ~LPLuaScriptMgr();

private:

    struct ltstr
    {
        inline bool operator()(const char* str1, const char* str2) const
        {
            return strncmp(str1, str2, MAX_FILE_NAME) < 0;
        }
    };

    typedef std::map<const char*, LPLuaScript*, ltstr> MAP_NAME_2_SCRIPT;
    MAP_NAME_2_SCRIPT        m_mapScript;

    typedef std::map<const char*, lua_CFunction, ltstr> MAP_NAME_2_PACKAGE;
    MAP_NAME_2_PACKAGE       m_mapPackage;

    typedef std::list<LUA_FUNC*> LST_LUA_FUNC;
    LST_LUA_FUNC             m_lstLuaFunc;

    BOOL                     m_bReload;
    char                     m_szRootPath[MAX_PATH];


};



//end声明所处的名字空间
NS_LZPL_END

#endif