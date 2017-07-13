//****************************************************************************/
//    author: caihy
//    date: December 21, 2015
//    description:
//
//****************************************************************************/
#ifndef _HTTP_MESSAGE_HANDLER_H_
#define _HTTP_MESSAGE_HANDLER_H_

#include "lp_lzpl.h"


#define ISSPACE(x) isspace((int)(x))

#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"



// Summary:
//      http连接对象类
class DECLARE LPHttpObject
{
public:

    // Summary:
    //      无
    LPHttpObject();

    // Summary:
    //      无
    ~LPHttpObject();

    // Summary:
    //      初始化
    BOOL LPAPI Init(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      反始化
    BOOL LPAPI UnInit(void);

    // Summary:
    //      关闭文件
    void LPAPI Close(void);

    // Summary:
    //      无
    lp_shared_ptr<ILPSocker> LPAPI GetSocker(void);

    BOOL LPAPI HaveUnResponseRequest(void);
    void LPAPI SetHaveUnResponseRequest(BOOL bHave);

    // Summary:
    //      无
    // Input
    //      szInputBuf[in]: input缓冲区
    //      dwInputLen[in]: input缓冲区大小
    //      szOutputBuf[out]: output缓冲区
    //      szOutputLen[in out]: in-输入缓冲区大小，out-输出大小
    void LPAPI ParseRequest(const char* szInputBuf, LPUINT32 dwInputLen, char* szOutputBuf, LPUINT32& szOutputLen);

    void LPAPI DoResponse_NotFound(void);
    void LPAPI DoResponse_BadRequest(void);
    void LPAPI DoResponse_CanNotExcute(void);
    void LPAPI DoResponse_Headers(void);
    void LPAPI DoResponse_UnImplemented(void);
    void LPAPI DoResponse_RecvSuccess(const char* pcszBuf, LPUINT32 dwSize);

private:

    lp_shared_ptr<ILPSocker>    m_pSocker;

    BOOL                        m_bHaveUnResponseRequest;
};



// Summary:
//      消息管理器类
class DECLARE CGTHttpMessageHandler : public ILPPacketParser, public ILPNetMessageHandler
{
public:

    // Summary:
    //      无
    CGTHttpMessageHandler();

    // Summary:
    //      无
    virtual ~CGTHttpMessageHandler();

    // Summary:
    //      初始化
    BOOL LPAPI Init(void);

    // Summary:
    //      反始化
    BOOL LPAPI UnInit(void);

public:

    // Summary:
    //      无
    virtual void LPAPI OnAccepted(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      无
    virtual void LPAPI OnConnected(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      无
    virtual void LPAPI OnConnectError(lp_shared_ptr<ILPConnector> pConnector, LPUINT32 dwErrorNo);

    // Summary:
    //      无
    virtual void LPAPI OnMessage(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);

    // Summary:
    //      无
    virtual void LPAPI OnDisconnected(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      无
    virtual void LPAPI OnConnectDisconnected(lp_shared_ptr<ILPSocker> pSocker, lp_shared_ptr<ILPConnector> pConnector);

public:

    // Summary:
    //      解包
    // Input:
    //      pLoopBuf：待解析数据缓冲区
    // Return:
    //      "<0"-解析错误，"=0"-不完整包，">0"-完整包长度
    virtual LPINT32 LPAPI Parse(lp_shared_ptr<ILPLoopBuf> pLoopBuf);

public:

    void CloseAllHttpObject(void);

private:

    LPHttpObject* LPAPI _NewHttpObject(void);
    BOOL LPAPI _DelHttpObject(LPHttpObject* pHttpObject);

    BOOL LPAPI _ParseHttpMessage(LPUINT32 dwSockerID, const char * pcszBuf, LPUINT32 dwSize);
    BOOL LPAPI _ParseHttpMessageImpl(LPUINT32 dwSockerID, const char * pcszBuf, LPUINT32 dwSize);

private:

    typedef std::map<LPUINT32, LPHttpObject*>  MAP_HTTP_OBJECT;
    MAP_HTTP_OBJECT                     m_mapHttpObject;
    MAP_HTTP_OBJECT::iterator           m_iterHttpObject;

    LPObjPool<LPHttpObject>             m_oHttpObjectPool;

};






#endif