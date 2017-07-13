/*
** Lua binding: ai
** Generated automatically by tolua++-1.0.92 on Thu Jul 13 22:03:42 2017.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_ai_open (lua_State* tolua_S);

#include "../../inc/lzpl/lp_bt.h"
#include "../../inc/gamelogic/bt_action.h"
#include "../../inc/common/ai_data.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"AI_DATA");
 tolua_usertype(tolua_S,"LPTabFile");
}

/* get function: nAIID of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_get_AI_DATA_nAIID
static int tolua_get_AI_DATA_nAIID(lua_State* tolua_S)
{
  AI_DATA* self = (AI_DATA*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'nAIID'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->nAIID);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: nAIID of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_set_AI_DATA_nAIID
static int tolua_set_AI_DATA_nAIID(lua_State* tolua_S)
{
  AI_DATA* self = (AI_DATA*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'nAIID'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->nAIID = (( signed int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szAIDesc of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_get_AI_DATA_szAIDesc
static int tolua_get_AI_DATA_szAIDesc(lua_State* tolua_S)
{
  AI_DATA* self = (AI_DATA*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szAIDesc'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szAIDesc);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szAIDesc of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_set_AI_DATA_szAIDesc
static int tolua_set_AI_DATA_szAIDesc(lua_State* tolua_S)
{
  AI_DATA* self = (AI_DATA*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szAIDesc'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szAIDesc,tolua_tostring(tolua_S,2,0),COMMON_NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szScriptName of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_get_AI_DATA_szScriptName
static int tolua_get_AI_DATA_szScriptName(lua_State* tolua_S)
{
  AI_DATA* self = (AI_DATA*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szScriptName'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szScriptName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szScriptName of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_set_AI_DATA_szScriptName
static int tolua_set_AI_DATA_szScriptName(lua_State* tolua_S)
{
  AI_DATA* self = (AI_DATA*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szScriptName'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szScriptName,tolua_tostring(tolua_S,2,0),COMMON_NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Print of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_ai_AI_DATA_Print00
static int tolua_ai_AI_DATA_Print00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"AI_DATA",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"AI_DATA",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  AI_DATA* pData = ((AI_DATA*)  tolua_tousertype(tolua_S,2,0));
 {
  AI_DATA::Print(pData);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Print'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadLine of class  AI_DATA */
#ifndef TOLUA_DISABLE_tolua_ai_AI_DATA_ReadLine00
static int tolua_ai_AI_DATA_ReadLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"AI_DATA",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"AI_DATA",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,4,"LPTabFile",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  signed int rnId = (( signed int)  tolua_tonumber(tolua_S,2,0));
  AI_DATA* pData = ((AI_DATA*)  tolua_tousertype(tolua_S,3,0));
  LPTabFile* pTabFile = ((LPTabFile*)  tolua_tousertype(tolua_S,4,0));
  signed int nRow = (( signed int)  tolua_tonumber(tolua_S,5,0));
 {
  int tolua_ret = (int)  AI_DATA::ReadLine(rnId,pData,pTabFile,nRow);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 tolua_pushnumber(tolua_S,(lua_Number)rnId);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_ai_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_constant(tolua_S,"btrvError",btrvError);
 tolua_constant(tolua_S,"btrvFail",btrvFail);
 tolua_constant(tolua_S,"btrvSuccess",btrvSuccess);
 tolua_constant(tolua_S,"btrvRunning",btrvRunning);
 tolua_constant(tolua_S,"btrvTotal",btrvTotal);
 tolua_constant(tolua_S,"btntInvalid",btntInvalid);
 tolua_constant(tolua_S,"btntSelector",btntSelector);
 tolua_constant(tolua_S,"btntSequencer",btntSequencer);
 tolua_constant(tolua_S,"btntParallel",btntParallel);
 tolua_constant(tolua_S,"btntNot",btntNot);
 tolua_constant(tolua_S,"btntDebug",btntDebug);
 tolua_constant(tolua_S,"btntRandom",btntRandom);
 tolua_constant(tolua_S,"btntRepeat",btntRepeat);
 tolua_constant(tolua_S,"btntUserDefine",btntUserDefine);
 tolua_constant(tolua_S,"btntTotal",btntTotal);
 tolua_constant(tolua_S,"btttInvalid",btttInvalid);
 tolua_constant(tolua_S,"btttType_Self",btttType_Self);
 tolua_constant(tolua_S,"btttType_Total",btttType_Total);
 tolua_constant(tolua_S,"btatInvalid",btatInvalid);
 tolua_constant(tolua_S,"btatSelector",btatSelector);
 tolua_constant(tolua_S,"btatSequencer",btatSequencer);
 tolua_constant(tolua_S,"btatParallel",btatParallel);
 tolua_constant(tolua_S,"btatNot",btatNot);
 tolua_constant(tolua_S,"btatDebug",btatDebug);
 tolua_constant(tolua_S,"btatRandom",btatRandom);
 tolua_constant(tolua_S,"btatRepeat",btatRepeat);
 tolua_constant(tolua_S,"btatBegin",btatBegin);
 tolua_constant(tolua_S,"btatReturnSuccess",btatReturnSuccess);
 tolua_constant(tolua_S,"btatReturnFail",btatReturnFail);
 tolua_constant(tolua_S,"btatTestAction",btatTestAction);
 tolua_constant(tolua_S,"btatEnd",btatEnd);
 tolua_constant(tolua_S,"AI_MAX_ID",AI_MAX_ID);
 tolua_cclass(tolua_S,"AI_DATA","AI_DATA","",NULL);
 tolua_beginmodule(tolua_S,"AI_DATA");
  tolua_variable(tolua_S,"nAIID",tolua_get_AI_DATA_nAIID,tolua_set_AI_DATA_nAIID);
  tolua_variable(tolua_S,"szAIDesc",tolua_get_AI_DATA_szAIDesc,tolua_set_AI_DATA_szAIDesc);
  tolua_variable(tolua_S,"szScriptName",tolua_get_AI_DATA_szScriptName,tolua_set_AI_DATA_szScriptName);
  tolua_function(tolua_S,"Print",tolua_ai_AI_DATA_Print00);
  tolua_function(tolua_S,"ReadLine",tolua_ai_AI_DATA_ReadLine00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_ai (lua_State* tolua_S) {
 return tolua_ai_open(tolua_S);
};
#endif

