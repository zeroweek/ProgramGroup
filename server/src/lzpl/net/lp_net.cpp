#include "lp_net.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPNetImpl::LPNetImpl(void)
{
	m_dwState = eCommonState_NoInit;
	m_dwRef = 0;
	m_dwMaxCreateId = 0;
	m_pNetMessageHandler = NULL;
	m_pReactor = nullptr;

	memset(&m_oNetConfig, 0, sizeof(m_oNetConfig));
	memset(&m_stModuleVersion, 0, sizeof(m_stModuleVersion));

	m_mapListener.clear();
	m_mapConnector.clear();
}

LPNetImpl::~LPNetImpl(void)
{
	UnInit();
}

std::shared_ptr<ILPListener> LPAPI LPNetImpl::CreateListenerCtrl(ILPPacketParser* pPacketParser)
{
	LPINT32 nResult = 0;
	std::shared_ptr<ILPListenerImpl> pListener;

	LOG_PROCESS_ERROR(pPacketParser);

	pListener = ILPListenerImpl::NewListenerImpl();
	LOG_PROCESS_ERROR(pListener != nullptr);

	nResult = pListener->Init(this, pPacketParser, _CreateId());
	LOG_PROCESS_ERROR(nResult);

	m_mapListener.insert(make_pair(pListener->GetId(), pListener));

	return pListener;
Exit0:
	return NULL;
}

std::shared_ptr<ILPConnector> LPAPI LPNetImpl::CreateConnectorCtrl(ILPPacketParser* pPacketParser)
{
	LPINT32 nResult = 0;
	std::shared_ptr<ILPConnectorImpl> pConnector;

	LOG_PROCESS_ERROR(pPacketParser);

	pConnector = ILPConnectorImpl::NewConnectorImpl();
	LOG_PROCESS_ERROR(pConnector);

	nResult = pConnector->Init(this, pPacketParser, _CreateId());
	LOG_PROCESS_ERROR(nResult);

	m_mapConnector.insert(make_pair(pConnector->GetId(), pConnector));

	return pConnector;
Exit0:
	return NULL;
}

std::shared_ptr<ILPListenerImpl> LPAPI LPNetImpl::FindListener(LPUINT32 dwId)
{
	std::shared_ptr<ILPListenerImpl> pListener;
	MAP_LISTENER::iterator fit;

	fit = m_mapListener.find(dwId);
	if (fit != m_mapListener.end())
	{
		pListener = fit->second;
	}

	return pListener;
}

std::shared_ptr<ILPConnectorImpl> LPAPI LPNetImpl::FindConnector(LPUINT32 dwId)
{
	std::shared_ptr<ILPConnectorImpl> pConnector;
	MAP_CONNECTOR::iterator fit;

	fit = m_mapConnector.find(dwId);
	if (fit != m_mapConnector.end())
	{
		pConnector = fit->second;
	}

	return pConnector;
}

BOOL LPAPI LPNetImpl::Run(LPINT32 nCount /*= -1*/)
{
	LPINT32 nResult = 0;

	do
	{
		nResult = m_oEventMgr.HaveEventForHandled();
		PROCESS_SUCCESS(!nResult);

		m_oEventMgr.HandleOneEvent();

	} while (--nCount != 0);

	nResult = m_oEventMgr.HaveEventForHandled();
	PROCESS_SUCCESS(!nResult);

	return FALSE;

Exit1:
	return TRUE;
}

BOOL LPAPI LPNetImpl::Init(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig)
{
	LPINT32 nResult = 0;

	m_dwState = eCommonState_Initing;

	m_pNetMessageHandler = pNetMessageHandler;
	LOG_PROCESS_ERROR(m_pNetMessageHandler);

	LOG_PROCESS_ERROR(pNetConfig);
	memcpy(&m_oNetConfig, pNetConfig, sizeof(m_oNetConfig));

	LOG_PROCESS_ERROR(m_oNetConfig.dwIoType > eIoType_None);

	nResult = m_oSockerMgr.Init(this);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oEventMgr.Init(this, m_pNetMessageHandler, m_oNetConfig.dwNetEventListCount);
	LOG_PROCESS_ERROR(nResult);

	m_pReactor = ILPReactor::NewReactor(m_oNetConfig.dwIoType);
	LOG_PROCESS_ERROR(m_pReactor != nullptr);

	m_dwState = eCommonState_Inited;

	return TRUE;

Exit0:

	UnInit();

	return FALSE;
}

