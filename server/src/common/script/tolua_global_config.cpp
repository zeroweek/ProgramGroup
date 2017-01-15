/*
** Lua binding: global_config
** Generated automatically by tolua++-1.0.92 on Sun Jan 15 21:11:49 2017.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_global_config_open (lua_State* tolua_S);

#include "../../inc/lzpl/lpi_net.h"
#include "../../inc/common/guard_config.h"
#include "../../inc/common/server_config.h"
#include "../../inc/common/global_config.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"GUARD_CONFIG");
 tolua_usertype(tolua_S,"SERVER_CONFIG::GT_CONFIG");
 tolua_usertype(tolua_S,"GLOBAL_CONFIG");
 tolua_usertype(tolua_S,"SERVER_CONFIG");
 tolua_usertype(tolua_S,"SERVER_CONFIG::GS_CONFIG");
 tolua_usertype(tolua_S,"COMMON");
 tolua_usertype(tolua_S,"NET_CONFIG");
 tolua_usertype(tolua_S,"SERVER_CONFIG::GAME_LOGIC_CONFIG");
}

/* get function: dwIoType of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_NET_CONFIG_unsigned_dwIoType
static int tolua_get_NET_CONFIG_unsigned_dwIoType(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwIoType'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwIoType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwIoType of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_NET_CONFIG_unsigned_dwIoType
static int tolua_set_NET_CONFIG_unsigned_dwIoType(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwIoType'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwIoType = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwRecvBufSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_NET_CONFIG_unsigned_dwRecvBufSize
static int tolua_get_NET_CONFIG_unsigned_dwRecvBufSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwRecvBufSize'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwRecvBufSize);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwRecvBufSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_NET_CONFIG_unsigned_dwRecvBufSize
static int tolua_set_NET_CONFIG_unsigned_dwRecvBufSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwRecvBufSize'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwRecvBufSize = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwSendBufSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_NET_CONFIG_unsigned_dwSendBufSize
static int tolua_get_NET_CONFIG_unsigned_dwSendBufSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwSendBufSize'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwSendBufSize);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwSendBufSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_NET_CONFIG_unsigned_dwSendBufSize
static int tolua_set_NET_CONFIG_unsigned_dwSendBufSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwSendBufSize'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwSendBufSize = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwConnectCount of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_NET_CONFIG_unsigned_dwConnectCount
static int tolua_get_NET_CONFIG_unsigned_dwConnectCount(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwConnectCount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwConnectCount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwConnectCount of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_NET_CONFIG_unsigned_dwConnectCount
static int tolua_set_NET_CONFIG_unsigned_dwConnectCount(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwConnectCount'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwConnectCount = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwNetEventPoolSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_NET_CONFIG_unsigned_dwNetEventPoolSize
static int tolua_get_NET_CONFIG_unsigned_dwNetEventPoolSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwNetEventPoolSize'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwNetEventPoolSize);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwNetEventPoolSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_NET_CONFIG_unsigned_dwNetEventPoolSize
static int tolua_set_NET_CONFIG_unsigned_dwNetEventPoolSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwNetEventPoolSize'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwNetEventPoolSize = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwNetEventListCount of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_NET_CONFIG_unsigned_dwNetEventListCount
static int tolua_get_NET_CONFIG_unsigned_dwNetEventListCount(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwNetEventListCount'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwNetEventListCount);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwNetEventListCount of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_NET_CONFIG_unsigned_dwNetEventListCount
static int tolua_set_NET_CONFIG_unsigned_dwNetEventListCount(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwNetEventListCount'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwNetEventListCount = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwNetRecvEventBufSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_NET_CONFIG_unsigned_dwNetRecvEventBufSize
static int tolua_get_NET_CONFIG_unsigned_dwNetRecvEventBufSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwNetRecvEventBufSize'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwNetRecvEventBufSize);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwNetRecvEventBufSize of class  NET_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_NET_CONFIG_unsigned_dwNetRecvEventBufSize
static int tolua_set_NET_CONFIG_unsigned_dwNetRecvEventBufSize(lua_State* tolua_S)
{
  NET_CONFIG* self = (NET_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwNetRecvEventBufSize'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwNetRecvEventBufSize = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szAddress of class  GUARD_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_GUARD_CONFIG_szAddress
static int tolua_get_GUARD_CONFIG_szAddress(lua_State* tolua_S)
{
  GUARD_CONFIG* self = (GUARD_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szAddress'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szAddress);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szAddress of class  GUARD_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_GUARD_CONFIG_szAddress
static int tolua_set_GUARD_CONFIG_szAddress(lua_State* tolua_S)
{
  GUARD_CONFIG* self = (GUARD_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szAddress'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szAddress,tolua_tostring(tolua_S,2,0),COMMON_NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwPort of class  GUARD_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_GUARD_CONFIG_unsigned_dwPort
static int tolua_get_GUARD_CONFIG_unsigned_dwPort(lua_State* tolua_S)
{
  GUARD_CONFIG* self = (GUARD_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwPort'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwPort of class  GUARD_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_GUARD_CONFIG_unsigned_dwPort
static int tolua_set_GUARD_CONFIG_unsigned_dwPort(lua_State* tolua_S)
{
  GUARD_CONFIG* self = (GUARD_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwPort'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwPort = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Net of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GS_CONFIG_Net
static int tolua_get_SERVER_CONFIG__GS_CONFIG_Net(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Net'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Net,"NET_CONFIG");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Net of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GS_CONFIG_Net
static int tolua_set_SERVER_CONFIG__GS_CONFIG_Net(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Net'",NULL);
 if (!tolua_isusertype(tolua_S,2,"NET_CONFIG",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Net = *((NET_CONFIG*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szListenIp of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GS_CONFIG_szListenIp
static int tolua_get_SERVER_CONFIG__GS_CONFIG_szListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szListenIp'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szListenIp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szListenIp of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GS_CONFIG_szListenIp
static int tolua_set_SERVER_CONFIG__GS_CONFIG_szListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szListenIp'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szListenIp,tolua_tostring(tolua_S,2,0),IP_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwListenPort of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GS_CONFIG_unsigned_dwListenPort
static int tolua_get_SERVER_CONFIG__GS_CONFIG_unsigned_dwListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwListenPort'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwListenPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwListenPort of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GS_CONFIG_unsigned_dwListenPort
static int tolua_set_SERVER_CONFIG__GS_CONFIG_unsigned_dwListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwListenPort'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwListenPort = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szClientListenIp of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GS_CONFIG_szClientListenIp
static int tolua_get_SERVER_CONFIG__GS_CONFIG_szClientListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szClientListenIp'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szClientListenIp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szClientListenIp of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GS_CONFIG_szClientListenIp
static int tolua_set_SERVER_CONFIG__GS_CONFIG_szClientListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szClientListenIp'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szClientListenIp,tolua_tostring(tolua_S,2,0),IP_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwClientListenPort of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GS_CONFIG_unsigned_dwClientListenPort
static int tolua_get_SERVER_CONFIG__GS_CONFIG_unsigned_dwClientListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwClientListenPort'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwClientListenPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwClientListenPort of class  GS_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GS_CONFIG_unsigned_dwClientListenPort
static int tolua_set_SERVER_CONFIG__GS_CONFIG_unsigned_dwClientListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GS_CONFIG* self = (SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwClientListenPort'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwClientListenPort = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Gs of class  SERVER_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG_Gs
static int tolua_get_SERVER_CONFIG_Gs(lua_State* tolua_S)
{
  SERVER_CONFIG* self = (SERVER_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Gs'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Gs,"SERVER_CONFIG::GS_CONFIG");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Gs of class  SERVER_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG_Gs
static int tolua_set_SERVER_CONFIG_Gs(lua_State* tolua_S)
{
  SERVER_CONFIG* self = (SERVER_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Gs'",NULL);
 if (!tolua_isusertype(tolua_S,2,"SERVER_CONFIG::GS_CONFIG",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Gs = *((SERVER_CONFIG::GS_CONFIG*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szConfigPath of class  GAME_LOGIC_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GAME_LOGIC_CONFIG_szConfigPath
static int tolua_get_SERVER_CONFIG__GAME_LOGIC_CONFIG_szConfigPath(lua_State* tolua_S)
{
  SERVER_CONFIG::GAME_LOGIC_CONFIG* self = (SERVER_CONFIG::GAME_LOGIC_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szConfigPath'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szConfigPath);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szConfigPath of class  GAME_LOGIC_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GAME_LOGIC_CONFIG_szConfigPath
static int tolua_set_SERVER_CONFIG__GAME_LOGIC_CONFIG_szConfigPath(lua_State* tolua_S)
{
  SERVER_CONFIG::GAME_LOGIC_CONFIG* self = (SERVER_CONFIG::GAME_LOGIC_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szConfigPath'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szConfigPath,tolua_tostring(tolua_S,2,0),COMMON_NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Gl of class  SERVER_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG_Gl
static int tolua_get_SERVER_CONFIG_Gl(lua_State* tolua_S)
{
  SERVER_CONFIG* self = (SERVER_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Gl'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Gl,"SERVER_CONFIG::GAME_LOGIC_CONFIG");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Gl of class  SERVER_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG_Gl
static int tolua_set_SERVER_CONFIG_Gl(lua_State* tolua_S)
{
  SERVER_CONFIG* self = (SERVER_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Gl'",NULL);
 if (!tolua_isusertype(tolua_S,2,"SERVER_CONFIG::GAME_LOGIC_CONFIG",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Gl = *((SERVER_CONFIG::GAME_LOGIC_CONFIG*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Net of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GT_CONFIG_Net
static int tolua_get_SERVER_CONFIG__GT_CONFIG_Net(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Net'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Net,"NET_CONFIG");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Net of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GT_CONFIG_Net
static int tolua_set_SERVER_CONFIG__GT_CONFIG_Net(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Net'",NULL);
 if (!tolua_isusertype(tolua_S,2,"NET_CONFIG",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Net = *((NET_CONFIG*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szListenIp of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GT_CONFIG_szListenIp
static int tolua_get_SERVER_CONFIG__GT_CONFIG_szListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szListenIp'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szListenIp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szListenIp of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GT_CONFIG_szListenIp
static int tolua_set_SERVER_CONFIG__GT_CONFIG_szListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szListenIp'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szListenIp,tolua_tostring(tolua_S,2,0),IP_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwListenPort of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GT_CONFIG_unsigned_dwListenPort
static int tolua_get_SERVER_CONFIG__GT_CONFIG_unsigned_dwListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwListenPort'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwListenPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwListenPort of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GT_CONFIG_unsigned_dwListenPort
static int tolua_set_SERVER_CONFIG__GT_CONFIG_unsigned_dwListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwListenPort'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwListenPort = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szClientListenIp of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GT_CONFIG_szClientListenIp
static int tolua_get_SERVER_CONFIG__GT_CONFIG_szClientListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szClientListenIp'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szClientListenIp);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szClientListenIp of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GT_CONFIG_szClientListenIp
static int tolua_set_SERVER_CONFIG__GT_CONFIG_szClientListenIp(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szClientListenIp'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szClientListenIp,tolua_tostring(tolua_S,2,0),IP_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwClientListenPort of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG__GT_CONFIG_unsigned_dwClientListenPort
static int tolua_get_SERVER_CONFIG__GT_CONFIG_unsigned_dwClientListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwClientListenPort'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwClientListenPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwClientListenPort of class  GT_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG__GT_CONFIG_unsigned_dwClientListenPort
static int tolua_set_SERVER_CONFIG__GT_CONFIG_unsigned_dwClientListenPort(lua_State* tolua_S)
{
  SERVER_CONFIG::GT_CONFIG* self = (SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwClientListenPort'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwClientListenPort = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Gt of class  SERVER_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_SERVER_CONFIG_Gt
static int tolua_get_SERVER_CONFIG_Gt(lua_State* tolua_S)
{
  SERVER_CONFIG* self = (SERVER_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Gt'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Gt,"SERVER_CONFIG::GT_CONFIG");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Gt of class  SERVER_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_SERVER_CONFIG_Gt
static int tolua_set_SERVER_CONFIG_Gt(lua_State* tolua_S)
{
  SERVER_CONFIG* self = (SERVER_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Gt'",NULL);
 if (!tolua_isusertype(tolua_S,2,"SERVER_CONFIG::GT_CONFIG",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Gt = *((SERVER_CONFIG::GT_CONFIG*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: szGuardAddr of class  COMMON */
