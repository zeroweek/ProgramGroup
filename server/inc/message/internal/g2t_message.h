//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description:
//
//****************************************************************************/
#ifndef _LP_G2T_MESSAGE_H_
#define _LP_G2T_MESSAGE_H_

#include "internal_message_header.h"



//begin声明所处的名字空间
NS_INTERNAL_MESSAGE_BEGIN

#pragma pack(push, 1)


enum G2T_MESSAGE_ID
{
    g2t_begin = g2t_message_begin,

    g2t_register = g2t_begin,


    g2t_end = g2t_message_end
};


struct DECLARE G2T_REGISTER : public INTERNAL_MESSAGE_HEADER
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