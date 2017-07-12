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



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      底层socke对象t封装类
class DECLARE LPSocker : public ILPSockerImpl
{
public:

    // Summary:
    //      无
    LPSocker();

    // Summary:
    //      无
    virtual ~LPSocker();

    // Summary:
    //      重置socker对象
    virtual void LPAPI Reset();

    // Summary:
    //      设置sock句柄
    // Input:
    //      sock：底层socket句柄值
    virtual void LPAPI SetSock(SOCKET sock);

    // Summary:
    //      设置socket
    virtual void LPAPI SetSocket(lp_shared_ptr<ip::tcp::socket> pSocket);

    // Summary:
    //      获取底层sock的句柄
    // Return:
    //      返回底层sock句柄
    virtual SOCKET LPAPI GetSock();

    // Summary:
    //      设置socker id
    // Input:
    //      dwSockerId：socker id
    virtual void LPAPI SetSockerId(LPUINT32 dwSockerId);

    // Summary:
    //      获取socker id
    // Return:
    //      返回socker id
    virtual LPUINT32 LPAPI GetSockerId();

    // Summary:
    //      设置远端连接ip
    // Input:
    //      dwIp：ip
    virtual void LPAPI SetRemoteIp(LPUINT32 dwIp);

    // Summary:
    //      获取对端连接的ip
    virtual LPUINT32 LPAPI GetRemoteIp();

    // Summary:
    //      获取对端连接的ip字符串
    virtual std::string& LPAPI GetRemoteIpStr();

    // Summary:
    //      设置远端连接端口
    // Input:
    //      wPort：端口
    virtual void LPAPI SetRemotePort(LPUINT16 wPort);

    // Summary:
    //      获取对端连接的port
    virtual LPUINT16 LPAPI GetRemotePort();

    // Summary:
    //      设置本地连接ip
    // Input:
    //      dwIp：ip
    virtual void LPAPI SetLocalIp(LPUINT32 dwIp);

    // Summary:
    //      获取本地连接的ip
    virtual LPUINT32 LPAPI GetLocalIp();

    // Summary:
    //      获取本地连接的ip字符串
    virtual std::string& LPAPI GetLocalIpStr();

    // Summary:
    //      设置本地连接端口
    // Input:
    //      wPort：端口
    virtual void LPAPI SetLocalPort(LPUINT16 wPort);

    // Summary:
    //      获取本地连接的port
    virtual LPUINT16 LPAPI GetLocalPort();

    // Summary:
    //      设置连接状态标记值
    // Input:
    //      bConnect：true-连接，false-断开
    virtual void LPAPI SetConnect(bool bConnect);

    // Summary:
    //      判断当前的socket是否处于连接状态
    // Return:
    //      TRUE-连接，FALSE-断开
    virtual BOOL LPAPI IsConnect();

    // Summary:
    //      设置是否被动关闭
    virtual void LPAPI SetPassiveClose(BOOL bPassiveClose);

    // Summary:
    //      判断是否被动关闭
    //  Return:
    //      TRUE-是，FALSE-不是
    virtual BOOL LPAPI IsPassiveClose();

    // Summary:
    //      设置父级对象的id（连接器或监听器的id）
    // Input:
    //      dwParentId：id
    virtual void LPAPI SetParentId(LPUINT32 dwParentId);

    // Summary:
    //      获取父级对象的id（连接器或监听器的id）
    // Return:
    //      id
    virtual LPUINT32 LPAPI GetParentId();

    // Summary:
    //      设置是否是accept创建的标记，accept还是connect创建
    virtual void LPAPI SetAcceptCreate(BOOL bAcceptCreate);

    // Summary:
    //      判断是否是accept创建
    //  Return:
    //      TRUE-是，FALSE-不是
    virtual BOOL LPAPI IsAcceptCreate();

    // Summary:
    //      绑定解析对象
    // Input:
    //      pPacketParser：解析对象
    virtual void LPAPI AttachPacketParser(lp_shared_ptr<ILPPacketParser> pPacketParser);

    // Summary:
    //      解除绑定解析对象
    virtual void LPAPI DetachPacketParser();

    // Summary:
    //      设置接收缓冲区
    // Input:
    //      pBuf：缓冲区
    virtual void LPAPI AttachRecvBuf(LPLoopBuf* pLoopBuf);

    // Summary:
    //      分离接收缓冲区
    // Return:
    //      接收缓冲区
    virtual LPLoopBuf* LPAPI DetachRecvBuf();

    // Summary:
    //      设置发送缓冲区
    // Input:
    //      pBuf：缓冲区
    virtual void LPAPI AttachSendBuf(LPLoopBuf* pLoopBuf);

