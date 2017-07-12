#include "lp_socker.h"
#include "lp_processerror.h"
#include "lp_system.h"
#include "lp_string.h"
#include "lp_dump.h"
#include "lp_net.h"
#include "lp_global.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



const char *LPAPI SOCK_ERR_CODE::GetDescription()
{
    switch(eSockErrCode)
    {
    case LZPL::eSockErrCode_Fail:
        return lpGetErrorString(eErrorString_Fail);
    case LZPL::eSockErrCode_Success:
        return lpGetErrorString(eErrorString_Success);
    case LZPL::eSockErrCode_AcceptFail:
    case LZPL::eSockErrCode_ConnectFail:
    case LZPL::eSockErrCode_SendBufOverflow:
    case LZPL::eSockErrCode_PostRecvFail:
    case LZPL::eSockErrCode_PostSendFail:
    case LZPL::eSockErrCode_ReactorErrorEvent:
    case LZPL::eSockErrCode_RecvError:
    case LZPL::eSockErrCode_Total:
        return lpGetErrorString(eSockErrCode - eSockErrCode_AcceptFail + eErrorString_Socket_AcceptFail);
    default:
        LOG_PROCESS_ERROR(FALSE);
        break;
    }

Exit0:
    return lpGetErrorString(eErrorString_Unknow);
}

