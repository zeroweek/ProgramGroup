//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_CONNECTOR_H_
#define _LP_CONNECTOR_H_

#include "lpi_connector.h"
#include "lpi_eventhandler.h"
#include "lpi_packetparser.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;
class LPSocker;



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
	//		strIP: ip��ַ�ַ�������ʽ��192.168.1.1��
	//		dwPort: ���ӵĶ˿�
	//		bAutoReconnect: �Ƿ��Զ�������
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect);

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
	//		��ȡ���
	virtual HANDLE LPAPI GetHandle();

	// Summary:
	//		��ȡ�����¼�����������
	virtual e_EventHandlerType LPAPI GetEventHandlerType();

	// Summary:
	//		�����¼�����
	virtual void LPAPI OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData);

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

	std::string                 m_strIP;
	LPUINT32                    m_dwPort;
	BOOL                        m_bReconnect;
	SOCKET                      m_hConnectSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;
	LPSocker*                   m_pSocker;

	LPFN_CONNECTEX              m_lpfnConnectEx;
	PER_IO_DATA*                m_pstPerIoData;
};

typedef std::map<LPUINT32, std::shared_ptr<LPConnector>> MAP_CONNECTOR;



//end�������������ֿռ�
NS_LZPL_END

#endif