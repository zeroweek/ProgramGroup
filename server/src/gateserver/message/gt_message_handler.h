//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: 消息管理器
//
//****************************************************************************/
#ifndef _GT_MESSAGE_HANDLER_H_
#define _GT_MESSAGE_HANDLER_H_

#include "lp_lzpl.h"
#include "lp_messageserializer.h"
#include "internal_message_header.h"
#include "external_message_header.h"


using namespace INTERNAL_MESSAGE;
using namespace EXTERNAL_MESSAGE;



// Summary:
//		
class DECLARE CGTMessageHandler : public ILPNetMessageHandler
{
public:
	
	friend class CGTInternalPacketParser;

public:

	// Summary:
	//		无
	CGTMessageHandler();
	
	// Summary:
	//		无
	virtual ~CGTMessageHandler();

	// Summary:
	//		初始化
	BOOL LPAPI Init(void);

	// Summary:
	//		反始化
	BOOL LPAPI UnInit(void);

public:

	// Summary:
	//		无
	virtual void LPAPI OnAccepted(ILPSocker* pSocker);

	// Summary:
	//		无
	virtual void LPAPI OnConnected(ILPSocker* pSocker);

	// Summary:
	//		无
	virtual void LPAPI OnConnectError(std::shared_ptr<ILPConnector> pConnector, LPUINT32 dwErrorNo);

	// Summary:
	//		无
	virtual void LPAPI OnMessage(ILPSocker* pSocker, const char* pcszBuf, LPUINT32 dwSize);

	// Summary:
	//		无
	virtual void LPAPI OnDisconnected(ILPSocker* pSocker);

	// Summary:
	//		无
	virtual void LPAPI OnConnectDisconnected(ILPSocker* pSocker, std::shared_ptr<ILPConnector> pConnector);


public:

	BOOL DoGSRegisterAck(ILPSocker* pSocker);
	void OnGameServerRegister(ILPSocker* pSocker, const char* pcszBuf, LPUINT32 dwSize);


public:

	BOOL DoCLientLoginAck(ILPSocker* pSocker);
	void OnClientLoginReq(ILPSocker* pSocker, const char* pcszBuf, LPUINT32 dwSize);


public:

	void CloseAllSocker(void);

private:

	LPExternalMessageSerializer      m_oRecvMessageSerializer;
	LPExternalMessageSerializer      m_oSendMessageSerializer;

	typedef std::map<LPUINT32, ILPSocker*>  MAP_SOCKER;
	MAP_SOCKER                       m_mapSocker;
	MAP_SOCKER::iterator             m_iterSocker;

	typedef void (CGTMessageHandler::*pfunMessageCallback)(ILPSocker* pSocker, const char* pcszBuf, LPUINT32 dwSize);
	pfunMessageCallback              m_MessageCallbackList[max_internal_message_count];
};






#endif