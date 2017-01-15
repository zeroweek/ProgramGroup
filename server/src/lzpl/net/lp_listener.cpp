#include "lp_listener.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_net.h"
#include "lp_global.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



std::shared_ptr<ILPListenerImpl> LPAPI ILPListenerImpl::NewListenerImpl(LPUINT32 dwIoType)
{
	switch (dwIoType)
	{
	case eIoType_CompletionPort:
		{
			return std::make_shared<LPWinNetListener>();
		}
		break;
	case eIoType_None:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit0:
	return nullptr;
}

void LPAPI ILPListenerImpl::DeleteListenerImpl(std::shared_ptr<ILPListenerImpl>& pListener)
{
	pListener = nullptr;
}



LPListener::LPListener()
{
	m_dwId = 0;
	_SetState(eCommonState_NoInit);
	m_dwPort = 0;
	m_bReUseAddr = FALSE;
	m_hListenSock = INVALID_SOCKET;
	m_pPacketParser = NULL;
	m_pNetImpl = NULL;

	m_pstPerIoDataArray = NULL;
}

LPListener::~LPListener()
{
	UnInit();
}

BOOL LPAPI LPListener::Init(LPNetImpl* pNetImpl, ILPPacketParser * pPacketParser, LPUINT32 dwId)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(pNetImpl);

	m_dwId = dwId;
	m_pPacketParser = pPacketParser;
	m_pNetImpl = pNetImpl;
	m_pPacketParser->AddRef();

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

BOOL LPAPI LPListener::UnInit()
{
	PROCESS_SUCCESS(_GetState() == eCommonState_NoInit || _GetState() >= eCommonState_UnIniting);

	_SetState(eCommonState_UnIniting);

	if (INVALID_SOCKET != m_hListenSock)
	{
		lpCancelIoEx(m_hListenSock);
		lpCloseSocket(m_hListenSock);
		m_hListenSock = INVALID_SOCKET;
	}

	if (NULL != m_pPacketParser)
	{
		m_pPacketParser->Release();
		m_pPacketParser = NULL;
	}

	if (NULL != m_pstPerIoDataArray)
	{
		SAFE_DELETE_SZ(m_pstPerIoDataArray);
	}

	_SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPListener::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

void LPAPI LPListener::Stop()
{
	PROCESS_SUCCESS(_GetState() >= eCommonState_Close);

	LOG_PROCESS_ERROR(_GetState() == eCommonState_Inited);
	_SetState(eCommonState_Close);

	if (INVALID_SOCKET != m_hListenSock)
	{
		lpCancelIoEx(m_hListenSock);
		lpCloseSocket(m_hListenSock);
		m_hListenSock = INVALID_SOCKET;
	}

Exit1:
Exit0:
	return;
}

LPUINT32 LPAPI LPListener::GetId()
{
	return m_dwId;
}

HANDLE LPAPI LPListener::GetHandle()
{
	return (HANDLE)m_hListenSock;
}

e_EventHandlerType LPAPI LPListener::GetEventHandlerType()
{
	return eEventHandlerType_Listener;
}

void LPAPI LPListener::OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return;
}

void LPAPI LPListener::OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return;
}

LPUINT32 LPAPI LPListener::_GetState()
{
	return m_dwState;
}

void LPAPI LPListener::_SetState(LPUINT32 dwState)
{
	m_dwState = dwState;
}

BOOL LPAPI LPListener::_InitAcceptEx()
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPListener::_PostAcceptEx(PER_IO_DATA* pstPerIoData)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}



LPWinNetListener::LPWinNetListener()
{
	m_lpfnAcceptEx = NULL;
	m_lpfnGetAcceptExSockaddrs = NULL;
}

LPWinNetListener::~LPWinNetListener()
{

}

