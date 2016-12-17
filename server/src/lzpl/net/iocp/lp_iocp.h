//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_IOCP_H_
#define _LP_IOCP_H_

#ifdef _WIN32


#include "lp_base.h"
#include "lpi_sockoption.h"
#include "lp_objpool.h"
#include "lpi_socker.h"
#include "lpi_eventhandler.h"
#include "lp_loopbuf.h"
#include "lp_lock.h"
#include "lpi_packetparser.h"
#include "lp_system.h"
#include "lp_lock.h"
#include "lp_thread.h"
#include "lpi_packetparser.h"
#include "lpi_connector.h"
#include "lpi_eventhandler.h"
#include <MSWSock.h>
#include "lpi_listener.h"
#include "lpi_connector.h"
#include "lpi_netmessagehandler.h"
#include "lpi_reactor.h"
#include "lpi_net.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN


#define POST_ACCEPTEX_COUNT            (128)



class LPSockerMgr;
class LPReactorImpl;
class LPNetImpl;


enum eCommonState
{
	eCommonState_NoInit,
	eCommonState_Initing,
	eCommonState_Inited,
	eCommonState_Close,
	eCommonState_UnIniting,
	eCommonState_UnInited,
};


// Summary:
//		ÿ��io��Ӧ�İ�����
struct PER_IO_DATA
{
	OVERLAPPED             stOverlapped;
	SOCKET                 hSock;
	e_IoOptType            eIoOptType;
	e_EventHandlerType     eHandlerType;
	WSABUF                 stWsaBuf;
	char                   szBuf[128];

	PER_IO_DATA()
	{
		memset(&stOverlapped, 0, sizeof(stOverlapped));
		hSock = INVALID_SOCKET;
		eIoOptType = eIoOptType_None;
		eHandlerType = eEventHandlerType_None;
		memset(&stWsaBuf, 0, sizeof(stWsaBuf));
		memset(szBuf, 0, sizeof(szBuf));
	}
};



//****************************************************************************/
//	����ѡ��ӿ���
//****************************************************************************/
// Summary:
//		����ѡ����
class DECLARE LPNetOption : public ILPNetOption
{
public:

	// Summary:
	//		none
	LPNetOption();

	// Summary:
	//		none
	virtual ~LPNetOption();

};



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
	//		��ȡ�¼�����������
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

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
	BOOL LPAPI PostRecv();

	// Summary:
	//		post�첽�������ݲ���
	// Return:
	//		TRUE-�з������ݣ�FALSE-û��������
	BOOL LPAPI PostSend();

	// Summary:
	//		�յ���Ϣ�ص�
	// Input:
	//		dwBytes�����յ����ֽ���
	void LPAPI OnRecv(DWORD dwBytes);

	// Summary:
	//		������Ϣ�ص�
	// Input:
	//		dwBytes�����յ����ֽ���
	void LPAPI OnSend(DWORD dwBytes);

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
	void LPAPI OnClose();

	// Summary:
	//		���ø��������id�����������������id��
	// Input:
	//		dwParentId��id
	void LPAPI SetParentId(LPUINT32 dwParentId);

	// Summary:
	//		��ȡ���������id�����������������id��
	// Return:
	//		id
	LPUINT32 LPAPI GetParentId();

	// Summary:
	//		�����Ƿ���accept�����ı�ǣ�accept����connect����
	void LPAPI SetAcceptCreate(BOOL bAcceptCreate);

	// Summary:
	//		�ж��Ƿ���accept����
	//	Return:
	//		TRUE-�ǣ�FALSE-����
	BOOL LPAPI IsAcceptCreate();

	// Summary:
	//		�����Ƿ񱻶��ر�
	void LPAPI SetPassiveClose(BOOL bPassiveClose);

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

	LPUINT32                    m_dwMaxSockId;        // ���sock id
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



// Summary:
//		��������
class DECLARE LPConnector : public ILPConnector, public ILPEventHandler
{
public:

	// Summary:
	//		��
	LPConnector();

	// Summary:
	//		��
	virtual ~LPConnector();

