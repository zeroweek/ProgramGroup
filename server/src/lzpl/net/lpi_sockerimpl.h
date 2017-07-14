//****************************************************************************/
//    author: caihy
//    date: October 20, 2014
//    description:
//
//****************************************************************************/
#ifndef _LPI_SOCKER_IMPL_H_
#define _LPI_SOCKER_IMPL_H_

#include "lpi_socker.h"
#include "lpi_packetparser.h"
#include "lpi_buf.h"
#include "lp_netdef.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;



// Summary:
//      底层socket对象封装接口类
class DECLARE ILPSockerImpl : public ILPSocker, public lp_enable_shared_from_this<ILPSockerImpl>
{
public:

    // Summary:
    //      无
    virtual ~ILPSockerImpl() {}

    // Summary:
    //      重置socker对象
    virtual void LPAPI Reset() = 0;

    // Summary:
    //      设置socket
    virtual void LPAPI SetSocket(lp_shared_ptr<ip::tcp::socket> pSocket) = 0;

    // Summary:
    //      设置连接状态标记值
    // Input:
    //      bConnect：true-连接，false-断开
    virtual void LPAPI SetConnect(bool bConnect) = 0;

    // Summary:
    //      设置父级对象的id（连接器或监听器的id）
    // Input:
    //      dwParentId：id
    virtual void LPAPI SetParentId(LPUINT32 dwParentId) = 0;

    // Summary:
    //      获取父级对象的id（连接器或监听器的id）
    // Return:
    //      id
    virtual LPUINT32 LPAPI GetParentId() = 0;

    // Summary:
    //      设置是否被动关闭
    virtual void LPAPI SetPassiveClose(BOOL bPassiveClose) = 0;

    // Summary:
    //      设置是否是accept创建的标记，accept还是connect创建
    virtual void LPAPI SetAcceptCreate(BOOL bAcceptCreate) = 0;

    // Summary:
    //      判断是否是accept创建
    //  Return:
    //      TRUE-是，FALSE-不是
    virtual BOOL LPAPI IsAcceptCreate() = 0;

    // Summary:
    //      异步关闭回调
    virtual void LPAPI OnClose() = 0;

    // Summary:
    //      post异步接收数据操作
    virtual void LPAPI PostRecv() = 0;

    // Summary:
    //      post异步发送数据操作
    // Return:
    //      TRUE-有发送数据，FALSE-没发送数据
    virtual BOOL LPAPI PostSend() = 0;

    // Summary:
    //      绑定解析对象
    // Input:
    //      pPacketParser：解析对象
    virtual void LPAPI AttachPacketParser(lp_shared_ptr<ILPPacketParser> pPacketParser) = 0;

    // Summary:
    //      解除绑定解析对象
    virtual void LPAPI DetachPacketParser() = 0;

    // Summary:
    //      设置接收缓冲区
    // Input:
    //      pBuf：缓冲区
    virtual void LPAPI AttachRecvBuf(lp_shared_ptr<ILPLoopBuf> pLoopBuf) = 0;

    // Summary:
    //      分离接收缓冲区
    // Return:
    //      接收缓冲区
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI DetachRecvBuf() = 0;

    // Summary:
    //      设置发送缓冲区
    // Input:
    //      pBuf：缓冲区
    virtual void LPAPI AttachSendBuf(lp_shared_ptr<ILPLoopBuf> pLoopBuf) = 0;

    // Summary:
    //      分离发送缓冲区
    // Return:
    //      发送缓冲区
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI DetachSendBuf() = 0;

    // Summary:
    //      设置socker id
    // Input:
    //      dwSockerId：socker id
    virtual void LPAPI SetSockerId(LPUINT32 dwSockerId) = 0;

    // Summary:
    //      无
    virtual void LPAPI SetNetImpl(lp_shared_ptr<LPNetImpl> pNetImpl) = 0;

public:

    static lp_shared_ptr<ILPSockerImpl> LPAPI NewSockerImpl();
    static void LPAPI DeleteSockerImpl(lp_shared_ptr<ILPSockerImpl>& pSocker);
};



//end声明所处的名字空间
NS_LZPL_END

#endif