//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_SOCKER_H_
#define _LP_SOCKER_H_

#include "lpi_socker.h"
#include "lp_objpool.h"
#include "lpi_eventhandler.h"
#include "lp_lock.h"
#include "lp_thread.h"
#include "lpi_packetparser.h"
#include "lp_loopbuf.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//		�ײ�socke����t��װ��
class DECLARE LPSocker : public ILPSocker, public ILPEventHandler
{
public:

	// Summary:
	//		��
	LPSocker();

	// Summary:
	//		��
	virtual ~LPSocker();

	// Summary:
	//		��ȡ�ײ�sock�ľ��
	// Return:
	//		���صײ�sock���
	virtual SOCKET LPAPI GetSock();

	// Summary:
	//		��ȡsocker id
	// Return:
	//		����socker id
	virtual LPUINT32 LPAPI GetSockerId();

	// Summary:
	//		��ȡ�Զ����ӵ�ip
	virtual LPUINT32 LPAPI GetRemoteIp();

	// Summary:
	//		��ȡ�Զ����ӵ�ip�ַ���
	virtual const char* LPAPI GetRemoteIpStr();

	// Summary:
	//		��ȡ�Զ����ӵ�port
	virtual LPUINT16 LPAPI GetRemotePort();

	// Summary:
	//		��ȡ�������ӵ�ip
	virtual LPUINT32 LPAPI GetLocalIp();

	// Summary:
	//		��ȡ�������ӵ�ip�ַ���
	virtual const char* LPAPI GetLocalIpStr();

	// Summary:
	//		��ȡ�������ӵ�port
	virtual LPUINT16 LPAPI GetLocalPort();

	// Summary:
	//		�жϵ�ǰ��socket�Ƿ�������״̬
	// Return:
	//		TRUE-���ӣ�FALSE-�Ͽ�
	virtual BOOL LPAPI IsConnect();

	// Summary:
	//		��������
	// Input:
	//		pData������
	//		dwLen������
	virtual BOOL LPAPI Send(const char* pData, LPUINT32 dwLen);

