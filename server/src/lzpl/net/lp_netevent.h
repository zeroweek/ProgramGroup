//****************************************************************************/
//    author: caihy
//    date: Jan 5, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_NET_EVENT_H_
#define _LP_NET_EVENT_H_

#include "lpi_net.h"
#include "lp_lock.h"
#include "lp_loopbuf.h"
#include "lpi_sockerimpl.h"
#include "lpi_connectorimpl.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      �¼�����
enum e_EventType
{
    eEventType_None = 0,
    eEventType_Recv = 1,
    eEventType_Terminate = 2,
    eEventType_Establish = 3,
    eEventType_ConnectError = 4,
    eEventType_Max
};



// Summary;
//      �����¼��ṹ��
struct DECLARE RECV_EVENT
{
    lp_shared_ptr<ILPSockerImpl>    pSocker;
    LPUINT32                        dwLen;
};



// Summary;
//      �Ͽ��¼��ṹ��
struct DECLARE TERMINATE_EVENT
{
    lp_shared_ptr<ILPSockerImpl>    pSocker;
    LPUINT32                        dwSockerId;
};



// Summary;
//      ���ӽ����¼��ṹ��
struct DECLARE ESTABLISH_EVENT
{
    lp_shared_ptr<ILPSockerImpl>    pSocker;
    BOOL                            bAccept;
};



// Summary;
//      ���Ӵ����¼��ṹ��
struct DECLARE CONNECT_ERROR_EVENT
{
    std::shared_ptr<ILPConnectorImpl>  pConnector;
    LPUINT32                           dwErrorNo;
};



// Summary;
//      �¼��ṹ��
struct DECLARE NET_EVENT
{
    e_EventType                             eEventType;
    LPUINT32                                dwFlag;      // ���ֶ�Ŀǰ�����ڷ�ɢ����ͬ�¼��б�ƽ��ѹ��

    lp_shared_ptr<RECV_EVENT>               pRecvEvent;
    lp_shared_ptr<TERMINATE_EVENT>          pTerminateEvent;
    lp_shared_ptr<ESTABLISH_EVENT>          pEstablishEvent;
    lp_shared_ptr<CONNECT_ERROR_EVENT>      pConnectErrorEvent;

    static LPINT32 ms_dwNetEventCount;
    static NET_EVENT* LPAPI NewNetEvent(LPUINT32 dwEventType)
    {
        ++ms_dwNetEventCount;
        NET_EVENT* pstEvent = new NET_EVENT();
        LOG_PROCESS_ERROR(pstEvent != nullptr);

        switch(dwEventType)
        {
        case eEventType_Recv:
            pstEvent->pRecvEvent = lp_make_shared<RECV_EVENT>();
            break;
        case eEventType_Terminate:
            pstEvent->pTerminateEvent = lp_make_shared<TERMINATE_EVENT>();
            break;
        case eEventType_Establish:
            pstEvent->pEstablishEvent = lp_make_shared<ESTABLISH_EVENT>();
            break;
        case eEventType_ConnectError:
            pstEvent->pConnectErrorEvent = lp_make_shared<CONNECT_ERROR_EVENT>();
            break;
        case eEventType_None:
        case eEventType_Max:
        default:
            LOG_CHECK_ERROR(FALSE);
            LPASSERT(FALSE);
            LOG_PROCESS_ERROR(FALSE);
        }

        return pstEvent;
Exit0:

        --ms_dwNetEventCount;
        SAFE_DELETE(pstEvent);
        return nullptr;
    }

    static void LPAPI DeleteNetEvent(NET_EVENT* & pNetEvent)
    {
        if(nullptr != pNetEvent)
        {
            --ms_dwNetEventCount;
        }
        SAFE_DELETE(pNetEvent);
    }
};



//������
class LPNetImpl;



// Summary:
//      �¼�������
class DECLARE LPEventMgr
{
public:

    // Summary��
    //     ���캯��
    // Returns:
    //     ��
    LPEventMgr();

    // Summary��
    //     ��������
    // Returns:
    //     ��
    ~LPEventMgr();

public:

    typedef std::list<NET_EVENT*> LPListEvent;

public:

    // Summary:
    //      ��ʼ������
    // Input:
    //      nEventListCount��
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    BOOL LPAPI Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPNetMessageHandler> pNetMessageHandler, LPINT32 nEventListCount);

    // Summary:
    //      �������
    BOOL LPAPI UnInit();

    // Summary:
    //      pushһ�������¼�
    BOOL LPAPI PushRecvEvent(lp_shared_ptr<ILPSockerImpl> pSocker, LPUINT32 dwSockerId, ILPLoopBuf& oLoopBuf, LPUINT32 dwLen);

    // Summary:
    //      pushһ���Ͽ��¼�
    void LPAPI PushTerminateEvent(lp_shared_ptr<ILPSockerImpl> pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose);

    // Summary:
    //      pushһ�����ӽ����¼�
    void LPAPI PushEstablishEvent(lp_shared_ptr<ILPSockerImpl> pSocker, BOOL bAccept);

    // Summary:
    //      pushһ�����Ӵ����¼�
    void LPAPI PushConnectErrorEvent(std::shared_ptr<ILPConnectorImpl> pConnector, LPUINT32 dwErrorNo);

    // Summary:
    //      �ж��Ƿ����¼�������
    BOOL HaveEventForHandled();

    // Summary:
    //      ����һ���¼����ú����������̵߳��ã�����Ӧ��һЩ�¼��������ڴˣ�
    //          ��֤ͬһ���̴߳���
    void HandleOneEvent();

private:

    // Summary:
    //      ��������¼�
    void LPAPI _ProcRecvEvent(std::shared_ptr<RECV_EVENT> pstRecvEvent, LPUINT32 dwFlag);

    // Summary:
    //      ����Ͽ��¼�
    void LPAPI _ProcTerminateEvent(std::shared_ptr<TERMINATE_EVENT> pstTerminateEvent);

    // Summary:
    //      �������ӽ����¼�
    void LPAPI _ProcEstablishEvent(std::shared_ptr<ESTABLISH_EVENT> pstEstablishEvent);

    // Summary:
    //      �������Ӵ����¼�
    void LPAPI _ProcConnectErrorEvent(std::shared_ptr<CONNECT_ERROR_EVENT> pstConnectErrorEvent);

private:

    BOOL                                m_bInit;
    char*                               m_pPacketTempBuf;             // ���ݰ���ʱ������
    LPINT32                             m_nEventListCount;            // �¼��б����
    LPListEvent*                        m_pEventList;                 // �¼��б�
    LPLock*                             m_pEventListLock;             // �¼��б���
    LPLoopBuf*                          m_pEventListRecvLoopBuf;      // �����¼����ݻ��������飨ÿ���¼��б��Ӧһ����
    LPLock*                             m_pEventListRecvLoopBufLock;  // �����¼����ݻ�����������
    lp_shared_ptr<ILPNetMessageHandler> m_pNetMessageHandler;         //
    lp_shared_ptr<LPNetImpl>            m_pNetImpl;                   //
};



//end�������������ֿռ�
NS_LZPL_END

#endif