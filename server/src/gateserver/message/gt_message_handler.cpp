#include "gt_message_handler.h"
#include "lp_processerror.h"
#include "g2t_message.h"
#include "t2g_message.h"
#include "external_message.h"
#include "gt_internal_packet_parser.h"


using namespace INTERNAL_MESSAGE;



CGTMessageHandler::CGTMessageHandler()
{
	memset(m_MessageCallbackList, 0, sizeof(m_MessageCallbackList));
}

CGTMessageHandler::~CGTMessageHandler()
{
	UnInit();
}

BOOL LPAPI CGTMessageHandler::Init(void)
{
	LPASSERT(max_internal_message <= max_internal_message_count);
	LPASSERT(max_external_message_count < min_internal_message);

	memset(g_MessageSize, 0, sizeof(g_MessageSize));

	//内部协议回调函数注册
	g_MessageSize[g2t_register] = sizeof(G2T_REGISTER);
	m_MessageCallbackList[g2t_register] = &CGTMessageHandler::OnGameServerRegister;


	//外部协议回调函数注册
	m_MessageCallbackList[c2t_login_req] = &CGTMessageHandler::OnClientLoginReq;

	return TRUE;
}

BOOL LPAPI CGTMessageHandler::UnInit(void)
{
	return TRUE;
}

