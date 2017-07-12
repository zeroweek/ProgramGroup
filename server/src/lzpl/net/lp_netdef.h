//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_NET_DEF_H_
#define _LP_NET_DEF_H_

#include "lp_base.h"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/asio/buffer.hpp"
#include "boost/thread.hpp"


using namespace boost::asio;


#if defined _WIN32
#include <MSWSock.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



#ifndef _WIN32
#   define ERROR_OPERATION_ABORTED          995L
#   define ERROR_IO_PENDING                 997L    // dderror
#   define WSA_IO_PENDING                   (ERROR_IO_PENDING)
#   define WSAID_CONNECTEX                  {0x25a207b9,0xddf3,0x4660,{0x8e,0xe9,0x76,0xe5,0x8c,0x74,0x06,0x3e}}
#   define WSAID_ACCEPTEX                   {0xb5367df1,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92}}
#   define WSAID_GETACCEPTEXSOCKADDRS       {0xb5367df2,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92}}
#else
#   define EPOLLIN                          0x0
#   define EPOLLOUT                         0x0
#   define EPOLLPRI                         0x0
#   define EPOLLERR                         0x0
#   define EPOLLHUP                         0x0
#   define EPOLLRDHUP                       0x0
#   define EPOLLET                          0x0
#   define EPOLLONESHOT                     0x0
#endif



// Summary:
//      事件处理器类型
enum e_EventHandlerType
{
    eEventHandlerType_None = 0,
    eEventHandlerType_Connector = 1,
    eEventHandlerType_Listener = 2,
    eEventHandlerType_Socker = 3,
    eEventHandlerType_Max
};



// Summary:
//      io操作类型枚举
enum e_IoOptType
{
    eIoOptType_None = 0,
    eIoOptType_RecvOrSend,
    eIoOptType_Recv,
    eIoOptType_Send,
    eIoOptType_Accept,
    eIoOptType_Connect,
    eIoOptType_Max,
};



#ifndef _WIN32

// Summary:
//   WSABUF结构定义
typedef struct _WSABUF
{
    LPUINT64      len;     /* the length of the buffer */
    char*         buf;     /* the pointer to the buffer */
} WSABUF, *LPWSABUF;

// Summary:
//   OVERLAPPED结构定义
typedef struct _OVERLAPPED
{
    LPUINT64* Internal;
    LPUINT64* InternalHigh;
    union
    {
        struct
        {
            LPUINT32 Offset;
            LPUINT32 OffsetHigh;
        } DUMMYSTRUCTNAME;
        void* Pointer;
    } DUMMYUNIONNAME;

    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

// Summary:
// GUID结构定义
typedef struct _GUID
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

typedef struct linger LINGER;
#endif



#ifndef _WIN32
typedef
BOOL
(*LPFN_CONNECTEX)(
    SOCKET s,
    const struct sockaddr* name,
    int namelen,
    void* lpSendBuffer,
    DWORD dwSendDataLength,
    LPDWORD lpdwBytesSent,
    LPOVERLAPPED lpOverlapped
);

typedef
BOOL
(*LPFN_ACCEPTEX)(
    SOCKET sListenSocket,
    SOCKET sAcceptSocket,
    void* lpOutputBuffer,
    DWORD dwReceiveDataLength,
    DWORD dwLocalAddressLength,
    DWORD dwRemoteAddressLength,
    LPDWORD lpdwBytesReceived,
    LPOVERLAPPED lpOverlapped
);

typedef
void
(* LPFN_GETACCEPTEXSOCKADDRS)(
    void* lpOutputBuffer,
    DWORD dwReceiveDataLength,
    DWORD dwLocalAddressLength,
    DWORD dwRemoteAddressLength,
    struct sockaddr **LocalSockaddr,
    LPINT32* LocalSockaddrLength,
    struct sockaddr **RemoteSockaddr,
    LPINT32* RemoteSockaddrLength
);
#else
typedef  union epoll_data
{
    void      *ptr;
    LPINT32    fd;
    LPUINT32   u32;
    LPUINT64   u64;
} epoll_data_t;

struct epoll_event
{
    LPUINT32       events;     /* Epoll events */
    epoll_data_t   data;       /* User data variable */
};
#endif



// Summary:
//      每个io对应的绑定数据
struct PER_IO_DATA
{
    OVERLAPPED             stOverlapped;
    SOCKET                 hSock;
    e_IoOptType            eIoOptType;
    e_EventHandlerType     eHandlerType;
    LPUINT64               qwByteTransferred;
    WSABUF                 stWsaBuf;
    char                   szBuf[128];
    BOOL                   bOperateRet;
    epoll_event*           ptEpollEvent;

    PER_IO_DATA()
    {
        memset(&stOverlapped, 0, sizeof(stOverlapped));
        hSock = INVALID_SOCKET;
        eIoOptType = eIoOptType_None;
        eHandlerType = eEventHandlerType_None;
        qwByteTransferred = 0;
        memset(&stWsaBuf, 0, sizeof(stWsaBuf));
        memset(szBuf, 0, sizeof(szBuf));
        bOperateRet = FALSE;
        ptEpollEvent = nullptr;
    }
};



//end声明所处的名字空间
NS_LZPL_END

#endif