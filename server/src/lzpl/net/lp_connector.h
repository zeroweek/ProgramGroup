//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_CONNECTOR_H_
#define _LP_CONNECTOR_H_

#include "lpi_connectorimpl.h"
#include "lpi_sockerimpl.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      连接器类
class DECLARE LPConnector : public ILPConnectorImpl
{
public:

    // Summary:
    //      无
    LPConnector();

    // Summary:
    //      无
    virtual ~LPConnector();

    // Summary：
    //      无
    // Input:
    //      pNetImpl:
    //      pPacketParser: 消息包解析对象
    //      dwId: 连接器id
    virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId);

    // Summary：
    //      无
    virtual BOOL LPAPI UnInit();

    // Summary:
    //      开始连接
    // Input:
    //      strIP: ip地址字符串（格式：192.168.1.1）
    //      dwPort: 连接的端口
    //      bAutoReconnect: 是否自动重连接
    // Return:
    //      TRUE-成功，FALSE-失败
    virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect);

    // Summary:
    //      停止连接
    virtual void LPAPI Close();

    // Summary:
    //      获取id
    virtual LPUINT32 LPAPI GetId();

    // Summary:
    //      重连
    virtual BOOL LPAPI Reconnect();

    // Summary:
    //      链接关闭回调
    virtual void LPAPI OnConnectionClose();

    // Summary:
    //      设置是否自动重连
    virtual void LPAPI SetReconnect(BOOL bReconnect);

    // Summary:
    //      是否自动重连
    virtual BOOL LPAPI IsReconnect();

    // Summary:
    //      设置关联的socker对象
    virtual void LPAPI SetSocker(lp_shared_ptr<ILPSockerImpl> pSocker);

    // Summary:
    //      获取关联的socker对象
    virtual lp_shared_ptr<ILPSockerImpl> LPAPI GetSocker();

    // Summary:
    //      设置连接器当前状态
    virtual void LPAPI SetState(LPUINT32 dwState);

    //Summary:
    //      获取连接器当前状态
    virtual LPUINT32 LPAPI GetState();

protected:

    // Summary:
    //   关闭socket，正常情况下，不应该调用这个接口，而是统一由LPSocker来执行关闭
    //   ，本函数仅限在connect状态不对时调用
    void LPAPI CloseConnectSockWhileStateError();

    // Summary:
    //      post异步connect操作
    // Return:
    //      TRUE-成功，FALSE-失败
    virtual BOOL LPAPI PostConnect();

    // Summary:
    //      设置监听器状态
    virtual void LPAPI HandleConnect(const system::error_code& err);

protected:

    LPUINT32                            m_dwId;
    volatile atomic_uint                m_dwState;

    std::string                         m_strIP;
    LPUINT32                            m_dwPort;
    BOOL                                m_bReconnect;
    lp_shared_ptr<ILPPacketParser>      m_pPacketParser;
    LPNetImpl*                          m_pNetImpl;
    lp_shared_ptr<ILPSockerImpl>        m_pSocker;

    lp_shared_ptr<ILPReactor>           m_pReactor;
    lp_shared_ptr<ip::tcp::socket>      m_pSocket;
};



//end声明所处的名字空间
NS_LZPL_END

#endif