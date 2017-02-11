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
	case eIoType_Epoll:
		{
			return std::make_shared<LPLinuxNetListener>();
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
	SetState(eCommonState_NoInit);
	m_dwPort = 0;
	m_bReUseAddr = FALSE;
	m_hListenSock = INVALID_SOCKET;
	m_pPacketParser = NULL;
	m_pNetImpl = NULL;

	m_stPerIoData.eIoOptType = eIoOptType_Accept;
	m_stPerIoData.eHandlerType = eEventHandlerType_Listener;
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
	if (INVALID_SOCKET != m_hListenSock)
	{
		//服务器主动关闭的，应该先注销再关闭
		m_pNetImpl->GetReactorImpl().UnRegisterEventHandler(this);
		lpCancelIoEx(m_hListenSock);
		lpCloseSocket(m_hListenSock);
		m_hListenSock = INVALID_SOCKET;
	}

	if (NULL != m_pPacketParser)
	{
		m_pPacketParser->Release();
		m_pPacketParser = NULL;
	}

	return TRUE;
}

void LPAPI LPListener::Stop()
{
	PROCESS_SUCCESS(GetState() >= eCommonState_Close);

	LOG_PROCESS_ERROR(GetState() == eCommonState_Inited);
	SetState(eCommonState_Close);

	if (INVALID_SOCKET != m_hListenSock)
	{
		//服务器主动关闭的，应该先注销再关闭
		m_pNetImpl->GetReactorImpl().UnRegisterEventHandler(this);
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

PER_IO_DATA* LPAPI LPListener::GetEventHandlerData()
{
	return &m_stPerIoData;
}

LPUINT32 LPAPI LPListener::GetState()
{
	return m_dwState;
}

void LPAPI LPListener::SetState(LPUINT32 dwState)
{
	m_dwState = dwState;
}



LPWinNetListener::LPWinNetListener()
{
	m_lpfnAcceptEx = NULL;
	m_lpfnGetAcceptExSockaddrs = NULL;
	m_pstPerIoDataArray = NULL;
}

LPWinNetListener::~LPWinNetListener()
{
	UnInit();
}

BOOL LPAPI LPWinNetListener::UnInit()
{
	PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

	SetState(eCommonState_UnIniting);

	LOG_CHECK_ERROR(LPListener::UnInit());

	SAFE_DELETE_SZ(m_pstPerIoDataArray);

	SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPWinNetListener::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr)
{
	LPINT32 nResult = 0;
	LPINT32 nReUse = 0;
	LINGER stLinger;
	sockaddr_in stAddr;

	LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
	SetState(eCommonState_Initing);

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

	//设置套接字延迟关闭（等待套接字发送缓冲区中的数据发送完成）
	stLinger.l_onoff = 1;
	stLinger.l_linger = 0;
	nResult = ::setsockopt(m_hListenSock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	LOG_CHECK_ERROR(0 == nResult);

	//设置监听地址重复使用
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

	nResult = ::bind(m_hListenSock, (sockaddr*)&stAddr, sizeof(struct sockaddr));
	if (nResult < 0)
	{
		FTL("%s bind at %s:%d failed, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = ::listen(m_hListenSock, LISTEN_QUEUE_COUNT);
	if (nResult < 0)
	{
		FTL("%s listen at %s:%d failed, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	nResult = InitAcceptEx();
	LOG_PROCESS_ERROR(nResult);

	if (NULL == m_pstPerIoDataArray)
	{
		m_pstPerIoDataArray = new PER_IO_DATA[LISTEN_QUEUE_COUNT];
	}
	LOG_PROCESS_ERROR(m_pstPerIoDataArray);

	//lpSleep(300);
	SetState(eCommonState_Inited);

	IMP("start listening %s:%d !", m_strIP.c_str(), m_dwPort);

	for (LPINT32 i = 0; i < LISTEN_QUEUE_COUNT; ++i)
	{
		nResult = PostAcceptEx(&m_pstPerIoDataArray[i]);
		LOG_PROCESS_ERROR(nResult);
	}

	return TRUE;
Exit0:

	return FALSE;
}

void LPAPI LPWinNetListener::OnNetEvent(PER_IO_DATA* pstPerIoData)
{
	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Accept);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Listener);

	OnAccept(pstPerIoData->bOperateRet, pstPerIoData);

Exit0:
	return;
}

void LPAPI LPWinNetListener::OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
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

	switch (GetState())
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
			nResult = PostAcceptEx(pstPerIoData);
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
				nResult = PostAcceptEx(pstPerIoData);
				LOG_PROCESS_ERROR(nResult);
				PROCESS_ERROR(FALSE);
			}
			else
			{
				nResult = PostAcceptEx(pstPerIoData);
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

BOOL LPAPI LPWinNetListener::InitAcceptEx()
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

BOOL LPAPI LPWinNetListener::PostAcceptEx(PER_IO_DATA* pstPerIoData)
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

LPLinuxNetListener::LPLinuxNetListener()
{

}

LPLinuxNetListener::~LPLinuxNetListener()
{

}

BOOL LPAPI LPLinuxNetListener::UnInit()
{
	PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

	SetState(eCommonState_UnIniting);

	LOG_CHECK_ERROR(LPListener::UnInit());

	SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPLinuxNetListener::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr)
{
	LPINT32 nResult = 0;
	LPINT32 nReUse = 0;
	LINGER stLinger;
	sockaddr_in stListenAddr;

	LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
	SetState(eCommonState_Initing);

	m_strIP = strIP;
	m_dwPort = dwPort;
	m_bReUseAddr = bReUseAddr;

#	ifndef _WIN32
	{
		m_hListenSock = socket(AF_INET, SOCK_STREAM, 0);
	}
#	endif

	if (INVALID_SOCKET == m_hListenSock)
	{
		FTL("%s create socket error, %s:%d, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	//设置IO数据的句柄
	m_stPerIoData.hSock = m_hListenSock;

	//sock选项设置
	//设置套接字延迟关闭（等待套接字发送缓冲区中的数据发送完成）
	stLinger.l_onoff = 1;
	stLinger.l_linger = 0;
	nResult = ::setsockopt(m_hListenSock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	LOG_PROCESS_ERROR(0 == nResult);

	//设置监听地址重复使用
	nReUse = 1;
	if (m_bReUseAddr)
	{
		nResult = ::setsockopt(m_hListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&nReUse, sizeof(nReUse));
		LOG_PROCESS_ERROR(0 == nResult);
	}

	//设置非阻塞模式
#	ifndef _WIN32
	{
		LPINT32 nFlag = fcntl(m_hListenSock, F_GETFL, 0);
		LOG_PROCESS_ERROR(nFlag >= 0);

		nResult = fcntl(m_hListenSock, F_SETFL, nFlag | O_NONBLOCK);
		LOG_PROCESS_ERROR(nResult >= 0);
	}
#	endif

	memset(&stListenAddr, 0, sizeof(stListenAddr));
	stListenAddr.sin_family = AF_INET;
	if (m_strIP == "0")
	{
		stListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		stListenAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	}
	stListenAddr.sin_port = htons(m_dwPort);

	nResult = ::bind(m_hListenSock, (struct sockaddr*)&stListenAddr, sizeof(struct sockaddr));
	if (nResult < 0)
	{
		FTL("%s bind at %s:%d failed, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = ::listen(m_hListenSock, LISTEN_QUEUE_COUNT);
	if (nResult < 0)
	{
		FTL("%s listen at %s:%d failed, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	//lpSleep(300);
	SetState(eCommonState_Inited);

	IMP("start listening %s:%d !", m_strIP.c_str(), m_dwPort);

	return TRUE;
Exit0:

	return FALSE;
}

void LPAPI LPLinuxNetListener::OnNetEvent(PER_IO_DATA* pstPerIoData)
{
	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->ptEpollEvent);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Accept);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Listener);

	if (pstPerIoData->ptEpollEvent->events & EPOLLRDHUP
		|| pstPerIoData->ptEpollEvent->events & EPOLLHUP
		|| pstPerIoData->ptEpollEvent->events & EPOLLERR)
	{
		pstPerIoData->bOperateRet = FALSE;
		OnAccept(pstPerIoData->bOperateRet, pstPerIoData);
	}
	else if (pstPerIoData->ptEpollEvent->events & EPOLLIN)
	{
		pstPerIoData->bOperateRet = TRUE;
		OnAccept(pstPerIoData->bOperateRet, pstPerIoData);
	}
	else
	{
		DBG("LPLinuxNetListener::OnNetEvent recv unkonw events");
	}

Exit0:
	return;
}

void LPAPI LPLinuxNetListener::OnAccept(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	ILPSockerImpl* pSocker = NULL;
	const char cArg = 1;
	LPUINT32 dwCurValidConnectCount = 0;

	LPINT32 idwRetLocal = 0;
	LPINT32 idwRemoteAddrLen = 0;
	LPINT32 idwLocalAddrLen = 0;
	sockaddr_in stRemoteAddr;
	sockaddr_in stLocalAddr;

	LOG_PROCESS_ERROR(pstPerIoData);
	
	if (bSuccess)
	{
		idwRemoteAddrLen = sizeof(struct sockaddr_in);
		memset(&stRemoteAddr, 0x00, sizeof(stRemoteAddr));
		hSock = accept(m_hListenSock, (struct sockaddr*)&stRemoteAddr, &idwRemoteAddrLen);
		LOG_PROCESS_ERROR(INVALID_SOCKET != hSock);
	}

	switch (GetState())
	{
		case eCommonState_NoInit:
		{
			if (bSuccess)
			{
				lpCloseSocket(hSock);
			}
			LOG_PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Initing:
		{
			INF("listener is initing, discard accept", __FUNCTION__);
			if (bSuccess)
			{
				lpCloseSocket(hSock);
			}
			PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Inited:
		{
			if (!bSuccess)
			{
				FTL("%s accept failed, errno %d", __FUNCTION__, WSAGetLastError());
				PROCESS_ERROR(FALSE);
			}
			else
			{
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
				
				//获取本地地址
				idwLocalAddrLen = sizeof(struct sockaddr_in);
				memset(&stLocalAddr, 0x00, sizeof(stLocalAddr));
				idwRetLocal = getsockname(hSock, (sockaddr*)&stLocalAddr, &idwLocalAddrLen);
				if (0 != idwRetLocal)
				{
					LOG_CHECK_ERROR(FALSE);
					m_pNetImpl->GetSockerMgr().Release(pSocker);
					lpCloseSocket(hSock);
					PROCESS_ERROR(FALSE);
				}

				//设置sock选项
				//禁用 Nagle’s Algorithm
				::setsockopt(hSock, IPPROTO_TCP, TCP_NODELAY, &cArg, sizeof(cArg));
				
				//设置非阻塞模式
#				ifndef _WIN32
				{
					LPINT32 nFlag = fcntl(m_hListenSock, F_GETFL, 0);
					LOG_PROCESS_ERROR(nFlag >= 0);

					nResult = fcntl(m_hListenSock, F_SETFL, nFlag | O_NONBLOCK);
					LOG_PROCESS_ERROR(nResult >= 0);
				}
#				endif

				//设置LPSocker对象
				pSocker->SetSock(hSock);
				pSocker->SetConnect(true);
				pSocker->SetRemoteIp(stRemoteAddr.sin_addr.s_addr);
				pSocker->SetRemotePort(stRemoteAddr.sin_port);
				pSocker->SetLocalIp(stLocalAddr.sin_addr.s_addr);
				pSocker->SetLocalPort(stLocalAddr.sin_port);

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
			if (bSuccess)
			{
				lpCloseSocket(hSock);
			}
			PROCESS_SUCCESS(TRUE);
		}
		break;
	case eCommonState_Close:
		{
			if (bSuccess)
			{
				lpCloseSocket(hSock);
			}
			PROCESS_SUCCESS(TRUE);
		}
		break;
	default:
		if (bSuccess)
		{
			lpCloseSocket(hSock);
		}
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit1:
Exit0:
	return;
}



//end声明所处的名字空间
NS_LZPL_END