//****************************************************************************/
//    author: caihy
//    date: October 20, 2014
//    description:
//
//****************************************************************************/
#ifndef _LPI_SOCKER_IMPL_H_
#define _LPI_SOCKER_IMPL_H_

#include "lpi_socker.h"
#include "lpi_packetparser.h"
#include "lpi_buf.h"
#include "lp_netdef.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//      �ײ�socket�����װ�ӿ���
class DECLARE ILPSockerImpl : public ILPSocker, public lp_enable_shared_from_this<ILPSockerImpl>
{
public:

    // Summary:
    //      ��
    virtual ~ILPSockerImpl() {}

    // Summary:
    //      ����socker����
    virtual void LPAPI Reset() = 0;

    // Summary:
    //      ����socket
    virtual void LPAPI SetSocket(lp_shared_ptr<ip::tcp::socket> pSocket) = 0;

    // Summary:
    //      ��������״̬���ֵ
    // Input:
    //      bConnect��true-���ӣ�false-�Ͽ�
    virtual void LPAPI SetConnect(bool bConnect) = 0;

    // Summary:
    //      ���ø��������id�����������������id��
    // Input:
    //      dwParentId��id
    virtual void LPAPI SetParentId(LPUINT32 dwParentId) = 0;

    // Summary:
    //      ��ȡ���������id�����������������id��
    // Return:
    //      id
    virtual LPUINT32 LPAPI GetParentId() = 0;

    // Summary:
    //      �����Ƿ񱻶��ر�
    virtual void LPAPI SetPassiveClose(BOOL bPassiveClose) = 0;

    // Summary:
    //      �����Ƿ���accept�����ı�ǣ�accept����connect����
    virtual void LPAPI SetAcceptCreate(BOOL bAcceptCreate) = 0;

    // Summary:
    //      �ж��Ƿ���accept����
    //  Return:
    //      TRUE-�ǣ�FALSE-����
    virtual BOOL LPAPI IsAcceptCreate() = 0;

    // Summary:
    //      �첽�رջص�
    virtual void LPAPI OnClose() = 0;

    // Summary:
    //      post�첽�������ݲ���
    virtual void LPAPI PostRecv() = 0;

    // Summary:
    //      post�첽�������ݲ���
    // Return:
    //      TRUE-�з������ݣ�FALSE-û��������
    virtual BOOL LPAPI PostSend() = 0;

    // Summary:
    //      �󶨽�������
    // Input:
    //      pPacketParser����������
    virtual void LPAPI AttachPacketParser(lp_shared_ptr<ILPPacketParser> pPacketParser) = 0;

    // Summary:
    //      ����󶨽�������
    virtual void LPAPI DetachPacketParser() = 0;

    // Summary:
    //      ���ý��ջ�����
    // Input:
    //      pBuf��������
    virtual void LPAPI AttachRecvBuf(lp_shared_ptr<ILPLoopBuf> pLoopBuf) = 0;

    // Summary:
    //      ������ջ�����
    // Return:
    //      ���ջ�����
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI DetachRecvBuf() = 0;

    // Summary:
    //      ���÷��ͻ�����
    // Input:
    //      pBuf��������
    virtual void LPAPI AttachSendBuf(lp_shared_ptr<ILPLoopBuf> pLoopBuf) = 0;

    // Summary:
    //      ���뷢�ͻ�����
    // Return:
    //      ���ͻ�����
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI DetachSendBuf() = 0;

    // Summary:
    //      ����socker id
    // Input:
    //      dwSockerId��socker id
    virtual void LPAPI SetSockerId(LPUINT32 dwSockerId) = 0;

    // Summary:
    //      ��
    virtual void LPAPI SetNetImpl(lp_shared_ptr<LPNetImpl> pNetImpl) = 0;

public:

    static lp_shared_ptr<ILPSockerImpl> LPAPI NewSockerImpl();
    static void LPAPI DeleteSockerImpl(lp_shared_ptr<ILPSockerImpl>& pSocker);
};



//end�������������ֿռ�
NS_LZPL_END

#endif