lp_shared_ptr<ILPSockerImpl> LPAPI ILPSockerImpl::NewSockerImpl(LPUINT32 dwIoType)
{
    switch(dwIoType)
    {
    case eIoType_CompletionPort:
    case eIoType_Epoll:
        {
            return lp_make_shared<LPSocker>();
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

void LPAPI ILPSockerImpl::DeleteSockerImpl(lp_shared_ptr<ILPSockerImpl> & pSocker)
{
    pSocker = nullptr;
}



LPSocker::LPSocker()
{
    Reset();
}

LPSocker::~LPSocker()
{

}

void LPAPI LPSocker::Reset()
{
    SetConnect(false);
    m_bSending = false;

    SetAcceptCreate(FALSE);
    SetSock(INVALID_SOCKET);
    SetSocket(nullptr);
    SetSockerId(INVALID_SOCKER_ID);

    m_dwRemoteIp = 0;
    m_wRemotePort = 0;
    m_dwLocalIp = 0;
    m_wLocalPort = 0;

    SetParentId(0);

    m_pPacketParser = nullptr;
    SetNetImpl(nullptr);

    m_pRecvLoopBuf = nullptr;
    m_pSendLoopBuf = nullptr;

    m_qwDelayCloseBeginTick = 0;
    m_qwDelayCloseDuration = 0;
    m_qwDelayReleaseBeginTick = 0;
    m_qwDelayReleaseDuration = 0;

    m_stRecvOrSendPerIoData.eIoOptType = eIoOptType_RecvOrSend;
    m_stRecvOrSendPerIoData.eHandlerType = eEventHandlerType_Socker;
}

void LPAPI LPSocker::SetSock(SOCKET sock)
{
    m_hSock = sock;
}

void LPAPI LPSocker::SetSocket(lp_shared_ptr<ip::tcp::socket> pSocket)
{
    m_pSocket = pSocket;
}

SOCKET LPAPI LPSocker::GetSock()
{
    return m_hSock;
}

void LPAPI LPSocker::SetSockerId(LPUINT32 dwSockerId)
{
    m_dwSockerId = dwSockerId;
}

LPUINT32 LPAPI LPSocker::GetSockerId()
{
    return m_dwSockerId;
}

void LPAPI LPSocker::SetRemoteIp(LPUINT32 dwIp)
{
    char szRemoteIpStr[IP_LEN];

    m_dwRemoteIp = dwIp;
    lpFastZeroCharArray(szRemoteIpStr);
    lpStrCpyN(szRemoteIpStr, inet_ntoa((in_addr&)m_dwRemoteIp), IP_LEN);
    m_strRemoteIp.append(szRemoteIpStr);
}

LPUINT32 LPAPI LPSocker::GetRemoteIp()
{
    return m_dwRemoteIp;
}

std::string& LPAPI LPSocker::GetRemoteIpStr()
{
    return m_strRemoteIp;
}

void LPAPI LPSocker::SetRemotePort(LPUINT16 wPort)
{
    m_wRemotePort = wPort;
}

LPUINT16 LPAPI LPSocker::GetRemotePort()
{
    return m_wRemotePort;
}

void LPAPI LPSocker::SetLocalIp(LPUINT32 dwIp)
{
    char szLocalIpStr[IP_LEN];

    m_dwLocalIp = dwIp;
    lpFastZeroCharArray(szLocalIpStr);
    memcpy(szLocalIpStr, inet_ntoa((in_addr&)m_dwLocalIp), IP_LEN);
    m_strLocalIp.append(szLocalIpStr);
}

LPUINT32 LPAPI LPSocker::GetLocalIp()
{
    return m_dwLocalIp;
}

std::string& LPAPI LPSocker::GetLocalIpStr()
{
    return m_strLocalIp;
}

void LPAPI LPSocker::SetLocalPort(LPUINT16 wPort)
{
    m_wLocalPort = wPort;
}

LPUINT16 LPAPI LPSocker::GetLocalPort()
{
    return m_wLocalPort;
}

void LPAPI LPSocker::SetConnect(bool bConnect)
{
    m_bConnect = bConnect;
}

BOOL LPAPI LPSocker::IsConnect()
{
    return m_bConnect ? TRUE : FALSE;
}

void LPAPI LPSocker::SetPassiveClose(BOOL bPassiveClose)
{
    m_bPassiveClose = bPassiveClose;
}

BOOL LPAPI LPSocker::IsPassiveClose()
{
    return m_bPassiveClose;
}

void LPAPI LPSocker::SetParentId(LPUINT32 dwParentId)
{
    m_dwParentId = dwParentId;
}

LPUINT32 LPAPI LPSocker::GetParentId()
{
    return m_dwParentId;
}

void LPAPI LPSocker::SetAcceptCreate(BOOL bAcceptCreate)
{
    m_bAcceptCreate = bAcceptCreate;
}

BOOL LPAPI LPSocker::IsAcceptCreate()
{
    return m_bAcceptCreate;
}

void LPAPI LPSocker::AttachPacketParser(lp_shared_ptr<ILPPacketParser> pPacketParser)
{
    m_pPacketParser = pPacketParser;
    LOG_PROCESS_ERROR(m_pPacketParser);

Exit0:
    return;
}

void LPAPI LPSocker::DetachPacketParser()
{
    LOG_PROCESS_ERROR(m_pPacketParser);
    m_pPacketParser = nullptr;

Exit0:
    return;
}

void LPAPI LPSocker::AttachRecvBuf(LPLoopBuf* pLoopBuf)
{
    m_pRecvLoopBuf = pLoopBuf;
}

LPLoopBuf* LPAPI LPSocker::DetachRecvBuf()
{
    LPLoopBuf* pLoopBuf = m_pRecvLoopBuf;
    m_pRecvLoopBuf = NULL;
    return pLoopBuf;
}

void LPAPI LPSocker::AttachSendBuf(LPLoopBuf* pLoopBuf)
{
    m_pSendLoopBuf = pLoopBuf;
}

LPLoopBuf* LPAPI LPSocker::DetachSendBuf()
{
    LPLoopBuf* pLoopBuf = m_pSendLoopBuf;
    m_pSendLoopBuf = NULL;
    return pLoopBuf;
}

void LPAPI LPSocker::SetDelayCloseBeginTick(LPUINT64 qwTick)
{
    m_qwDelayCloseBeginTick = qwTick;
}

LPUINT64 LPAPI LPSocker::GetDelayCloseBeginTick()
{
    return m_qwDelayCloseBeginTick;
}

void LPAPI LPSocker::SetDelayCloseDuration(LPUINT64 qwDuration)
{
    m_qwDelayCloseDuration = qwDuration;
}

LPUINT64 LPAPI LPSocker::GetDelayCloseDuration()
{
    return m_qwDelayCloseDuration;
}

void LPAPI LPSocker::SetDelayReleaseBeginTick(LPUINT64 qwTick)
{
    m_qwDelayReleaseBeginTick = qwTick;
}

LPUINT64 LPAPI LPSocker::GetDelayReleaseBeginTick()
{
    return m_qwDelayReleaseBeginTick;
}

void LPAPI LPSocker::SetDelayReleaseDuration(LPUINT64 qwDuration)
{
    m_qwDelayReleaseDuration = qwDuration;
}

LPUINT64 LPAPI LPSocker::GetDelayReleaseDuration()
{
    return m_qwDelayReleaseDuration;
}

void LPAPI LPSocker::OnClose()
{
    //延迟关闭
    LOG_PROCESS_ERROR(GetNetImpl());
    GetNetImpl()->GetSockerMgr().Release(lp_shared_from_this());

Exit0:
    return;
}

void LPAPI LPSocker::SetNetImpl(LPNetImpl * pNetImpl)
{
    m_pNetImpl = pNetImpl;
}

LPNetImpl* LPAPI LPSocker::GetNetImpl()
{
    return m_pNetImpl;
}

BOOL LPAPI LPSocker::Send(const char* pData, LPUINT32 dwLen)
{
    LPINT32 nResult = 0;
    BOOL bOverflow = FALSE;

    LOG_PROCESS_ERROR(pData);
    LOG_PROCESS_ERROR(dwLen > 0);
    LOG_PROCESS_ERROR(m_pSendLoopBuf);

    //检测是否sock是否连接中
    PROCESS_SUCCESS(!IsConnect());

    if(m_pSendLoopBuf->GetTotalWritableLen() < dwLen)
    {
        bOverflow = TRUE;
    }
    else
    {
        nResult = m_pSendLoopBuf->Write(pData, dwLen);
        LOG_PROCESS_ERROR(nResult);
    }

    if(bOverflow)
    {
        Close(SOCK_ERR_CODE(eSockErrCode_SendBufOverflow, 1, 0), FALSE);
        LOG_PROCESS_ERROR(FALSE);
    }

    PostSend();

Exit1:
    return TRUE;
Exit0:
    return FALSE;
}

void LPAPI LPSocker::Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose)
{
    system::error_code err;

    IMP("socker close, socker_id=%d, socket=%d, eSockErrCode=%d, dwParam=%d, dwErrno=%d, desc=%s",
        GetSockerId(), GetSock(), stSockErrCode.eSockErrCode, stSockErrCode.dwParam, stSockErrCode.dwErrno, stSockErrCode.GetDescription());

    //检测是否连接
    PROCESS_SUCCESS(!IsConnect());
    SetConnect(false);

    //关闭接收链接，但不关闭发送链接
    LOG_CHECK_ERROR(nullptr != GetSocket());
    if(nullptr != GetSocket())
    {
        GetSocket()->close(err);
    }

    GetNetImpl()->GetEventMgr().PushTerminateEvent(lp_shared_from_this(), GetSockerId(), bPassiveClose);

Exit1:
    return;
}

void LPAPI LPSocker::PostRecv()
{
    LPINT32 nResult = 0;
    system::error_code err;

    LOG_PROCESS_ERROR(GetSocket());
    LOG_PROCESS_ERROR(m_pRecvLoopBuf);

    GetSocket()->async_receive(asio::buffer(m_pRecvLoopBuf->WritePtr(), m_pRecvLoopBuf->GetOnceWritableLen()), boost::bind(&LPSocker::HandleRecv, this, _1, _2));

Exit0:
    return;
}

void LPAPI LPSocker::HandleRecv(const system::error_code& err, const size_t nSize)
{
    LPINT32 nResult = 0;
    LPINT32 nUsed = 0;

    LOG_PROCESS_ERROR(m_pRecvLoopBuf);
    LOG_PROCESS_ERROR(m_pPacketParser);

    if(system::errc::success != err || nSize == 0)
    {
        Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 5, err.value()), FALSE);
        PROCESS_ERROR(FALSE);
    }

    m_pRecvLoopBuf->FinishWrite((LPUINT32)nSize);

    while(m_pRecvLoopBuf->GetTotalReadableLen() > 0)
    {
        nUsed = m_pPacketParser->Parse(m_pRecvLoopBuf);
        if(0 == nUsed)
        {
            //接收区已满，但还没有装下一个包，只好断开连接
            if(m_pRecvLoopBuf->GetTotalWritableLen() <= 0)
            {
                Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 1, 0), FALSE);
                LOG_PROCESS_ERROR(FALSE);
            }

            break;
        }
        else if(nUsed > 0)
        {
            if((LPUINT32)nUsed > m_pRecvLoopBuf->GetTotalReadableLen())
            {
                Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 2, 0), FALSE);
                LOG_PROCESS_ERROR(FALSE);
            }

            //m_pRecvLoopBuf通过引用而不是指针来传递，可以防止函数内部存储指针
            nResult = GetNetImpl()->GetEventMgr().PushRecvEvent(lp_shared_from_this(), GetSockerId(), *m_pRecvLoopBuf, nUsed);
            if(!nResult)
            {
                Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 3, 0), FALSE);
                LOG_PROCESS_ERROR(FALSE);
            }
        }
        else
        {
            Close(SOCK_ERR_CODE(eSockErrCode_RecvError, 4, 0), FALSE);
            LOG_PROCESS_ERROR(FALSE);
        }
    }

    PostRecv();

    return;
