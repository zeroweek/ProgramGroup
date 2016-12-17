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
	BOOL LPAPI Init(char* pszSerializeBuf, LPUINT32 dwSerializeBufSize, const char* pszUnSerializeBuf, LPUINT32 dwUnSerializeBufSize);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint8(LPUINT8* pbyValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint8(LPUINT8 byValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint16(LPUINT16* pwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint16(LPUINT16 wValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint32(LPUINT32* pdwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint32(LPUINT32 dwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadUint64(LPUINT64* pqwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteUint64(LPUINT64 qwValue);

	// Summary:
	//		��
	virtual BOOL LPAPI ReadString(char* pcszData);

	// Summary:
	//		��
	virtual BOOL LPAPI WriteString(const char* pcszData, LPUINT32 dwLen, LPUINT32 dwMaxLen);

	// Summary:
	//		��
	virtual char* LPAPI GetSerializeBuf(void);

	// Summary:
	//		��
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













//end�������������ֿռ�
NS_LZPL_END

#endif