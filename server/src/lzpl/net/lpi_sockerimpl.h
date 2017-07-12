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
#include "lp_loopbuf.h"
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
    //      设置sock句柄
    // Input:
    //      sock：底层socket句柄值
    virtual void LPAPI SetSock(SOCKET sock) = 0;

    // Summary:
    //      设置socket
    virtual void LPAPI SetSocket(lp_shared_ptr<ip::tcp::socket> pSocket) = 0;

    // Summary:
    //      设置连接状态标记值
    // Input:
    //      bConnect：true-连接，false-断开
    virtual void LPAPI SetConnect(bool bConnect) = 0;

    // Summary:
    //      绑定解析对象
    // Input:
    //      pPacketParser：解析对象
    virtual void LPAPI AttachPacketParser(lp_shared_ptr<ILPPacketParser> pPacketParser) = 0;

    // Summary:
    //      解除绑定解析对象
    virtual void LPAPI DetachPacketParser() = 0;

    // Summary:
    //      post异步接收数据操作
    virtual void LPAPI PostRecv() = 0;

    // Summary:
    //      post异步发送数据操作
    // Return:
    //      TRUE-有发送数据，FALSE-没发送数据
    virtual BOOL LPAPI PostSend() = 0;

    // Summary:
    //      设置接收缓冲区
    // Input:
    //      pBuf：缓冲区
    virtual void LPAPI AttachRecvBuf(LPLoopBuf* pLoopBuf) = 0;

    // Summary:
    //      分离接收缓冲区
    // Return:
    //      接收缓冲区
    virtual LPLoopBuf* LPAPI DetachRecvBuf() = 0;

    // Summary:
    //      设置发送缓冲区
    // Input:
    //      pBuf：缓冲区
    virtual void LPAPI AttachSendBuf(LPLoopBuf* pLoopBuf) = 0;

    // Summary:
    //      分离发送缓冲区
    // Return:
    //      发送缓冲区
    virtual LPLoopBuf* LPAPI DetachSendBuf() = 0;

    // Summary:
    //      设置远端连接ip
    // Input:
    //      dwIp：ip
    virtual void LPAPI SetRemoteIp(LPUINT32 dwIp) = 0;

    // Summary:
    //      设置远端连接端口
    // Input:
    //      wPort：端口
    virtual void LPAPI SetRemotePort(LPUINT16 wPort) = 0;

    // Summary:
    //      设置本地连接ip
    // Input:
    //      dwIp：ip
    virtual void LPAPI SetLocalIp(LPUINT32 dwIp) = 0;

    // Summary:
    //      设置本地连接端口
    // Input:
    //      wPort：端口
    virtual void LPAPI SetLocalPort(LPUINT16 wPort) = 0;

    // Summary:
    //      设置开始延迟关闭tick
    virtual void LPAPI SetDelayCloseBeginTick(LPUINT64 qwTick) = 0;

    // Summary:
    //      获取开始延迟关闭tick
    virtual LPUINT64 LPAPI GetDelayCloseBeginTick() = 0;

    // Summary:
    //      设置延迟关闭持续时间
    virtual void LPAPI SetDelayCloseDuration(LPUINT64 qwDuration) = 0;

    // Summary:
    //      获取延迟关闭持续时间
    virtual LPUINT64 LPAPI GetDelayCloseDuration() = 0;

    // Summary:
    //      设置开始延迟释放tick
    virtual void LPAPI SetDelayReleaseBeginTick(LPUINT64 qwTick) = 0;

    // Summary:
    //      获取开始延迟释放tick
    virtual LPUINT64 LPAPI GetDelayReleaseBeginTick() = 0;

    // Summary:
    //      设置延迟释放持续时间
    virtual void LPAPI SetDelayReleaseDuration(LPUINT64 qwDuration) = 0;

    // Summary:
    //      获取延迟释放持续时间
    virtual LPUINT64 LPAPI GetDelayReleaseDuration() = 0;

    // Summary:
    //      设置socker id
    // Input:
    //      dwSockerId：socker id
    virtual void LPAPI SetSockerId(LPUINT32 dwSockerId) = 0;

    // Summary:
    //      无
    virtual void LPAPI SetNetImpl(LPNetImpl* pNetImpl) = 0;

    // Summary:
    //      无
    virtual LPNetImpl* LPAPI GetNetImpl() = 0;

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

public:

    static lp_shared_ptr<ILPSockerImpl> LPAPI NewSockerImpl(LPUINT32 dwIoType);
    static void LPAPI DeleteSockerImpl(lp_shared_ptr<ILPSockerImpl>& pSocker);
};



//end声明所处的名字空间
NS_LZPL_END

#endif