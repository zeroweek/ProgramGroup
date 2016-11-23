//****************************************************************************/
//    author: caihy
//    date: December 1, 2015
//    description: 
//
//****************************************************************************/
#ifndef _BT_ACTION_H_
#define _BT_ACTION_H_

#include "lp_bt.h"



// tolua_begin



enum eBTTargetType
{
	btttInvalid = 0,

	btttType_Self,

	btttType_Total
};

enum eBTActionType
{
	btatInvalid         = LZPL::btntInvalid,
	btatSelector        = LZPL::btntSelector,
	btatSequencer       = LZPL::btntSequencer,
	btatParallel        = LZPL::btntParallel,
	btatNot             = LZPL::btntNot,
	btatDebug           = LZPL::btntDebug,
	btatRandom          = LZPL::btntRandom,
	btatRepeat          = LZPL::btntRepeat,

	btatBegin           = LZPL::btntUserDefine,
	btatReturnSuccess,
	btatReturnFail,

	btatTestAction,

	btatEnd
};



// tolua_end







#endif