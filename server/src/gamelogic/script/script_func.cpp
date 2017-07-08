#include "script_func.h"
#include "common.h"
#include "lp_lzpl.h"
#include "bt_manager.h"
#include "tolua++.h"



using namespace LZPL;



int lua_get_global_config(lua_State* L)
{
    lua_pushlightuserdata(L, &g_GlobalConfig);

    return 1;
}

int lua_create_bt_node(lua_State* L)
{
    LPINT32 nType = 0;
    LPINT32 nParam0 = 0;
    LPINT32 nParam1 = 0;
    LPINT32 nParam2 = 0;
    LPINT32 nParam3 = 0;
    LPINT32 nFlag = 0;
    BT_NODE* pNode = NULL;
    BT_NODE* pParent = NULL;
    const char* pcszName = NULL;

    pParent = (BT_NODE*)lua_touserdata(L, 1);
    nType = (LPINT32)lua_tonumber(L, 2);
    pcszName = lua_tostring(L, 3);
    nParam0 = (LPINT32)lua_tonumber(L, 4);
    nParam1 = (LPINT32)lua_tonumber(L, 5);
    nParam2 = (LPINT32)lua_tonumber(L, 6);
    nParam3 = (LPINT32)lua_tonumber(L, 7);
    nFlag = (LPINT32)lua_tonumber(L, 8);

    pNode = CBTManager::Instance().CreateBTNode(pParent, nType, 0, pcszName, nParam0, nParam1, nParam2, nParam3, nFlag);
    LOG_PROCESS_ERROR(pNode);

    lua_pushlightuserdata(L, pNode);

    return 1;
Exit0:
    return 0;
}

int lua_destroy_bt_tree(lua_State* L)
{
    LPINT32 nResult = 0;
    BT_NODE* pNode = NULL;

    pNode = (BT_NODE*)lua_touserdata(L, 1);
    LOG_PROCESS_ERROR(pNode);

    nResult = CBTManager::Instance().DestroyBTTree(pNode);
    LOG_PROCESS_ERROR(nResult);

Exit0:
    return 0;
}

int lua_find_bt_action_id(lua_State* L)
{
    LPINT32 nActionId = btntInvalid;
    const char* pcszName = NULL;

    pcszName = lua_tostring(L, 1);
    nActionId = CBTManager::Instance().FindBTActionByName(pcszName);
    PROCESS_ERROR(nActionId != btntInvalid);

    lua_pushnumber(L, nActionId);

    return 1;
Exit0:
    return 0;
}

int lua_register_bt_action(lua_State* L)
{
    LPINT32 nActionId = btntInvalid;
    const char* pcszName = NULL;

    pcszName = lua_tostring(L, 1);
    nActionId = CBTManager::Instance().RegisterBTAction(pcszName);

    lua_pushnumber(L, nActionId);

    return 1;
}

int lua_print_bt_tree(lua_State* L)
{
    BT_NODE* pNode = NULL;

    pNode = (BT_NODE*)lua_touserdata(L, 1);
    CBTManager::Instance().PrintBTTree(pNode, 0);

    return 0;
}

int lua_call_bt_action(lua_State* L)
{
    LPINT32 nResult = btrvError;
    LPINT32 nNodeType;
    LPINT32 nParam[BT_MAX_PARAM];

    nNodeType = (LPINT32)lua_tonumber(L, 1);
    nParam[0] = (LPINT32)lua_tonumber(L, 2);
    nParam[1] = (LPINT32)lua_tonumber(L, 3);
    nParam[2] = (LPINT32)lua_tonumber(L, 4);
    nParam[3] = (LPINT32)lua_tonumber(L, 5);

    nResult = CBTManager::Instance().CallBTAction(nNodeType, nParam);

    lua_pushnumber(L, nResult);

    return 1;
}

int lua_test_bt_action(lua_State* L)
{
    void* pObj = NULL;
    LPINT32 nResult = btrvError;
    LPINT32 nNodeType;
    LPINT32 nParam[BT_MAX_PARAM];

    pObj = tolua_touserdata(L, 1, 0);
    nNodeType = (LPINT32)lua_tonumber(L, 1);
    nParam[0] = (LPINT32)lua_tonumber(L, 2);
    nParam[1] = (LPINT32)lua_tonumber(L, 3);
    nParam[2] = (LPINT32)lua_tonumber(L, 4);
    nParam[3] = (LPINT32)lua_tonumber(L, 5);

    nResult = CBTManager::Instance().TestBTAction(pObj, nNodeType, nParam);

    lua_pushnumber(L, nResult);

    return 1;
}

int lua_get_ctrl_role_id(lua_State* L)
{
    LPUINT32 dwRoleId = 0;

    dwRoleId = CBTManager::Instance().GetCtrlRoleId();

    lua_pushnumber(L, dwRoleId);

    return 1;
}

struct AI_DATA;
extern const AI_DATA* g_pstLoadingNewAIData;
int lua_load_ai_data(lua_State* L)
{
    if(g_pstLoadingNewAIData)
    {
        tolua_pushusertype(L, (void*)g_pstLoadingNewAIData, "AI_DATA");
        lua_setglobal(L, "current_ai_data");
        return 1;
    }

    return 0;
}



LZPL::LUA_FUNC g_LuaScriptFuncTable[] =
{
    {"get_global_config",             lua_get_global_config},
    {"create_bt_node",                lua_create_bt_node},
    {"destroy_bt_tree",               lua_destroy_bt_tree},
    {"find_bt_action_id",             lua_find_bt_action_id},
    {"register_bt_action",            lua_register_bt_action},
    {"print_bt_tree",                 lua_print_bt_tree},
    {"call_bt_action",                lua_call_bt_action},
    {"test_bt_action",                lua_test_bt_action},
    {"get_ctrl_role_id",              lua_get_ctrl_role_id},
    {"load_ai_data",                  lua_load_ai_data},
    {0, 0}
};