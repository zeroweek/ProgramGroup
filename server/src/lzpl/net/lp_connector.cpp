#include "lp_connector.h"
#include "lp_processerror.h"
#include "lp_net.h"
#include "lp_global.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



std::shared_ptr<ILPConnectorImpl> LPAPI ILPConnectorImpl::NewConnectorImpl(LPUINT32 dwIoType)
{
	switch (dwIoType)
	{
	case eIoType_CompletionPort:
		{
			return std::make_shared<LPWinNetConnector>();
		}
		break;
	case eIoType_Epoll:
		{
			return std::make_shared<LPLinuxNetConnector>();
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

void LPAPI ILPConnectorImpl::DeleteConnectorImpl(std::shared_ptr<ILPConnectorImpl>& pConnector)
{
	pConnector = nullptr;
}



LPConnector::LPConnector()
{
	m_dwId = 0;
	SetState(eCommonState_NoInit);
	m_dwPort = 0;
	SetReconnect(FALSE);
	m_hConnectSock = INVALID_SOCKET;
	m_pPacketParser = NULL;
	m_pNetImpl = NULL;
	SetSocker(nullptr);

	m_stPerIoData.eIoOptType = eIoOptType_Connect;
	m_stPerIoData.eHandlerType = eEventHandlerType_Connector;
}

LPConnector::~LPConnector()
{
	UnInit();
}

BOOL LPAPI LPConnector::Init(LPNetImpl* pNetImpl, ILPPacketParser* pPacketParser, LPUINT32 dwId)
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

BOOL LPAPI LPConnector::UnInit()
{
	PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

	SetState(eCommonState_UnIniting);

	if (INVALID_SOCKET != m_hConnectSock)
	{
		lpCancelIoEx(m_hConnectSock);
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
	}

	if (NULL != m_pPacketParser)
	{
		m_pPacketParser->Release();
		m_pPacketParser = NULL;
	}

	SetSocker(nullptr);

	SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPConnector::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect)
{
	LPINT32 nResult = 0;

	m_strIP = strIP;
	m_dwPort = dwPort;

	SetReconnect(bReconnect);

	nResult = InitConnectEx();
	LOG_PROCESS_ERROR(nResult);

	LPPRINTF("start connecting %s:%d !\n", m_strIP.c_str(), m_dwPort);
	IMP("start connecting %s:%d !", m_strIP.c_str(), m_dwPort);

	nResult = PostConnectEx(m_stPerIoData);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	return FALSE;
}

void LPAPI LPConnector::Stop()
{
	PROCESS_SUCCESS(GetState() >= eCommonState_Close);

	LOG_PROCESS_ERROR(GetState() == eCommonState_Inited);
	SetState(eCommonState_Close);

	// stop时，不需关闭socket，统一由ILPSocker的Close关闭
	//if (INVALID_SOCKET != m_hConnectSock)
	//{
	//	CancelIo((HANDLE)m_hConnectSock);
	//	lpCloseSocket(m_hConnectSock);
	//	m_hConnectSock = INVALID_SOCKET;
	//}

Exit1:
Exit0:
	return;
}

LPUINT32 LPAPI LPConnector::GetId()
{
	return m_dwId;
}

void LPAPI LPConnector::SetReconnect(BOOL bReconnect)
{
	m_bReconnect = bReconnect;
}

BOOL LPAPI LPConnector::IsReconnect()
{
	return m_bReconnect;
}

BOOL LPAPI LPConnector::Reconnect()
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(eCommonState_Inited == GetState());
	LOG_PROCESS_ERROR(NULL == GetSocker() && INVALID_SOCKET == m_hConnectSock);

	nResult = PostConnectEx(m_stPerIoData);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

HANDLE LPAPI LPConnector::GetHandle()
{
	return (HANDLE)m_hConnectSock;
}

e_EventHandlerType LPAPI LPConnector::GetEventHandlerType()
{
	return eEventHandlerType_Connector;
}

PER_IO_DATA* LPAPI LPConnector::GetEventHandlerData()
{
	return &m_stPerIoData;
}

void LPAPI LPConnector::OnClose()
{
	LPINT32 nResult = 0;

	m_hConnectSock = INVALID_SOCKET;
	SetSocker(NULL);

	if (IsReconnect())
	{
		nResult = Reconnect();
		LOG_PROCESS_ERROR(nResult);
	}

Exit0:
	return;
}

void LPAPI LPConnector::SetSocker(ILPSockerImpl* pSocker)
{
	m_pSocker = pSocker;
}

ILPSockerImpl* LPAPI LPConnector::GetSocker()
{
	return m_pSocker;
}

void LPAPI LPConnector::SetState(LPUINT32 dwState)
{
	m_dwState = dwState;
}

LPUINT32 LPAPI LPConnector::GetState()
{
	return m_dwState;
}

void LPAPI LPConnector::CloseConnectSock()
{
	lpCloseSocket(m_hConnectSock);
	m_hConnectSock = INVALID_SOCKET;
}



LPWinNetConnector::LPWinNetConnector()
{
	m_lpfnConnectEx = nullptr;
}

LPWinNetConnector::~LPWinNetConnector()
{
	UnInit();
}

BOOL LPAPI LPWinNetConnector::UnInit()
{
	PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

	SetState(eCommonState_UnIniting);

	LOG_CHECK_ERROR(LPConnector::UnInit());

	SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPWinNetConnector::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
	SetState(eCommonState_Initing);

	nResult = LPConnector::Start(strIP, dwPort, bReconnect);
	LOG_PROCESS_ERROR(nResult);

	//lpSleep(10);
	SetState(eCommonState_Inited);

	return TRUE;
Exit0:

	return FALSE;
}

void LPAPI LPWinNetConnector::OnNetEvent(PER_IO_DATA* pstPerIoData)
{
	LPINT32 nLastError;

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Connect
		|| pstPerIoData->eIoOptType == eIoOptType_Send
		|| pstPerIoData->eIoOptType == eIoOptType_Recv);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Connector);

	if (pstPerIoData->eIoOptType == eIoOptType_Connect)
	{
		OnConnect(pstPerIoData->bOperateRet, pstPerIoData);
	}
	else
	{
		//这里WSAGetLastError获得的结果是否是对应本次io操作？？？或许需要用GetLastError比较准确？？？
		nLastError = WSAGetLastError();

		//调用lpCloseSocket时，会触发收回连接句柄关联的正在执行的io操作，
		//此处进行判断拦截，防止访问已经被释放的socker对象
		if (!pstPerIoData->bOperateRet && ERROR_OPERATION_ABORTED == nLastError)
		{
			PROCESS_SUCCESS(TRUE);
		}

		LOG_PROCESS_ERROR(GetSocker());
		if (pstPerIoData->bOperateRet)
		{
			if (0 == pstPerIoData->qwByteTransferred)
			{
				GetSocker()->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 2, nLastError), TRUE);
			}
			else
			{
				if (eIoOptType_Recv == pstPerIoData->eIoOptType)
				{
					GetSocker()->OnRecv((LPUINT32)pstPerIoData->qwByteTransferred);
				}
				else
				{
					GetSocker()->OnSend((LPUINT32)pstPerIoData->qwByteTransferred);
				}
			}
		}
		else
		{
			GetSocker()->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 1, nLastError), TRUE);
		}
	}

