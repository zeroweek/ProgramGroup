//****************************************************************************/
//    author: caihy
//    date: Jan 9, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LPI_LISTENER_IMPL_H_
#define _LPI_LISTENER_IMPL_H_

#include "lpi_listener.h"
#include "lpi_packetparser.h"
#include "lpi_eventhandler.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//		�������ӿ���
class DECLARE ILPListenerImpl : public ILPListener, public ILPEventHandler
{
public:

	virtual ~ILPListenerImpl() {}

	// Summary��
	//		��
	// Input:
	//		pNetImpl: 
	//		pPacketParser: ��Ϣ����������
	//		dwId: ������id
	virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId) = 0;

public:

	static std::shared_ptr<ILPListenerImpl> LPAPI NewListenerImpl(LPUINT32 dwIoType);
	static void LPAPI DeleteListenerImpl(std::shared_ptr<ILPListenerImpl>& pListener);
};

typedef std::map<LPUINT32, std::shared_ptr<ILPListenerImpl>> MAP_LISTENER;



//end�������������ֿռ�
NS_LZPL_END

#endif