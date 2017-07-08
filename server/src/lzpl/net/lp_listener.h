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
    virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);

    // Summary��
    //      ��
    virtual BOOL LPAPI UnInit();

    // Summary:
    //      ֹͣ����
    virtual void LPAPI Stop();

    // Summary:
    //      ��ȡid
    virtual LPUINT32 LPAPI GetId();

    // Summary:
    //      ��ȡ���
    virtual HANDLE LPAPI GetHandle();

    // Summary:
    //      ��ȡ�����¼�����������
    virtual e_EventHandlerType LPAPI GetEventHandlerType();

    // Summary:
    //      ��ȡ�������Զ�������ݣ����û���򷵻�null
    virtual PER_IO_DATA* LPAPI GetEventHandlerData();

protected:

    //Summary:
    //      ��ȡ������״̬
    virtual LPUINT32 LPAPI GetState();

    // Summary:
    //      ���ü�����״̬
    virtual void LPAPI SetState(LPUINT32 dwState);

protected:

    LPUINT32                    m_dwId;
    volatile atomic_uint        m_dwState;

    std::string                 m_strIP;
    LPUINT32                    m_dwPort;
    BOOL                        m_bReUseAddr;
    SOCKET                      m_hListenSock;
    ILPPacketParser*            m_pPacketParser;
    LPNetImpl*                  m_pNetImpl;

    PER_IO_DATA                 m_stPerIoData;//Ŀǰֻ��linux�õ�

};



// Summary:
//      windows����ͨѶ��ʽ��listenerʵ����
class DECLARE LPWinNetListener : public LPListener
{
public:

    // Summary:
    //      ��
    LPWinNetListener();

    // Summary:
    //      ��
    virtual ~LPWinNetListener();

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
    //      �����¼�����
    virtual void LPAPI OnNetEvent(PER_IO_DATA* pstPerIoData);

private:

    // Summary:
    //      �������ӻص�
    void LPAPI OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

    // Summary:
    //      ��ȡAcceptEx��GetAcceptExSockaddrs����ָ�룬����post�첽accept����
    BOOL LPAPI InitAcceptEx();

    // Summary:
    //      post�첽accept����
    // Input:
    //      pstPerIoData�������������sock�󶨵�io����
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    BOOL LPAPI PostAcceptEx(PER_IO_DATA* pstPerIoData);

private:

    LPFN_ACCEPTEX               m_lpfnAcceptEx;
    LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
    PER_IO_DATA*                m_pstPerIoDataArray;

};



// Summary:
//      linux����ͨѶ��ʽ��listenerʵ����
class DECLARE LPLinuxNetListener : public LPListener
{
public:

    // Summary:
    //      ��
    LPLinuxNetListener();

    // Summary:
    //      ��
    virtual ~LPLinuxNetListener();

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
    //      �����¼�����
    virtual void LPAPI OnNetEvent(PER_IO_DATA* pstPerIoData);

private:

    // Summary:
    //      �������ӻص�
    void LPAPI OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

};



//end�������������ֿռ�
NS_LZPL_END

#endif