	// Summary��
	//		��
	// Input:
	//		pNetImpl: 
	//		pPacketParser: ��Ϣ����������
	//		dwId: ������id
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);
	// Summary��
	//		��     
	BOOL LPAPI UnInit();

	// Summary:
	//		��ʼ����
	// Input:
	//		pcszIp: ip��ַ�ַ�������ʽ��192.168.1.1��
	//		dwPort: ���ӵĶ˿�
	//		bAutoReconnect: �Ƿ��Զ�������
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReconnect);

	// Summary:
	//		ֹͣ����
	virtual void LPAPI Stop();

	// Summary:
	//		��ȡid
	virtual LPUINT32 LPAPI GetId();

	// Summary:
	//		����
	virtual BOOL LPAPI Reconnect();

	// Summary:
	//		�ͷ�
	virtual void LPAPI Release();

	// Summary:
	//		��ȡ���
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		��ȡ�¼�����������
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		���ӷ��ؽ���ص�
	void LPAPI OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

	// Summary:
	//		���ӹرջص�
	void LPAPI OnClose();

	// Summary:
	//		�����Ƿ��Զ�����
	void LPAPI SetReconnect(BOOL bReconnect);

	// Summary:
	//		�Ƿ��Զ�����
	BOOL LPAPI IsReconnect();

	// Summary:
	//		���ù�����socker����
	void LPAPI SetSocker(LPSocker* pSocker);

	// Summary:
	//		��ȡ������socker����
	LPSocker* LPAPI GetSocker();

	// Summary:
	//		������������ǰ״̬
	void LPAPI SetState(LPUINT32 dwState);

	//Summary:
	//		��ȡ��������ǰ״̬
	LPUINT32 LPAPI GetState();

protected:

	// Summary:
	//		��ȡConnectEx����ָ�룬���ҵ����첽connect����
	BOOL LPAPI _InitConnectEx();

	// Summary:
	//		post�첽connect����
	// Input:
	//		pstPerIoData�������ӽ�����sock�󶨵�io����
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	BOOL LPAPI _PostConnectEx(PER_IO_DATA* pstPerIoData);

private:

	LPUINT32                     m_dwId;
	volatile atomic_uint        m_dwState;

	char                        m_szIp[IP_LEN];
	LPUINT32                     m_dwPort;
	BOOL                        m_bReconnect;
	SOCKET                      m_hConnectSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;
	LPSocker*                   m_pSocker;

	LPFN_CONNECTEX              m_lpfnConnectEx;
	PER_IO_DATA*                m_pstPerIoData;
};

typedef std::map<LPUINT32, LPConnector*> MAP_CONNECTOR;



// Summary:
//		��������
class DECLARE LPListener : public ILPListener, public ILPEventHandler
{
public:

	// Summary:
	//		��
	LPListener();

	// Summary:
	//		��
	virtual ~LPListener();

	// Summary��
	//		��
	// Input:
	//		pNetImpl: 
	//		pPacketParser: ��Ϣ����������
	//		dwId: ������id
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId);
	// Summary��
	//		��     
	BOOL LPAPI UnInit();

	// Summary:
	//		��ʼ����
	// Input:
	//		pcszIp: ip��ַ�ַ�������ʽ��192.168.1.1��
	//		dwPort: �����˿�
	//		bReUseAddr: �Ƿ��ظ����õ�ַ
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReUseAddr);

	// Summary:
	//		ֹͣ����
	virtual void LPAPI Stop();

	// Summary:
	//		��ȡid
	virtual LPUINT32 LPAPI GetId();

	// Summary:
	//		�ͷ�
	virtual void LPAPI Release();

	// Summary:
	//		��ȡ���
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		��ȡ�¼�����������
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		�������ӻص�
	void LPAPI OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData);

protected:

	//Summary:
	//		��ȡ������״̬
	LPUINT32 LPAPI _GetState();

	// Summary:
	//		���ü�����״̬
	void LPAPI _SetState(LPUINT32 dwState);

	// Summary:
	//		��ȡAcceptEx��GetAcceptExSockaddrs����ָ�룬����post�첽accept����
	BOOL LPAPI _InitAcceptEx();

	// Summary:
	//		post�첽accept����
	// Input:
	//		pstPerIoData�������������sock�󶨵�io����
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	BOOL LPAPI _PostAcceptEx(PER_IO_DATA* pstPerIoData);

private:

	LPUINT32                     m_dwId;
	volatile atomic_uint        m_dwState;

	char                        m_szIp[IP_LEN];
	LPUINT32                     m_dwPort;
	BOOL                        m_bReUseAddr;
	SOCKET                      m_hListenSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;

	LPFN_ACCEPTEX               m_lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
	PER_IO_DATA*                m_pstPerIoDataArray;

};

typedef std::map<LPUINT32, LPListener*> MAP_LISTENER;



// Summary:
//		�¼�����
enum e_EventType
{
	eEventType_None = 0,
	eEventType_Recv = 1,
	eEventType_Terminate = 2,
	eEventType_Establish = 3,
	eEventType_ConnectError = 4,
	eEventType_Max
};



// Summary;
//		�����¼��ṹ��
struct RECV_EVENT
{
	LPSocker*                pSocker;
	LPUINT32                  dwLen;
};



// Summary;
//		�Ͽ��¼��ṹ��
struct TERMINATE_EVENT
{
	LPSocker*                pSocker;
	LPUINT32                  dwSockerId;
};



// Summary;
//		���ӽ����¼��ṹ��
struct ESTABLISH_EVENT
{
	LPSocker*                pSocker;
	BOOL                     bAccept;
};



