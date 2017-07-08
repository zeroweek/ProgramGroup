//****************************************************************************/
//    author: caihy
//    date: February 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _GS_INTERNAL_PACKET_PARSER_H_
#define _GS_INTERNAL_PACKET_PARSER_H_

#include "lpi_packetparser.h"
#include "internal_message_header.h"



using namespace INTERNAL_MESSAGE;



// Summary:
//
class DECLARE CGSInternalPacketParser : public ILPPacketParser
{
public:

    // Summary:
    //      ��
    CGSInternalPacketParser();

    // Summary:
    //      ��
    ~CGSInternalPacketParser();
    // Summary:
    //      ���ü�����1
    virtual void LPAPI AddRef(void);

    // Summary:
    //      ���ü�����1
    virtual LPUINT32 LPAPI QueryRef(void);

    // Summary:
    //      �ͷŶ���
    virtual void LPAPI Release(void);

    // Summary:
    //      ���
    // Input:
    //      pLoopBuf�����������ݻ�����
    // Return:
    //      "<0"-��������"=0"-����������">0"-����������
    virtual LPINT32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

private:

    LPUINT32        m_dwRef;
};

extern LPINT32  g_MessageSize[max_internal_message_count];






#endif