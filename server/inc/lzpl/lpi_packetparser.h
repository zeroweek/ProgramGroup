//****************************************************************************/
//    author: caihy
//    date: October 16, 2014
//    description: ����������ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_PACKET_PARSER_H_
#define _LPI_PACKET_PARSER_H_

#include "lpi_buf.h"

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		����������ӿ���
class DECLARE ILPPacketParser : public ILPObject
{
public:

	// Summary:
	//		��
	virtual ~ILPPacketParser(){}

	// Summary:
	//		���
	// Input:
	//		pLoopBuf�����������ݻ�����
	// Return:
	//		"<0"-��������"=0"-����������">0"-����������
	virtual INT_32 LPAPI Parse(ILPLoopBuf* pLoopBuf) = 0;
};




//end�������������ֿռ�
NS_LZPL_END

#endif