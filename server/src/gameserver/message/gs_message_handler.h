//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: ��Ϣ������
//
//****************************************************************************/
#ifndef _GS_MESSAGE_HANDLER_H_
#define _GS_MESSAGE_HANDLER_H_

#include "lp_lzpl.h"
#include "lpi_message.h"
#include "lp_messageserializer.h"
#include "LPMsg.pb.h"
#include "LPDefine.pb.h"



// Summary:
//
class DECLARE CGSMessageHandler : public ILPNetMessageHandler
{
public:

    // Summary:
    //      ��
    CGSMessageHandler();

    // Summary:
    //      ��
    virtual ~CGSMessageHandler();

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

    BOOL DoGTRegister(lp_shared_ptr<ILPSocker> pSocker);
    void OnGTRegisterAck(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);


public:

    void CloseAllSocker(void);

    LPUINT32 GetSockerCount(void);

    BOOL SendMessage(lp_shared_ptr<ILPSocker> pSocker, const LPUINT32 nMsgID, const std::string& strMsgData);

private:

    lp_shared_ptr<MessageHead>              m_pRecvMsgHead;
    lp_shared_ptr<LPMessageSerializer>      m_pMessageSerializer;

    lp_shared_ptr<LPMessageSerializer>      m_pRecvMessageSerializer;
    lp_shared_ptr<LPMessageSerializer>      m_pSendMessageSerializer;

    typedef std::map<LPUINT32, lp_shared_ptr<ILPSocker>>  MAP_SOCKER;
    MAP_SOCKER                              m_mapSocker;
    MAP_SOCKER::iterator                    m_iterSocker;

    typedef void (CGSMessageHandler::*pfunMessageCallback)(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);
    pfunMessageCallback                m_MessageCallbackList[LPDefine::msg_end];
};






#endif