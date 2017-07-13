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



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//      socker���������
class DECLARE LPSockerMgr
{
public:

    // Summary��
    //     ���캯��
    // Returns:
    //     ��
    LPSockerMgr();

    // Summary��
    //     ��������
    // Returns:
    //     ��
    ~LPSockerMgr();

public:

    typedef std::map<LPUINT32, lp_shared_ptr<ILPSockerImpl>> LPMapSocker;

public:

    // Summary:
    //      ��ʼ������
    // Input:
    //      pNetImpl:
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    BOOL LPAPI Init(lp_shared_ptr<LPNetImpl> pNetImpl);

    // Summary:
    //      �������
    BOOL LPAPI UnInit();

    // Summary:
    //      ����socker����
    // Input:
    //      pPacketParser����������
    //      dwParentId���������������������������id
    //      bAcceptCreate���Ƿ�accept����
    // Return:
    //      socker����
    lp_shared_ptr<ILPSockerImpl> LPAPI Create(lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

    // Summary:
    //      �ͷ�socker����
    // Input:
    //      pSocker��socker����
    void LPAPI Release(lp_shared_ptr<ILPSockerImpl> pSocker);

    // Summary:
    //      ����socker
    lp_shared_ptr<ILPSockerImpl> LPAPI Find(LPUINT32 dwSockerId);

    // Summary:
    //      ��ȡ��ǰ��Ч�����Ӹ���

    LPUINT32 LPAPI GetCurValidConnectCount();

private:

    // Summary:
    //      ����socker����
    // Input:
    //      pPacketParser����������
    //      dwParentId���������������������������id
    //      bAcceptCreate���Ƿ�accept����
    // Return:
    //      socker����
    lp_shared_ptr<ILPSockerImpl> LPAPI _Create(lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

    // Summary:
    //      �ͷ�socker����
    // Input:
    //      pSocker��socker����
    void LPAPI _Release(lp_shared_ptr<ILPSockerImpl> pSocker);

    // Summary:
    //      ����sock id
    // Return:
    //      sock id
    LPUINT32 LPAPI _CreateSockId();

private:

    LPUINT32                        m_dwMaxSockId;       // ���sock id
    LPMapSocker                     m_oValidMap;         // ��ǰ������Ч��socker map
    LPLock                          m_oValidLock;        // ��ǰ������Ч��socker map��

    lp_shared_ptr<LPNetImpl>        m_pNetImpl;          //
    lp_shared_ptr<ILPLoopBufPool>   m_pRecvLoopBufPool;  // ���ܻ������أ���Ҫ֧�ֶ��size����ʹ��map pool��
    lp_shared_ptr<ILPLoopBufPool>   m_pSendLoopBufPool;  // ���ͻ������أ���Ҫ֧�ֶ��size����ʹ��map pool��
};



//end�������������ֿռ�
NS_LZPL_END

#endif