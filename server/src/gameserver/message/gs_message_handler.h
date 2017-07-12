//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: ��Ϣ������
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
    virtual void LPAPI OnConnectError(std::shared_ptr<ILPConnector> pConnector, LPUINT32 dwErrorNo);

    // Summary:
    //      ��
    virtual void LPAPI OnMessage(lp_shared_ptr<ILPSocker> pSocker, const char* pcszBuf, LPUINT32 dwSize);

    // Summary:
    //      ��
    virtual void LPAPI OnDisconnected(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      ��
    virtual void LPAPI OnConnectDisconnected(lp_shared_ptr<ILPSocker> pSocker, std::shared_ptr<ILPConnector> pConnector);

public:

    // Summary:
    //      ���
    // Input:
    //      pLoopBuf�����������ݻ�����
    // Return:
    //      "<0"-��������"=0"-����������">0"-����������
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