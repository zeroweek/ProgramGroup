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
    virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);

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
    virtual void LPAPI Stop();

    // Summary:
    //      ��ȡid
    virtual LPUINT32 LPAPI GetId();

    // Summary:
    //      ����
    virtual BOOL LPAPI Reconnect();

    // Summary:
    //      ��ȡ���
    virtual HANDLE LPAPI GetHandle();

    // Summary:
    //      ��ȡ�����¼�����������
    virtual e_EventHandlerType LPAPI GetEventHandlerType();

    // Summary:
    //      ��ȡ�������Զ�������ݣ����û���򷵻�null
    virtual PER_IO_DATA* LPAPI GetEventHandlerData();

    // Summary:
    //      ���ӷ��ؽ���ص�
    virtual void LPAPI OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData) = 0;

    // Summary:
    //      ���ӹرջص�
    virtual void LPAPI OnClose();

    // Summary:
    //      �����Ƿ��Զ�����
    virtual void LPAPI SetReconnect(BOOL bReconnect);

    // Summary:
    //      �Ƿ��Զ�����
    virtual BOOL LPAPI IsReconnect();

    // Summary:
    //      ���ù�����socker����
    virtual void LPAPI SetSocker(ILPSockerImpl* pSocker);

    // Summary:
    //      ��ȡ������socker����
    virtual ILPSockerImpl* LPAPI GetSocker();

    // Summary:
    //      ������������ǰ״̬
    virtual void LPAPI SetState(LPUINT32 dwState);

    //Summary:
    //      ��ȡ��������ǰ״̬
    virtual LPUINT32 LPAPI GetState();

protected:

    // Summary:
    //      �ر�socket
    void LPAPI CloseConnectSock();

    // Summary:
    //      ��ȡConnectEx����ָ�룬���ҵ����첽connect����
    virtual BOOL LPAPI InitConnectEx() = 0;

    // Summary:
    //      post�첽connect����
    // Input:
    //      stPerIoData�������ӽ�����sock�󶨵�io����
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI PostConnectEx(PER_IO_DATA& stPerIoData) = 0;

protected:

    LPUINT32                    m_dwId;
    volatile atomic_uint        m_dwState;

    std::string                 m_strIP;
    LPUINT32                    m_dwPort;
    BOOL                        m_bReconnect;
    SOCKET                      m_hConnectSock;
    ILPPacketParser*            m_pPacketParser;
    LPNetImpl*                  m_pNetImpl;
    ILPSockerImpl*              m_pSocker;

    PER_IO_DATA                 m_stPerIoData;
};



// Summary:
//      windows����ͨѶ��ʽ��connectorʵ����
class DECLARE LPWinNetConnector : public LPConnector
{
public:

    // Summary:
    //      ��
    LPWinNetConnector();

    // Summary:
    //      ��
    virtual ~LPWinNetConnector();

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
    //      �����¼�����
    virtual void LPAPI OnNetEvent(PER_IO_DATA* pstPerIoData);

    // Summary:
    //      ���ӷ��ؽ���ص�
    virtual void LPAPI OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

protected:

    // Summary:
    //      ��ȡConnectEx����ָ�룬���ҵ����첽connect����
    virtual BOOL LPAPI InitConnectEx();

    // Summary:
    //      post�첽connect����
    // Input:
    //      stPerIoData�������ӽ�����sock�󶨵�io����
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI PostConnectEx(PER_IO_DATA& stPerIoData);

private:
    LPFN_CONNECTEX              m_lpfnConnectEx;
};



// Summary:
//      linux����ͨѶ��ʽ��connectorʵ����
class DECLARE LPLinuxNetConnector : public LPConnector
{
public:

    // Summary:
    //      ��
    LPLinuxNetConnector();

    // Summary:
    //      ��
    virtual ~LPLinuxNetConnector();

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
    //      �����¼�����
    virtual void LPAPI OnNetEvent(PER_IO_DATA* pstPerIoData);

    // Summary:
    //      ���ӷ��ؽ���ص�
    virtual void LPAPI OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

protected:

    // Summary:
    //      ��ȡConnectEx����ָ�룬���ҵ����첽connect����
    virtual BOOL LPAPI InitConnectEx();

    // Summary:
    //      post�첽connect����
    // Input:
    //      stPerIoData�������ӽ�����sock�󶨵�io����
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI PostConnectEx(PER_IO_DATA& stPerIoData);

private:
};



//end�������������ֿռ�
NS_LZPL_END

#endif