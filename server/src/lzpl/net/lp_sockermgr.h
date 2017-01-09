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
//		socker���������
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

	typedef std::list<ILPSockerImpl*> LPListSocker;
	typedef std::map<LPUINT32, ILPSockerImpl*> LPMapSocker;

public:

	// Summary:
	//		��ʼ������
	// Input:
	//		pNetImpl: 
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	BOOL LPAPI Init(LPNetImpl* pNetImpl);

	// Summary:
	//		�������
	BOOL LPAPI UnInit();

	// Summary:
	//		����socker����
	// Input:
	//		pPacketParser����������
	//		dwParentId���������������������������id
	//		bAcceptCreate���Ƿ�accept����
	// Return:
	//		socker����
	ILPSockerImpl* LPAPI Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		�ͷ�socker����
	// Input:
	//		pSocker��socker����
	void LPAPI Release(ILPSockerImpl* pSocker);

	// Summary:
	//		�ӳٹر�socker����
	// Input:
	//		pSocker��socker����
	void LPAPI DelayClose(ILPSockerImpl* pSocker);

	// Summary:
	//		�ӳ��ͷ�socker����ע����Release����λ�õ�����
	// Input:
	//		pSocker��socker����
	void LPAPI DelayRelease(ILPSockerImpl* pSocker);

	// Summary:
	//		����ӳ��ͷŵĶ���
	void LPAPI CheckDelay();

	// Summary:
	//		��������
	// Return:
	//		�����ݷ��͵�LPSocker�������
	LPINT32 LPAPI PostSend();

	// Summary:
	//		����socker
	ILPSockerImpl* LPAPI Find(LPUINT32 dwSockerId);

	// Summary:
	//		��ȡ��ǰ��Ч�����Ӹ���

	LPUINT32 LPAPI GetCurValidConnectCount();

private:

	// Summary:
	//		����socker����
	// Input:
	//		pPacketParser����������
	//		dwParentId���������������������������id
	//		bAcceptCreate���Ƿ�accept����
	// Return:
	//		socker����
	ILPSockerImpl* LPAPI _Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		�ͷ�socker����
	// Input:
	//		pSocker��socker����
	void LPAPI _Release(ILPSockerImpl* pSocker);

	// Summary:
	//		����sock id
	// Return:
	//		sock id
	LPUINT32 LPAPI _CreateSockId();

private:

	static THREAD_FUNC_DECLARE(_CheckDelayThreadProc)(void* pParam);
	static THREAD_FUNC_DECLARE(_PostSendThreadProc)(void* pParam);

private:

	LPUINT32                   m_dwMaxSockId;        // ���sock id
	LPListSocker               m_oValidList;         // ��ǰ������Ч��socker�б�
	LPMapSocker                m_oValidMap;          // ��ǰ������Ч��socker map
	LPListSocker               m_oDelayCloseList;    // �ӳٹر������б�
	LPListSocker               m_oDelayReleaseList;  // �ӳ��ͷŵ�socker�б�
	LPLock                     m_oLock;              // ��
	LPLock                     m_oDelayLock;         // �ӳ��ͷ��б���
	BOOL                       m_bCheckDelayRun;     // check delay�߳��Ƿ�ֹͣ
	BOOL                       m_bPostSendRun;       // post send�߳��Ƿ�ֹͣ
	LPThread                   m_oCheckDelayThread;  // check delay�߳�
	LPThread                   m_oPostSendThread;    // post send�߳�

	LPNetImpl*                 m_pNetImpl;           // 
	LPLoopBufPool*             m_pRecvLoopBufPool;   // ���ܻ������أ���Ҫ֧�ֶ��size����ʹ��map pool��
	LPLoopBufPool*             m_pSendLoopBufPool;   // ���ͻ������أ���Ҫ֧�ֶ��size����ʹ��map pool��

	time_t                     m_tLastCheckTime;
};



//end�������������ֿռ�
NS_LZPL_END

#endif