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
    ILPReactor*       pReactor;
    LPINT32           nIndex;

    REACTOR_THREAD_PARAM()
    {
        pReactor = nullptr;
        nIndex = -1;
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

    // Summary:
    //   无
    virtual boost::asio::io_service& GetIoService(LPUINT32 dwId);

    // Summary：
    //     线程处理函数的主逻辑
    virtual void LPAPI OnExecute(LPUINT32 nIndex);

    // Summary：
    //     完成端口线程处理函数
    virtual void ThreadFunc(LPUINT32 nIndex);

protected:

    // Summary：
    //      获取当前状态
    virtual LPUINT32 LPAPI GetState();

    // Summary：
    //      设置当前状态
    virtual void LPAPI SetState(LPUINT32 dwState);

protected:

    volatile atomic_uint                            m_dwState;
    NET_CONFIG                                      m_stNetConfig;

    LPUINT32                                        m_nIoServiceNum;
    std::vector<lp_shared_ptr<asio::io_service>>    m_vecIoService;
    std::vector<lp_shared_ptr<std::thread>>         m_vecThread;
};



//end声明所处的名字空间
NS_LZPL_END

#endif