	// Summary:
	//		�ر����ӣ������������رջ��Ǳ����رգ���ͳһ���ô˽ӿڹر��Ѿ�������socker
	virtual void LPAPI Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose);

	// Summary:
	//		��ȡ���
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		��ȡ�����¼�����������
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		�����¼�����
	virtual void LPAPI OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData);

	// Summary:
	//		����sock���
	// Input:
	//		sock���ײ�socket���ֵ
	void LPAPI SetSock(SOCKET sock);

	// Summary:
	//		��������״̬���ֵ
	// Input:
	//		bConnect��true-���ӣ�false-�Ͽ�
	void LPAPI SetConnect(bool bConnect);

	// Summary:
	//		�󶨽�������
	// Input:
	//		pPacketParser����������
	void LPAPI AttachPacketParser(ILPPacketParser* pPacketParser);

	// Summary:
	//		����󶨽�������
	void LPAPI DetachPacketParser();

	// Summary:
	//		����socker����
	void LPAPI Reset();

	// Summary:
	//		post�첽�������ݲ���
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI PostRecv();

	// Summary:
	//		post�첽�������ݲ���
	// Return:
	//		TRUE-�з������ݣ�FALSE-û��������
	BOOL LPAPI PostSend();

	// Summary:
	//		�յ���Ϣ�ص�
	// Input:
	//		dwBytes�����յ����ֽ���
	void LPAPI OnRecv(LPUINT32 dwBytes);

	// Summary:
	//		������Ϣ�ص�
	// Input:
	//		dwBytes�����յ����ֽ���
	void LPAPI OnSend(LPUINT32 dwBytes);

	// Summary:
	//		���ý��ջ�����
	// Input:
	//		pBuf��������
	void LPAPI AttachRecvBuf(LPLoopBuf* pLoopBuf);

	// Summary:
	//		������ջ�����
	// Return:
	//		���ջ�����
	LPLoopBuf* LPAPI DetachRecvBuf();

	// Summary:
	//		���÷��ͻ�����
	// Input:
	//		pBuf��������
	void LPAPI AttachSendBuf(LPLoopBuf* pLoopBuf);

	// Summary:
	//		���뷢�ͻ�����
	// Return:
	//		���ͻ�����
	LPLoopBuf* LPAPI DetachSendBuf();

	// Summary:
	//		����Զ������ip
	// Input:
	//		dwIp��ip
	void LPAPI SetRemoteIp(LPUINT32 dwIp);

	// Summary:
	//		����Զ�����Ӷ˿�
	// Input:
	//		wPort���˿�
	void LPAPI SetRemotePort(LPUINT16 wPort);

	// Summary:
	//		���ñ�������ip
	// Input:
	//		dwIp��ip
	void LPAPI SetLocalIp(LPUINT32 dwIp);

	// Summary:
	//		���ñ������Ӷ˿�
	// Input:
	//		wPort���˿�
	void LPAPI SetLocalPort(LPUINT16 wPort);

	// Summary:
	//		���ÿ�ʼ�ӳٹر�tick
	void LPAPI SetDelayCloseBeginTick(LPUINT64 qwTick);

	// Summary:
	//		��ȡ��ʼ�ӳٹر�tick
	LPUINT64 LPAPI GetDelayCloseBeginTick();

	// Summary:
	//		�����ӳٹرճ���ʱ��
	void LPAPI SetDelayCloseDuration(LPUINT64 qwDuration);

	// Summary:
	//		��ȡ�ӳٹرճ���ʱ��
	LPUINT64 LPAPI GetDelayCloseDuration();

	// Summary:
	//		���ÿ�ʼ�ӳ��ͷ�tick
	void LPAPI SetDelayReleaseBeginTick(LPUINT64 qwTick);

	// Summary:
	//		��ȡ��ʼ�ӳ��ͷ�tick
	LPUINT64 LPAPI GetDelayReleaseBeginTick();

	// Summary:
	//		�����ӳ��ͷų���ʱ��
	void LPAPI SetDelayReleaseDuration(LPUINT64 qwDuration);

	// Summary:
	//		��ȡ�ӳ��ͷų���ʱ��
	LPUINT64 LPAPI GetDelayReleaseDuration();

	// Summary:
	//		����socker id
	// Input:
	//		dwSockerId��socker id
	void LPAPI SetSockerId(LPUINT32 dwSockerId);

	// Summary:
	//		�첽�رջص�
	virtual void LPAPI OnClose();

	// Summary:
	//		���ø��������id�����������������id��
	// Input:
	//		dwParentId��id
	void LPAPI SetParentId(LPUINT32 dwParentId);

	// Summary:
	//		��ȡ���������id�����������������id��
	// Return:
	//		id
	virtual LPUINT32 LPAPI GetParentId();

	// Summary:
	//		�����Ƿ���accept�����ı�ǣ�accept����connect����
	void LPAPI SetAcceptCreate(BOOL bAcceptCreate);

	// Summary:
	//		�ж��Ƿ���accept����
	//	Return:
	//		TRUE-�ǣ�FALSE-����
	virtual BOOL LPAPI IsAcceptCreate();

	// Summary:
	//		�����Ƿ񱻶��ر�
	virtual void LPAPI SetPassiveClose(BOOL bPassiveClose);

	// Summary:
	//		�ж��Ƿ񱻶��ر�
	//	Return:
	//		TRUE-�ǣ�FALSE-����
	virtual BOOL LPAPI IsPassiveClose();

	// Summary:
	//		��
	void LPAPI SetNetImpl(LPNetImpl* pNetImpl);

