//****************************************************************************/
//    author: caihy
//    date: October 21, 2015
//    description: ��Ϣ�����ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_MESSAGE_PARSER_H_
#define _LPI_MESSAGE_PARSER_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		��Ϣ���ӿ���
class DECLARE ILPMessageSerializer
{
public:

	// Summary:
	//		��
	virtual ~ILPMessageSerializer() {}

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint8(UINT_8* pbyValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint8(UINT_8 byValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint16(UINT_16* pwValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint16(UINT_16 wValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint32(UINT_32* pdwValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint32(UINT_32 dwValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint64(UINT_64* pqwValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint64(UINT_64 qwValue) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI ReadString(char* pcszData) = 0;

	// Summary:
	//		��
	virtual BOOL LPAPI WriteString(const char* pcszData, UINT_32 dwLen, UINT_32 dwMaxLen) = 0;

	// Summary:
	//		��
	virtual char* LPAPI GetSerializeBuf(void) = 0;

	// Summary:
	//		��
	virtual UINT_32 LPAPI GetSerializeSize(void) = 0;

};








//end�������������ֿռ�
NS_LZPL_END

#endif