//****************************************************************************/
//    author: caihy
//    date: Jan 5, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_NET_EVENT_H_
#define _LP_NET_EVENT_H_

#include "lpi_net.h"
#include "lp_lock.h"
#include "lp_loopbuf.h"
#include "lpi_sockerimpl.h"
#include "lpi_connectorimpl.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



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
struct DECLARE RECV_EVENT
{
	ILPSockerImpl*           pSocker;
	LPUINT32                 dwLen;
};



// Summary;
//		�Ͽ��¼��ṹ��
struct DECLARE TERMINATE_EVENT
{
	ILPSockerImpl*           pSocker;
	LPUINT32                 dwSockerId;
};



// Summary;
//		���ӽ����¼��ṹ��
struct DECLARE ESTABLISH_EVENT
{
	ILPSockerImpl*           pSocker;
	BOOL                     bAccept;
};



// Summary;
//		���Ӵ����¼��ṹ��
struct DECLARE CONNECT_ERROR_EVENT
{
	ILPConnectorImpl*        pConnector;
	LPUINT32                 dwErrorNo;
};



// Summary;
//		�¼��ṹ��
struct DECLARE NET_EVENT
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

	static LPINT32 ms_dwNetEventCount;
	static NET_EVENT* LPAPI NewNetEvent()
	{
		++ms_dwNetEventCount;
		return new NET_EVENT();
	}

	static void LPAPI DeleteNetEvent(NET_EVENT* & pNetEvent)
	{
		if (nullptr != pNetEvent)
		{
			--ms_dwNetEventCount;
		}
		SAFE_DELETE(pNetEvent);
	}
};



//������
class LPNetImpl;



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
	//		nEventListCount��
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPNetMessageHandler* pNetMessageHandler, LPINT32 nEventListCount);

	// Summary:
	//		�������
	BOOL LPAPI UnInit();

	// Summary:
	//		pushһ�������¼�
	BOOL LPAPI PushRecvEvent(ILPSockerImpl* pSocker, LPUINT32 dwSockerId, ILPLoopBuf* pLoopBuf, LPUINT32 dwLen);

	// Summary:
	//		pushһ���Ͽ��¼�
	void LPAPI PushTerminateEvent(ILPSockerImpl* pSocker, LPUINT32 dwSockerId, BOOL bPassiveClose);

	// Summary:
	//		pushһ�����ӽ����¼�
	void LPAPI PushEstablishEvent(ILPSockerImpl* pSocker, BOOL bAccept);

	// Summary:
	//		pushһ�����Ӵ����¼�
	void LPAPI PushConnectErrorEvent(std::shared_ptr<ILPConnectorImpl> pConnector, LPUINT32 dwErrorNo);

	// Summary:
	//		�ж��Ƿ����¼�������
	BOOL HaveEventForHandled();

	// Summary:
	//		����һ���¼����ú����������̵߳��ã�����Ӧ��һЩ�¼��������ڴˣ�
	//			��֤ͬһ���̴߳���
	void HandleOneEvent();

private:

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
	LPLoopBuf*                  m_pRecvLoopBuf;        // �����¼����ݻ�����
	char*                       m_pPacketTempBuf;      // ���ݰ���ʱ������
	LPINT32                     m_nEventListCount;     // �¼��б����
	LPListEvent*                m_pEventList;          // �¼��б�
	LPLock*                     m_pEventListLock;      // �¼��б���
	ILPNetMessageHandler*       m_pNetMessageHandler;  // 
	LPNetImpl*                  m_pNetImpl;            //
};



//end�������������ֿռ�
NS_LZPL_END

#endif