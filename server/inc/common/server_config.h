//****************************************************************************/
//    author: caihy
//    date: November 26, 2015
//    description: 
//
//****************************************************************************/
#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include "lp_lzpl.h"



// tolua_begin
#pragma pack(push, 8)



struct SERVER_CONFIG
{
	struct GS_CONFIG
	{
		NET_CONFIG     Net;

		char           szListenIp[IP_LEN];
		UINT_32        dwListenPort;

		char           szClientListenIp[IP_LEN];
		UINT_32        dwClientListenPort;
	};
	GS_CONFIG Gs;

	struct GAME_LOGIC_CONFIG
	{
		char   szConfigPath[COMMON_NAME_LEN];
	};
	GAME_LOGIC_CONFIG Gl;

	struct GT_CONFIG
	{
		NET_CONFIG     Net;

		char           szListenIp[IP_LEN];
		UINT_32        dwListenPort;

		char           szClientListenIp[IP_LEN];
		UINT_32        dwClientListenPort;
	};
	GT_CONFIG Gt;
};



#pragma pack(pop)
// tolua_end




#endif