Exit1:
Exit0:
	return;
}

void LPAPI LPWinNetConnector::OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	ILPSockerImpl* pSocker = NULL;
	sockaddr_in stLocalAddr;
	LPINT32 idwRetLocal = 0;
	sockaddr_in stRemoteAddr;
	LPINT32 idwRetRemote = 0;
	const char cArg = 1;

#	ifdef _WIN32
	LPINT32 idwLocalAddrLen = 0;
	LPINT32 idwRemoteAddrLen = 0;
#	else
	socklen_t idwLocalAddrLen = 0;
	socklen_t idwRemoteAddrLen = 0;
#	endif

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(eIoOptType_Connect == pstPerIoData->eIoOptType);

	LOG_PROCESS_ERROR(pstPerIoData->hSock == m_hConnectSock);

	switch (GetState())
	{
	case eCommonState_NoInit:
		{
			CloseConnectSock();
			LOG_PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Initing:
		{
			INF("connector is initing, discard connect", __FUNCTION__);
			CloseConnectSock();
			nResult = PostConnectEx(m_stPerIoData);
			LOG_PROCESS_ERROR(nResult);
			PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Inited:
		{
			if (!bSuccess)
			{
				m_pNetImpl->GetEventMgr().PushConnectErrorEvent(std::shared_ptr<ILPConnectorImpl>(this), WSAGetLastError());
				CloseConnectSock();
				SetSocker(NULL);
				nResult = PostConnectEx(m_stPerIoData);
				LOG_PROCESS_ERROR(nResult);
			}
			else
			{
				pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, FALSE);
				if (NULL == pSocker)
				{
					LOG_CHECK_ERROR(FALSE);
					CloseConnectSock();
					nResult = PostConnectEx(m_stPerIoData);
					LOG_PROCESS_ERROR(nResult);
					PROCESS_ERROR(FALSE);
				}

				IMP("connector create socker, socker_id=%d, hSock=%d !", pSocker->GetSockerId(), m_hConnectSock);

				//获取本地地址
				memset(&stLocalAddr, 0, sizeof(stLocalAddr));
				idwLocalAddrLen = sizeof(stLocalAddr);
				idwRetLocal = getsockname(m_hConnectSock, (sockaddr*)&stLocalAddr, &idwLocalAddrLen);
				if (0 != idwRetLocal)
				{
					LOG_CHECK_ERROR(FALSE);
					m_pNetImpl->GetSockerMgr().Release(pSocker);
					CloseConnectSock();
					nResult = PostConnectEx(m_stPerIoData);
					LOG_PROCESS_ERROR(nResult);
					PROCESS_ERROR(FALSE);
				}

				//获取远端地址
				memset(&stRemoteAddr, 0, sizeof(stRemoteAddr));
				idwRemoteAddrLen = sizeof(stRemoteAddr);
				idwRetRemote = getpeername(m_hConnectSock, (sockaddr*)&stRemoteAddr, &idwRemoteAddrLen);
				if (0 != idwRetLocal)
				{
					LOG_CHECK_ERROR(FALSE);
					m_pNetImpl->GetSockerMgr().Release(pSocker);
					CloseConnectSock();
					nResult = PostConnectEx(m_stPerIoData);
					LOG_PROCESS_ERROR(nResult);
					PROCESS_ERROR(FALSE);
				}

				//设置sock选项
#				ifdef _WIN32
				{
					::setsockopt(m_hConnectSock, IPPROTO_TCP, TCP_NODELAY, &cArg, sizeof(cArg));
				}
#				endif

				//设置LPSocker对象
				pSocker->SetSock(m_hConnectSock);
				pSocker->SetConnect(true);
				pSocker->SetRemoteIp(stRemoteAddr.sin_addr.s_addr);
				pSocker->SetRemotePort(ntohs(stRemoteAddr.sin_port));
				pSocker->SetLocalIp(stLocalAddr.sin_addr.s_addr);
				pSocker->SetLocalPort(ntohs(stLocalAddr.sin_port));

				//设置本连接器关联的socker对象
				//注意顺序：需要在socker收发数据之前设置，否则reactor处理获取socker为NULL
				SetSocker(pSocker);

				//push连接建立事件
				m_pNetImpl->GetEventMgr().PushEstablishEvent(pSocker, FALSE);
			}
		}
		break;
	case eCommonState_UnIniting:
		case eCommonState_UnInited:
		{
			CloseConnectSock();
			LOG_PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Close:
		{
			CloseConnectSock();
			PROCESS_SUCCESS(TRUE);
		}
		break;
	default:
		CloseConnectSock();
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit1:
Exit0:
	return;
}

BOOL LPAPI LPWinNetConnector::InitConnectEx()
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	DWORD dwBytes = 0;
	GUID stGuidAcceptEx = WSAID_CONNECTEX;

#	ifdef _WIN32
	{
		hSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == hSock)
		{
			FTL("%s ::WSASocket failed, %s:%d, errno %d", __FUNCTION__, m_strIP.c_str(), m_dwPort, WSAGetLastError());
			PROCESS_ERROR(FALSE);
		}


		nResult = ::WSAIoctl(
			hSock,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&stGuidAcceptEx,
			sizeof(stGuidAcceptEx),
			&m_lpfnConnectEx,
			sizeof(LPFN_CONNECTEX),
			&dwBytes,
			NULL,
			NULL
		);
		LOG_PROCESS_ERROR(nResult == 0);
	}
#	endif

	//该sock只用于获取函数指针
	lpCloseSocket(hSock);

	LOG_PROCESS_ERROR(m_lpfnConnectEx);

	return TRUE;
Exit0:

	if (INVALID_SOCKET != hSock)
	{
		lpCloseSocket(hSock);
	}
	return FALSE;
}

