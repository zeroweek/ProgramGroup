//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_LISTENER_H_
#define _LP_LISTENER_H_

#include "lpi_listenerimpl.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define LISTEN_QUEUE_COUNT             (128)



//类声明
class LPNetImpl;



// Summary:
//      监听器类
class DECLARE LPListener : public ILPListenerImpl
{
public:

    // Summary:
    //      无
    LPListener();

    // Summary:
    //      无
    virtual ~LPListener();

    // Summary：
    //      无
    // Input:
    //      pNetImpl:
    //      pPacketParser: 消息包解析对象
    //      dwId: 监听器id
    virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);

    // Summary：
    //      无
    virtual BOOL LPAPI UnInit();

    // Summary:
    //      停止监听
    virtual void LPAPI Stop();

    // Summary:
    //      获取id
    virtual LPUINT32 LPAPI GetId();

    // Summary:
    //      获取句柄
    virtual HANDLE LPAPI GetHandle();

    // Summary:
    //      获取网络事件处理器类型
    virtual e_EventHandlerType LPAPI GetEventHandlerType();

    // Summary:
    //      获取处理器自定义的数据，如果没有则返回null
    virtual PER_IO_DATA* LPAPI GetEventHandlerData();

protected:

    //Summary:
    //      获取监听器状态
    virtual LPUINT32 LPAPI GetState();

    // Summary:
    //      设置监听器状态
    virtual void LPAPI SetState(LPUINT32 dwState);

protected:

    LPUINT32                    m_dwId;
    volatile atomic_uint        m_dwState;

    std::string                 m_strIP;
    LPUINT32                    m_dwPort;
    BOOL                        m_bReUseAddr;
    SOCKET                      m_hListenSock;
    ILPPacketParser*            m_pPacketParser;
    LPNetImpl*                  m_pNetImpl;

    PER_IO_DATA                 m_stPerIoData;//目前只有linux用到

};



// Summary:
//      windows网络通讯方式的listener实现类
class DECLARE LPWinNetListener : public LPListener
{
public:

    // Summary:
    //      无
    LPWinNetListener();

    // Summary:
    //      无
    virtual ~LPWinNetListener();

    // Summary：
    //      无
    virtual BOOL LPAPI UnInit();

    // Summary:
    //      开始监听
    // Input:
    //      strIP: ip地址字符串（格式：192.168.1.1，"0"表示任意地址）
    //      dwPort: 监听端口
    //      bReUseAddr: 是否重复利用地址
    // Return:
    //      TRUE-成功，FALSE-失败
    virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr);

    // Summary:
    //      网络事件处理
    virtual void LPAPI OnNetEvent(PER_IO_DATA* pstPerIoData);

private:

    // Summary:
    //      接受链接回调
    void LPAPI OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

    // Summary:
    //      获取AcceptEx和GetAcceptExSockaddrs函数指针，并且post异步accept操作
    BOOL LPAPI InitAcceptEx();

    // Summary:
    //      post异步accept操作
    // Input:
    //      pstPerIoData：与接受上来的sock绑定的io数据
    // Return:
    //      TRUE-成功，FALSE-失败
    BOOL LPAPI PostAcceptEx(PER_IO_DATA* pstPerIoData);

private:

    LPFN_ACCEPTEX               m_lpfnAcceptEx;
    LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
    PER_IO_DATA*                m_pstPerIoDataArray;

};



// Summary:
//      linux网络通讯方式的listener实现类
class DECLARE LPLinuxNetListener : public LPListener
{
public:

    // Summary:
    //      无
    LPLinuxNetListener();

    // Summary:
    //      无
    virtual ~LPLinuxNetListener();

    // Summary：
    //      无
    virtual BOOL LPAPI UnInit();

    // Summary:
    //      开始监听
    // Input:
    //      strIP: ip地址字符串（格式：192.168.1.1，"0"表示任意地址）
    //      dwPort: 监听端口
    //      bReUseAddr: 是否重复利用地址
    // Return:
    //      TRUE-成功，FALSE-失败
    virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr);

    // Summary:
    //      网络事件处理
    virtual void LPAPI OnNetEvent(PER_IO_DATA* pstPerIoData);

private:

    // Summary:
    //      接受链接回调
    void LPAPI OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

};



//end声明所处的名字空间
NS_LZPL_END

#endif