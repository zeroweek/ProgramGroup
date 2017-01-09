#include "lp_connector.h"
#include "lp_processerror.h"
#include "lp_net.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



std::shared_ptr<ILPConnectorImpl> LPAPI ILPConnectorImpl::NewConnectorImpl()
{
	return std::make_shared<LPConnector>();
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
	m_pSocker = NULL;

	m_lpfnConnectEx = NULL;
	m_pstPerIoData = NULL;
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
		CancelIo((HANDLE)m_hConnectSock);
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
	}

	if (NULL != m_pPacketParser)
	{
		m_pPacketParser->Release();
		m_pPacketParser = NULL;
	}

	m_pSocker = NULL;
	m_lpfnConnectEx = NULL;

	SAFE_DELETE(m_pstPerIoData);

	SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

BOOL LPAPI LPConnector::Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
	SetState(eCommonState_Initing);

	m_strIP = strIP;
	m_dwPort = dwPort;

	SetReconnect(bReconnect);

	nResult = _InitConnectEx();
	LOG_PROCESS_ERROR(nResult);

	if (NULL == m_pstPerIoData)
	{
		m_pstPerIoData = new PER_IO_DATA();
	}
	LOG_PROCESS_ERROR(m_pstPerIoData);

	//lpSleep(10);
	SetState(eCommonState_Inited);

	LPPRINTF("start connecting %s:%d !\n", m_strIP.c_str(), m_dwPort);
	IMP("start connecting %s:%d !", m_strIP.c_str(), m_dwPort);

	nResult = _PostConnectEx(m_pstPerIoData);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	UnInit();

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

	nResult = _PostConnectEx(m_pstPerIoData);
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

void LPAPI LPConnector::OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nLastError;

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(pstPerIoData->eIoOptType == eIoOptType_Connect 
		|| pstPerIoData->eIoOptType == eIoOptType_Send
		|| pstPerIoData->eIoOptType == eIoOptType_Recv);
	LOG_PROCESS_ERROR(pstPerIoData->eHandlerType == eEventHandlerType_Connector);
	
	if (pstPerIoData->eIoOptType == eIoOptType_Connect)
	{
		OnConnect(bOperateRet, pstPerIoData);
	}
	else
	{
		//这里WSAGetLastError获得的结果是否是对应本次io操作？？？或许需要用GetLastError比较准确？？？
		nLastError = WSAGetLastError();

		//调用lpCloseSocket时，会触发收回连接句柄关联的正在执行的io操作，
		//此处进行判断拦截，防止访问已经被释放的socker对象
		if (!bOperateRet && ERROR_OPERATION_ABORTED == nLastError)
		{
			PROCESS_SUCCESS(TRUE);
		}

		LOG_PROCESS_ERROR(m_pSocker);
		if (bOperateRet)
		{
			if (0 == pstPerIoData->qwByteTransferred)
			{
				m_pSocker->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 2, nLastError), TRUE);
			}
			else
			{
				if (eIoOptType_Recv == pstPerIoData->eIoOptType)
				{
					m_pSocker->OnRecv((LPUINT32)pstPerIoData->qwByteTransferred);
				}
				else
				{
					m_pSocker->OnSend((LPUINT32)pstPerIoData->qwByteTransferred);
				}
			}
		}
		else
		{
			m_pSocker->Close(SOCK_ERR_CODE(eSockErrCode_ReactorErrorEvent, 1, nLastError), TRUE);
		}
	}

Exit1:
Exit0:
	return;
}