BOOL LPAPI LPWinNetConnector::PostConnectEx(PER_IO_DATA& stPerIoData)
{
	LPINT32 nResult = 0;
	DWORD dwBytes = 0;
	sockaddr_in stRemotetAddr;
	sockaddr_in stLocalAddr;

#	ifdef _WIN32
	{
		m_hConnectSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	}
#	endif

	if (INVALID_SOCKET == m_hConnectSock)
	{
		FTL("%s WSASocket failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	//需要如果不bind，后面调用ConnectEx会出现10022错误码
	memset(&stLocalAddr, 0, sizeof(stLocalAddr));
	stLocalAddr.sin_family = AF_INET;
	nResult = ::bind(m_hConnectSock, (sockaddr*)&stLocalAddr, sizeof(struct sockaddr));
	if (nResult < 0)
	{
		FTL("%s bind failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	//注意：connector在此注册，之后连接创建的socker不需要重新注册（注册也会返回失败），
	//即socker与connector公用一个回调接口
	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	memset(&stRemotetAddr, 0, sizeof(stRemotetAddr));
	stRemotetAddr.sin_family = AF_INET;
	stRemotetAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	stRemotetAddr.sin_port = htons(m_dwPort);

	stPerIoData.hSock = m_hConnectSock;
	memset(&stPerIoData.stOverlapped, 0, sizeof(stPerIoData.stOverlapped));

	dwBytes = 0;
	nResult = m_lpfnConnectEx(
		m_hConnectSock,
		(const sockaddr*)&stRemotetAddr,
		sizeof(stRemotetAddr),
		NULL,
		0,
		NULL,
		&stPerIoData.stOverlapped
	);

	if (!nResult)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			LOG_PROCESS_FATAL_WITH_MSG(FALSE, "m_lpfnConnectEx failed, errno %d", WSAGetLastError());
		}
	}

	return TRUE;
Exit0:

	if (INVALID_SOCKET != m_hConnectSock)
	{
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
	}

	return FALSE;
}



LPLinuxNetConnector::LPLinuxNetConnector()
{

}

LPLinuxNetConnector::~LPLinuxNetConnector()
{
	UnInit();
}

BOOL LPAPI LPLinuxNetConnector::UnInit()
{
	PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

	SetState(eCommonState_UnIniting);

	LOG_CHECK_ERROR(LPConnector::UnInit());

	SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPLinuxNetConnector::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
	SetState(eCommonState_Initing);

	nResult = LPConnector::Start(strIP, dwPort, bReconnect);
	LOG_PROCESS_ERROR(nResult);

	//lpSleep(10);
	SetState(eCommonState_Inited);

	return TRUE;
Exit0:

	return FALSE;
}

void LPAPI LPLinuxNetConnector::OnNetEvent(PER_IO_DATA* pstPerIoData)
{
	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Connect);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Connector);

	if (pstPerIoData->ptEpollEvent->events & EPOLLRDHUP
		|| pstPerIoData->ptEpollEvent->events & EPOLLHUP
		|| pstPerIoData->ptEpollEvent->events & EPOLLERR)
	{
		pstPerIoData->bOperateRet = FALSE;
		OnConnect(pstPerIoData->bOperateRet, pstPerIoData);
	}
	else if (pstPerIoData->ptEpollEvent->events & EPOLLIN || pstPerIoData->ptEpollEvent->events & EPOLLOUT)
	{
		pstPerIoData->bOperateRet = TRUE;
		OnConnect(pstPerIoData->bOperateRet, pstPerIoData);
	}
	else
	{
		DBG("LPLinuxNetConnector::OnNetEvent recv unkonw events");
	}

Exit0:
	return;
}

