//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description: 
//
//****************************************************************************/
#ifndef _GT_INTERNAL_PACKET_PARSER_H_
#define _GT_INTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "internal_message_header.h"



using namespace INTERNAL_MESSAGE;



// Summary:
//		
class DECLARE CGTInternalPacketParser : public ILPPacketParser
{
public:

	// Summary:
	//		��
	CGTInternalPacketParser();

	// Summary:
	//		��
	~CGTInternalPacketParser();
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

	UINT_32        m_dwRef;
};

extern INT_32  g_MessageSize[max_internal_message_count];






#endif