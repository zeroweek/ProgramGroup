//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description:
//
//****************************************************************************/
#ifndef _LP_REACTOR_H_
#define _LP_REACTOR_H_

#include "lpi_reactor.h"
#include "lpi_net.h"
#include "lp_thread.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//      sql�����̲߳���
struct REACTOR_THREAD_PARAM
{
    ILPReactor*       pReactorImpl;
    LPINT32           nCompletionPortIndex;

    REACTOR_THREAD_PARAM()
    {
        pReactorImpl = nullptr;
        nCompletionPortIndex = -1;
    }
};



// Summary��
//     ������Ӧ��
class DECLARE LPReactor : public ILPReactor
{
public:

    // Summary��
    //     ���캯��
    // Returns:
    //     ��
    LPReactor();

    // Summary��
    //     ��������
    // Returns:
    //     ��
    ~LPReactor();

    // Summary��
    //      ��
    virtual BOOL LPAPI Init(NET_CONFIG& stNetConfig);
    // Summary��
    //      ��
    virtual BOOL LPAPI UnInit();

    // Summary��
    //     �̴߳����������߼�
    virtual void LPAPI OnExecute(REACTOR_THREAD_PARAM& tThreadParam) = 0;

    // Summary��
    //     ��ɶ˿��̴߳�����
    static THREAD_FUNC_DECLARE(ThreadFunc)(void* pParam);

protected:

    // Summary��
    //      ��ȡ��ǰ״̬
    virtual LPUINT32 LPAPI GetState();

    // Summary��
    //      ���õ�ǰ״̬
    virtual void LPAPI SetState(LPUINT32 dwState);

protected:

    volatile atomic_uint    m_dwState;
    NET_CONFIG              m_stNetConfig;
};



// Summary��
//     windows IOCP��Ӧ��ʵ����
class DECLARE LPIocpReactor : public LPReactor
{
public:

    // Summary��
    //     ���캯��
    // Returns:
    //     ��
    LPIocpReactor();

    // Summary��
    //     ��������
    // Returns:
    //     ��
    ~LPIocpReactor();

    // Summary��
    //      ��
    virtual BOOL LPAPI Init(NET_CONFIG& stNetConfig);
    // Summary��
    //      ��
    virtual BOOL LPAPI UnInit();

    // Summary:
    //     ע��ILPEventHandler
    // Returns:
    //     TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary��
    //     ע��ILPEventHandler
    // Returns:
    //     TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary��
    //     �̴߳����������߼�
    virtual void LPAPI OnExecute(REACTOR_THREAD_PARAM& tThreadParam);

protected:

    LPINT32                  m_nCompletionPortCount;
    HANDLE*                  m_pCompletionPort;
    LPINT32                  m_nWorkerCountPerCompIo;
    HANDLE**                 m_ppWorkerArray;
};



// Summary��
//     linux epoll��Ӧ��ʵ����
class DECLARE LPEpollReactor : public LPReactor
{
public:

    // Summary��
    //     ���캯��
    // Returns:
    //     ��
    LPEpollReactor();

    // Summary��
    //     ��������
    // Returns:
    //     ��
    ~LPEpollReactor();

    // Summary��
    //      ��
    virtual BOOL LPAPI Init(NET_CONFIG& stNetConfig);
    // Summary��
    //      ��
    virtual BOOL LPAPI UnInit();

    // Summary:
    //     ע��ILPEventHandler
    // Returns:
    //     TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary��
    //     ע��ILPEventHandler
    // Returns:
    //     TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary��
    //     �̴߳����������߼�
    virtual void LPAPI OnExecute(REACTOR_THREAD_PARAM& tThreadParam);

protected:

    HANDLE                   m_hEpoll;
    BOOL                     m_bRun;
    LPThread                 m_oThread;  // check delay�߳�
};



//end�������������ֿռ�
NS_LZPL_END

#endif