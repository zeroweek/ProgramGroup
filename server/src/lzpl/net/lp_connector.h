//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_CONNECTOR_H_
#define _LP_CONNECTOR_H_

#include "lpi_connectorimpl.h"
#include "lpi_sockerimpl.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      ��������
class DECLARE LPConnector : public ILPConnectorImpl
{
public:

    // Summary:
    //      ��
    LPConnector();

    // Summary:
    //      ��
    virtual ~LPConnector();

    // Summary��
    //      ��
    // Input:
    //      pNetImpl:
    //      pPacketParser: ��Ϣ����������
    //      dwId: ������id
    virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId);

    // Summary��
    //      ��
    virtual BOOL LPAPI UnInit();

    // Summary:
    //      ��ʼ����
    // Input:
    //      strIP: ip��ַ�ַ�������ʽ��192.168.1.1��
    //      dwPort: ���ӵĶ˿�
    //      bAutoReconnect: �Ƿ��Զ�������
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect);

    // Summary:
    //      ֹͣ����
    virtual void LPAPI Close();

    // Summary:
    //      ��ȡid
    virtual LPUINT32 LPAPI GetId();

    // Summary:
    //      ����
    virtual BOOL LPAPI Reconnect();

    // Summary:
    //      ���ӹرջص�
    virtual void LPAPI OnConnectionClose();

    // Summary:
    //      �����Ƿ��Զ�����
    virtual void LPAPI SetReconnect(BOOL bReconnect);

    // Summary:
    //      �Ƿ��Զ�����
    virtual BOOL LPAPI IsReconnect();

    // Summary:
    //      ���ù�����socker����
    virtual void LPAPI SetSocker(lp_shared_ptr<ILPSockerImpl> pSocker);

    // Summary:
    //      ��ȡ������socker����
    virtual lp_shared_ptr<ILPSockerImpl> LPAPI GetSocker();

    // Summary:
    //      ������������ǰ״̬
    virtual void LPAPI SetState(LPUINT32 dwState);

    //Summary:
    //      ��ȡ��������ǰ״̬
    virtual LPUINT32 LPAPI GetState();

protected:

    // Summary:
    //   �ر�socket����������£���Ӧ�õ�������ӿڣ�����ͳһ��LPSocker��ִ�йر�
    //   ��������������connect״̬����ʱ����
    void LPAPI CloseConnectSockWhileStateError();

    // Summary:
    //      post�첽connect����
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI PostConnect();

    // Summary:
    //      ���ü�����״̬
    virtual void LPAPI HandleConnect(const system::error_code& err);

protected:

    LPUINT32                            m_dwId;
    volatile atomic_uint                m_dwState;

    std::string                         m_strIP;
    LPUINT32                            m_dwPort;
    BOOL                                m_bReconnect;
    lp_shared_ptr<ILPPacketParser>      m_pPacketParser;
    LPNetImpl*                          m_pNetImpl;
    lp_shared_ptr<ILPSockerImpl>        m_pSocker;

    lp_shared_ptr<ILPReactor>           m_pReactor;
    lp_shared_ptr<ip::tcp::socket>      m_pSocket;
};



//end�������������ֿռ�
NS_LZPL_END

#endif