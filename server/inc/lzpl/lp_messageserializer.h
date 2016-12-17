//****************************************************************************/
//    author: caihy
//    date: October 21, 2015
//    description: 消息解析类定义
//
//****************************************************************************/
#ifndef _LP_MESSAGE_PARSER_H_
#define _LP_MESSAGE_PARSER_H_

#include "lpi_messageserializer.h"


#define MESSAGE_MAX_LEN               (MAX_PACKET_LEN)



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		消息解析类
class DECLARE LPExternalMessageSerializer : public ILPMessageSerializer
{
public:

	// Summary:
	//		无
	LPExternalMessageSerializer();

	// Summary:
	//		无
	virtual ~LPExternalMessageSerializer();

	// Summary:
	//		初始化
	// Input:
	//		pszSerializeBuf: 外部传入serialize buf（NULL则使用内部serialize buf）
	//		dwSerializeBufSize: 外部传入serialize buf大小（pszSerializeBuf为NULL时，此参数填0）
	//		pszUnSerializeBuf: 外部传入unserialize buf（NULL则使用内部unserialize buf）
	//		dwUnSerializeBufSize: 外部传入unserialize buf大小（pszUnSerializeBuf为NULL时，此参数填0）
	BOOL LPAPI Init(char* pszSerializeBuf, LPUINT32 dwSerializeBufSize, const char* pszUnSerializeBuf, LPUINT32 dwUnSerializeBufSize);

	// Summary:
	//		反始化
	BOOL LPAPI UnInit(void);

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint8(LPUINT8* pbyValue);

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint8(LPUINT8 byValue);

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint16(LPUINT16* pwValue);

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint16(LPUINT16 wValue);

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint32(LPUINT32* pdwValue);

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint32(LPUINT32 dwValue);

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint64(LPUINT64* pqwValue);

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint64(LPUINT64 qwValue);

	// Summary:
	//		无
	virtual BOOL LPAPI ReadString(char* pcszData);

	// Summary:
	//		无
	virtual BOOL LPAPI WriteString(const char* pcszData, LPUINT32 dwLen, LPUINT32 dwMaxLen);

	// Summary:
	//		无
	virtual char* LPAPI GetSerializeBuf(void);

	// Summary:
	//		无
	virtual LPUINT32 LPAPI GetSerializeSize(void);

private:

	char                     m_szSerializeBuf[MESSAGE_MAX_LEN];
	char                     m_szUnSerializeBuf[MESSAGE_MAX_LEN];

	LPUINT32                  m_dwSerializeSize;
	LPUINT32                  m_dwSerializeBufSize;
	char*                    m_pSerializeBuf;

	LPUINT32                  m_dwUnSerializeSize;
	LPUINT32                  m_dwUnSerializeBufSize;
	const char*              m_pUnSerializeBuf;
};













//end声明所处的名字空间
NS_LZPL_END

#endif