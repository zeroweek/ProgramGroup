//****************************************************************************/
//    author: caihy
//    date: November 26, 2015
//    description: 
//
//****************************************************************************/
#ifndef _GUARD_CONFIG_H_
#define _GUARD_CONFIG_H_

#include "lp_lzpl.h"



// tolua_begin
#pragma pack(push, 8)



struct GUARD_CONFIG
{
	char      szAddress[COMMON_NAME_LEN];
	UINT_32   dwPort;
};



#pragma pack(pop)
// tolua_end


#endif