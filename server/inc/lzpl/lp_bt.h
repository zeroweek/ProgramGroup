//****************************************************************************/
//    author: caihy
//    date: November 12, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_BT_H_
#define _LP_BT_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define BT_MAX_CHILD_COUNT                 (64)
#define BT_MAX_CALL_STACK_NODE             (16)
#define BT_MAX_PARAM                       (4)
#define BT_MAX_REPEAT_TIMES                (32)
#define BT_MAX_LAYER                       (64)


// tolua_begin

enum eBTReturnValue
{
	btrvError = -1,

	btrvFail,
	btrvSuccess,
	btrvRunning,

	btrvTotal
};



enum eBTNodeType
{
	btntInvalid = 0,

	btntSelector,
	btntSequencer,
	btntParallel,
	btntNot,
	btntDebug,
	btntRandom,
	btntRepeat,

	btntUserDefine,

	btntTotal = 1024
};

// tolua_end



struct BT_NODE
{
	INT_16            type;
	INT_16            childCount;
	BT_NODE*          pChild;
	BT_NODE*          pSibling;

	INT_32            params[BT_MAX_PARAM];

	union
	{
		INT_32        flag;
		struct
		{
			unsigned     luaParamMask    : 8;
			unsigned     repeatCounter   : 8;
			unsigned     nodeTag         : 16;
		};
	};
};



struct BT_RUN_NODE
{
	BT_NODE*     pNode;
	BT_NODE*     pChild;
};



struct BT_CTRL
{
	UINT_8                 debugging;
	INT_16                 returnValue;
	UINT_8                 runStackNodeCount;

	BT_RUN_NODE            nodeRunStack[BT_MAX_CALL_STACK_NODE];
	
};



DECLARE BOOL LPAPI lpBTAddChild(BT_NODE* pRootNode, BT_NODE* pChild);
DECLARE BOOL LPAPI lpBTRemoveChild(BT_NODE* pRootNode, BT_NODE* pChild);
DECLARE BOOL LPAPI lpBTInsertChildAtTag(BT_NODE* pParent, INT_32 nTag, BT_NODE* pNewChild);

DECLARE BT_NODE* LPAPI lpBTGetChild(BT_NODE* pRootNode, INT_32 nIndex);

DECLARE void LPAPI lpBTRunBtCtrl(BT_CTRL& ctrl);

typedef INT_32 (*pfunBTUserDefinedNodeFunc)(BT_CTRL* pCtrl, BT_NODE* pNode);
DECLARE void lpBTRegisterUserDefinedNodeFunc(pfunBTUserDefinedNodeFunc pfNodeFunc);



//end声明所处的名字空间
NS_LZPL_END
#endif