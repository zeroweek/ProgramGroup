//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: 网络消息处理器接口定义
//
//****************************************************************************/
#ifndef _LPI_NET_MESSAGE_HANDLER_H_
#define _LPI_NET_MESSAGE_HANDLER_H_

#include "lp_base.h"
#include "lpi_socker.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      网络消息处理器接口类
class DECLARE ILPNetMessageHandler
{
public:

    // Summary:
    //      无
    virtual ~ILPNetMessageHandler() {}

    // Summary:
    //      监听到新的链接
    virtual void LPAPI OnAccepted(lp_shared_ptr<ILPSocker> pSocker) = 0;

    // Summary:
    //      成功创建新的链接
    virtual void LPAPI OnConnected(lp_shared_ptr<ILPSocker> pSocker) = 0;

    // Summary:
    //      创建链接错误
    virtual void LPAPI OnConnectError(lp_shared_ptr<ILPConnector> pConnector, LPUINT32 dwErrorNo) = 0;

    // Summary:
    //      链接收到消息
    virtual void LPAPI OnMessage(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize) = 0;

    // Summary:
    //      链接断开（OnAccepted的连接）
    virtual void LPAPI OnDisconnected(lp_shared_ptr<ILPSocker> pSocker) = 0;

    // Summary:
    //      链接断开（OnConnected的连接）
    virtual void LPAPI OnConnectDisconnected(lp_shared_ptr<ILPSocker> pSocker, lp_shared_ptr<ILPConnector> pConnector) = 0;

};




//end声明所处的名字空间
NS_LZPL_END

#endif