void LPAPI LPLinuxNetConnector::OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	ILPSockerImpl* pSocker = NULL;

	LPINT32 idwRetLocal = 0;
	LPINT32 idwRetRemote = 0;
	sockaddr_in stLocalAddr;
	sockaddr_in stRemoteAddr;

	const char cArg = 1;

#	ifdef _WIN32
	LPINT32 idwLocalAddrLen = 0;
	LPINT32 idwRemoteAddrLen = 0;
#	else
	socklen_t idwLocalAddrLen = 0;
	socklen_t idwRemoteAddrLen = 0;
#	endif

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(eIoOptType_Connect == pstPerIoData->eIoOptType);

	LOG_PROCESS_ERROR(pstPerIoData->hSock == m_hConnectSock);

	switch (GetState())
	{
	case eCommonState_NoInit:
		{
			CloseConnectSock();
			LOG_PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Initing:
		{
			INF("connector is initing, discard connect", __FUNCTION__);
			CloseConnectSock();
			nResult = PostConnectEx(m_stPerIoData);
			LOG_PROCESS_ERROR(nResult);
			PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Inited:
		{
			if (!bSuccess)
			{
				m_pNetImpl->GetEventMgr().PushConnectErrorEvent(std::shared_ptr<ILPConnectorImpl>(this), WSAGetLastError());
				CloseConnectSock();
				SetSocker(NULL);
				nResult = PostConnectEx(m_stPerIoData);
				LOG_PROCESS_ERROR(nResult);
			}
			else
			{
				pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, FALSE);
				if (NULL == pSocker)
				{
					LOG_CHECK_ERROR(FALSE);
					CloseConnectSock();
					nResult = PostConnectEx(m_stPerIoData);
					LOG_PROCESS_ERROR(nResult);
					PROCESS_ERROR(FALSE);
				}

				IMP("connector create socker, socker_id=%d, hSock=%d !", pSocker->GetSockerId(), m_hConnectSock);

				//获取本地地址
				idwLocalAddrLen = sizeof(struct sockaddr_in);
				memset(&stLocalAddr, 0, sizeof(stLocalAddr));
				idwRetLocal = getsockname(m_hConnectSock, (sockaddr*)&stLocalAddr, &idwLocalAddrLen);
				if (0 != idwRetLocal)
				{
					LOG_CHECK_ERROR(FALSE);
					m_pNetImpl->GetSockerMgr().Release(pSocker);
					CloseConnectSock();
					nResult = PostConnectEx(m_stPerIoData);
					LOG_PROCESS_ERROR(nResult);
					PROCESS_ERROR(FALSE);
				}

				//获取远端地址
				idwRemoteAddrLen = sizeof(struct sockaddr_in);
				memset(&stRemoteAddr, 0, sizeof(stRemoteAddr));
				idwRetRemote = getpeername(m_hConnectSock, (sockaddr*)&stRemoteAddr, &idwRemoteAddrLen);
				if (0 != idwRetLocal)
				{
					LOG_CHECK_ERROR(FALSE);
					m_pNetImpl->GetSockerMgr().Release(pSocker);
					CloseConnectSock();
					nResult = PostConnectEx(m_stPerIoData);
					LOG_PROCESS_ERROR(nResult);
					PROCESS_ERROR(FALSE);
				}

				//设置sock选项
				//禁用 Nagle’s Algorithm
				::setsockopt(m_hConnectSock, IPPROTO_TCP, TCP_NODELAY, &cArg, sizeof(cArg));

				//设置非阻塞模式
#				ifndef _WIN32
				{
					LPINT32 nFlag = fcntl(m_hConnectSock, F_GETFL, 0);
					LOG_PROCESS_ERROR(nFlag >= 0);

					nResult = fcntl(m_hConnectSock, F_SETFL, nFlag | O_NONBLOCK);
					LOG_PROCESS_ERROR(nResult >= 0);
				}
#				endif

				//设置LPSocker对象
				pSocker->SetSock(m_hConnectSock);
				pSocker->SetConnect(true);
				pSocker->SetRemoteIp(stRemoteAddr.sin_addr.s_addr);
				pSocker->SetRemotePort(ntohs(stRemoteAddr.sin_port));
				pSocker->SetLocalIp(stLocalAddr.sin_addr.s_addr);
				pSocker->SetLocalPort(ntohs(stLocalAddr.sin_port));

				//设置本连接器关联的socker对象
				//注意顺序：需要在socker收发数据之前设置，否则reactor处理获取socker为NULL
				SetSocker(pSocker);

				//注册事件连接改成socket
				nResult = m_pNetImpl->GetReactorImpl().UnRegisterEventHandler(this);
				LOG_CHECK_ERROR(nResult);

				nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(pSocker);
				if (!nResult)
				{
					LOG_CHECK_ERROR(FALSE);
					m_pNetImpl->GetSockerMgr().Release(pSocker);
					CloseConnectSock();
					PROCESS_ERROR(FALSE);
				}

				//push连接建立事件
				m_pNetImpl->GetEventMgr().PushEstablishEvent(pSocker, FALSE);
			}
		}
		break;
	case eCommonState_UnIniting:
		case eCommonState_UnInited:
		{
			CloseConnectSock();
			LOG_PROCESS_ERROR(FALSE);
		}
		break;
	case eCommonState_Close:
		{
		CloseConnectSock();
			PROCESS_SUCCESS(TRUE);
		}
		break;
	default:
		CloseConnectSock();
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit1:
Exit0:
	return;
}

BOOL LPAPI LPLinuxNetConnector::InitConnectEx()
{
	return TRUE;
}

BOOL LPAPI LPLinuxNetConnector::PostConnectEx(PER_IO_DATA& stPerIoData)
{
	LPINT32 nResult = 0;
	LPINT32 nError = 0;
	sockaddr_in stRemotetAddr;
	sockaddr_in stLocalAddr;

#	ifndef _WIN32
	{
		m_hConnectSock = socket(AF_INET, SOCK_STREAM, 0);
	}
#	endif

	if (INVALID_SOCKET == m_hConnectSock)
	{
		LOG_PROCESS_ERROR_WITH_MSG(FALSE, "socket failed, errno %d", WSAGetLastError());
	}

	//sock选项设置
	//设置非阻塞模式
#	ifndef _WIN32
	{
		LPINT32 nFlag = fcntl(m_hConnectSock, F_GETFL, 0);
		LOG_PROCESS_ERROR(nFlag >= 0);

		nResult = fcntl(m_hConnectSock, F_SETFL, nFlag | O_NONBLOCK);
		LOG_PROCESS_ERROR(nResult >= 0);
	}
#	endif

	//如果不bind，后面调用connect会不会像ConnectEx出现10022错误码
	memset(&stLocalAddr, 0, sizeof(stLocalAddr));
	stLocalAddr.sin_family = AF_INET;
	nResult = ::bind(m_hConnectSock, (sockaddr*)&stLocalAddr, sizeof(struct sockaddr));
	if (nResult < 0)
	{
		LOG_PROCESS_ERROR_WITH_MSG(FALSE, "bind failed, errno %d", WSAGetLastError());
	}

	//注意：connector在此注册，之后连接创建的socker不需要重新注册（注册也会返回失败），
	//即socker与connectorl公用一个回调接口
	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	memset(&stRemotetAddr, 0, sizeof(stRemotetAddr));
	stRemotetAddr.sin_family = AF_INET;
	stRemotetAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	stRemotetAddr.sin_port = htons(m_dwPort);

	stPerIoData.hSock = m_hConnectSock;

	nResult = connect(m_hConnectSock, (struct sockaddr*)&stRemotetAddr, sizeof(struct sockaddr));
	if (nResult < 0)
	{
		nError = WSAGetLastError();
		if (nError != EINPROGRESS && nError != EINTR
			&& nError != EALREADY && nError != EISCONN)
		{
			LOG_PROCESS_ERROR_WITH_MSG(FALSE, "connect %s:%d failed, errno %d", m_strIP.c_str(), m_dwPort, nError);
		}
	}

	return TRUE;
Exit0:

	if (INVALID_SOCKET != m_hConnectSock)
	{
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
	}

	return FALSE;
}



//end声明所处的名字空间
NS_LZPL_END