// Summary;
//		���Ӵ����¼��ṹ��
struct CONNECT_ERROR_EVENT
{
	LPConnector*             pConnector;
	LPUINT32                  dwErrorNo;
};



// Summary;
//		�¼��ṹ��
struct NET_EVENT
{
	e_EventType              eEventType;
	LPUINT32                  dwFlag;      // ���ֶ�Ŀǰ�����ڷ�ɢ����ͬ�¼��б�ƽ��ѹ��
	union
	{
		RECV_EVENT             stRecvEvent;
		TERMINATE_EVENT        stTerminateEvent;
		ESTABLISH_EVENT        stEstablishEvent;
		CONNECT_ERROR_EVENT    stConnectErrorEvent;
	} stUn;
};



// Summary:
//		�¼�������
class DECLARE LPEventMgr
{
public:

	// Summary��
	//     ���캯��
	// Returns:
	//     ��
	LPEventMgr();

	// Summary��
	//     ��������
	// Returns:
	//     ��
	~LPEventMgr();

public:

	typedef std::list<NET_EVENT*> LPListEvent;

public:

	// Summary:
	//		��ʼ������
	// Input:
	//		dwSize�������������¼�����
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPNetMessageHandler* pNetMessageHandler, LPUINT32 dwSize, LPINT32 nEventListCount);

	// Summary:
	//		�������
	BOOL LPAPI UnInit();

	// Summary:
	//		pushһ�������¼�
	BOOL LPAPI PushRecvEvent(LPSocker* pSocker, LPUINT32 dwSockerId, ILPLoopBuf* pLoopBuf, LPUINT32 dwLen);

	// Summary:
	//		pushһ���Ͽ��¼�
	void LPAPI PushTerminateEvent(LPSocker* pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose);

	// Summary:
	//		pushһ�����ӽ����¼�
	void LPAPI PushEstablishEvent(LPSocker* pSocker, BOOL bAccept);

	// Summary:
	//		pushһ�����Ӵ����¼�
	void LPAPI PushConnectErrorEvent(LPConnector* pConnector, LPUINT32 dwErrorNo);

	// Summary:
	//		�ж��Ƿ����¼�������
	BOOL HaveEventForHandled();

	// Summary:
	//		����һ���¼����ú����������̵߳��ã�����Ӧ��һЩ�¼��������ڴˣ�
	//			��֤ͬһ���̴߳���
	void HandleOneEvent();

private:

	// Summary:
	//		�����¼�
	NET_EVENT* LPAPI _MallocEvent();

	// Summary:
	//		�ͷ��¼�
	void LPAPI _FreeEvent(NET_EVENT* pstEvent);

	// Summary:
	//		��������¼�
	void LPAPI _ProcRecvEvent(RECV_EVENT* pstRecvEvent);

	// Summary:
	//		����Ͽ��¼�
	void LPAPI _ProcTerminateEvent(TERMINATE_EVENT* pstTerminateEvent);

	// Summary:
	//		�������ӽ����¼�
	void LPAPI _ProcEstablishEvent(ESTABLISH_EVENT* pstEstablishEvent);

	// Summary:
	//		�������Ӵ����¼�
	void LPAPI _ProcConnectErrorEvent(CONNECT_ERROR_EVENT* pstConnectErrorEvent);

private:

	BOOL                        m_bInit;
	LPObjPool<NET_EVENT>         m_oEventPool;          // �¼������
	LPLoopBuf*                  m_pRecvLoopBuf;        // �����¼����ݻ�����
	char*                       m_pPacketTempBuf;      // ���ݰ���ʱ������
	LPINT32                      m_nEventListCount;     // �¼��б����
	LPListEvent*                m_pEventList;          // �¼��б�
	LPLock*                     m_pEventListLock;      // �¼��б���
	ILPNetMessageHandler*       m_pNetMessageHandler;  // 
	LPNetImpl*                  m_pNetImpl;            //
};



// Summary:
//		sql�����̲߳���
class LPReactorImpl;
struct REACTOR_THREAD_PARAM
{
	LPReactorImpl   *pReactorImpl;
	LPINT32           nCompletionPortIndex;
};



// Summary��
//     windowsƽ̨��Ӧ����
class DECLARE LPReactorImpl : public ILPReactor
{
public:

	// Summary��
	//     ���캯��
	// Returns:
	//     ��
	LPReactorImpl();

	// Summary��
	//     ��������
	// Returns:
	//     ��
	~LPReactorImpl();

public:

	// Summary:
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler);

	// Summary��
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler);

