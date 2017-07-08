#include "lp_reactor.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lpi_net.h"
#include "lp_global.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



#define IOCP_THREAD_PER_CPU     (2)



std::shared_ptr<ILPReactor> LPAPI ILPReactor::NewReactor(NET_CONFIG& stNetConfig)
{
    LPINT32 nResult = 0;
    std::shared_ptr<ILPReactor> pReactor;

    switch(stNetConfig.dwIoType)
    {
    case eIoType_CompletionPort:
        {
            pReactor = std::make_shared<LPIocpReactor>();
            LOG_PROCESS_ERROR(pReactor != nullptr);

            nResult = ((LPIocpReactor*)pReactor.get())->Init(stNetConfig);
            LOG_PROCESS_ERROR(nResult);
        }
        break;
    case eIoType_Epoll:
        {
            pReactor = std::make_shared<LPEpollReactor>();
            LOG_PROCESS_ERROR(pReactor != nullptr);

            nResult = ((LPEpollReactor*)pReactor.get())->Init(stNetConfig);
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
    SetState(eCommonState_NoInit);
}

LPReactor::~LPReactor()
{

}

BOOL LPAPI LPReactor::Init(NET_CONFIG& stNetConfig)
{
    m_stNetConfig = stNetConfig;
    return TRUE;
}

BOOL LPAPI LPReactor::UnInit()
{
    return FALSE;
}

THREAD_FUNC_DECLARE(LPReactor::ThreadFunc)(void * pParam)
{
    REACTOR_THREAD_PARAM stThreadParam;
    REACTOR_THREAD_PARAM* pThreadParam = (REACTOR_THREAD_PARAM*)pParam;

    LOG_PROCESS_ERROR(pThreadParam);
    stThreadParam = *pThreadParam;
    SAFE_DELETE(pThreadParam);

    LOG_PROCESS_ERROR(stThreadParam.pReactorImpl);
    ((LPReactor*)stThreadParam.pReactorImpl)->OnExecute(stThreadParam);

Exit0:
    return 0;
}

LPUINT32 LPAPI LPReactor::GetState()
{
    return m_dwState;
}

void LPAPI LPReactor::SetState(LPUINT32 dwState)
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

BOOL LPAPI LPIocpReactor::Init(NET_CONFIG& stNetConfig)
{
#   ifdef _WIN32
    SYSTEM_INFO stSysInfo;
    LPUINT32 dwThreadId = 0;
#   endif

    LPINT32 nResult = FALSE;

    //TRUE-��1�������ɶ˿ڣ�����ÿ����ɶ˿�ֻ��Ӧ1���̣߳������������ȷ��ÿ��socket���̴߳���
    //FALSE-ֻ��1����ɶ˿ڣ����Ҷ�Ӧ1������̣߳����������������Ƿ���ȷ��ÿ��socket���̴߳���
    BOOL bOneCompletionPortOneThread = TRUE;

    LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
    SetState(eCommonState_Initing);

    nResult = LPReactor::Init(stNetConfig);
    LOG_PROCESS_ERROR(nResult);

#   if defined _WIN32
    {
        GetSystemInfo(&stSysInfo);

        if(bOneCompletionPortOneThread)
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
        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            m_pCompletionPort[i] = INVALID_HANDLE_VALUE;
        }
        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            m_pCompletionPort[i] = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
            LOG_PROCESS_ERROR(m_pCompletionPort[i] != INVALID_HANDLE_VALUE);
        }

        m_ppWorkerArray = new HANDLE*[m_nCompletionPortCount];
        LOG_PROCESS_ERROR(m_ppWorkerArray);
        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            m_ppWorkerArray[i] = NULL;
        }
        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            m_ppWorkerArray[i] = new HANDLE[m_nWorkerCountPerCompIo];
            LOG_PROCESS_ERROR(m_ppWorkerArray[i]);
            for(LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
            {
                m_ppWorkerArray[i][j] = INVALID_HANDLE_VALUE;
            }
        }

        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            for(LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
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

    SetState(eCommonState_Inited);

    return TRUE;
Exit0:

    UnInit();

    return FALSE;
}

BOOL LPAPI LPIocpReactor::UnInit()
{
    LPINT32 nResult = 0;

    PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

    SetState(eCommonState_UnIniting);
    IMP("reactor uniniting ...");
    LPPRINTF("reactor uniniting ...\n");

#   if defined _WIN32
    {
        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            for(LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
            {
                nResult = PostQueuedCompletionStatus(m_pCompletionPort[i], 0, NULL, NULL);
                LOG_CHECK_ERROR(nResult);
            }
        }

        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            nResult = WaitForMultipleObjects(m_nWorkerCountPerCompIo, m_ppWorkerArray[i], TRUE, 60 * 1000);
            LOG_CHECK_ERROR(nResult != WAIT_FAILED);
            LOG_CHECK_ERROR(nResult != WAIT_TIMEOUT);

            for(LPINT32 j = 0; j < m_nWorkerCountPerCompIo; ++j)
            {
                lpCloseHandle(m_ppWorkerArray[i][j]);
                m_ppWorkerArray[i][j] = INVALID_HANDLE_VALUE;
            }
        }

        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            SAFE_DELETE_SZ(m_ppWorkerArray[i]);
        }
        SAFE_DELETE_SZ(m_ppWorkerArray);


        for(LPINT32 i = 0; i < m_nCompletionPortCount; ++i)
        {
            lpCloseHandle(m_pCompletionPort[i]);
            m_pCompletionPort[i] = INVALID_HANDLE_VALUE;
        }
        SAFE_DELETE_SZ(m_pCompletionPort);
    }
