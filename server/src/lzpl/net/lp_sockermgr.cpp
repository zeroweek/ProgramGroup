#include "lp_sockermgr.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_dump.h"
#include "lp_net.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



LPSockerMgr::LPSockerMgr()
{
	m_dwMaxSockId = INVALID_SOCKER_ID;

	m_oValidList.clear();
	m_oValidMap.clear();
	m_oDelayCloseList.clear();
	m_oDelayReleaseList.clear();

	m_bCheckDelayRun = FALSE;
	m_bPostSendRun = FALSE;

	m_pNetImpl = NULL;
	m_pRecvLoopBufPool = NULL;
	m_pSendLoopBufPool = NULL;

	m_tLastCheckTime = 0;
}

LPSockerMgr::~LPSockerMgr()
{
	UnInit();
}

BOOL LPAPI LPSockerMgr::Init(LPNetImpl* pNetImpl)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pNetImpl);
	m_pNetImpl = pNetImpl;

	m_pRecvLoopBufPool = new LPLoopBufPool();
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	nResult = m_pRecvLoopBufPool->Init(m_pNetImpl->GetNetConfig().dwRecvBufSize, TRUE, m_pNetImpl->GetNetConfig().dwConnectCount);
	LOG_PROCESS_ERROR(nResult);

	m_pSendLoopBufPool = new LPLoopBufPool();
	LOG_PROCESS_ERROR(m_pSendLoopBufPool);
	nResult = m_pSendLoopBufPool->Init(m_pNetImpl->GetNetConfig().dwSendBufSize, TRUE, m_pNetImpl->GetNetConfig().dwConnectCount);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oCheckDelayThread.Start(_CheckDelayThreadProc, this);
	LOG_PROCESS_ERROR(nResult);
	m_bCheckDelayRun = TRUE;

	nResult = m_oPostSendThread.Start(_PostSendThreadProc, this);
	LOG_PROCESS_ERROR(nResult);
	m_bPostSendRun = TRUE;

	return TRUE;

Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL LPAPI LPSockerMgr::UnInit()
{
	if (m_bPostSendRun)
	{
		m_bPostSendRun = FALSE;
		m_oPostSendThread.Wait();
	}

	if (m_bCheckDelayRun)
	{
		m_bCheckDelayRun = FALSE;
		m_oCheckDelayThread.Wait();
	}

	SAFE_DELETE(m_pRecvLoopBufPool);
	SAFE_DELETE(m_pSendLoopBufPool);

	return TRUE;
}

ILPSockerImpl* LPAPI LPSockerMgr::Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate)
{
	LPINT32 nResult = 0;
	ILPSockerImpl* pSocker = nullptr;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	LOG_PROCESS_ERROR(m_pSendLoopBufPool);
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool->GetBufSize() == m_pNetImpl->GetNetConfig().dwRecvBufSize);
	LOG_PROCESS_ERROR(m_pSendLoopBufPool->GetBufSize() == m_pNetImpl->GetNetConfig().dwSendBufSize);

	pSocker = _Create(pPacketParser, dwParentId, bAcceptCreate);

	return pSocker;

Exit0:

	SAFE_DELETE(m_pRecvLoopBufPool);
	SAFE_DELETE(m_pSendLoopBufPool);

	return pSocker;
}

void LPAPI LPSockerMgr::Release(ILPSockerImpl* pSocker)
{
	LOG_CHECK_ERROR(pSocker);

	_Release(pSocker);
}

void LPAPI LPSockerMgr::DelayClose(ILPSockerImpl * pSocker)
{
	LOG_PROCESS_ERROR(pSocker);

	m_oDelayLock.Lock();
	pSocker->SetDelayCloseBeginTick(lpGetTickCountEx());
	if (pSocker->IsPassiveClose())
	{
		//�����رգ������ӳ�close socket
		pSocker->SetDelayCloseDuration(0);
	}
	else
	{
		//�����رգ���Ҫ��δ����������ݷ�����
		pSocker->SetDelayCloseDuration(DELAY_CLOSE_SOCKET_TIME);
	}
	m_oDelayCloseList.push_back(pSocker);
	m_oDelayLock.UnLock();

Exit0:
	return;
}

void LPAPI LPSockerMgr::DelayRelease(ILPSockerImpl* pSocker)
{
	LOG_PROCESS_ERROR(pSocker);

	m_oDelayLock.Lock();
	pSocker->SetDelayReleaseBeginTick(lpGetTickCountEx());
	if (pSocker->IsPassiveClose())
	{
		//�����رգ������ӳ�close socket
		//���迼�ǻ�������ִ�е�io�������˻ض������¼������·����ѱ��ͷŵ�socker����
		//��socker��send��recv�����¼��ӿڻص��Ѿ���io�˻��¼����˴���
		pSocker->SetDelayReleaseDuration(0);
	}
	else
	{
		//�����رգ���Ҫ�ӳ��ͷţ���Ϊsocker��close����ֻ�ر��˷���
		//������ӳ��ͷţ��������յĹ��̷��ʵ����ͷŵ�socker
		pSocker->SetDelayReleaseDuration(DELAY_RELEASE_SOCKER_TIME);
	}
	m_oDelayReleaseList.push_back(pSocker);
	m_oDelayLock.UnLock();

Exit0:
	return;
}

