//****************************************************************************/
//    author: caihy
//    date: Jan 9, 2017
//    description: 
//
//****************************************************************************/
#ifndef _LPI_CONNECTOR_IMPL_H_
#define _LPI_CONNECTOR_IMPL_H_

#include "lpi_connector.h"
#include "lpi_packetparser.h"
#include "lpi_eventhandler.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;



// Summary:
//		连接器接口类
class DECLARE ILPConnectorImpl : public ILPConnector, public ILPEventHandler
{
public:

	// Summary:
	//		无
	virtual ~ILPConnectorImpl() {}

	// Summary：
	//		无
	// Input:
	//		pNetImpl: 
	//		pPacketParser: 消息包解析对象
	//		dwId: 连接器id
	virtual BOOL LPAPI Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId) = 0;

	// Summary:
	//		链接关闭回调
	virtual void LPAPI OnClose() = 0;

public:

	static std::shared_ptr<ILPConnectorImpl> LPAPI NewConnectorImpl();
	static void LPAPI DeleteConnectorImpl(std::shared_ptr<ILPConnectorImpl>& pConnector);
};

typedef std::map<LPUINT32, std::shared_ptr<ILPConnectorImpl>> MAP_CONNECTOR;



//end声明所处的名字空间
NS_LZPL_END

#endif