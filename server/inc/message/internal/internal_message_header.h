//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_INTERNAL_MESSAGE_HEADER_H_
#define _LP_INTERNAL_MESSAGE_HEADER_H_

#include "lp_base.h"



//命名空间宏定义: INTERNAL_MESSAGE 
#ifndef NS_INTERNAL_MESSAGE_BEGIN
#define NS_INTERNAL_MESSAGE_BEGIN                namespace INTERNAL_MESSAGE { 
#endif
#ifndef NS_INTERNAL_MESSAGE_END      
#define NS_INTERNAL_MESSAGE_END                  }
#endif


//begin声明所处的名字空间
NS_INTERNAL_MESSAGE_BEGIN



#define CONVERT_MSG_ID_ENDIAN(__msg_id)          __msg_id = (__msg_id >> 8 | __msg_id << 8)



#pragma pack(push, 1)

struct INTERNAL_MESSAGE_HEADER
{
	UINT_16 wMsgId;
};

enum
{
	max_internal_message_count = 65535,
};

enum
{
	min_internal_message  = 2000,
	
	u2t_message_begin     = min_internal_message +     1,
	u2t_message_end       = min_internal_message +  1000,
	t2u_message_begin     = min_internal_message +  1001,
	t2u_message_end       = min_internal_message +  2000,
	u2g_message_begin     = min_internal_message +  2001,
	u2g_message_end       = min_internal_message +  3000,
	g2u_message_begin     = min_internal_message +  3001,
	g2u_message_end       = min_internal_message +  4000,
	t2g_message_begin     = min_internal_message +  4001,
	t2g_message_end       = min_internal_message +  5001,
	g2t_message_begin     = min_internal_message +  5001,
	g2t_message_end       = min_internal_message +  6000,

	max_internal_message
};


#pragma pack(pop)

//end声明所处的名字空间
NS_INTERNAL_MESSAGE_END

#endif