//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_LISTENER_H_
#define _LP_LISTENER_H_

#include "lpi_listenerimpl.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



#define LISTEN_QUEUE_COUNT             (128)



//������
class LPNetImpl;



// Summary:
//      ��������
class DECLARE LPListener : public ILPListenerImpl
{
public:

    // Summary:
    //      ��
    LPListener();

    // Summary:
    //      ��
    virtual ~LPListener();

    // Summary��
    //      ��
    // Input:
    //      pNetImpl:
    //      pPacketParser: ��Ϣ����������
    //      dwId: ������id
    virtual BOOL LPAPI Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId);

    // Summary��
    //      ��
    virtual BOOL LPAPI UnInit();

    // Summary:
    //      ��ʼ����
    // Input:
    //      strIP: ip��ַ�ַ�������ʽ��192.168.1.1��"0"��ʾ�����ַ��
    //      dwPort: �����˿�
    //      bReUseAddr: �Ƿ��ظ����õ�ַ
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr);

    // Summary:
    //      ֹͣ����
    virtual void LPAPI Close();

    // Summary:
    //      ��ȡid
    virtual LPUINT32 LPAPI GetId();

    // Summary:
    //      ��ȡip�ַ���
    virtual std::string& LPAPI GetIp();

    // Summary:
    //      ��ȡport
    virtual LPUINT16 LPAPI GetPort();

protected:

    //Summary:
    //      ��ȡ������״̬
    virtual LPUINT32 LPAPI GetState();

    // Summary:
    //      ���ü�����״̬
    virtual void LPAPI SetState(LPUINT32 dwState);

    // Summary:
    //      post�첽accept����
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI PostAccept();

    // Summary:
    //      ���ü�����״̬
    virtual void LPAPI HandleAccept(lp_shared_ptr<ip::tcp::socket> pSocket, const system::error_code& err);

protected:

    LPUINT32                            m_dwId;
    volatile atomic_uint                m_dwState;

    std::string                         m_strIP;
    LPUINT32                            m_dwPort;
    BOOL                                m_bReUseAddr;

    lp_shared_ptr<ILPPacketParser>      m_pPacketParser;
    lp_shared_ptr<LPNetImpl>            m_pNetImpl;
    lp_shared_ptr<ip::tcp::acceptor>    m_pAcceptor;
    lp_shared_ptr<ILPReactor>           m_pReactor;
};



//end�������������ֿռ�
NS_LZPL_END

#endif