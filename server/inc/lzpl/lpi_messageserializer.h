//****************************************************************************/
//    author: caihy
//    date: October 21, 2015
//    description: 消息解析接口定义
//
//****************************************************************************/
#ifndef _LPI_MESSAGE_PARSER_H_
#define _LPI_MESSAGE_PARSER_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		消息包接口类
class DECLARE ILPMessageSerializer
{
public:

	// Summary:
	//		无
	virtual ~ILPMessageSerializer() {}

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint8(UINT_8* pbyValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint8(UINT_8 byValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint16(UINT_16* pwValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint16(UINT_16 wValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint32(UINT_32* pdwValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint32(UINT_32 dwValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI ReadUint64(UINT_64* pqwValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI WriteUint64(UINT_64 qwValue) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI ReadString(char* pcszData) = 0;

	// Summary:
	//		无
	virtual BOOL LPAPI WriteString(const char* pcszData, UINT_32 dwLen, UINT_32 dwMaxLen) = 0;

	// Summary:
	//		无
	virtual char* LPAPI GetSerializeBuf(void) = 0;

	// Summary:
	//		无
	virtual UINT_32 LPAPI GetSerializeSize(void) = 0;

};








//end声明所处的名字空间
NS_LZPL_END

#endif