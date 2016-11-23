//****************************************************************************/
//    author: caihy
//    date: December 3, 2015
//    description: 
//
//****************************************************************************/
#ifndef _TAB_TEST_DATA_H_
#define _TAB_TEST_DATA_H_

#include "lp_lzpl.h"

using namespace LZPL;



struct TAB_TEST_DATA
{
	INT_32 nId;
	char szName[COMMON_NAME_LEN];
	char szBeginTime[COMMON_NAME_LEN];
	char szEndTime[COMMON_NAME_LEN];

	static void Print(TAB_TEST_DATA* pData);
	static BOOL ReadLine(INT_32& rnId, TAB_TEST_DATA* pData, LPTabFile* pTabFile, INT_32 nRow);

};




#endif