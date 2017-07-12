//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: 消息管理器
//
//****************************************************************************/
#ifndef _GS_MESSAGE_HANDLER_H_
#define _GS_MESSAGE_HANDLER_H_

#include "lp_lzpl.h"
#include "lp_messageserializer.h"
#include "internal_message_header.h"
#include "external_message_header.h"



using namespace INTERNAL_MESSAGE;
using namespace EXTERNAL_MESSAGE;



// Summary:
//
class DECLARE CGSMessageHandler : public ILPNetMessageHandler
{
public:

    // Summary:
    //      无
    CGSMessageHandler();

    // Summary:
    //      无
    virtual ~CGSMessageHandler();

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
    virtual void LPAPI OnConnectError(std::shared_ptr<ILPConnector> pConnector, LPUINT32 dwErrorNo);

    // Summary:
    //      无
    virtual void LPAPI OnMessage(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);

    // Summary:
    //      无
    virtual void LPAPI OnDisconnected(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      无
    virtual void LPAPI OnConnectDisconnected(lp_shared_ptr<ILPSocker> pSocker, std::shared_ptr<ILPConnector> pConnector);

public:

    // Summary:
    //      解包
    // Input:
    //      pLoopBuf：待解析数据缓冲区
    // Return:
    //      "<0"-解析错误，"=0"-不完整包，">0"-完整包长度
    virtual LPINT32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

public:

    BOOL DoGTRegister(lp_shared_ptr<ILPSocker> pSocker);
    void OnGTRegisterAck(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);


public:

    void CloseAllSocker(void);

    LPUINT32 GetSockerCount(void);

private:

    typedef std::map<LPUINT32, lp_shared_ptr<ILPSocker>>  MAP_SOCKER;
    MAP_SOCKER                         m_mapSocker;
    MAP_SOCKER::iterator               m_iterSocker;

    typedef void (CGSMessageHandler::*pfunMessageCallback)(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);
    pfunMessageCallback                m_MessageCallbackList[max_internal_message_count];
};






#endif