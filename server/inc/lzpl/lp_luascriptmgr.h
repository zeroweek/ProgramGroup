//****************************************************************************/
//    author: caihy
//    date: October 14, 2015
//    description: lua�ű�����
//
//****************************************************************************/
#ifndef _LP_LUA_SCRIPT_MGR_H_
#define _LP_LUA_SCRIPT_MGR_H_

#include "lp_base.h"
#include "lp_luascript.h"



//begin�������������ֿռ�
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
    //      TRUE-�ɹ���FALSE-ʧ��
    BOOL LPAPI Init(const char* pcszRootPath);

    // Summary:
    //      ����ʼ��
    BOOL LPAPI UnInit(void);

    // Summary:
    //      ��
    BOOL LPAPI Reload(LPINT32& nErrorCount);

    // Summary:
    //      ��
    BOOL LPAPI Reload(const char* pcszSciptName);

    // Summary:
    //      ��
    LPLuaScript* LPAPI NewScript(const char* pcszSciptName);

    // Summary:
    //      ��
    BOOL LPAPI DelScript(LPLuaScript* poScript);

    // Summary:
    //      ��
    LPLuaScript* LPAPI FindScript(const char* pcszSciptName);

    // Summary:
    //      ��
    BOOL LPAPI RegisterPackage(const char* pcszPackageName, lua_CFunction pLoagPackage);

    // Summary:
    //      ��
    lua_CFunction LPAPI FindPackage(const char* pcszPackageName);

    // Summary:
    //      ��
    BOOL LPAPI RegisterFuncList(LUA_FUNC* pstFuncList);

    // Summary:
    //      ��
    void LPAPI SetRootPath(const char* pcszRootPath);

    // Summary:
    //      ��
    const char* LPAPI GetRootPath(void);

    // Summary:
    //      ��
    LPINT64 LPAPI PrintMemoryStat(void);

private:

    // Summary:
    //      ��
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



//end�������������ֿռ�
NS_LZPL_END

#endif