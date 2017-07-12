//****************************************************************************/
//    author: caihy
//    date: Jan 9, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_SOCKER_MGR_H_
#define _LP_SOCKER_MGR_H_

#include "lpi_sockerimpl.h"
#include "lp_objpool.h"
#include "lp_lock.h"
#include "lp_thread.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;



// Summary:
//      socker对象管理类
class DECLARE LPSockerMgr
{
public:

    // Summary：
    //     构造函数
    // Returns:
    //     无
    LPSockerMgr();

    // Summary：
    //     析构函数
    // Returns:
    //     无
    ~LPSockerMgr();

public:

    typedef std::list<lp_shared_ptr<ILPSockerImpl>> LPListSocker;
    typedef std::map<LPUINT32, lp_shared_ptr<ILPSockerImpl>> LPMapSocker;

public:

    // Summary:
    //      初始化函数
    // Input:
    //      pNetImpl:
    // Return:
    //      TRUE-成功，FALSE-失败
    BOOL LPAPI Init(LPNetImpl* pNetImpl);

    // Summary:
    //      清除函数
    BOOL LPAPI UnInit();

    // Summary:
    //      创建socker对象
    // Input:
    //      pPacketParser：解析对象
    //      dwParentId：创建对象（连接器或控制器）的id
    //      bAcceptCreate：是否accept创建
    // Return:
    //      socker对象
    lp_shared_ptr<ILPSockerImpl> LPAPI Create(lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

    // Summary:
    //      释放socker对象
    // Input:
    //      pSocker：socker对象
    void LPAPI Release(lp_shared_ptr<ILPSockerImpl> pSocker);

    // Summary:
    //      查找socker
    lp_shared_ptr<ILPSockerImpl> LPAPI Find(LPUINT32 dwSockerId);

    // Summary:
    //      获取当前有效的连接个数

    LPUINT32 LPAPI GetCurValidConnectCount();

private:

    // Summary:
    //      创建socker对象
    // Input:
    //      pPacketParser：解析对象
    //      dwParentId：创建对象（连接器或控制器）的id
    //      bAcceptCreate：是否accept创建
    // Return:
    //      socker对象
    lp_shared_ptr<ILPSockerImpl> LPAPI _Create(lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

    // Summary:
    //      释放socker对象
    // Input:
    //      pSocker：socker对象
    void LPAPI _Release(lp_shared_ptr<ILPSockerImpl> pSocker);

    // Summary:
    //      创建sock id
    // Return:
    //      sock id
    LPUINT32 LPAPI _CreateSockId();

private:

    LPUINT32                   m_dwMaxSockId;       // 最大sock id
    LPMapSocker                m_oValidMap;         // 当前连接有效的socker map
    LPLock                     m_oValidLock;        // 当前连接有效的socker map锁

    LPNetImpl*                 m_pNetImpl;          //
    LPLoopBufPool*             m_pRecvLoopBufPool;  // 接受缓冲区池（若要支持多个size，可使用map pool）
    LPLoopBufPool*             m_pSendLoopBufPool;  // 发送缓冲区池（若要支持多个size，可使用map pool）
};



//end声明所处的名字空间
NS_LZPL_END

#endif