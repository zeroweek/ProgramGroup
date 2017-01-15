#include "lp_reactor.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lpi_net.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define IOCP_THREAD_PER_CPU     (2)



std::shared_ptr<ILPReactor> LPAPI ILPReactor::NewReactor(LPUINT32 dwIoType)
{
	LPINT32 nResult = 0;
	std::shared_ptr<ILPReactor> pReactor;

	switch (dwIoType)
	{
	case eIoType_CompletionPort:
		{
			pReactor = std::make_shared<LPIocpReactor>();
			LOG_PROCESS_ERROR(pReactor != nullptr);

			nResult = ((LPIocpReactor*)pReactor.get())->Init(FALSE);
			LOG_PROCESS_ERROR(nResult);
		}
		break;
	case eIoType_None:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		LOG_PROCESS_ERROR(FALSE);
		break;
	}

	return pReactor;

Exit0:

	pReactor = nullptr;
	return FALSE;
}

void LPAPI ILPReactor::DeleteReactor(std::shared_ptr<ILPReactor>& pReactor)
{
	pReactor = nullptr;
}



LPReactor::LPReactor()
{
	_SetState(eCommonState_NoInit);
}

LPReactor::~LPReactor()
{
	
}

BOOL LPAPI LPReactor::Init(BOOL bOneCompletionPortOneThread)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPReactor::UnInit()
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPReactor::RegisterEventHandler(ILPEventHandler* pEventHandler)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPReactor::UnRegisterEventHandler(ILPEventHandler* pEventHandler)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

unsigned LPAPI LPReactor::ThreadFunc(void* pParam)
{
	REACTOR_THREAD_PARAM stThreadParam;
	REACTOR_THREAD_PARAM* pThreadParam = (REACTOR_THREAD_PARAM*)pParam;

	LOG_PROCESS_ERROR(pThreadParam);
	stThreadParam.pReactorImpl = pThreadParam->pReactorImpl;
	stThreadParam.nCompletionPortIndex = pThreadParam->nCompletionPortIndex;

	SAFE_DELETE(pThreadParam);

	LOG_PROCESS_ERROR(stThreadParam.pReactorImpl);
	((LPReactor*)stThreadParam.pReactorImpl)->OnExecute(stThreadParam.nCompletionPortIndex);

Exit0:
	return 0;
}

void LPAPI LPReactor::OnExecute(LPINT32 nCompletionPortIndex)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return ;
}

LPUINT32 LPAPI LPReactor::_GetState()
{
	return m_dwState;
}

void LPAPI LPReactor::_SetState(LPUINT32 dwState)
{
	m_dwState = dwState;
}



LPIocpReactor::LPIocpReactor()
{

}

LPIocpReactor::~LPIocpReactor()
{
	UnInit();
}

BOOL LPAPI LPIocpReactor::RegisterEventHandler(ILPEventHandler* pEventHandler)
{
	LOG_PROCESS_ERROR(pEventHandler);
	LOG_PROCESS_ERROR(m_pCompletionPort);
	LOG_PROCESS_ERROR(m_nCompletionPortCount > 0);

#   if defined _WIN32
	{
		if (NULL == CreateIoCompletionPort(pEventHandler->GetHandle(), m_pCompletionPort[(LPUINT64)(pEventHandler->GetHandle()) % m_nCompletionPortCount], (ULONG_PTR)pEventHandler, 0))
		{
			FTL("function %s in file %s at line %d : errno %d", __FUNCTION__, __FILE__, __LINE__, WSAGetLastError());
			PROCESS_ERROR(FALSE);
		}
	}
#   endif

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPIocpReactor::UnRegisterEventHandler(ILPEventHandler* pEventHandler)
{
	return FALSE;

}

BOOL LPAPI LPIocpReactor::Init(BOOL bOneCompletionPortOneThread)
{
#	ifdef _WIN32
	SYSTEM_INFO stSysInfo;
	LPUINT32 dwThreadId = 0;
#	endif

	LOG_PROCESS_ERROR(eCommonState_NoInit == _GetState());
	_SetState(eCommonState_Initing);

#   if defined _WIN32
	{
		GetSystemInfo(&stSysInfo);

		if (bOneCompletionPortOneThread)
		{
			m_nCompletionPortCount = stSysInfo.dwNumberOfProcessors * IOCP_THREAD_PER_CPU;
			m_nWorkerCountPerCompIo = 1;
		}
		else
		{
			m_nCompletionPortCount = 1;
			m_nWorkerCountPerCompIo = stSysInfo.dwNumberOfProcessors * IOCP_THREAD_PER_CPU;
		}

		m_pCompletionPort = new HANDLE[m_nCompletionPortCount];
		LOG_PROCESS_ERROR(m_pCompletionPort);
		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			m_pCompletionPort[i] = INVALID_HANDLE_VALUE;
		}
		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			m_pCompletionPort[i] = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			LOG_PROCESS_ERROR(m_pCompletionPort[i] != INVALID_HANDLE_VALUE);
		}

		m_ppWorkerArray = new HANDLE*[m_nCompletionPortCount];
		LOG_PROCESS_ERROR(m_ppWorkerArray);
		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			m_ppWorkerArray[i] = NULL;
		}
		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			m_ppWorkerArray[i] = new HANDLE[m_nWorkerCountPerCompIo];
			LOG_PROCESS_ERROR(m_ppWorkerArray[i]);
			for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
			{
				m_ppWorkerArray[i][j] = INVALID_HANDLE_VALUE;
			}
		}

		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
			{
				REACTOR_THREAD_PARAM* pThreadParam = new REACTOR_THREAD_PARAM();
				LOG_PROCESS_ERROR(pThreadParam);
				pThreadParam->pReactorImpl = this;
				pThreadParam->nCompletionPortIndex = i;

				m_ppWorkerArray[i][j] = (HANDLE)_beginthreadex(NULL,          // Security
					0,             // Stack size - use default
					ThreadFunc,    // Thread fn entry point
					(void*)pThreadParam,   // Param for thread
					0,             // Init flag
					&dwThreadId);  // Thread address

				LOG_PROCESS_ERROR(m_ppWorkerArray[i][j] != INVALID_HANDLE_VALUE);
			}
		}
	}
