//****************************************************************************/
//    author: caihy
//    date: December 3, 2015
//    description: 
//
//****************************************************************************/
#ifndef _AI_DATA_H_
#define _AI_DATA_H_

#include "lp_lzpl.h"

using namespace LZPL;



// tolua_begin

#define AI_MAX_ID                       (10000)


struct AI_DATA
{
	INT_32 nAIID;
	char szAIDesc[COMMON_NAME_LEN];
	char szScriptName[COMMON_NAME_LEN];

	static void Print(AI_DATA* pData);
	static BOOL ReadLine(INT_32& rnId, AI_DATA* pData, LPTabFile* pTabFile, INT_32 nRow);

};

// tolua_end



#endif