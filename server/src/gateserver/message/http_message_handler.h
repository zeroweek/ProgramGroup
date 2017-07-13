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
//      http���Ӷ�����
class DECLARE LPHttpObject
{
public:

    // Summary:
    //      ��
    LPHttpObject();

    // Summary:
    //      ��
    ~LPHttpObject();

    // Summary:
    //      ��ʼ��
    BOOL LPAPI Init(lp_shared_ptr<ILPSocker> pSocker);

    // Summary:
    //      ��ʼ��
    BOOL LPAPI UnInit(void);

    // Summary:
    //      �ر��ļ�
    void LPAPI Close(void);

    // Summary:
    //      ��
    lp_shared_ptr<ILPSocker> LPAPI GetSocker(void);

    BOOL LPAPI HaveUnResponseRequest(void);
    void LPAPI SetHaveUnResponseRequest(BOOL bHave);

    // Summary:
    //      ��
    // Input
    //      szInputBuf[in]: input������
    //      dwInputLen[in]: input��������С
    //      szOutputBuf[out]: output������
    //      szOutputLen[in out]: in-���뻺������С��out-�����С
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
//      ��Ϣ��������
class DECLARE CGTHttpMessageHandler : public ILPPacketParser, public ILPNetMessageHandler
{
public:

    // Summary:
    //      ��
    CGTHttpMessageHandler();

    // Summary:
    //      ��
    virtual ~CGTHttpMessageHandler();

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

    // Summary:
    //      ���
    // Input:
    //      pLoopBuf�����������ݻ�����
    // Return:
    //      "<0"-��������"=0"-����������">0"-����������
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