BOOL LPAPI LPWinNetListener::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr)
{
	LPINT32 nResult = 0;
	LPINT32 nReUse = 0;
	LINGER stLinger;
	sockaddr_in stAddr;

	LOG_PROCESS_ERROR(eCommonState_NoInit == _GetState());
	_SetState(eCommonState_Initing);

	m_strIP = strIP;
	m_dwPort = dwPort;
	m_bReUseAddr = bReUseAddr;

#	ifdef _WIN32
	{
		m_hListenSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	}
#	endif
	if (-1 == m_hListenSock)
	{
		FTL("%s create socket error, %s:%d, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	stLinger.l_onoff = 1;
	stLinger.l_linger = 0;
	nResult = ::setsockopt(m_hListenSock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	LOG_CHECK_ERROR(0 == nResult);

	nReUse = 1;
	if (m_bReUseAddr)
	{
		::setsockopt(m_hListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&nReUse, sizeof(nReUse));
	}

	memset(&stAddr, 0, sizeof(stAddr));
	stAddr.sin_family = AF_INET;
	if (m_strIP == "0")
	{
		stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		stAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	}
	stAddr.sin_port = htons(m_dwPort);

	nResult = ::bind(m_hListenSock, (sockaddr*)&stAddr, sizeof(stAddr));
	if (nResult < 0)
	{
		FTL("%s bind at %s:%d failed, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = ::listen(m_hListenSock, 128);
	if (nResult < 0)
	{
		FTL("%s listen at %s:%d failed, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	nResult = _InitAcceptEx();
	LOG_PROCESS_ERROR(nResult);

	if (NULL == m_pstPerIoDataArray)
	{
		m_pstPerIoDataArray = new PER_IO_DATA[POST_ACCEPTEX_COUNT];
	}
	LOG_PROCESS_ERROR(m_pstPerIoDataArray);

	//lpSleep(300);
	_SetState(eCommonState_Inited);

	IMP("start listening %s:%d !", m_strIP.c_str(), m_dwPort);

	for (LPINT32 i = 0; i < POST_ACCEPTEX_COUNT; ++i)
	{
		nResult = _PostAcceptEx(&m_pstPerIoDataArray[i]);
		LOG_PROCESS_ERROR(nResult);
	}

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

void LPAPI LPWinNetListener::OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData)
{
	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Accept);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Listener);

	OnAccept(bOperateRet, pstPerIoData);

Exit0:
	return;
}

void LPAPI LZPL::LPWinNetListener::OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
		LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	ILPSockerImpl* pSocker = NULL;
	const char cArg = 1;
	LPUINT32 dwCurValidConnectCount = 0;

	LPINT32 idwAddrLen = 0;
	LPINT32 idwRemoteAddrLen = 0;
	LPINT32 idwLocalAddrLen = 0;
	sockaddr_in* pstRemoteAddr = NULL;
	sockaddr_in* pstLocalAddr = NULL;

	LOG_PROCESS_ERROR(pstPerIoData);

	hSock = pstPerIoData->hSock;

	switch (_GetState())
	{
		case eCommonState_NoInit:
		{
			lpCloseSocket(hSock);
			LOG_PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Initing:
		{
			INF("listener is initing, discard accept", __FUNCTION__);
			lpCloseSocket(hSock);
			nResult = _PostAcceptEx(pstPerIoData);
			LOG_PROCESS_ERROR(nResult);
			PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Inited:
		{
			if (!bSuccess)
			{
				FTL("%s accept failed, errno %d", __FUNCTION__, WSAGetLastError());
				lpCloseSocket(hSock);
				nResult = _PostAcceptEx(pstPerIoData);
				LOG_PROCESS_ERROR(nResult);
				PROCESS_ERROR(FALSE);
			}
			else
			{
				nResult = _PostAcceptEx(pstPerIoData);
				LOG_CHECK_ERROR(nResult);

				dwCurValidConnectCount = m_pNetImpl->GetSockerMgr().GetCurValidConnectCount();
				if (dwCurValidConnectCount >= m_pNetImpl->GetNetConfig().dwConnectCount)
				{
					INF("encounter max connect count, connect count(current/max): %d/%d !",
						dwCurValidConnectCount, m_pNetImpl->GetNetConfig().dwConnectCount);
					lpCloseSocket(hSock);
					PROCESS_ERROR(FALSE);
				}

				pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, TRUE);
				if (NULL == pSocker)
				{
					LOG_CHECK_ERROR(FALSE);
					FTL("%s create LPSocker failed", __FUNCTION__);
					lpCloseSocket(hSock);
					PROCESS_ERROR(FALSE);
				}

				IMP("listener create socker, socker_id=%d, hSock=%d !", pSocker->GetSockerId(), hSock);

				//设置sock选项
#				ifdef _WIN32
				{
					::setsockopt(hSock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_hListenSock, sizeof(SOCKET));
					::setsockopt(hSock, IPPROTO_TCP, TCP_NODELAY, &cArg, sizeof(cArg));


					idwRemoteAddrLen = sizeof(sockaddr_in);
					idwLocalAddrLen = sizeof(sockaddr_in);
					idwAddrLen = sizeof(sockaddr_in) + 16;

					m_lpfnGetAcceptExSockaddrs(
						pstPerIoData->szBuf,
						0,
						idwAddrLen,
						idwAddrLen,
						(SOCKADDR**)&pstLocalAddr,
						&idwLocalAddrLen,
						(SOCKADDR**)&pstRemoteAddr,
						&idwRemoteAddrLen
					);
				}
#				endif

				//设置LPSocker对象
				pSocker->SetSock(hSock);
				pSocker->SetConnect(true);
				pSocker->SetRemoteIp(pstRemoteAddr->sin_addr.s_addr);
				pSocker->SetRemotePort(pstRemoteAddr->sin_port);
				pSocker->SetLocalIp(pstLocalAddr->sin_addr.s_addr);
				pSocker->SetLocalPort(pstLocalAddr->sin_port);

				//注册事件处理器
				nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(pSocker);
				if (!nResult)
				{
					LOG_CHECK_ERROR(FALSE);
					m_pNetImpl->GetSockerMgr().Release(pSocker);
					lpCloseSocket(hSock);
					PROCESS_ERROR(FALSE);
				}

				//push连接建立事件
				m_pNetImpl->GetEventMgr().PushEstablishEvent(pSocker, TRUE);
			}
		}
		break;
	case eCommonState_UnIniting:
	case eCommonState_UnInited:
		{
			LOG_CHECK_ERROR(bSuccess == FALSE);
			lpCloseSocket(hSock);
			PROCESS_SUCCESS(TRUE);
		}
		break;
	case eCommonState_Close:
		{
			lpCloseSocket(hSock);
			PROCESS_SUCCESS(TRUE);
		}
		break;
	default:
		lpCloseSocket(hSock);
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit1:
Exit0:
	return;
}

BOOL LPAPI LPWinNetListener::_InitAcceptEx()
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	DWORD dwBytes = 0;
	GUID stGuidAcceptEx = WSAID_ACCEPTEX;
	GUID stGuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;

#	ifdef _WIN32
	{
		hSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == hSock)
		{
			FTL("%s WSASocket failed, errno %d", __FUNCTION__, WSAGetLastError());
			PROCESS_ERROR(FALSE);
		}

		nResult = ::WSAIoctl(
			hSock,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&stGuidAcceptEx,
			sizeof(stGuidAcceptEx),
			&m_lpfnAcceptEx,
			sizeof(LPFN_ACCEPTEX),
			&dwBytes,
			NULL,
			NULL
		);
		LOG_PROCESS_ERROR(nResult == 0);

		dwBytes = 0;
		nResult = ::WSAIoctl(
			hSock,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&stGuidGetAcceptExSockaddrs,
			sizeof(stGuidGetAcceptExSockaddrs),
			&m_lpfnGetAcceptExSockaddrs,
			sizeof(LPFN_GETACCEPTEXSOCKADDRS),
			&dwBytes,
			NULL,
			NULL
		);
		LOG_PROCESS_ERROR(nResult == 0);
	}
#	endif

	//该sock只用于获取函数指针
	lpCloseSocket(hSock);

	LOG_PROCESS_ERROR(m_lpfnAcceptEx);
	LOG_PROCESS_ERROR(m_lpfnGetAcceptExSockaddrs);

	return TRUE;
Exit0:

	if (INVALID_SOCKET != hSock)
	{
		lpCloseSocket(hSock);
	}
	return FALSE;
}

BOOL LPAPI LPWinNetListener::_PostAcceptEx(PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	SOCKET hNewSock = INVALID_SOCKET;
	DWORD dwBytes = 0;

	LOG_PROCESS_ERROR(pstPerIoData);

#	ifdef _WIN32
	{
		hNewSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == hNewSock)
		{
			FTL("%s WSASocket failed, errno %d", __FUNCTION__, WSAGetLastError());
			PROCESS_ERROR(FALSE);
		}

		pstPerIoData->hSock = hNewSock;
		pstPerIoData->eIoOptType = eIoOptType_Accept;
		pstPerIoData->eHandlerType = eEventHandlerType_Listener;
		memset(&pstPerIoData->stOverlapped, 0, sizeof(pstPerIoData->stOverlapped));

		nResult = m_lpfnAcceptEx(
			m_hListenSock,
			hNewSock,
			pstPerIoData->szBuf,
			0,
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&dwBytes,
			&pstPerIoData->stOverlapped
		);
	}
#	endif

	if (!nResult)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			FTL("%s m_lpfnAcceptEx failed, errno %d", __FUNCTION__, WSAGetLastError());
			lpCloseSocket(hNewSock);
			PROCESS_ERROR(FALSE);
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}



//end声明所处的名字空间
NS_LZPL_END