//****************************************************************************/
//    author: caihy
//    date: December 1, 2015
//    description:
//
//****************************************************************************/
#ifndef _BT_EVENT_H_
#define _BT_EVENT_H_

#include "lp_bt.h"


#define LOAD_BT_TREE             "load_bt_tree"



enum eBTEventType
{
    btetInvalid,

    // ai event
    btetAIBegin,

    btetAIInit = btetAIBegin,
    btetAIIdle,
    btetAIFight,
    btetAIDie,
    btetAIRevive,
    btetAIInteract,

    btetAIEnd,

    btetTotal = btetAIEnd,

};

extern char const* const g_BTEventName[btetTotal];

#endif