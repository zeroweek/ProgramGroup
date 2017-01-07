//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LP_LISTENER_H_
#define _LP_LISTENER_H_

#include "lpi_listener.h"
#include "lpi_eventhandler.h"
#include "lpi_packetparser.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



#define POST_ACCEPTEX_COUNT            (128)



//������
class LPNetImpl;



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
	//		strIP: ip��ַ�ַ�������ʽ��192.168.1.1��"0"��ʾ�����ַ��
	//		dwPort: �����˿�
	//		bReUseAddr: �Ƿ��ظ����õ�ַ
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr);

	// Summary:
	//		ֹͣ����
	virtual void LPAPI Stop();

	// Summary:
	//		��ȡid
	virtual LPUINT32 LPAPI GetId();

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

	std::string                 m_strIP;
	LPUINT32                    m_dwPort;
	BOOL                        m_bReUseAddr;
	SOCKET                      m_hListenSock;
	ILPPacketParser*            m_pPacketParser;
	LPNetImpl*                  m_pNetImpl;

	LPFN_ACCEPTEX               m_lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
	PER_IO_DATA*                m_pstPerIoDataArray;

};

typedef std::map<LPUINT32, std::shared_ptr<LPListener>> MAP_LISTENER;



//end�������������ֿռ�
NS_LZPL_END

#endif