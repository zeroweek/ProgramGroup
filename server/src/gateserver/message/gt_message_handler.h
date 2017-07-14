//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: ��Ϣ������
//
//****************************************************************************/
#ifndef _GT_MESSAGE_HANDLER_H_
#define _GT_MESSAGE_HANDLER_H_

#include "lp_lzpl.h"
#include "lp_messageserializer.h"
#include "internal_message_header.h"
#include "external_message_header.h"


using namespace INTERNAL_MESSAGE;
using namespace EXTERNAL_MESSAGE;



// Summary:
//
class DECLARE CGTMessageHandler : public ILPNetMessageHandler
{
public:

    friend class CGTInternalPacketParser;

public:

    // Summary:
    //      ��
    CGTMessageHandler();

    // Summary:
    //      ��
    virtual ~CGTMessageHandler();

    // Summary:
    //      ��ʼ��
    BOOL LPAPI Init(void);

    // Summary:
    //      ��ʼ��
    BOOL LPAPI UnInit(void);

public:

    // Summary:
    //      ��
    virtual void LPAPI OnAccepted(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      ��
    virtual void LPAPI OnConnected(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      ��
    virtual void LPAPI OnConnectError(lp_shared_ptr<ILPConnector> pConnector, LPUINT32 dwErrorNo);

    // Summary:
    //      ��
    virtual void LPAPI OnMessage(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);

    // Summary:
    //      ��
    virtual void LPAPI OnDisconnected(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      ��
    virtual void LPAPI OnConnectDisconnected(lp_shared_ptr<ILPSocker> pSocker, lp_shared_ptr<ILPConnector> pConnector);


public:

    BOOL DoGSRegisterAck(lp_shared_ptr<ILPSocker> pSocker);
    void OnGameServerRegister(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);


public:

    BOOL DoCLientLoginAck(lp_shared_ptr<ILPSocker> pSocker);
    void OnClientLoginReq(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);


public:

    void CloseAllSocker(void);

private:

    LPMessageSerializer      m_oRecvMessageSerializer;
    LPMessageSerializer      m_oSendMessageSerializer;

    typedef std::map<LPUINT32, lp_shared_ptr<ILPSocker>>  MAP_SOCKER;
    MAP_SOCKER                       m_mapSocker;
    MAP_SOCKER::iterator             m_iterSocker;

    typedef void (CGTMessageHandler::*pfunMessageCallback)(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);
    pfunMessageCallback              m_MessageCallbackList[max_internal_message_count];
};






#endif