#   endif

	_SetState(eCommonState_Inited);

	return TRUE;
Exit0:

	UnInit();

	return FALSE;
}

BOOL LPAPI LPIocpReactor::UnInit()
{
	LPINT32 nResult = 0;

	PROCESS_SUCCESS(_GetState() == eCommonState_NoInit || _GetState() >= eCommonState_UnIniting);

	_SetState(eCommonState_UnIniting);
	IMP("reactor uniniting ...");
	LPPRINTF("reactor uniniting ...\n");

#   if defined _WIN32
	{
		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
			{
				nResult = PostQueuedCompletionStatus(m_pCompletionPort[i], 0, NULL, NULL);
				LOG_CHECK_ERROR(nResult);
			}
		}

		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			nResult = WaitForMultipleObjects(m_nWorkerCountPerCompIo, m_ppWorkerArray[i], TRUE, 60 * 1000);
			LOG_CHECK_ERROR(nResult != WAIT_FAILED);
			LOG_CHECK_ERROR(nResult != WAIT_TIMEOUT);

			for (LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
			{
				lpCloseHandle(m_ppWorkerArray[i][j]);
				m_ppWorkerArray[i][j] = INVALID_HANDLE_VALUE;
			}
		}

		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			SAFE_DELETE_SZ(m_ppWorkerArray[i]);
		}
		SAFE_DELETE_SZ(m_ppWorkerArray);


		for (LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
		{
			lpCloseHandle(m_pCompletionPort[i]);
			m_pCompletionPort[i] = INVALID_HANDLE_VALUE;
		}
		SAFE_DELETE_SZ(m_pCompletionPort);
	}
#   endif

	IMP("reactor uninit success !");
	LPPRINTF("reactor uninit success !\n");
	_SetState(eCommonState_UnInited);

Exit1:
	return TRUE;
}

void LPAPI LPIocpReactor::OnExecute(LPINT32 nCompletionPortIndex)
{
	BOOL             bRet = FALSE;
	DWORD            dwByteTransferred;
	ILPEventHandler* pEventHandler;
	PER_IO_DATA*     pstPerIoData;

	LOG_PROCESS_ERROR(nCompletionPortIndex >= 0 && nCompletionPortIndex < m_nCompletionPortCount);

	while (TRUE)
	{
		pEventHandler = NULL;
		pstPerIoData = NULL;
		dwByteTransferred = 0;

#       if defined _WIN32
		{
			//IMP("LPReactor::OnExecute start GetQueuedCompletionStatus ...");
			bRet = GetQueuedCompletionStatus(
				m_pCompletionPort[nCompletionPortIndex],
				&dwByteTransferred,
				(PDWORD_PTR)&pEventHandler,
				(LPOVERLAPPED*)&pstPerIoData,
				INFINITE);
		}
#       endif

		//检查线程退出
		if (_GetState() == eCommonState_Inited)
		{
			LOG_PROCESS_ERROR(pEventHandler);
		}
		else if (_GetState() >= eCommonState_Close)
		{
			PROCESS_SUCCESS(TRUE);
		}
		else
		{
			LOG_PROCESS_ERROR(FALSE);
		}

		//io绑定数据检测
		if (NULL == pstPerIoData)
		{
			LOG_CHECK_ERROR(pstPerIoData);
			continue;
		}

		if (bRet)
		{
			DBG("LPIocpCtrl::OnExecute bRet=TRUE, e_EventHandlerType=%s, e_IoOptType=%s",
				ILPEventHandler::GetEventHandlerTypeName(pstPerIoData->eHandlerType), ILPReactor::GetIoOptTypeName(pstPerIoData->eIoOptType));
		}
		else
		{
			DBG("LPIocpCtrl::OnExecute bRet=FALSE, e_EventHandlerType=%s, e_IoOptType=%s",
				ILPEventHandler::GetEventHandlerTypeName(pstPerIoData->eHandlerType), ILPReactor::GetIoOptTypeName(pstPerIoData->eIoOptType));
		}

		pstPerIoData->qwByteTransferred = dwByteTransferred;
		pEventHandler->OnNetEvent(bRet, pstPerIoData);
	}

Exit1:
Exit0:
	return;
}



//end声明所处的名字空间
NS_LZPL_END