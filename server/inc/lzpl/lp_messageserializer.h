//****************************************************************************/
//    author: caihy
//    date: October 21, 2015
//    description: ��Ϣ�����ඨ��
//
//****************************************************************************/
#ifndef _LP_MESSAGE_PARSER_H_
#define _LP_MESSAGE_PARSER_H_

#include "lpi_messageserializer.h"


#define MESSAGE_MAX_LEN               (MAX_PACKET_LEN)



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		��Ϣ������
class DECLARE LPExternalMessageSerializer : public ILPMessageSerializer
{
public:

	// Summary:
	//		��
	LPExternalMessageSerializer();

	// Summary:
	//		��
	virtual ~LPExternalMessageSerializer();

	// Summary:
	//		��ʼ��
	// Input:
	//		pszSerializeBuf: �ⲿ����serialize buf��NULL��ʹ���ڲ�serialize buf��
	//		dwSerializeBufSize: �ⲿ����serialize buf��С��pszSerializeBufΪNULLʱ���˲�����0��
	//		pszUnSerializeBuf: �ⲿ����unserialize buf��NULL��ʹ���ڲ�unserialize buf��
	//		dwUnSerializeBufSize: �ⲿ����unserialize buf��С��pszUnSerializeBufΪNULLʱ���˲�����0��
	BOOL LPAPI Init(char* pszSerializeBuf, UINT_32 dwSerializeBufSize, const char* pszUnSerializeBuf, UINT_32 dwUnSerializeBufSize);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint8(UINT_8* pbyValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint8(UINT_8 byValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint16(UINT_16* pwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint16(UINT_16 wValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint32(UINT_32* pdwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint32(UINT_32 dwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint64(UINT_64* pqwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint64(UINT_64 qwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadString(char* pcszData);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteString(const char* pcszData, UINT_32 dwLen, UINT_32 dwMaxLen);

	// Summary:
	//		��
	virtual char* LPAPI GetSerializeBuf(void);

	// Summary:
	//		��
	virtual UINT_32 LPAPI GetSerializeSize(void);

private:

	char                     m_szSerializeBuf[MESSAGE_MAX_LEN];
	char                     m_szUnSerializeBuf[MESSAGE_MAX_LEN];

	UINT_32                  m_dwSerializeSize;
	UINT_32                  m_dwSerializeBufSize;
	char*                    m_pSerializeBuf;

	UINT_32                  m_dwUnSerializeSize;
	UINT_32                  m_dwUnSerializeBufSize;
	const char*              m_pUnSerializeBuf;
};













//end�������������ֿռ�
NS_LZPL_END

#endif