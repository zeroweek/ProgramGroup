#include "lp_reactor.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lpi_net.h"
#include "lp_global.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define IOCP_THREAD_PER_CPU     (1)



lp_shared_ptr<ILPReactor> LPAPI ILPReactor::NewReactor(NET_CONFIG& stNetConfig)
{
    LPINT32 nResult = 0;
    lp_shared_ptr<ILPReactor> pReactor;

    pReactor = std::make_shared<LPReactor>();
    LOG_PROCESS_ERROR(pReactor != nullptr);

    nResult = ((LPReactor*)pReactor.get())->Init(stNetConfig);
    LOG_PROCESS_ERROR(nResult);

    return pReactor;

Exit0:

    DeleteReactor(pReactor);
    return FALSE;
}

void LPAPI ILPReactor::DeleteReactor(lp_shared_ptr<ILPReactor>& pReactor)
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
    m_nIoServiceNum = std::thread::hardware_concurrency() * IOCP_THREAD_PER_CPU;

    LOG_PROCESS_ERROR(GetState() == eCommonState_NoInit);
    SetState(eCommonState_Initing);

    for(int i = 0; i < m_nIoServiceNum; i++)
    {
        m_vecIoService.push_back(lp_make_shared<asio::io_service>());
    }

    for(int i = 0; i < m_nIoServiceNum; i++)
    {
        m_vecThread.push_back(lp_make_shared<std::thread>(&LPReactor::ThreadFunc, this, i));
    }

    SetState(eCommonState_Inited);

    return TRUE;
Exit0:
    return FALSE;
}

BOOL LPAPI LPReactor::UnInit()
{
    LPINT32 nResult = 0;

    PROCESS_SUCCESS(GetState() == eCommonState_NoInit || GetState() >= eCommonState_UnIniting);

    SetState(eCommonState_UnIniting);
    IMP("reactor uniniting ...");
    LPPRINTF("reactor uniniting ...\n");

    for(int i = 0; i < m_nIoServiceNum; i++)
    {
        m_vecThread[i]->join();
    }
    m_vecThread.clear();

    for(int i = 0; i < m_nIoServiceNum; i++)
    {
        m_vecIoService[i]->stop();
    }
    m_vecIoService.clear();

    IMP("reactor uninit success !");
    LPPRINTF("reactor uninit success !\n");
    SetState(eCommonState_UnInited);

Exit1:
    return TRUE;
}

boost::asio::io_service& LPReactor::GetIoService(LPUINT32 dwId)
{
    LPUINT32 nIndex = dwId % m_nIoServiceNum;
    return *m_vecIoService[nIndex];
}

void LPAPI LPReactor::OnExecute(LPUINT32 nIndex)
{
    while(true)
    {
        //检查线程退出
        if(GetState() == eCommonState_Inited)
        {
            LOG_PROCESS_ERROR(nIndex < m_nIoServiceNum);
            m_vecIoService[nIndex]->run();
            boost::this_thread::sleep(boost::posix_time::millisec(1));
        }
        else if(GetState() == eCommonState_Initing)
        {
            boost::this_thread::sleep(boost::posix_time::millisec(1));
        }
        else if(GetState() >= eCommonState_Close)
        {
            PROCESS_SUCCESS(TRUE);
        }
        else
        {
            LOG_PROCESS_ERROR(FALSE);
        }
    }

Exit1:
Exit0:
    return;
}

void LPReactor::ThreadFunc(LPUINT32 nIndex)
{
    OnExecute(nIndex);
}

LPUINT32 LPAPI LPReactor::GetState()
{
    return m_dwState;
}

void LPAPI LPReactor::SetState(LPUINT32 dwState)
{
    m_dwState = dwState;
}



//end声明所处的名字空间
NS_LZPL_END