void LPAPI LPSockerMgr::CheckDelay()
{
	LPINT32 nResult = 0;
	LPUINT64 qwTick = 0;
	ILPSockerImpl* pSocker = NULL;
	std::shared_ptr<ILPConnectorImpl> pConnector;
	std::list<SOCKET> ltCancelHandle;
	std::list<SOCKET>::iterator ch_lit;
	std::list<LPUINT32> ltConnectSockerParentId;
	std::list<LPUINT32>::iterator csp_lit;

	qwTick = lpGetTickCountEx();

	//����ӳٵĹر�����
	m_oDelayLock.Lock();
	for (LPListSocker::iterator it = m_oDelayCloseList.begin(); it != m_oDelayCloseList.end();)
	{
		pSocker = (*it);
		if (NULL == pSocker)
		{
			LOG_CHECK_ERROR(FALSE);
			it = m_oDelayCloseList.erase(it);
			continue;
		}

		if (qwTick >= pSocker->GetDelayCloseBeginTick() + pSocker->GetDelayCloseDuration())
		{
			if (INVALID_SOCKET != pSocker->GetSock())
			{
				ltCancelHandle.push_back(pSocker->GetSock());
			}
			else
			{
				LOG_CHECK_ERROR(FALSE);
			}

			it = m_oDelayCloseList.erase(it);
			DelayRelease(pSocker);

			continue;
		}

		++it;
	}

	//����ӳ��ͷŵ�����
	for (LPListSocker::iterator it = m_oDelayReleaseList.begin(); it != m_oDelayReleaseList.end();)
	{
		pSocker = (*it);
		if (NULL == pSocker)
		{
			LOG_CHECK_ERROR(FALSE);
			it = m_oDelayReleaseList.erase(it);
			continue;
		}

		if (qwTick >= pSocker->GetDelayReleaseBeginTick() + pSocker->GetDelayReleaseDuration())
		{
			it = m_oDelayReleaseList.erase(it);

			//������������ӣ���Ҫ֪ͨ������
			//��Ҫ����ȫ�رգ����ͷ�socker֮����֪ͨ����ֹ��������ɵ�������Ϣ��ͻ
			if (!pSocker->IsAcceptCreate())
			{
				ltConnectSockerParentId.push_back(pSocker->GetParentId());
			}

			//���ﲻ���ǲ��Ƿ���������رյģ������ȵ���ע��
			m_pNetImpl->GetReactorImpl().UnRegisterEventHandler(pSocker);

			_Release(pSocker);
			continue;
		}

		++it;
	}
	m_oDelayLock.UnLock();

	//��ȫ�ر�����close socket
	if (!ltCancelHandle.empty())
	{
		for (ch_lit = ltCancelHandle.begin(); ch_lit != ltCancelHandle.end(); ++ch_lit)
		{
			lpCancelIoEx((*ch_lit));
			lpCloseSocket((*ch_lit));
		}
	}

	//֪ͨ�����������ѶϿ�
	if (!ltConnectSockerParentId.empty())
	{
		for (csp_lit = ltConnectSockerParentId.begin(); csp_lit != ltConnectSockerParentId.end(); ++csp_lit)
		{
			pConnector = m_pNetImpl->FindConnector((*csp_lit));
			LOG_CHECK_ERROR(pConnector);
			if (pConnector)
			{
				pConnector->OnClose();
			}
		}
	}

	return;
}

LPINT32 LPAPI LPSockerMgr::PostSend()
{
	LPINT32 nHaveDataCount = 0;
	ILPSockerImpl* pSocker = NULL;

	m_oLock.Lock();
	for (LPListSocker::iterator it = m_oValidList.begin(); it != m_oValidList.end(); ++it)
	{
		pSocker = (*it);
		if (NULL == pSocker)
		{
			LOG_CHECK_ERROR(FALSE);
			continue;
		}

		if (INVALID_SOCKET != pSocker->GetSock())
		{
			if (pSocker->PostSend())
			{
				++nHaveDataCount;
			}
		}
		else
		{
			if (pSocker->IsConnect())
			{
				LOG_CHECK_ERROR(FALSE);
			}
		}
	}
	m_oLock.UnLock();

	return nHaveDataCount;
}

