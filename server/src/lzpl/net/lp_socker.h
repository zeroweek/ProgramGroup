//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_SOCKER_H_
#define _LP_SOCKER_H_

#include "lpi_sockerimpl.h"
#include "lp_lock.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      �ײ�socke����t��װ��
class DECLARE LPSocker : public ILPSockerImpl
{
public:

    // Summary:
    //      ��
    LPSocker();

    // Summary:
    //      ��
    virtual ~LPSocker();

    // Summary:
    //      ����socker����
    virtual void LPAPI Reset();

    // Summary:
    //      ����sock���
    // Input:
    //      sock���ײ�socket���ֵ
    virtual void LPAPI SetSock(SOCKET sock);

    // Summary:
    //      ����socket
    virtual void LPAPI SetSocket(lp_shared_ptr<ip::tcp::socket> pSocket);

    // Summary:
    //      ��ȡ�ײ�sock�ľ��
    // Return:
    //      ���صײ�sock���
    virtual SOCKET LPAPI GetSock();

    // Summary:
    //      ����socker id
    // Input:
    //      dwSockerId��socker id
    virtual void LPAPI SetSockerId(LPUINT32 dwSockerId);

    // Summary:
    //      ��ȡsocker id
    // Return:
    //      ����socker id
    virtual LPUINT32 LPAPI GetSockerId();

    // Summary:
    //      ����Զ������ip
    // Input:
    //      dwIp��ip
    virtual void LPAPI SetRemoteIp(LPUINT32 dwIp);

    // Summary:
    //      ��ȡ�Զ����ӵ�ip
    virtual LPUINT32 LPAPI GetRemoteIp();

    // Summary:
    //      ��ȡ�Զ����ӵ�ip�ַ���
    virtual std::string& LPAPI GetRemoteIpStr();

    // Summary:
    //      ����Զ�����Ӷ˿�
    // Input:
    //      wPort���˿�
    virtual void LPAPI SetRemotePort(LPUINT16 wPort);

    // Summary:
    //      ��ȡ�Զ����ӵ�port
    virtual LPUINT16 LPAPI GetRemotePort();

    // Summary:
    //      ���ñ�������ip
    // Input:
    //      dwIp��ip
    virtual void LPAPI SetLocalIp(LPUINT32 dwIp);

    // Summary:
    //      ��ȡ�������ӵ�ip
    virtual LPUINT32 LPAPI GetLocalIp();

    // Summary:
    //      ��ȡ�������ӵ�ip�ַ���
    virtual std::string& LPAPI GetLocalIpStr();

    // Summary:
    //      ���ñ������Ӷ˿�
    // Input:
    //      wPort���˿�
    virtual void LPAPI SetLocalPort(LPUINT16 wPort);

    // Summary:
    //      ��ȡ�������ӵ�port
    virtual LPUINT16 LPAPI GetLocalPort();

    // Summary:
    //      ��������״̬���ֵ
    // Input:
    //      bConnect��true-���ӣ�false-�Ͽ�
    virtual void LPAPI SetConnect(bool bConnect);

    // Summary:
    //      �жϵ�ǰ��socket�Ƿ�������״̬
    // Return:
    //      TRUE-���ӣ�FALSE-�Ͽ�
    virtual BOOL LPAPI IsConnect();

    // Summary:
    //      �����Ƿ񱻶��ر�
    virtual void LPAPI SetPassiveClose(BOOL bPassiveClose);

    // Summary:
    //      �ж��Ƿ񱻶��ر�
    //  Return:
    //      TRUE-�ǣ�FALSE-����
    virtual BOOL LPAPI IsPassiveClose();

    // Summary:
    //      ���ø��������id�����������������id��
    // Input:
    //      dwParentId��id
    virtual void LPAPI SetParentId(LPUINT32 dwParentId);

    // Summary:
    //      ��ȡ���������id�����������������id��
    // Return:
    //      id
    virtual LPUINT32 LPAPI GetParentId();

    // Summary:
    //      �����Ƿ���accept�����ı�ǣ�accept����connect����
    virtual void LPAPI SetAcceptCreate(BOOL bAcceptCreate);

    // Summary:
    //      �ж��Ƿ���accept����
    //  Return:
    //      TRUE-�ǣ�FALSE-����
    virtual BOOL LPAPI IsAcceptCreate();

    // Summary:
    //      �󶨽�������
    // Input:
    //      pPacketParser����������
    virtual void LPAPI AttachPacketParser(lp_shared_ptr<ILPPacketParser> pPacketParser);

    // Summary:
    //      ����󶨽�������
    virtual void LPAPI DetachPacketParser();

    // Summary:
    //      ���ý��ջ�����
    // Input:
    //      pBuf��������
    virtual void LPAPI AttachRecvBuf(LPLoopBuf* pLoopBuf);

    // Summary:
    //      ������ջ�����
    // Return:
    //      ���ջ�����
    virtual LPLoopBuf* LPAPI DetachRecvBuf();

    // Summary:
    //      ���÷��ͻ�����
    // Input:
    //      pBuf��������
    virtual void LPAPI AttachSendBuf(LPLoopBuf* pLoopBuf);