Exit0:
    return;
}

BOOL LPAPI LPSocker::PostSend()
{
    LPINT32 nResult = 0;
    BOOL bSend = FALSE;
    DWORD dwNumberOfBytesSend = 0;
    LPUINT32 dwOnceReadableLen = 0;
    system::error_code err;

    LOG_PROCESS_ERROR(m_pSendLoopBuf);
    LOG_PROCESS_ERROR(GetSocket());

    PROCESS_SUCCESS(!IsConnect());

    //判断是否有数据正在发送
    PROCESS_SUCCESS(m_bSending);

    dwOnceReadableLen = m_pSendLoopBuf->GetOnceReadableLen();
    if(dwOnceReadableLen > 0)
    {
        bSend = TRUE;
        m_bSending = true;

        GetSocket()->async_send(asio::buffer(m_pSendLoopBuf->ReadPtr(), dwOnceReadableLen), boost::bind(&LPSocker::HandleSend, this, _1, _2));
    }

Exit1:
Exit0:

    nResult = bSend;
    return nResult;
}

void LPAPI LPSocker::HandleSend(const system::error_code& err, const size_t nSize)
{
    LOG_PROCESS_ERROR(m_pSendLoopBuf);

    if(system::errc::success != err)
    {
        Close(SOCK_ERR_CODE(eSockErrCode_PostSendFail, 1, err.value()), FALSE);
        PROCESS_ERROR(FALSE);
    }

    m_pSendLoopBuf->FinishRead((LPUINT32)nSize);

    //有数据正在发送标记清除
    m_bSending = false;

Exit0:
    return;
}

lp_shared_ptr<boost::asio::ip::tcp::socket> LPAPI LPSocker::GetSocket()
{
    return m_pSocket;
}



//end声明所处的名字空间
NS_LZPL_END