ILPSockerImpl* LPAPI LPSockerMgr::_Create(ILPPacketParser* pPacketParser, LPUINT32 dwParentId, BOOL bAcceptCreate)
{
	LPINT32 nResult = 0;
	ILPSockerImpl* pSocker = NULL;
	LPLoopBuf* pRecvLoopBuf = NULL;
	LPLoopBuf* pSendLoopBuf = NULL;

	LOG_PROCESS_ERROR(pPacketParser);
	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	LOG_PROCESS_ERROR(m_pSendLoopBufPool);

	pSocker = ILPSockerImpl::NewSockerImpl(m_pNetImpl->GetNetConfig().dwIoType);
	LOG_PROCESS_ERROR(pSocker);

	pRecvLoopBuf = m_pRecvLoopBufPool->Create();
	pSendLoopBuf = m_pSendLoopBufPool->Create();
	LOG_PROCESS_ERROR(pRecvLoopBuf);
	LOG_PROCESS_ERROR(pSendLoopBuf);

	pSocker->AttachRecvBuf(pRecvLoopBuf);
	pSocker->AttachSendBuf(pSendLoopBuf);

	pSocker->SetSockerId(_CreateSockId());
	pSocker->AttachPacketParser(pPacketParser);

	pSocker->SetParentId(dwParentId);
	pSocker->SetAcceptCreate(bAcceptCreate);

	pSocker->SetNetImpl(m_pNetImpl);

	m_oLock.Lock();
	m_oValidList.push_back(pSocker);
	m_oValidMap.insert(make_pair(pSocker->GetSockerId(), pSocker));
	m_oLock.UnLock();

	return pSocker;

Exit0:

	ILPSockerImpl::DeleteSockerImpl(pSocker);
	if (pRecvLoopBuf) m_pRecvLoopBufPool->Release(pRecvLoopBuf);
	if (pSendLoopBuf) m_pSendLoopBufPool->Release(pSendLoopBuf);

	return pSocker;
}

void LPAPI LPSockerMgr::_Release(ILPSockerImpl* pSocker)
{
	LPINT32 nResult = 0;
	LPLoopBuf* pRecvLoopBuf = NULL;
	LPLoopBuf* pSendLoopBuf = NULL;

	LOG_PROCESS_ERROR(pSocker);

	pRecvLoopBuf = pSocker->DetachRecvBuf();
	pSendLoopBuf = pSocker->DetachSendBuf();
	LOG_CHECK_ERROR(pRecvLoopBuf);
	LOG_CHECK_ERROR(pSendLoopBuf);

	pSocker->SetSock(INVALID_SOCKET);
	pSocker->SetSockerId(INVALID_SOCKER_ID);
	pSocker->DetachPacketParser();
	pSocker->SetNetImpl(NULL);

	m_oLock.Lock();
	m_oValidList.remove(pSocker);
	m_oValidMap.erase(pSocker->GetSockerId());
	m_oLock.UnLock();

	LOG_PROCESS_ERROR(m_pRecvLoopBufPool);
	m_pRecvLoopBufPool->Release(pRecvLoopBuf);

	LOG_PROCESS_ERROR(m_pSendLoopBufPool);
	m_pSendLoopBufPool->Release(pSendLoopBuf);

	ILPSockerImpl::DeleteSockerImpl(pSocker);

Exit0:
	return;
}

LPUINT32 LPAPI LPSockerMgr::_CreateSockId()
{
	return ++m_dwMaxSockId;
}

THREAD_FUNC_DECLARE(LPSockerMgr::_CheckDelayThreadProc)(void * pParam)
{
	LPINT32 nResult = 0;
	LPSockerMgr* pSockerMgr = NULL;

	LOG_PROCESS_ERROR(pParam);
	pSockerMgr = (LPSockerMgr*)pParam;
	LOG_PROCESS_ERROR(pSockerMgr);

	__TRY__
	{
		while (pSockerMgr->m_bCheckDelayRun)
		{
			pSockerMgr->CheckDelay();
			lpSleep(5);
		}
	}
	__EXCEPT__
	{

	}

	pSockerMgr->CheckDelay();


Exit0:

	return 0;
}
THREAD_FUNC_DECLARE(LPSockerMgr::_PostSendThreadProc)(void * pParam)
{
	LPINT32 nResult = 0;
	LPSockerMgr* pSockerMgr = NULL;

	LOG_PROCESS_ERROR(pParam);
	pSockerMgr = (LPSockerMgr*)pParam;
	LOG_PROCESS_ERROR(pSockerMgr);

	while (pSockerMgr->m_bPostSendRun)
	{
		if (pSockerMgr->PostSend() == 0)
		{
			lpSleep(1);
		}
	}

	pSockerMgr->PostSend();

Exit0:

	return 0;
}
ILPSockerImpl* LPSockerMgr::Find(LPUINT32 dwSockerId)
{
	ILPSockerImpl* pSocker = NULL;
	LPMapSocker::iterator fit;

	m_oLock.Lock();
	fit = m_oValidMap.find(dwSockerId);
	if (fit != m_oValidMap.end())
	{
		pSocker = fit->second;
	}
	m_oLock.UnLock();

	return pSocker;
}

LPUINT32 LPAPI LPSockerMgr::GetCurValidConnectCount()
{
	LPUINT32 dwValidCount = 0;

	m_oLock.Lock();
	dwValidCount = (LPUINT32)m_oValidMap.size();
	m_oLock.UnLock();

	return dwValidCount;
}



//end�������������ֿռ�
NS_LZPL_END