public:

	// Summary��
	//     ��ɶ˿��̴߳�����
	static unsigned LPAPI ThreadFunc(LPVOID pParam);

	// Summary��
	//		��
	// Input:
	//		pNetImpl: 
	//		bOneCompletionPortOneThread: �Ƿ񵥸���ɶ˿�ģʽ
	//			TRUE-һ����ɶ˿ڶ�Ӧ����̣߳�FALSE-�����ɶ˿ڣ�ÿ����ɶ˿ڶ�Ӧһ���߳�
	BOOL LPAPI Init(LPNetImpl* pNetImpl, BOOL bOneCompletionPortOneThread);
	// Summary��
	//		��     
	BOOL LPAPI UnInit();

	// Summary��
	//     �̴߳����������߼�
	void LPAPI OnExecute(LPINT32 nCompletionPortIndex);

private:

	// Summary��
	//		��ȡ��ǰ״̬
	LPUINT32 LPAPI _GetState();

	// Summary��
	//		���õ�ǰ״̬
	void LPAPI _SetState(LPUINT32 dwState);

	void LPAPI _OnConnect(BOOL bOperateRet, ILPEventHandler* pEventHandler, PER_IO_DATA* pstPerIoData);
	void LPAPI _OnAccept(BOOL bOperateRet, ILPEventHandler* pEventHandler, PER_IO_DATA* pstPerIoData);
	void LPAPI _OnSendRecv(BOOL bOperateRet, ILPEventHandler* pEventHandler, PER_IO_DATA* pstPerIoData, LPUINT32 dwByteTransferred);


protected:

	BOOL	               m_bInit;
	volatile atomic_uint   m_dwState;

	LPINT32                 m_nCompletionPortCount;
	HANDLE*                m_pCompletionPort;
	LPINT32                 m_nWorkerCountPerCompIo;
	HANDLE**               m_ppWorkerArray;
	LPNetImpl*             m_pNetImpl;
};




//Summary:
//		windowsƽ̨��net���ʵ����
class DECLARE LPNetImpl : public ILPNet
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
	//		���ü�����1
	virtual void LPAPI AddRef(void);

	// Summary:
	//		���ü�����1
	virtual LPUINT32 LPAPI QueryRef(void);

	// Summary:
	//		�ͷŶ���
	virtual void LPAPI Release(void);

	// Summary:
	//		��ȡ�汾��Ϣ
	virtual MODULE_VERSION LPAPI GetVersion(void);

	// Summary:
	//		��ȡ������ƣ�ÿ������������ش˺��������ṩ�����������
	virtual const char* LPAPI GetModuleName(void);

	// Summary:
	//		����ILPListener��������������
	//Input:
	//		eIoType������IO����
	//		pPacketParser����������
	//Return:
	//		����������
	virtual ILPListener* LPAPI CreateListenerCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser);

	// Summary:
	//		����ILPConnector����������
	//Input
	//		eIoType������IO����
	//		pPacketParser����������
	//Return:
	//		����������
	virtual ILPConnector* LPAPI CreateConnectorCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser);

	// Summary:
	//		�������������
	//Input:
	//		nCount-ÿ�μ�⴦������������������-1��ʾ��������
	//Return:
	//		TRUE-������������ѱ�����FALSE-��ʣ��δ���������
	virtual BOOL LPAPI Run(LPINT32 nCount = -1);

	// Summary:
	//		��ʼ������
	//Return:
	//		�ɹ�����TRUE��ʧ�ܷ���FALSE
	BOOL LPAPI Init(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig);

	// Summary:
	//		����ʼ������
	//Return:
	//		�ɹ�����TRUE��ʧ�ܷ���FALSE
	void LPAPI LPNetImpl::UnInit(void);

	// Summary:
	//		���Ҽ���������
	LPListener* LPAPI FindListener(LPUINT32 dwId);

	// Summary:
	//		��������������
	LPConnector* LPAPI FindConnector(LPUINT32 dwId);

	// Summary:
	//		��
	LPSockerMgr& LPAPI GetSockerMgr(void);

	// Summary:
	//		��
	LPEventMgr& LPAPI GetEventMgr(void);

	// Summary:
	//		��
	LPReactorImpl& LPAPI GetReactorImpl(void);

	// Summary:
	//		��
	NET_CONFIG&  LPAPI GetNetConfig(void);

protected:

	// Summary:
	//		����id
	LPUINT32 LPAPI _CreateId();

protected:

	LPUINT32                  m_dwRef;
	LPUINT32                  m_dwMaxCreateId;
	LPLock                   m_oLock;
	ILPNetMessageHandler*    m_pNetMessageHandler;
	NET_CONFIG               m_oNetConfig;
	MODULE_VERSION        m_stModuleVersion;

	LPSockerMgr              m_oSockerMgr;
	LPEventMgr               m_oEventMgr;
	LPReactorImpl            m_oReactorImpl;

	MAP_LISTENER             m_mapListener;
	MAP_CONNECTOR            m_mapConnector;
};
















//end�������������ֿռ�
NS_LZPL_END

#endif /* END OF _WIN32*/

#endif