LPSockerMgr &LPAPI LPNetImpl::GetSockerMgr(void)
{
	return m_oSockerMgr;;
}

LPEventMgr &LPAPI LPNetImpl::GetEventMgr(void)
{
	return m_oEventMgr;
}

ILPReactor& LPAPI LPNetImpl::GetReactorImpl(void)
{
	return *m_pReactor;
}

NET_CONFIG& LPAPI LPNetImpl::GetNetConfig(void)
{
	return m_oNetConfig;
}

void LPAPI LPNetImpl::UnInit()
{
	LPINT32 nResult = 0;
	LPUINT64 qwTickStart = 0;
	MAP_LISTENER::iterator iterListener;
	MAP_CONNECTOR::iterator iterConnector;

	if (m_dwState <= eCommonState_NoInit || m_dwState > eCommonState_Inited)
	{
		return;
	}
	m_dwState = eCommonState_UnIniting;

	IMP("net uniniting ...");
	LPPRINTF("net uniniting ...\n");

	//等待所有关闭的socker数据发送完毕，并且确定所有socker都关闭
	qwTickStart = lpGetTickCountEx();
	while (lpGetTickCountEx() < qwTickStart + (DELAY_CLOSE_SOCKET_TIME + DELAY_RELEASE_SOCKER_TIME))
	{
		Run();
	}

	//释放reactor
	ILPReactor::DeleteReactor(m_pReactor);

	nResult = m_oEventMgr.UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = m_oSockerMgr.UnInit();
	LOG_CHECK_ERROR(nResult);

	for (iterListener = m_mapListener.begin(); iterListener != m_mapListener.end(); ++iterListener)
	{
		LOG_CHECK_ERROR(iterListener->second);
		IF_NULL_CONTINUE(iterListener->second);
		ILPListenerImpl::DeleteListenerImpl(iterListener->second);
	}
	m_mapListener.clear();

	for (iterConnector = m_mapConnector.begin(); iterConnector != m_mapConnector.end(); ++iterConnector)
	{
		LOG_CHECK_ERROR(iterConnector->second);
		IF_NULL_CONTINUE(iterConnector->second);
		ILPConnectorImpl::DeleteConnectorImpl(iterConnector->second);
	}
	m_mapConnector.clear();

	IMP("net uninit success !");
	LPPRINTF("net uninit success !\n");

	m_dwState = eCommonState_UnInited;
}

LPUINT32 LPAPI LPNetImpl::_CreateId()
{
	LPUINT32 dwNewId = 0;

	m_oLock.Lock();
	dwNewId = ++m_dwMaxCreateId;
	m_oLock.UnLock();

	return dwNewId;
}

BOOL LPAPI ILPNet::GlobalInit()
{
#   if defined _WIN32
	{
		WSADATA stData;
		::WSAStartup(MAKEWORD(2, 2), &stData);
	}
#   endif

	return TRUE;
}

void LPAPI ILPNet::GlobalUnInit()
{
#   if defined _WIN32
	{
		::WSACleanup();
	}
#   endif
}

std::shared_ptr<ILPNet> LPAPI ILPNet::CreateNetModule(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig)
{
	LPINT32 nResult = 0;
	std::shared_ptr<ILPNet> poNetImpl;

	PRINTF_PROCESS_ERROR(lpGetLzplLoggerCtrl());

	LOG_PROCESS_ERROR(pNetMessageHandler);
	LOG_PROCESS_ERROR(pNetConfig);

	poNetImpl = std::make_shared<LPNetImpl>();
	LOG_PROCESS_ERROR(poNetImpl != nullptr);

	nResult = ((LPNetImpl*)poNetImpl.get())->Init(pNetMessageHandler, pNetConfig);
	LOG_PROCESS_ERROR(nResult);

	return poNetImpl;

Exit0:

	if (poNetImpl != nullptr)
	{
		((LPNetImpl*)poNetImpl.get())->UnInit();
		ILPNet::DeleteNetModule(poNetImpl);
	}

	return nullptr;
}

void LPAPI ILPNet::DeleteNetModule(std::shared_ptr<ILPNet>& poNet)
{
	poNet = nullptr;
}



//end声明所处的名字空间
NS_LZPL_END