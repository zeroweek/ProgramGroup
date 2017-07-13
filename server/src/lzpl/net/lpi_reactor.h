//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description:
//
//        反应器ILPReactor接口声明。
//
//****************************************************************************/
#ifndef _LPI_REACTOR_H_
#define _LPI_REACTOR_H_

#include "lp_netdef.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//结构体声明
struct NET_CONFIG;



// Summary：
//     反应器，与ILPEventHandler交互，负责网络事件处理
class DECLARE ILPReactor
{
public:

    // Summary:
    //      无
    virtual ~ILPReactor() {}

    // Summary:
    //   无
    virtual boost::asio::io_service& GetIoService(LPUINT32 dwId) = 0;

public:

    static lp_shared_ptr<ILPReactor> LPAPI NewReactor(NET_CONFIG& stNetConfig);
    static void LPAPI DeleteReactor(lp_shared_ptr<ILPReactor>& pReactor);
};



//end声明所处的名字空间
NS_LZPL_END

#endif