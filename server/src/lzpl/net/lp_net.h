//****************************************************************************/
//    author: caihy
//    date: Jan 5, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_NET_H_
#define _LP_NET_H_

#include "lpi_net.h"
#include "lp_lock.h"
#include "lpi_sockerimpl.h"
#include "lpi_listenerimpl.h"
#include "lpi_connectorimpl.h"
#include "lp_netevent.h"
#include "lpi_reactor.h"
#include "lp_sockermgr.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//Summary:
//      windows平台的net组件实现类
class DECLARE LPNetImpl : public ILPNet, public lp_enable_shared_from_this<LPNetImpl>
{
public:

    // Summary：
    //     无
    LPNetImpl();

    // Summary：
    //     无
    ~LPNetImpl();

public:

    // Summary:
    //      创建ILPListener监听控制器对象
    //Input:
    //      pPacketParser：解析对象
    //Return:
    //      监听器对象
    virtual std::shared_ptr<ILPListener> LPAPI CreateListenerCtrl(lp_shared_ptr<ILPPacketParser> pPacketParser);

    // Summary:
    //      创建ILPConnector连接器对象
    //Input
    //      pPacketParser：解析对象
    //Return:
    //      连接器对象
    virtual std::shared_ptr<ILPConnector> LPAPI CreateConnectorCtrl(lp_shared_ptr<ILPPacketParser> pPacketParser);

    // Summary:
    //      处理网络包函数
    //Input:
    //      nCount-每次检测处理网络包的最大数量，-1表示处理所有
    //Return:
    //      TRUE-所有网络包都已被处理，FALSE-有剩余未处理网络包
    virtual BOOL LPAPI Run(LPINT32 nCount = -1);

    // Summary:
    //      初始化函数
    //Return:
    //      成功返回TRUE，失败返回FALSE
    BOOL LPAPI Init(lp_shared_ptr<ILPNetMessageHandler> pNetMessageHandler, NET_CONFIG* pNetConfig);

    // Summary:
    //      反初始化函数
    //Return:
    //      成功返回TRUE，失败返回FALSE
    void LPAPI UnInit();

    // Summary:
    //      查找监听器对象
    std::shared_ptr<ILPListenerImpl> LPAPI FindListener(LPUINT32 dwId);

    // Summary:
    //      查找连接器对象
    std::shared_ptr<ILPConnectorImpl> LPAPI FindConnector(LPUINT32 dwId);

    // Summary:
    //      无
    LPSockerMgr& LPAPI GetSockerMgr();

    // Summary:
    //      无
    LPEventMgr& LPAPI GetEventMgr();

    // Summary:
    //      无
    NET_CONFIG&  LPAPI GetNetConfig();

protected:

    // Summary:
    //      创建id
    LPUINT32 LPAPI _CreateId();

protected:

    LPUINT32                                m_dwState;
    LPUINT32                                m_dwRef;
    LPUINT32                                m_dwMaxCreateId;
    LPLock                                  m_oLock;
    lp_shared_ptr<ILPNetMessageHandler>     m_pNetMessageHandler;
    NET_CONFIG                              m_oNetConfig;

    LPSockerMgr                             m_oSockerMgr;
    LPEventMgr                              m_oEventMgr;
    std::shared_ptr<ILPReactor>             m_pReactor;

    MAP_LISTENER                            m_mapListener;
    MAP_CONNECTOR                           m_mapConnector;
};



//end声明所处的名字空间
NS_LZPL_END

#endif