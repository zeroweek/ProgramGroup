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



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;



// Summary:
//      sql工作线程参数
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



// Summary：
//     基础反应器
class DECLARE LPReactor : public ILPReactor
{
public:

    // Summary：
    //     构造函数
    // Returns:
    //     无
    LPReactor();

    // Summary：
    //     析构函数
    // Returns:
    //     无
    ~LPReactor();

    // Summary：
    //      无
    virtual BOOL LPAPI Init(NET_CONFIG& stNetConfig);
    // Summary：
    //      无
    virtual BOOL LPAPI UnInit();

    // Summary：
    //     线程处理函数的主逻辑
    virtual void LPAPI OnExecute(REACTOR_THREAD_PARAM& tThreadParam) = 0;

    // Summary：
    //     完成端口线程处理函数
    static THREAD_FUNC_DECLARE(ThreadFunc)(void* pParam);

protected:

    // Summary：
    //      获取当前状态
    virtual LPUINT32 LPAPI GetState();

    // Summary：
    //      设置当前状态
    virtual void LPAPI SetState(LPUINT32 dwState);

protected:

    volatile atomic_uint    m_dwState;
    NET_CONFIG              m_stNetConfig;
};



// Summary：
//     windows IOCP反应器实现类
class DECLARE LPIocpReactor : public LPReactor
{
public:

    // Summary：
    //     构造函数
    // Returns:
    //     无
    LPIocpReactor();

    // Summary：
    //     析构函数
    // Returns:
    //     无
    ~LPIocpReactor();

    // Summary：
    //      无
    virtual BOOL LPAPI Init(NET_CONFIG& stNetConfig);
    // Summary：
    //      无
    virtual BOOL LPAPI UnInit();

    // Summary:
    //     注册ILPEventHandler
    // Returns:
    //     TRUE-成功，FALSE-失败
    virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary：
    //     注销ILPEventHandler
    // Returns:
    //     TRUE-成功，FALSE-失败
    virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary：
    //     线程处理函数的主逻辑
    virtual void LPAPI OnExecute(REACTOR_THREAD_PARAM& tThreadParam);

protected:

    LPINT32                  m_nCompletionPortCount;
    HANDLE*                  m_pCompletionPort;
    LPINT32                  m_nWorkerCountPerCompIo;
    HANDLE**                 m_ppWorkerArray;
};



// Summary：
//     linux epoll反应器实现类
class DECLARE LPEpollReactor : public LPReactor
{
public:

    // Summary：
    //     构造函数
    // Returns:
    //     无
    LPEpollReactor();

    // Summary：
    //     析构函数
    // Returns:
    //     无
    ~LPEpollReactor();

    // Summary：
    //      无
    virtual BOOL LPAPI Init(NET_CONFIG& stNetConfig);
    // Summary：
    //      无
    virtual BOOL LPAPI UnInit();

    // Summary:
    //     注册ILPEventHandler
    // Returns:
    //     TRUE-成功，FALSE-失败
    virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary：
    //     注销ILPEventHandler
    // Returns:
    //     TRUE-成功，FALSE-失败
    virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler);

    // Summary：
    //     线程处理函数的主逻辑
    virtual void LPAPI OnExecute(REACTOR_THREAD_PARAM& tThreadParam);

protected:

    HANDLE                   m_hEpoll;
    BOOL                     m_bRun;
    LPThread                 m_oThread;  // check delay线程
};



//end声明所处的名字空间
NS_LZPL_END

#endif