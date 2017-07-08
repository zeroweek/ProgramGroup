//****************************************************************************/
//    author: caihy
//    date: November 26, 2015
//    description:
//
//****************************************************************************/
#ifndef _GLOBAL_CONFIG_H_
#define _GLOBAL_CONFIG_H_

#include "guard_config.h"
#include "server_config.h"



// tolua_begin
#pragma pack(push, 8)



struct COMMON
{
    char       szGuardAddr[COMMON_NAME_LEN];
    LPUINT32    dwGuardPort;
};

struct GLOBAL_CONFIG
{
    COMMON Common;
    SERVER_CONFIG  Server;
    GUARD_CONFIG   Guard;
};



#pragma pack(pop)
// tolua_end


extern GLOBAL_CONFIG g_GlobalConfig;
extern char g_szGlobalConfigFileName[MAX_PATH];

extern BOOL g_LoadGlobalConfig(const char* pcszFileName);




#endif