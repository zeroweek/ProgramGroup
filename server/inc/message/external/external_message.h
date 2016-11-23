//****************************************************************************/
//    author: caihy
//    date: February 17, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LP_EXTERNAL_MESSAGE_H_
#define _LP_EXTERNAL_MESSAGE_H_

#include "external_message_header.h"




//begin�������������ֿռ�
NS_EXTERNAL_MESSAGE_BEGIN

#pragma pack(push, 1)


enum C2T_MESSAGE_ID
{
	c2t_begin = c2t_message_begin,

	c2t_login_req = c2t_begin,


	c2t_end = c2t_message_end
};


enum T2C_MESSAGE_ID
{
	t2c_begin = t2c_message_begin,

	t2c_login_ack = t2c_begin,


	t2c_end = t2c_message_end
};


struct DECLARE C2T_LOGIN_REQ : public EXTERNAL_MESSAGE_HEADER
{
	UINT_8     byValue;
	UINT_16    wValue;
	UINT_32    dwValue;
	UINT_64    qwValue;
	char       szValue[MAX_PATH];

public:

	virtual BOOL LPAPI Serialize(ILPMessageSerializer* pMessageSerializer);
	virtual BOOL LPAPI UnSerialize(ILPMessageSerializer* pMessageSerializer);
};


struct DECLARE T2C_LOGIN_ACK : public EXTERNAL_MESSAGE_HEADER
{
	UINT_8     byValue;
	UINT_16    wValue;
	UINT_32    dwValue;
	UINT_64    qwValue;
	char       szValue[MAX_PATH];

	virtual BOOL LPAPI Serialize(ILPMessageSerializer* pMessageSerializer);
	virtual BOOL LPAPI UnSerialize(ILPMessageSerializer* pMessageSerializer);
};










#pragma pack(pop)

//end�������������ֿռ�
NS_EXTERNAL_MESSAGE_END

#endif