#   endif

    IMP("reactor uninit success !");
    LPPRINTF("reactor uninit success !\n");
    SetState(eCommonState_UnInited);

Exit1:
    return TRUE;
}

BOOL LPAPI LPIocpReactor::RegisterEventHandler(ILPEventHandler* pEventHandler)
{
    LOG_PROCESS_ERROR(pEventHandler);
    LOG_PROCESS_ERROR(m_pCompletionPort);
    LOG_PROCESS_ERROR(m_nCompletionPortCount > 0);

#   if defined _WIN32
    {
        if(NULL == CreateIoCompletionPort(pEventHandler->GetHandle(), m_pCompletionPort[(LPUINT64)(pEventHandler->GetHandle()) % m_nCompletionPortCount], (ULONG_PTR)pEventHandler, 0))
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

void LPAPI LPIocpReactor::OnExecute(REACTOR_THREAD_PARAM& tThreadParam)
{
    BOOL             bRet = FALSE;
    DWORD            dwByteTransferred;
    ILPEventHandler* pEventHandler;
    PER_IO_DATA*     pstPerIoData;

    LOG_PROCESS_ERROR(tThreadParam.nCompletionPortIndex >= 0 && tThreadParam.nCompletionPortIndex < m_nCompletionPortCount);

    while(TRUE)
    {
        pEventHandler = NULL;
        pstPerIoData = NULL;
        dwByteTransferred = 0;

#       if defined _WIN32
        {
            //IMP("LPReactor::OnExecute start GetQueuedCompletionStatus ...");
            bRet = GetQueuedCompletionStatus(
                       m_pCompletionPort[tThreadParam.nCompletionPortIndex],
                       &dwByteTransferred,
                       (PDWORD_PTR)&pEventHandler,
                       (LPOVERLAPPED*)&pstPerIoData,
                       INFINITE);
        }
#       endif

        //����߳��˳�
        if(GetState() == eCommonState_Inited)
        {
            LOG_PROCESS_ERROR(pEventHandler);
        }
        else if(GetState() >= eCommonState_Close)
        {
            PROCESS_SUCCESS(TRUE);
        }
        else
        {
            LOG_PROCESS_ERROR(FALSE);
        }

        //io�����ݼ��
        if(NULL == pstPerIoData)
        {
            LOG_CHECK_ERROR(pstPerIoData);
            continue;
        }

        if(bRet)
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
        pstPerIoData->bOperateRet = bRet;
        pEventHandler->OnNetEvent(pstPerIoData);
    }

Exit1:
Exit0:
    return;
}



LPEpollReactor::LPEpollReactor()
{
    m_hEpoll = INVALID_HANDLE_VALUE;
}



LPEpollReactor::~LPEpollReactor()
{

}

BOOL LPAPI LPEpollReactor::Init(NET_CONFIG& stNetConfig)
{
    LPINT32 nResult = FALSE;

    LOG_PROCESS_ERROR(eCommonState_NoInit == GetState());
    SetState(eCommonState_Initing);

    nResult = LPReactor::Init(stNetConfig);
    LOG_PROCESS_ERROR(nResult);

#   ifndef _WIN32
    {
        m_hEpoll = epoll_create(m_stNetConfig.dwConnectCount);
        LOG_PROCESS_ERROR(m_hEpoll != INVALID_HANDLE_VALUE);

        REACTOR_THREAD_PARAM* pThreadParam = new REACTOR_THREAD_PARAM();
        LOG_PROCESS_ERROR(pThreadParam);
        pThreadParam->pReactorImpl = this;
        pThreadParam->nCompletionPortIndex = 0;

        nResult = m_oThread.Start(ThreadFunc, pThreadParam);
        LOG_PROCESS_ERROR(nResult);
        m_bRun = TRUE;
    }
#   endif

    SetState(eCommonState_Inited);

    return TRUE;
Exit0:

    UnInit();
    return FALSE;
}

BOOL LPAPI LPEpollReactor::UnInit()
{
    PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

    SetState(eCommonState_UnIniting);

    if(m_bRun == TRUE)
    {
        m_bRun = FALSE;
        m_oThread.Wait();
    }

    lpCloseHandle(m_hEpoll);

    SetState(eCommonState_UnInited);

Exit1:
    return TRUE;
}

BOOL LPAPI LPEpollReactor::RegisterEventHandler(ILPEventHandler* pEventHandler)
{
    LPINT32 nResult = 0;

    struct epoll_event ev;

    LOG_PROCESS_ERROR(pEventHandler != nullptr);

    ev.data.ptr = pEventHandler;

    switch(pEventHandler->GetEventHandlerType())
    {
    case eEventHandlerType_Connector:
        {
            ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
        }
        break;
    case eEventHandlerType_Listener:
        {
            ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
        }
        break;
    case eEventHandlerType_Socker:
        {
            ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
        }
        break;
    case eEventHandlerType_None:
    case eEventHandlerType_Max:
    default:
        LOG_CHECK_ERROR(FALSE);
        LPASSERT(FALSE);
        LOG_PROCESS_ERROR(FALSE);
        break;
    }

#   ifndef _WIN32
    {
        nResult = epoll_ctl(m_hEpoll, EPOLL_CTL_ADD, pEventHandler->GetHandle(), &ev);
        LOG_PROCESS_ERROR(nResult == 0);
    }
#   endif

    return TRUE;
Exit0:
    return FALSE;
}

BOOL LPAPI LPEpollReactor::UnRegisterEventHandler(ILPEventHandler* pEventHandler)
{
    LPINT32 nResult = 0;

    struct epoll_event ev = { 0, {0} };

    LOG_PROCESS_ERROR(pEventHandler != nullptr);

#   ifndef _WIN32
    {
        //ע�⣺����������رյ�socketӦ����ע�����ٹرգ���ֹ����EPOLLIN�¼�
        nResult = epoll_ctl(m_hEpoll, EPOLL_CTL_DEL, pEventHandler->GetHandle(), &ev);
        LOG_PROCESS_ERROR(nResult == 0);
    }
#   endif

    return TRUE;
Exit0:
    return FALSE;
}

void LPAPI LPEpollReactor::OnExecute(REACTOR_THREAD_PARAM& tThreadParam)
{
    LPINT32 nFdCount = 0;
    HANDLE hFd = INVALID_HANDLE_VALUE;
    LPINT32 nMaxEvents = m_stNetConfig.dwConnectCount;
    LPINT32 nTimeout = -1;
    PER_IO_DATA* pstPerIoData = nullptr;
    ILPEventHandler* pEventHandler = nullptr;

    epoll_event* ptEvents = new epoll_event[nMaxEvents];

    LOG_PROCESS_ERROR(ptEvents != nullptr);
    memset(ptEvents, 0x00, sizeof(epoll_event) * nMaxEvents);

    while(m_bRun)
    {
#       ifndef _WIN32
        {
            nFdCount = epoll_wait(m_hEpoll, ptEvents, nMaxEvents, nTimeout);
        }
#       endif

        if(nFdCount == -1)
        {
            //���źŴ��
            if(WSAGetLastError() == EINTR || WSAGetLastError() == EAGAIN)
            {
                continue;
            }

            LOG_PROCESS_ERROR_WITH_MSG(FALSE, "errno=%d", errno);
        }

        //����߳��˳�
        if(GetState() == eCommonState_Inited)
        {

        }
        else if(GetState() >= eCommonState_Close)
        {
            PROCESS_SUCCESS(TRUE);
        }
        else
        {
            LOG_PROCESS_ERROR(FALSE);
        }

        //EPOLLIN�� ��ʾ��Ӧ���ļ����������Զ��������Զ�SOCKET�����رգ���
        //EPOLLOUT�� ��ʾ��Ӧ���ļ�����������д�����������б仯ʱ����write������sock������ʱ�����仯�������������������ᴥ���¼���
        //EPOLLPRI�� ��ʾ��Ӧ���ļ��������н��������ݿɶ�������Ӧ�ñ�ʾ�д������ݵ�������
        //EPOLLERR�� ��ʾ��Ӧ���ļ���������������д�ѹر�socket pipe broken����ע���¼���ʱ������¼���Ĭ����ӡ�
        //EPOLLHUP�� ��ʾ��Ӧ���ļ����������Ҷϣ�Ʃ���յ�RST������ע���¼���ʱ������¼���Ĭ����ӡ�
        //EPOLLRDHUP�� ��ʾ��Ӧ���ļ��������Զ�socket�ر��¼�����ҪӦ����ETģʽ�¡�
        //  ��ˮƽ����ģʽ�£�����Զ�socket�رգ����һֱ����epollin�¼�������ȥ����client socket��
        //  �ڱ��ش���ģʽ�£����client���ȷ���Э��Ȼ��shutdownд�ˡ���ᴥ��epollin�¼�����������������ֻ����һ��recv����ʱ������recv��ȡ�������ݳ������ж������
        //  ������Ҫ�����Э��ʱ������ʧ�ͻ��˹ر��¼���
        //EPOLLET�� ��EPOLL��Ϊ��Ե����(Edge Triggered)ģʽ�����������ˮƽ����(Level Triggered)��˵�ġ�
        //EPOLLONESHOT��ֻ����һ���¼���������������¼�֮���������Ҫ�����������socket�Ļ�����Ҫ�ٴΰ����socket���뵽EPOLL������

        //ע�⣺����io�¼�ʱ�������¼����͵��ж�˳��Ӧ�����жϴ������ͣ����жϿɶ����д

        for(LPINT32 i = 0; i < nFdCount; ++i)
        {
            pEventHandler = (ILPEventHandler*)ptEvents[i].data.ptr;
            if(pEventHandler != nullptr)
            {
                pstPerIoData = pEventHandler->GetEventHandlerData();
                if(pstPerIoData != nullptr)
                {
                    pstPerIoData->ptEpollEvent = &ptEvents[i];
                    pEventHandler->OnNetEvent(pstPerIoData);
                }
                else
                {
                    LOG_CHECK_ERROR(FALSE);
                }
            }
            else
            {
                LOG_CHECK_ERROR(FALSE);
            }
        }
    }

Exit1:
Exit0:

    SAFE_DELETE_SZ(ptEvents);

    return;
}



//end�������������ֿռ�
NS_LZPL_END