#ifndef TOLUA_DISABLE_tolua_get_COMMON_szGuardAddr
static int tolua_get_COMMON_szGuardAddr(lua_State* tolua_S)
{
  COMMON* self = (COMMON*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szGuardAddr'",NULL);
#endif
 tolua_pushstring(tolua_S,(const char*)self->szGuardAddr);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: szGuardAddr of class  COMMON */
#ifndef TOLUA_DISABLE_tolua_set_COMMON_szGuardAddr
static int tolua_set_COMMON_szGuardAddr(lua_State* tolua_S)
{
  COMMON* self = (COMMON*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'szGuardAddr'",NULL);
 if (!tolua_isstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
#ifdef _WIN32
 strncpy_s
#else
 strncpy
#endif
 (self->szGuardAddr,tolua_tostring(tolua_S,2,0),COMMON_NAME_LEN-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dwGuardPort of class  COMMON */
#ifndef TOLUA_DISABLE_tolua_get_COMMON_unsigned_dwGuardPort
static int tolua_get_COMMON_unsigned_dwGuardPort(lua_State* tolua_S)
{
  COMMON* self = (COMMON*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwGuardPort'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->dwGuardPort);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dwGuardPort of class  COMMON */
#ifndef TOLUA_DISABLE_tolua_set_COMMON_unsigned_dwGuardPort
static int tolua_set_COMMON_unsigned_dwGuardPort(lua_State* tolua_S)
{
  COMMON* self = (COMMON*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dwGuardPort'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dwGuardPort = (( unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Common of class  GLOBAL_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_GLOBAL_CONFIG_Common
static int tolua_get_GLOBAL_CONFIG_Common(lua_State* tolua_S)
{
  GLOBAL_CONFIG* self = (GLOBAL_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Common'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Common,"COMMON");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Common of class  GLOBAL_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_GLOBAL_CONFIG_Common
static int tolua_set_GLOBAL_CONFIG_Common(lua_State* tolua_S)
{
  GLOBAL_CONFIG* self = (GLOBAL_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Common'",NULL);
 if (!tolua_isusertype(tolua_S,2,"COMMON",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Common = *((COMMON*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Server of class  GLOBAL_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_GLOBAL_CONFIG_Server
static int tolua_get_GLOBAL_CONFIG_Server(lua_State* tolua_S)
{
  GLOBAL_CONFIG* self = (GLOBAL_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Server'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Server,"SERVER_CONFIG");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Server of class  GLOBAL_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_GLOBAL_CONFIG_Server
static int tolua_set_GLOBAL_CONFIG_Server(lua_State* tolua_S)
{
  GLOBAL_CONFIG* self = (GLOBAL_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Server'",NULL);
 if (!tolua_isusertype(tolua_S,2,"SERVER_CONFIG",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Server = *((SERVER_CONFIG*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Guard of class  GLOBAL_CONFIG */
#ifndef TOLUA_DISABLE_tolua_get_GLOBAL_CONFIG_Guard
static int tolua_get_GLOBAL_CONFIG_Guard(lua_State* tolua_S)
{
  GLOBAL_CONFIG* self = (GLOBAL_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Guard'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->Guard,"GUARD_CONFIG");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Guard of class  GLOBAL_CONFIG */
#ifndef TOLUA_DISABLE_tolua_set_GLOBAL_CONFIG_Guard
static int tolua_set_GLOBAL_CONFIG_Guard(lua_State* tolua_S)
{
  GLOBAL_CONFIG* self = (GLOBAL_CONFIG*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Guard'",NULL);
 if (!tolua_isusertype(tolua_S,2,"GUARD_CONFIG",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Guard = *((GUARD_CONFIG*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_global_config_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_cclass(tolua_S,"NET_CONFIG","NET_CONFIG","",NULL);
 tolua_beginmodule(tolua_S,"NET_CONFIG");
  tolua_variable(tolua_S,"dwIoType",tolua_get_NET_CONFIG_unsigned_dwIoType,tolua_set_NET_CONFIG_unsigned_dwIoType);
  tolua_variable(tolua_S,"dwRecvBufSize",tolua_get_NET_CONFIG_unsigned_dwRecvBufSize,tolua_set_NET_CONFIG_unsigned_dwRecvBufSize);
  tolua_variable(tolua_S,"dwSendBufSize",tolua_get_NET_CONFIG_unsigned_dwSendBufSize,tolua_set_NET_CONFIG_unsigned_dwSendBufSize);
  tolua_variable(tolua_S,"dwConnectCount",tolua_get_NET_CONFIG_unsigned_dwConnectCount,tolua_set_NET_CONFIG_unsigned_dwConnectCount);
  tolua_variable(tolua_S,"dwNetEventPoolSize",tolua_get_NET_CONFIG_unsigned_dwNetEventPoolSize,tolua_set_NET_CONFIG_unsigned_dwNetEventPoolSize);
  tolua_variable(tolua_S,"dwNetEventListCount",tolua_get_NET_CONFIG_unsigned_dwNetEventListCount,tolua_set_NET_CONFIG_unsigned_dwNetEventListCount);
  tolua_variable(tolua_S,"dwNetRecvEventBufSize",tolua_get_NET_CONFIG_unsigned_dwNetRecvEventBufSize,tolua_set_NET_CONFIG_unsigned_dwNetRecvEventBufSize);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"GUARD_CONFIG","GUARD_CONFIG","",NULL);
 tolua_beginmodule(tolua_S,"GUARD_CONFIG");
  tolua_variable(tolua_S,"szAddress",tolua_get_GUARD_CONFIG_szAddress,tolua_set_GUARD_CONFIG_szAddress);
  tolua_variable(tolua_S,"dwPort",tolua_get_GUARD_CONFIG_unsigned_dwPort,tolua_set_GUARD_CONFIG_unsigned_dwPort);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"SERVER_CONFIG","SERVER_CONFIG","",NULL);
 tolua_beginmodule(tolua_S,"SERVER_CONFIG");
  tolua_cclass(tolua_S,"GS_CONFIG","SERVER_CONFIG::GS_CONFIG","",NULL);
  tolua_beginmodule(tolua_S,"GS_CONFIG");
   tolua_variable(tolua_S,"Net",tolua_get_SERVER_CONFIG__GS_CONFIG_Net,tolua_set_SERVER_CONFIG__GS_CONFIG_Net);
   tolua_variable(tolua_S,"szListenIp",tolua_get_SERVER_CONFIG__GS_CONFIG_szListenIp,tolua_set_SERVER_CONFIG__GS_CONFIG_szListenIp);
   tolua_variable(tolua_S,"dwListenPort",tolua_get_SERVER_CONFIG__GS_CONFIG_unsigned_dwListenPort,tolua_set_SERVER_CONFIG__GS_CONFIG_unsigned_dwListenPort);
   tolua_variable(tolua_S,"szClientListenIp",tolua_get_SERVER_CONFIG__GS_CONFIG_szClientListenIp,tolua_set_SERVER_CONFIG__GS_CONFIG_szClientListenIp);
   tolua_variable(tolua_S,"dwClientListenPort",tolua_get_SERVER_CONFIG__GS_CONFIG_unsigned_dwClientListenPort,tolua_set_SERVER_CONFIG__GS_CONFIG_unsigned_dwClientListenPort);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"Gs",tolua_get_SERVER_CONFIG_Gs,tolua_set_SERVER_CONFIG_Gs);
  tolua_cclass(tolua_S,"GAME_LOGIC_CONFIG","SERVER_CONFIG::GAME_LOGIC_CONFIG","",NULL);
  tolua_beginmodule(tolua_S,"GAME_LOGIC_CONFIG");
   tolua_variable(tolua_S,"szConfigPath",tolua_get_SERVER_CONFIG__GAME_LOGIC_CONFIG_szConfigPath,tolua_set_SERVER_CONFIG__GAME_LOGIC_CONFIG_szConfigPath);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"Gl",tolua_get_SERVER_CONFIG_Gl,tolua_set_SERVER_CONFIG_Gl);
  tolua_cclass(tolua_S,"GT_CONFIG","SERVER_CONFIG::GT_CONFIG","",NULL);
  tolua_beginmodule(tolua_S,"GT_CONFIG");
   tolua_variable(tolua_S,"Net",tolua_get_SERVER_CONFIG__GT_CONFIG_Net,tolua_set_SERVER_CONFIG__GT_CONFIG_Net);
   tolua_variable(tolua_S,"szListenIp",tolua_get_SERVER_CONFIG__GT_CONFIG_szListenIp,tolua_set_SERVER_CONFIG__GT_CONFIG_szListenIp);
   tolua_variable(tolua_S,"dwListenPort",tolua_get_SERVER_CONFIG__GT_CONFIG_unsigned_dwListenPort,tolua_set_SERVER_CONFIG__GT_CONFIG_unsigned_dwListenPort);
   tolua_variable(tolua_S,"szClientListenIp",tolua_get_SERVER_CONFIG__GT_CONFIG_szClientListenIp,tolua_set_SERVER_CONFIG__GT_CONFIG_szClientListenIp);
   tolua_variable(tolua_S,"dwClientListenPort",tolua_get_SERVER_CONFIG__GT_CONFIG_unsigned_dwClientListenPort,tolua_set_SERVER_CONFIG__GT_CONFIG_unsigned_dwClientListenPort);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"Gt",tolua_get_SERVER_CONFIG_Gt,tolua_set_SERVER_CONFIG_Gt);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"COMMON","COMMON","",NULL);
 tolua_beginmodule(tolua_S,"COMMON");
  tolua_variable(tolua_S,"szGuardAddr",tolua_get_COMMON_szGuardAddr,tolua_set_COMMON_szGuardAddr);
  tolua_variable(tolua_S,"dwGuardPort",tolua_get_COMMON_unsigned_dwGuardPort,tolua_set_COMMON_unsigned_dwGuardPort);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"GLOBAL_CONFIG","GLOBAL_CONFIG","",NULL);
 tolua_beginmodule(tolua_S,"GLOBAL_CONFIG");
  tolua_variable(tolua_S,"Common",tolua_get_GLOBAL_CONFIG_Common,tolua_set_GLOBAL_CONFIG_Common);
  tolua_variable(tolua_S,"Server",tolua_get_GLOBAL_CONFIG_Server,tolua_set_GLOBAL_CONFIG_Server);
  tolua_variable(tolua_S,"Guard",tolua_get_GLOBAL_CONFIG_Guard,tolua_set_GLOBAL_CONFIG_Guard);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_global_config (lua_State* tolua_S) {
 return tolua_global_config_open(tolua_S);
};
#endif

