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



//begin�������������ֿռ�
NS_LZPL_BEGIN



//Summary:
//      windowsƽ̨��net���ʵ����
class DECLARE LPNetImpl : public ILPNet, public lp_enable_shared_from_this<LPNetImpl>
{
public:

    // Summary��
    //     ��
    LPNetImpl();

    // Summary��
    //     ��
    ~LPNetImpl();

public:

    // Summary:
    //      ����ILPListener��������������
    //Input:
    //      pPacketParser����������
    //Return:
    //      ����������
    virtual std::shared_ptr<ILPListener> LPAPI CreateListenerCtrl(lp_shared_ptr<ILPPacketParser> pPacketParser);

    // Summary:
    //      ����ILPConnector����������
    //Input
    //      pPacketParser����������
    //Return:
    //      ����������
    virtual std::shared_ptr<ILPConnector> LPAPI CreateConnectorCtrl(lp_shared_ptr<ILPPacketParser> pPacketParser);

    // Summary:
    //      �������������
    //Input:
    //      nCount-ÿ�μ�⴦������������������-1��ʾ��������
    //Return:
    //      TRUE-������������ѱ�����FALSE-��ʣ��δ���������
    virtual BOOL LPAPI Run(LPINT32 nCount = -1);

    // Summary:
    //      ��ʼ������
    //Return:
    //      �ɹ�����TRUE��ʧ�ܷ���FALSE
    BOOL LPAPI Init(lp_shared_ptr<ILPNetMessageHandler> pNetMessageHandler, NET_CONFIG* pNetConfig);

    // Summary:
    //      ����ʼ������
    //Return:
    //      �ɹ�����TRUE��ʧ�ܷ���FALSE
    void LPAPI UnInit();

    // Summary:
    //      ���Ҽ���������
    std::shared_ptr<ILPListenerImpl> LPAPI FindListener(LPUINT32 dwId);

    // Summary:
    //      ��������������
    std::shared_ptr<ILPConnectorImpl> LPAPI FindConnector(LPUINT32 dwId);

    // Summary:
    //      ��
    LPSockerMgr& LPAPI GetSockerMgr();

    // Summary:
    //      ��
    LPEventMgr& LPAPI GetEventMgr();

    // Summary:
    //      ��
    NET_CONFIG&  LPAPI GetNetConfig();

protected:

    // Summary:
    //      ����id
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



//end�������������ֿռ�
NS_LZPL_END

#endif