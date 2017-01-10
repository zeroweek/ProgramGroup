//****************************************************************************/
//    author: caihy
//    date: October 20, 2014
//    description: 
//
//****************************************************************************/
#ifndef _LPI_SOCKER_IMPL_H_
#define _LPI_SOCKER_IMPL_H_

#include "lpi_socker.h"
#include "lpi_packetparser.h"
#include "lpi_eventhandler.h"
#include "lp_loopbuf.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//		�ײ�socket�����װ�ӿ���
class DECLARE ILPSockerImpl : public ILPSocker, public ILPEventHandler
{
public:

	// Summary:
	//		��
	virtual ~ILPSockerImpl() {}

	// Summary:
	//		����sock���
	// Input:
	//		sock���ײ�socket���ֵ
	virtual void LPAPI SetSock(SOCKET sock) = 0;

	// Summary:
	//		��������״̬���ֵ
	// Input:
	//		bConnect��true-���ӣ�false-�Ͽ�
	virtual void LPAPI SetConnect(bool bConnect) = 0;

	// Summary:
	//		�󶨽�������
	// Input:
	//		pPacketParser����������
	virtual void LPAPI AttachPacketParser(ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		����󶨽�������
	virtual void LPAPI DetachPacketParser() = 0;

	// Summary:
	//		����socker����
	virtual void LPAPI Reset() = 0;

	// Summary:
	//		post�첽�������ݲ���
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI PostRecv() = 0;

	// Summary:
	//		post�첽�������ݲ���
	// Return:
	//		TRUE-�з������ݣ�FALSE-û��������
	virtual BOOL LPAPI PostSend() = 0;

	// Summary:
	//		�յ���Ϣ�ص�
	// Input:
	//		dwBytes�����յ����ֽ���
	virtual void LPAPI OnRecv(LPUINT32 dwBytes) = 0;

	// Summary:
	//		������Ϣ�ص�
	// Input:
	//		dwBytes�����յ����ֽ���
	virtual void LPAPI OnSend(LPUINT32 dwBytes) = 0;

	// Summary:
	//		���ý��ջ�����
	// Input:
	//		pBuf��������
	virtual void LPAPI AttachRecvBuf(LPLoopBuf* pLoopBuf) = 0;

	// Summary:
	//		������ջ�����
	// Return:
	//		���ջ�����
	virtual LPLoopBuf* LPAPI DetachRecvBuf() = 0;

	// Summary:
	//		���÷��ͻ�����
	// Input:
	//		pBuf��������
	virtual void LPAPI AttachSendBuf(LPLoopBuf* pLoopBuf) = 0;

	// Summary:
	//		���뷢�ͻ�����
	// Return:
	//		���ͻ�����
	virtual LPLoopBuf* LPAPI DetachSendBuf() = 0;

	// Summary:
	//		����Զ������ip
	// Input:
	//		dwIp��ip
	virtual void LPAPI SetRemoteIp(LPUINT32 dwIp) = 0;

	// Summary:
	//		����Զ�����Ӷ˿�
	// Input:
	//		wPort���˿�
	virtual void LPAPI SetRemotePort(LPUINT16 wPort) = 0;

	// Summary:
	//		���ñ�������ip
	// Input:
	//		dwIp��ip
	virtual void LPAPI SetLocalIp(LPUINT32 dwIp) = 0;

	// Summary:
	//		���ñ������Ӷ˿�
	// Input:
	//		wPort���˿�
	virtual void LPAPI SetLocalPort(LPUINT16 wPort) = 0;

	// Summary:
	//		���ÿ�ʼ�ӳٹر�tick
	virtual void LPAPI SetDelayCloseBeginTick(LPUINT64 qwTick) = 0;

	// Summary:
	//		��ȡ��ʼ�ӳٹر�tick
	virtual LPUINT64 LPAPI GetDelayCloseBeginTick() = 0;

	// Summary:
	//		�����ӳٹرճ���ʱ��
	virtual void LPAPI SetDelayCloseDuration(LPUINT64 qwDuration) = 0;

	// Summary:
	//		��ȡ�ӳٹرճ���ʱ��
	virtual LPUINT64 LPAPI GetDelayCloseDuration() = 0;

	// Summary:
	//		���ÿ�ʼ�ӳ��ͷ�tick
	virtual void LPAPI SetDelayReleaseBeginTick(LPUINT64 qwTick) = 0;

	// Summary:
	//		��ȡ��ʼ�ӳ��ͷ�tick
	virtual LPUINT64 LPAPI GetDelayReleaseBeginTick() = 0;

	// Summary:
	//		�����ӳ��ͷų���ʱ��
	virtual void LPAPI SetDelayReleaseDuration(LPUINT64 qwDuration) = 0;

	// Summary:
	//		��ȡ�ӳ��ͷų���ʱ��
	virtual LPUINT64 LPAPI GetDelayReleaseDuration() = 0;

	// Summary:
	//		����socker id
	// Input:
	//		dwSockerId��socker id
	virtual void LPAPI SetSockerId(LPUINT32 dwSockerId) = 0;

	// Summary:
	//		��
	virtual void LPAPI SetNetImpl(LPNetImpl* pNetImpl) = 0;

	// Summary:
	//		���ø��������id�����������������id��
	// Input:
	//		dwParentId��id
	virtual void LPAPI SetParentId(LPUINT32 dwParentId) = 0;

	// Summary:
	//		��ȡ���������id�����������������id��
	// Return:
	//		id
	virtual LPUINT32 LPAPI GetParentId() = 0;

	// Summary:
	//		�����Ƿ񱻶��ر�
	virtual void LPAPI SetPassiveClose(BOOL bPassiveClose) = 0;

	// Summary:
	//		�����Ƿ���accept�����ı�ǣ�accept����connect����
	virtual void LPAPI SetAcceptCreate(BOOL bAcceptCreate) = 0;

	// Summary:
	//		�ж��Ƿ���accept����
	//	Return:
	//		TRUE-�ǣ�FALSE-����
	virtual BOOL LPAPI IsAcceptCreate() = 0;

	// Summary:
	//		�첽�رջص�
	virtual void LPAPI OnClose() = 0;

public:

	static ILPSockerImpl* LPAPI NewSockerImpl();
	static void LPAPI DeleteSockerImpl(ILPSockerImpl* & pSocker);
};



//end�������������ֿռ�
NS_LZPL_END

#endif