//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description: 
//
//****************************************************************************/
#ifndef _GS_EXTERNAL_PACKET_PARSER_H_
#define _GS_EXTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "external_message_header.h"


using namespace EXTERNAL_MESSAGE;



// Summary:
//		
class DECLARE CGSExternalPacketParser : public ILPPacketParser
{
public:

	// Summary:
	//		��
	CGSExternalPacketParser();

	// Summary:
	//		��
	~CGSExternalPacketParser();
	// Summary:
	//		���ü�����1
	virtual void LPAPI AddRef(void);

	// Summary:
	//		���ü�����1
	virtual UINT_32 LPAPI QueryRef(void);

	// Summary:
	//		�ͷŶ���
	virtual void LPAPI Release(void);

	// Summary:
	//		���
	// Input:
	//		pLoopBuf�����������ݻ�����
	// Return:
	//		"<0"-��������"=0"-����������">0"-����������
	virtual INT_32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

private:

	UINT_32                       m_dwRef;
	EXTERNAL_MESSAGE_HEADER       m_stMessageHead;
};






#endif