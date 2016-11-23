#include "gs_message_handler.h"
#include "lp_processerror.h"
#include "t2g_message.h"
#include "g2t_message.h"
#include "gs_internal_packet_parser.h"



CGSMessageHandler::CGSMessageHandler()
{
	memset(m_MessageCallbackList, 0, sizeof(m_MessageCallbackList));
}

CGSMessageHandler::~CGSMessageHandler()
{
	UnInit();
}

BOOL LPAPI CGSMessageHandler::Init(void)
{
	LPASSERT(max_internal_message <= max_internal_message_count);
	LPASSERT(max_external_message_count < min_internal_message);

	memset(g_MessageSize, 0, sizeof(g_MessageSize));

	
	//�ڲ�Э��ص�����ע��
	g_MessageSize[t2g_register_ack] = sizeof(T2G_REGISTER_ACK);
	m_MessageCallbackList[t2g_register_ack] = &CGSMessageHandler::OnGTRegisterAck;


	//�ⲿЭ��ص�����ע��


	return TRUE;
}

BOOL LPAPI CGSMessageHandler::UnInit(void)
{
	return TRUE;
}

void LPAPI CGSMessageHandler::OnAccepted(ILPSocker * pSocker)
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

void LPAPI CGSMessageHandler::OnConnected(ILPSocker * pSocker)
{
	std::pair<MAP_SOCKER::iterator, bool> InsRet;

	LOG_PROCESS_ERROR(pSocker);

	IMP("%s new socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

	InsRet = m_mapSocker.insert(std::make_pair(pSocker->GetSockerId(), pSocker));
	LOG_PROCESS_ERROR(InsRet.second);

	DoGTRegister(pSocker);

	return;

Exit0:

	ERR("%s new socker insert fail : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());

	return;
}

void LPAPI CGSMessageHandler::OnConnectError(ILPConnector * pConnector, UINT_32 dwErrorNo)
{
	LOG_PROCESS_ERROR(pConnector);

	ERR("connect to gt fail, connector id %d, errno %d", pConnector->GetId(), dwErrorNo);
	
Exit0:
	return;
}

void LPAPI CGSMessageHandler::OnMessage(ILPSocker * pSocker, const char * pcszBuf, UINT_32 dwSize)
{
	INT_32 nResult = 0;
	UINT_16 wMsgId = 0;

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pcszBuf);
	LOG_PROCESS_ERROR(dwSize >= sizeof(wMsgId));

	wMsgId = *((UINT_16*)pcszBuf);
	CONVERT_MSG_ID_ENDIAN(wMsgId);

	LOG_PROCESS_ERROR(wMsgId > min_internal_message && wMsgId < max_internal_message
						|| wMsgId > min_external_message && wMsgId < max_external_message);

	LOG_PROCESS_ERROR(m_MessageCallbackList[wMsgId]);

	(this->*m_MessageCallbackList[wMsgId])(pSocker, pcszBuf, dwSize);

Exit0:
	return;
}

void LPAPI CGSMessageHandler::OnDisconnected(ILPSocker * pSocker)
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

void LPAPI CGSMessageHandler::OnConnectDisconnected(ILPSocker* pSocker, ILPConnector * pConnector)
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

INT_32 LPAPI CGSMessageHandler::Parse(ILPLoopBuf * pLoopBuf)
{
	INT_32 nResult = 0;
	UINT_16 wMsgId = 0;
	INT_32 nParseCount = 0;

	LOG_PROCESS_ERROR(pLoopBuf);

	if (pLoopBuf->GetTotalReadableLen() < sizeof(wMsgId))
	{
		PROCESS_SUCCESS(TRUE);
	}

	nResult = pLoopBuf->Read((char*)(&wMsgId), sizeof(wMsgId), FALSE, FALSE);
	LOG_PROCESS_ERROR(nResult);
	CONVERT_MSG_ID_ENDIAN(wMsgId);

	LOG_PROCESS_ERROR(wMsgId > min_internal_message && wMsgId < max_internal_message);

	if (pLoopBuf->GetTotalReadableLen() >= g_MessageSize[wMsgId])
	{
		nParseCount = g_MessageSize[wMsgId];
	}

	PROCESS_SUCCESS(TRUE);

Exit0:
	nParseCount = -1;

Exit1:
	return nParseCount;
}

BOOL CGSMessageHandler::DoGTRegister(ILPSocker* pSocker)
{
	INT_32 nResult = 0;
	G2T_REGISTER tMsg;

	LOG_PROCESS_ERROR(pSocker);

	memset(&tMsg, 0, sizeof(tMsg));
	tMsg.wMsgId = g2t_register;
	CONVERT_MSG_ID_ENDIAN(tMsg.wMsgId);
	tMsg.byValue = 1;
	tMsg.wValue = 2;
	tMsg.dwValue = 3;
	tMsg.qwValue = 4;
	lpStrCpyN(tMsg.szValue, "I'm gameserver, i want to registering !", MAX_PATH);

	nResult = pSocker->Send((const char*)(&tMsg), sizeof(tMsg));
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

void CGSMessageHandler::OnGTRegisterAck(ILPSocker * pSocker, const char * pcszBuf, UINT_32 dwSize)
{
	INT_32 nResult = 0;
	T2G_REGISTER_ACK* ptMsg = NULL;

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pcszBuf);
	LOG_PROCESS_ERROR(dwSize == sizeof(T2G_REGISTER_ACK));

	ptMsg = (T2G_REGISTER_ACK*)pcszBuf;
	CONVERT_MSG_ID_ENDIAN(ptMsg->wMsgId);

	IMP("recv gateserver regist ack, regist successfully !");
	IMP("T2G_REGISTER_ACK(byValue=%d,wValue=%d,dwValue=%d,qwValue=%d,szValue=%s)",
		ptMsg->byValue, ptMsg->wValue, ptMsg->dwValue, ptMsg->qwValue, ptMsg->szValue);

Exit0:
	return;
}

void CGSMessageHandler::CloseAllSocker(void)
{
	ILPSocker* pSocker = NULL;

	for (m_iterSocker = m_mapSocker.begin(); m_iterSocker != m_mapSocker.end(); ++m_iterSocker)
	{
		pSocker = m_iterSocker->second;
		LOG_CHECK_ERROR(pSocker);
		IF_NULL_CONTINUE(pSocker);

		pSocker->Close(SOCK_ERR_CODE(eSockErrCode_Success, 1, 0), FALSE);
	}
}

UINT_32 CGSMessageHandler::GetSockerCount(void)
{
	return (UINT_32)m_mapSocker.size();
}
