//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_T2G_MESSAGE_H_
#define _LP_T2G_MESSAGE_H_

#include "internal_message_header.h"



//begin声明所处的名字空间
NS_INTERNAL_MESSAGE_BEGIN

#pragma pack(push, 1)


enum T2G_MESSAGE_ID
{
	t2g_begin = t2g_message_begin,

	t2g_register_ack = t2g_begin,


	t2g_end = t2g_message_end
};


struct DECLARE T2G_REGISTER_ACK : public INTERNAL_MESSAGE_HEADER
{
	LPUINT8     byValue;
	LPUINT16    wValue;
	LPUINT32    dwValue;
	LPUINT64    qwValue;
	char       szValue[MAX_PATH];
};




#pragma pack(pop)

//end声明所处的名字空间
NS_INTERNAL_MESSAGE_END

#endif