    // Summary:
    //      分离发送缓冲区
    // Return:
    //      发送缓冲区
    virtual LPLoopBuf* LPAPI DetachSendBuf();

    // Summary:
    //      设置开始延迟关闭tick
    virtual void LPAPI SetDelayCloseBeginTick(LPUINT64 qwTick);

    // Summary:
    //      获取开始延迟关闭tick
    virtual LPUINT64 LPAPI GetDelayCloseBeginTick();

    // Summary:
    //      设置延迟关闭持续时间
    virtual void LPAPI SetDelayCloseDuration(LPUINT64 qwDuration);

    // Summary:
    //      获取延迟关闭持续时间
    virtual LPUINT64 LPAPI GetDelayCloseDuration();

    // Summary:
    //      设置开始延迟释放tick
    virtual void LPAPI SetDelayReleaseBeginTick(LPUINT64 qwTick);

    // Summary:
    //      获取开始延迟释放tick
    virtual LPUINT64 LPAPI GetDelayReleaseBeginTick();

    // Summary:
    //      设置延迟释放持续时间
    virtual void LPAPI SetDelayReleaseDuration(LPUINT64 qwDuration);

    // Summary:
    //      获取延迟释放持续时间
    virtual LPUINT64 LPAPI GetDelayReleaseDuration();

    // Summary:
    //      异步关闭回调
    virtual void LPAPI OnClose();

    // Summary:
    //      无
    virtual void LPAPI SetNetImpl(LPNetImpl* pNetImpl);

    // Summary:
    //      无
    virtual LPNetImpl* LPAPI GetNetImpl();

    // Summary:
    //      发送数据
    // Input:
    //      pData：数据
    //      dwLen：长度
    virtual BOOL LPAPI Send(const char* pData, LPUINT32 dwLen);

    // Summary:
    //      关闭链接，不管是主动关闭还是被动关闭，都统一调用此接口关闭已经建立的socker
    virtual void LPAPI Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose);

    // Summary:
    //      post异步接收数据操作
    virtual void LPAPI PostRecv();

    // Summary:
    //   无
    virtual void LPAPI HandleRecv(const system::error_code& err, const size_t nSize);

    // Summary:
    //      post异步发送数据操作
    // Return:
    //      TRUE-有发送数据，FALSE-没发送数据
    virtual BOOL LPAPI PostSend();

    // Summary:
    //   无
    virtual void LPAPI HandleSend(const system::error_code& err, const size_t nSize);

protected:

    // Summary:
    //      设置socket
    virtual lp_shared_ptr<ip::tcp::socket> LPAPI GetSocket();

protected:

    volatile atomic_bool            m_bConnect;                 // 连接状态标记
    volatile atomic_bool            m_bSending;                 // 数据是否正在发送标记

    BOOL                            m_bPassiveClose;            // 是否被动关闭
    SOCKET                          m_hSock;                    // sock句柄
    LPUINT32                        m_dwSockerId;               // socker id

    LPUINT32                        m_dwRemoteIp;               // 远端ip
    LPUINT16                        m_wRemotePort;              // 远端端口
    std::string                     m_strRemoteIp;              // 远端ip字符串
    LPUINT32                        m_dwLocalIp;                // 本地ip
    LPUINT16                        m_wLocalPort;               // 本地端口
    std::string                     m_strLocalIp;               // 本地ip字符串

    LPUINT32                        m_dwParentId;               // 父级对象的id（连接器或监听器的id）
    LPNetImpl*                      m_pNetImpl;                 //
    BOOL                            m_bAcceptCreate;            // 是否是accept创建
    lp_shared_ptr<ILPPacketParser>  m_pPacketParser;            // 解析对象

    LPLoopBuf*                      m_pRecvLoopBuf;             // 接收缓冲区（只有PostRecv和OnRecv有操作，无需写锁，因为没有PostRecv是不会收到OnRecv）
    LPLoopBuf*                      m_pSendLoopBuf;             // 发送缓冲区（单线程写不用锁，读需要锁）

    LPUINT64                        m_qwDelayCloseBeginTick;    // 延迟关闭开始tick
    LPUINT64                        m_qwDelayCloseDuration;     // 延迟关闭持续时间
    LPUINT64                        m_qwDelayReleaseBeginTick;  // 延迟释放开始tick
    LPUINT64                        m_qwDelayReleaseDuration;   // 延迟释放持续时间

    PER_IO_DATA                     m_stRecvOrSendPerIoData;    // io绑定的数据，目前只有linux用到

    lp_shared_ptr<ip::tcp::socket>  m_pSocket;
};



//end声明所处的名字空间
NS_LZPL_END

#endif