    // Summary:
    //      ���뷢�ͻ�����
    // Return:
    //      ���ͻ�����
    virtual LPLoopBuf* LPAPI DetachSendBuf();

    // Summary:
    //      ���ÿ�ʼ�ӳٹر�tick
    virtual void LPAPI SetDelayCloseBeginTick(LPUINT64 qwTick);

    // Summary:
    //      ��ȡ��ʼ�ӳٹر�tick
    virtual LPUINT64 LPAPI GetDelayCloseBeginTick();

    // Summary:
    //      �����ӳٹرճ���ʱ��
    virtual void LPAPI SetDelayCloseDuration(LPUINT64 qwDuration);

    // Summary:
    //      ��ȡ�ӳٹرճ���ʱ��
    virtual LPUINT64 LPAPI GetDelayCloseDuration();

    // Summary:
    //      ���ÿ�ʼ�ӳ��ͷ�tick
    virtual void LPAPI SetDelayReleaseBeginTick(LPUINT64 qwTick);

    // Summary:
    //      ��ȡ��ʼ�ӳ��ͷ�tick
    virtual LPUINT64 LPAPI GetDelayReleaseBeginTick();

    // Summary:
    //      �����ӳ��ͷų���ʱ��
    virtual void LPAPI SetDelayReleaseDuration(LPUINT64 qwDuration);

    // Summary:
    //      ��ȡ�ӳ��ͷų���ʱ��
    virtual LPUINT64 LPAPI GetDelayReleaseDuration();

    // Summary:
    //      �첽�رջص�
    virtual void LPAPI OnClose();

    // Summary:
    //      ��
    virtual void LPAPI SetNetImpl(LPNetImpl* pNetImpl);

    // Summary:
    //      ��
    virtual LPNetImpl* LPAPI GetNetImpl();

    // Summary:
    //      ��������
    // Input:
    //      pData������
    //      dwLen������
    virtual BOOL LPAPI Send(const char* pData, LPUINT32 dwLen);

    // Summary:
    //      �ر����ӣ������������رջ��Ǳ����رգ���ͳһ���ô˽ӿڹر��Ѿ�������socker
    virtual void LPAPI Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose);

    // Summary:
    //      post�첽�������ݲ���
    virtual void LPAPI PostRecv();

    // Summary:
    //   ��
    virtual void LPAPI HandleRecv(const system::error_code& err, const size_t nSize);

    // Summary:
    //      post�첽�������ݲ���
    // Return:
    //      TRUE-�з������ݣ�FALSE-û��������
    virtual BOOL LPAPI PostSend();

    // Summary:
    //   ��
    virtual void LPAPI HandleSend(const system::error_code& err, const size_t nSize);

protected:

    // Summary:
    //      ����socket
    virtual lp_shared_ptr<ip::tcp::socket> LPAPI GetSocket();

protected:

    volatile atomic_bool            m_bConnect;                 // ����״̬���
    volatile atomic_bool            m_bSending;                 // �����Ƿ����ڷ��ͱ��

    BOOL                            m_bPassiveClose;            // �Ƿ񱻶��ر�
    SOCKET                          m_hSock;                    // sock���
    LPUINT32                        m_dwSockerId;               // socker id

    LPUINT32                        m_dwRemoteIp;               // Զ��ip
    LPUINT16                        m_wRemotePort;              // Զ�˶˿�
    std::string                     m_strRemoteIp;              // Զ��ip�ַ���
    LPUINT32                        m_dwLocalIp;                // ����ip
    LPUINT16                        m_wLocalPort;               // ���ض˿�
    std::string                     m_strLocalIp;               // ����ip�ַ���

    LPUINT32                        m_dwParentId;               // ���������id�����������������id��
    LPNetImpl*                      m_pNetImpl;                 //
    BOOL                            m_bAcceptCreate;            // �Ƿ���accept����
    lp_shared_ptr<ILPPacketParser>  m_pPacketParser;            // ��������

    LPLoopBuf*                      m_pRecvLoopBuf;             // ���ջ�������ֻ��PostRecv��OnRecv�в���������д������Ϊû��PostRecv�ǲ����յ�OnRecv��
    LPLoopBuf*                      m_pSendLoopBuf;             // ���ͻ����������߳�д������������Ҫ����

    LPUINT64                        m_qwDelayCloseBeginTick;    // �ӳٹرտ�ʼtick
    LPUINT64                        m_qwDelayCloseDuration;     // �ӳٹرճ���ʱ��
    LPUINT64                        m_qwDelayReleaseBeginTick;  // �ӳ��ͷſ�ʼtick
    LPUINT64                        m_qwDelayReleaseDuration;   // �ӳ��ͷų���ʱ��

    PER_IO_DATA                     m_stRecvOrSendPerIoData;    // io�󶨵����ݣ�Ŀǰֻ��linux�õ�

    lp_shared_ptr<ip::tcp::socket>  m_pSocket;
};



//end�������������ֿռ�
NS_LZPL_END

#endif