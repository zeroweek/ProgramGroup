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
//      ����������ӿ���
class DECLARE ILPPacketParser
{
public:

    // Summary:
    //      ��
    virtual ~ILPPacketParser() {}

    // Summary:
    //      ���ü�����1
    virtual void LPAPI AddRef(void) = 0;

    // Summary:
    //      ���ü�����1
    virtual LPUINT32 LPAPI QueryRef(void) = 0;

    // Summary:
    //      �ͷŶ���
    virtual void LPAPI Release(void) = 0;

    // Summary:
    //      ���
    // Input:
    //      pLoopBuf�����������ݻ�����
    // Return:
    //      "<0"-��������"=0"-����������">0"-����������
    virtual LPINT32 LPAPI Parse(ILPLoopBuf* pLoopBuf) = 0;
};




//end�������������ֿռ�
NS_LZPL_END

#endif