void LPAPI CGTMessageHandler::OnAccepted(ILPSocker * pSocker)
{
	std::pair<MAP_SOCKER::iterator, bool> InsRet;

	LOG_PROCESS_ERROR(pSocker);

	IMP("%s new socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

	InsRet = m_mapSocker.insert(std::make_pair(pSocker->GetSockerId(), pSocker));
	LOG_PROCESS_ERROR(InsRet.second);

	return;

Exit0:
	
	ERR("%s new socker insert fail : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

	return;
}

void LPAPI CGTMessageHandler::OnConnected(ILPSocker * pSocker)
{
	std::pair<MAP_SOCKER::iterator, bool> InsRet;

	LOG_PROCESS_ERROR(pSocker);

	IMP("%s new socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

	InsRet = m_mapSocker.insert(std::make_pair(pSocker->GetSockerId(), pSocker));
	LOG_PROCESS_ERROR(InsRet.second);

	return;

Exit0:

	ERR("%s new socker insert fail : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

	return;
}

void LPAPI CGTMessageHandler::OnConnectError(ILPConnector * pConnector, LPUINT32 dwErrorNo)
{
	LOG_PROCESS_ERROR(pConnector);

	ERR("connect to fail, connector id %d, errno %d", pConnector->GetId(), dwErrorNo);

Exit0:
	return;
}

void LPAPI CGTMessageHandler::OnMessage(ILPSocker * pSocker, const char * pcszBuf, LPUINT32 dwSize)
{
	LPINT32 nResult = 0;
	LPUINT16 wMsgId = 0;

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pcszBuf);
	LOG_PROCESS_ERROR(dwSize >= sizeof(wMsgId));

	wMsgId = *(LPUINT16*)(pcszBuf);
	CONVERT_MSG_ID_ENDIAN(wMsgId);

	LOG_PROCESS_ERROR(wMsgId > min_internal_message && wMsgId < max_internal_message
						|| wMsgId > min_external_message && wMsgId < max_external_message);

	LOG_PROCESS_ERROR(m_MessageCallbackList[wMsgId]);

	(this->*m_MessageCallbackList[wMsgId])(pSocker, pcszBuf, dwSize);
	
Exit0:
	return;
}

void LPAPI CGTMessageHandler::OnDisconnected(ILPSocker * pSocker)
{
	LOG_PROCESS_ERROR(pSocker);

	if (pSocker->IsPassiveClose())
	{
		IMP("%s peer close the socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}
	else
	{
		IMP("%s local close the socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}
	m_mapSocker.erase(pSocker->GetSockerId());

	return;

Exit0:

	return;
}

void LPAPI CGTMessageHandler::OnConnectDisconnected(ILPSocker * pSocker, ILPConnector * pConnector)
{
	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pConnector);

	if (pSocker->IsPassiveClose())
	{
		IMP("%s peer close the connector socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}
	else
	{
		IMP("%s local close the connector socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}
	m_mapSocker.erase(pSocker->GetSockerId());

	return;

Exit0:

	return;
}

BOOL CGTMessageHandler::DoGSRegisterAck(ILPSocker* pSocker)
{
	LPINT32 nResult = 0;
	T2G_REGISTER_ACK tMsg;
	 
	LOG_PROCESS_ERROR(pSocker);

	memset(&tMsg, 0, sizeof(tMsg));
	tMsg.wMsgId = t2g_register_ack;
	CONVERT_MSG_ID_ENDIAN(tMsg.wMsgId);
	tMsg.byValue = 1;
	tMsg.wValue = 2;
	tMsg.dwValue = 3;
	tMsg.qwValue = 4;
	lpStrCpyN(tMsg.szValue, "I'm gateserver, i pass you register !", MAX_PATH);

	nResult = pSocker->Send((const char*)(&tMsg), sizeof(tMsg));
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

void CGTMessageHandler::OnGameServerRegister(ILPSocker * pSocker, const char * pcszBuf, LPUINT32 dwSize)
{
	LPINT32 nResult = 0;
	G2T_REGISTER* ptMsg = NULL;

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pcszBuf);
	LOG_PROCESS_ERROR(dwSize == sizeof(G2T_REGISTER));

	ptMsg = (G2T_REGISTER*)pcszBuf;
	CONVERT_MSG_ID_ENDIAN(ptMsg->wMsgId);

	IMP("recv gameserver regist req, regist successfully !");
	IMP("G2T_REGISTER(byValue=%d,wValue=%d,dwValue=%d,qwValue=%d,szValue=%s)", 
		ptMsg->byValue, ptMsg->wValue, ptMsg->dwValue, ptMsg->qwValue, ptMsg->szValue);

	DoGSRegisterAck(pSocker);

Exit0:
	return;
}

BOOL CGTMessageHandler::DoCLientLoginAck(ILPSocker * pSocker)
{
	LPINT32 nResult = 0;
	T2C_LOGIN_ACK stMsg;

	LOG_PROCESS_ERROR(pSocker);

	stMsg.wMsgId = t2c_login_ack;
	stMsg.wMsgSize = sizeof(T2C_LOGIN_ACK);
	stMsg.byValue = 1;
	stMsg.wValue = 2;
	stMsg.dwValue = 3;
	stMsg.qwValue = 4;
	lpStrCpyN(stMsg.szValue, "I'm gateserver, i pass you login !", MAX_PATH);

	nResult = m_oSendMessageSerializer.Init(NULL, 0, NULL, 0);
	LOG_PROCESS_ERROR(nResult);

	nResult = stMsg.Serialize(&m_oSendMessageSerializer);
	LOG_PROCESS_ERROR(nResult);

	m_iterSocker = m_mapSocker.begin();
	LOG_PROCESS_ERROR(m_iterSocker != m_mapSocker.end());

	pSocker = m_iterSocker->second;
	LOG_PROCESS_ERROR(pSocker);

	nResult = pSocker->Send(m_oSendMessageSerializer.GetSerializeBuf(), m_oSendMessageSerializer.GetSerializeSize());
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oSendMessageSerializer.UnInit();
	LOG_CHECK_ERROR(nResult);

	return TRUE;
Exit0:

	nResult = m_oSendMessageSerializer.UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

void CGTMessageHandler::OnClientLoginReq(ILPSocker * pSocker, const char * pcszBuf, LPUINT32 dwSize)
{
	LPINT32 nResult = 0;
	C2T_LOGIN_REQ stMsg;

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pcszBuf);

	nResult = m_oRecvMessageSerializer.Init(NULL, 0, pcszBuf, dwSize);
	LOG_PROCESS_ERROR(nResult);

	memset(&stMsg, 0, sizeof(stMsg));
	nResult = stMsg.UnSerialize(&m_oRecvMessageSerializer);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oRecvMessageSerializer.UnInit();
	LOG_PROCESS_ERROR(nResult);

	IMP("client login success !");
	IMP("C2T_LOGIN_REQ(byValue=%d,wValue=%d,dwValue=%d,qwValue=%d,szValue=%s)",
		stMsg.byValue, stMsg.wValue, stMsg.dwValue, stMsg.qwValue, stMsg.szValue);

	DoCLientLoginAck(pSocker);

Exit0:

	nResult = m_oRecvMessageSerializer.UnInit();
	LOG_CHECK_ERROR(nResult);

	return;
}

void CGTMessageHandler::CloseAllSocker(void)
{
	ILPSocker* pSocker = NULL;

	for (m_iterSocker = m_mapSocker.begin(); m_iterSocker != m_mapSocker.end(); ++m_iterSocker)
	{
		pSocker = m_iterSocker->second;
		LOG_CHECK_ERROR(pSocker);
		IF_NULL_CONTINUE(pSocker);

		pSocker->Close(SOCK_ERR_CODE(eSockErrCode_Success, 1, 0), FALSE);
	}

	m_mapSocker.clear();
}