protected:

	volatile atomic_bool      m_bConnect;                 // ����״̬���
	volatile atomic_bool      m_bSending;                 // �����Ƿ����ڷ��ͱ��

	BOOL                      m_bAcceptCreate;            // �Ƿ���accept����
	BOOL                      m_bPassiveClose;            // �Ƿ񱻶��ر�
	SOCKET                    m_hSock;                    // sock���
	LPUINT32                   m_dwSockerId;               // socker id
	LPUINT32                   m_dwParentId;               // ���������id�����������������id��
	ILPPacketParser*          m_pPacketParser;            // ��������
	LPNetImpl*                m_pNetImpl;                 //

	LPUINT32                   m_dwRemoteIp;               // Զ��ip
	LPUINT16                   m_wRemotePort;              // Զ�˶˿�
	char                      m_szRemoteIpStr[IP_LEN];    // Զ��ip�ַ���
	LPUINT32                   m_dwLocalIp;                // ����ip
	LPUINT16                   m_wLocalPort;               // ���ض˿�
	char                      m_szLocalIpStr[IP_LEN];     // ����ip�ַ���

	LPLoopBuf*                m_pRecvLoopBuf;             // ���ջ�������ֻ��PostRecv��OnRecv�в���������д������Ϊû��PostRecv�ǲ����յ�OnRecv��
	LPLoopBuf*                m_pSendLoopBuf;             // ���ͻ����������߳�д������������Ҫ����

	LPUINT64                   m_qwDelayCloseBeginTick;    // �ӳٹرտ�ʼtick
	LPUINT64                   m_qwDelayCloseDuration;     // �ӳٹرճ���ʱ��
	LPUINT64                   m_qwDelayReleaseBeginTick;  // �ӳ��ͷſ�ʼtick
	LPUINT64                   m_qwDelayReleaseDuration;   // �ӳ��ͷų���ʱ��

	PER_IO_DATA               m_stRecvPerIoData;          // io���հ󶨵����� 
	PER_IO_DATA               m_stSendPerIoData;          // io���Ͱ󶨵�����

	static LPLock             m_oSendBufLock;             // ���ͻ�����������̬���ǷǾ�̬������������������sockЧ��ƽ�⣩
};



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

	typedef std::list<LPSocker*> LPListSocker;
	typedef std::map<LPUINT32, LPSocker*> LPMapSocker;

public:

	// Summary:
	//		��ʼ������
	// Input:
	//		pNetImpl: 
	//		dwSize��socker����س�ʼ����С
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	BOOL LPAPI Init(LPNetImpl* pNetImpl, LPUINT32 dwSize);

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
	LPSocker* LPAPI Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		�ͷ�socker����
	// Input:
	//		pSocker��socker����
	void LPAPI Release(LPSocker* pSocker);

	// Summary:
	//		�ӳٹر�socker����
	// Input:
	//		pSocker��socker����
	void LPAPI DelayClose(LPSocker* pSocker);

	// Summary:
	//		�ӳ��ͷ�socker����ע����Release����λ�õ�����
	// Input:
	//		pSocker��socker����
	void LPAPI DelayRelease(LPSocker* pSocker);

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
	LPSocker* LPAPI Find(LPUINT32 dwSockerId);

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
	LPSocker* LPAPI _Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate);

	// Summary:
	//		�ͷ�socker����
	// Input:
	//		pSocker��socker����
	void LPAPI _Release(LPSocker* pSocker);

	// Summary:
	//		����sock id
	// Return:
	//		sock id
	LPUINT32 LPAPI _CreateSockId();

	// Summary:
	//		Malloc
	LPSocker* LPAPI _MallocSocker();

	// Summary:
	//		Free
	void LPAPI _FreeSocker(LPSocker* pSocker);

private:

	static THREAD_FUNC_DECLARE(_CheckDelayThreadProc)(void* pParam);
	static THREAD_FUNC_DECLARE(_PostSendThreadProc)(void* pParam);

private:

	LPUINT32                   m_dwMaxSockId;        // ���sock id
	LPObjPool<LPSocker>        m_oSockerPool;        // socker�����
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