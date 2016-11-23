//****************************************************************************/
//    author: caihy
//    date: December 14, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_EXTERNAL_MESSAGE_HEADER_H_
#define _LP_EXTERNAL_MESSAGE_HEADER_H_

#include "lp_base.h"
#include "lpi_messageserializer.h"



//命名空间宏定义: INTERNAL_MESSAGE 
#ifndef NS_EXTERNAL_MESSAGE_BEGIN
#define NS_EXTERNAL_MESSAGE_BEGIN                namespace EXTERNAL_MESSAGE { 
#endif
#ifndef NS_EXTERNAL_MESSAGE_END      
#define NS_EXTERNAL_MESSAGE_END                  }
#endif


//begin声明所处的名字空间
NS_EXTERNAL_MESSAGE_BEGIN


#define MESSAGE_SERIALIZE_BEGIN(_class) \
	BOOL LPAPI _class::Serialize(ILPMessageSerializer * pMessageSerializer)\
	{\
		INT_32 nResult = 0;\
		UINT_16 wSize = 0;\
		char* pBuf = NULL;\
		LOG_PROCESS_ERROR(pMessageSerializer);

#define MESSAGE_PARENT_SERIALIZE(_parent_class) \
		nResult = _parent_class::Serialize(pMessageSerializer);\
		LOG_PROCESS_ERROR(nResult);

#define MESSAGE_WRITE_NUM_VALUE(_type, _num) \
		nResult = pMessageSerializer->Write##_type(_num);\
		LOG_PROCESS_ERROR(nResult);

#define MESSAGE_WRITE_STRING_VALUE(_str) \
		nResult = pMessageSerializer->WriteString(_str, (UINT_32)strlen(_str), MAX_PATH);\
		LOG_PROCESS_ERROR(nResult);

#define MESSAGE_SERIALIZE_END\
		pBuf = pMessageSerializer->GetSerializeBuf();\
		LOG_PROCESS_ERROR(pBuf);\
		wSize = pMessageSerializer->GetSerializeSize();\
		LOG_PROCESS_ERROR(wSize >= 4);\
		*(UINT_8*)(pBuf + 2) = wSize >> 8;\
		*(UINT_8*)(pBuf + 3) = (wSize << 8) >> 8;\
		return TRUE;\
		Exit0:\
		return FALSE;\
	}

#define MESSAGE_UNSERIALIZE_BEGIN(_class) \
	BOOL LPAPI _class::UnSerialize(ILPMessageSerializer * pMessageSerializer)\
	{\
		INT_32 nResult = 0;\
		LOG_PROCESS_ERROR(pMessageSerializer);

#define MESSAGE_PARENT_UNSERIALIZE(_parent_class)\
		nResult = _parent_class::UnSerialize(pMessageSerializer);\
		LOG_PROCESS_ERROR(nResult);

#define MESSAGE_READ_NUM_VALUE(_type, _num) \
		nResult = pMessageSerializer->Read##_type(&_num);\
		LOG_PROCESS_ERROR(nResult);

#define MESSAGE_READ_STRING_VALUE(_str) \
		nResult = pMessageSerializer->ReadString(_str);\
		LOG_PROCESS_ERROR(nResult);

#define MESSAGE_UNSERIALIZE_END \
		return TRUE;\
	Exit0:\
		return FALSE;\
	}

#pragma pack(push, 1)



// Summary:
//		消息接口类
struct DECLARE EXTERNAL_MESSAGE_HEADER
{
	UINT_16   wMsgId;       // 消息id
	UINT_16   wMsgSize;     // 

public:

	virtual BOOL LPAPI Serialize(ILPMessageSerializer* pMessageSerializer);
	virtual BOOL LPAPI UnSerialize(ILPMessageSerializer* pMessageSerializer);

};


enum
{
	max_external_message_count = 1024,
};

enum
{
	min_external_message = 0,

	c2t_message_begin = 1,
	c2t_message_end   = 100,
	t2c_message_begin = 101,
	t2c_message_end   = 200,
	c2g_message_begin = 201,
	c2g_message_end   = 400,
	g2c_message_begin = 401,
	g2c_message_end   = 600,
	c2m_message_begin = 601,
	c2m_message_end   = 800,
	m2c_message_begin = 801,
	m2c_message_end   = 1000,

	max_external_message
};







#pragma pack(pop)

//end声明所处的名字空间
NS_EXTERNAL_MESSAGE_END

#endif