void LPConnector::OnConnect(BOOL bSuccess, PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	ILPSockerImpl* pSocker = NULL;
	sockaddr_in stLocalAddr;
	LPINT32 idwLocalAddrLen = 0;
	LPINT32 idwRetLocal = 0;
	sockaddr_in stRemoteAddr;
	LPINT32 idwRemoteAddrLen = 0;
	LPINT32 idwRetRemote = 0;
	const char cArg = 1;

	LOG_PROCESS_ERROR(pstPerIoData);
	LOG_PROCESS_ERROR(eIoOptType_Connect == pstPerIoData->eIoOptType);

	LOG_PROCESS_ERROR(pstPerIoData->hSock == m_hConnectSock);

	switch (GetState())
	{
	case eCommonState_NoInit:
	{
		lpCloseSocket(m_hConnectSock);
		LOG_PROCESS_ERROR(FALSE);
	}
	break;
	case eCommonState_Initing:
	{
		INF("connector is initing, discard connect", __FUNCTION__);
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
		nResult = _PostConnectEx(m_pstPerIoData);
		LOG_PROCESS_ERROR(nResult);
		PROCESS_ERROR(FALSE);
	}
	break;
	case eCommonState_Inited:
	{
		if (!bSuccess)
		{
			m_pNetImpl->GetEventMgr().PushConnectErrorEvent(std::shared_ptr<ILPConnectorImpl>(this), WSAGetLastError());
			lpCloseSocket(m_hConnectSock);
			m_hConnectSock = INVALID_SOCKET;
			SetSocker(NULL);
			nResult = _PostConnectEx(m_pstPerIoData);
			LOG_PROCESS_ERROR(nResult);
		}
		else
		{
			pSocker = m_pNetImpl->GetSockerMgr().Create(m_pPacketParser, m_dwId, FALSE);
			if (NULL == pSocker)
			{
				LOG_CHECK_ERROR(FALSE);
				lpCloseSocket(m_hConnectSock);
				m_hConnectSock = INVALID_SOCKET;
				nResult = _PostConnectEx(m_pstPerIoData);
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
				lpCloseSocket(m_hConnectSock);
				m_hConnectSock = INVALID_SOCKET;
				nResult = _PostConnectEx(m_pstPerIoData);
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
				lpCloseSocket(m_hConnectSock);
				m_hConnectSock = INVALID_SOCKET;
				nResult = _PostConnectEx(m_pstPerIoData);
				LOG_PROCESS_ERROR(nResult);
				PROCESS_ERROR(FALSE);
			}

			//设置sock选项
			::setsockopt(m_hConnectSock, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
			::setsockopt(m_hConnectSock, IPPROTO_TCP, TCP_NODELAY, &cArg, sizeof(cArg));

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
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
		LOG_PROCESS_ERROR(FALSE);
	}
	break;
	case eCommonState_Close:
	{
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
		PROCESS_SUCCESS(TRUE);
	}
	break;
	default:
		lpCloseSocket(m_hConnectSock);
		m_hConnectSock = INVALID_SOCKET;
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

Exit1:
Exit0:
	return;
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

BOOL LPAPI LPConnector::_InitConnectEx()
{
	LPINT32 nResult = 0;
	SOCKET hSock = INVALID_SOCKET;
	DWORD dwBytes = 0;
	GUID stGuidAcceptEx = WSAID_CONNECTEX;

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

BOOL LPAPI LPConnector::_PostConnectEx(PER_IO_DATA* pstPerIoData)
{
	LPINT32 nResult = 0;
	DWORD dwBytes = 0;
	sockaddr_in stAddr;
	sockaddr_in stLocalAddr;

	LOG_PROCESS_ERROR(pstPerIoData);

	m_hConnectSock = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_hConnectSock)
	{
		FTL("%s WSASocket failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	//需要如果不bind，后面调用ConnectEx会出现10022错误码
	memset(&stLocalAddr, 0, sizeof(stLocalAddr));
	stLocalAddr.sin_family = AF_INET;
	nResult = ::bind(m_hConnectSock, (sockaddr*)&stLocalAddr, sizeof(stLocalAddr));
	if (nResult < 0)
	{
		FTL("%s bind failed, errno %d", __FUNCTION__, WSAGetLastError());
		PROCESS_ERROR(FALSE);
	}

	//注意：connectorctr在此注册，之后连接创建的socker不需要重新注册（注册也会返回失败），
	//即socker与connectorctrl公用一个回调接口
	nResult = m_pNetImpl->GetReactorImpl().RegisterEventHandler(this);
	LOG_PROCESS_ERROR(nResult);

	memset(&stAddr, 0, sizeof(stAddr));
	stAddr.sin_family = AF_INET;
	stAddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	stAddr.sin_port = htons(m_dwPort);

	pstPerIoData->hSock = m_hConnectSock;
	pstPerIoData->eIoOptType = eIoOptType_Connect;
	pstPerIoData->eHandlerType = eEventHandlerType_Connector;
	memset(&pstPerIoData->stOverlapped, 0, sizeof(pstPerIoData->stOverlapped));

	dwBytes = 0;
	nResult = m_lpfnConnectEx(
		m_hConnectSock,
		(const sockaddr*)&stAddr,
		sizeof(stAddr),
		NULL,
		0,
		NULL,
		&pstPerIoData->stOverlapped
	);

	if (!nResult)
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			FTL("%s m_lpfnConnectEx failed, errno %d", __FUNCTION__, WSAGetLastError());
			lpCloseSocket(m_hConnectSock);
			m_hConnectSock = INVALID_SOCKET;
			PROCESS_ERROR(FALSE);
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