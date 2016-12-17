//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: ������Ϣ�������ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_NET_MESSAGE_HANDLER_H_
#define _LPI_NET_MESSAGE_HANDLER_H_

#include "lp_base.h"
#include "lpi_socker.h"

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		������Ϣ�������ӿ���
class DECLARE ILPNetMessageHandler
{
public:

	// Summary:
	//		��
	virtual ~ILPNetMessageHandler() {}

	// Summary:
	//		�������µ�����
	virtual void LPAPI OnAccepted(ILPSocker* pSocker) = 0;

	// Summary:
	//		�ɹ������µ�����
	virtual void LPAPI OnConnected(ILPSocker* pSocker) = 0;

	// Summary:
	//		�������Ӵ���
	virtual void LPAPI OnConnectError(ILPConnector* pConnector, LPUINT32 dwErrorNo) = 0;

	// Summary:
	//		�����յ���Ϣ
	virtual void LPAPI OnMessage(ILPSocker* pSocker, const char* pcszBuf, LPUINT32 dwSize) = 0;

	// Summary:
	//		���ӶϿ���OnAccepted�����ӣ�
	virtual void LPAPI OnDisconnected(ILPSocker* pSocker) = 0;

	// Summary:
	//		���ӶϿ���OnConnected�����ӣ�
	virtual void LPAPI OnConnectDisconnected(ILPSocker* pSocker, ILPConnector* pConnector) = 0;

};




//end�������������ֿռ�
NS_LZPL_END

#endif