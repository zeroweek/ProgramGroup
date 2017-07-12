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
    virtual BOOL LPAPI Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId);

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
    //      停止监听
    virtual void LPAPI Close();

    // Summary:
    //      获取id
    virtual LPUINT32 LPAPI GetId();

    // Summary:
    //      获取ip字符串
    virtual std::string& LPAPI GetIp();

    // Summary:
    //      获取port
    virtual LPUINT16 LPAPI GetPort();

protected:

    //Summary:
    //      获取监听器状态
    virtual LPUINT32 LPAPI GetState();

    // Summary:
    //      设置监听器状态
    virtual void LPAPI SetState(LPUINT32 dwState);

    // Summary:
    //      post异步accept操作
    // Return:
    //      TRUE-成功，FALSE-失败
    virtual BOOL LPAPI PostAccept();

    // Summary:
    //      设置监听器状态
    virtual void LPAPI HandleAccept(lp_shared_ptr<ip::tcp::socket> pSocket, const system::error_code& err);

protected:

    LPUINT32                            m_dwId;
    volatile atomic_uint                m_dwState;

    std::string                         m_strIP;
    LPUINT32                            m_dwPort;
    BOOL                                m_bReUseAddr;

    lp_shared_ptr<ILPPacketParser>      m_pPacketParser;
    lp_shared_ptr<LPNetImpl>            m_pNetImpl;
    lp_shared_ptr<ip::tcp::acceptor>    m_pAcceptor;
    lp_shared_ptr<ILPReactor>           m_pReactor;
};



//end声明所处的名字空间
NS_LZPL_END

#endif