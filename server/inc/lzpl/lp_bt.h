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
	LPINT16            type;
	LPINT16            childCount;
	BT_NODE*          pChild;
	BT_NODE*          pSibling;

	LPINT32            params[BT_MAX_PARAM];

	union
	{
		LPINT32        flag;
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
	LPUINT8                 debugging;
	LPINT16                 returnValue;
	LPUINT8                 runStackNodeCount;

	BT_RUN_NODE            nodeRunStack[BT_MAX_CALL_STACK_NODE];
	
};



DECLARE BOOL LPAPI lpBTAddChild(BT_NODE* pRootNode, BT_NODE* pChild);
DECLARE BOOL LPAPI lpBTRemoveChild(BT_NODE* pRootNode, BT_NODE* pChild);
DECLARE BOOL LPAPI lpBTInsertChildAtTag(BT_NODE* pParent, LPINT32 nTag, BT_NODE* pNewChild);

DECLARE BT_NODE* LPAPI lpBTGetChild(BT_NODE* pRootNode, LPINT32 nIndex);

DECLARE void LPAPI lpBTRunBtCtrl(BT_CTRL& ctrl);

typedef LPINT32 (*pfunBTUserDefinedNodeFunc)(BT_CTRL* pCtrl, BT_NODE* pNode);
DECLARE void lpBTRegisterUserDefinedNodeFunc(pfunBTUserDefinedNodeFunc pfNodeFunc);



//end声明所处的名字空间
NS_LZPL_END
#endif