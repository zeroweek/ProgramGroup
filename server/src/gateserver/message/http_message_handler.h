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
//		http���Ӷ�����
class DECLARE LPHttpObject
{
public:

	// Summary:
	//		��
	LPHttpObject();

	// Summary:
	//		��
	~LPHttpObject();

	// Summary:
	//		��ʼ��
	BOOL LPAPI Init(ILPSocker* pSocker);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		�ر��ļ�
	void LPAPI Close(void);

	// Summary:
	//		��
	ILPSocker* LPAPI GetSocker(void);

	BOOL LPAPI HaveUnResponseRequest(void);
	void LPAPI SetHaveUnResponseRequest(BOOL bHave);

	// Summary:
	//		��
	// Input
	//		szInputBuf[in]: input������
	//		dwInputLen[in]: input��������С
	//		szOutputBuf[out]: output������
	//		szOutputLen[in out]: in-���뻺������С��out-�����С
	void LPAPI ParseRequest(const char* szInputBuf, UINT_32 dwInputLen, char* szOutputBuf, UINT_32& szOutputLen);

	void LPAPI DoResponse_NotFound(void);
	void LPAPI DoResponse_BadRequest(void);
	void LPAPI DoResponse_CanNotExcute(void);
	void LPAPI DoResponse_Headers(void);
	void LPAPI DoResponse_UnImplemented(void);
	void LPAPI DoResponse_RecvSuccess(const char* pcszBuf, UINT_32 dwSize);

private:

	ILPSocker*          m_pSocker;

	BOOL                m_bHaveUnResponseRequest;
};



// Summary:
//		��Ϣ��������
class DECLARE CGTHttpMessageHandler : public ILPPacketParser, public ILPNetMessageHandler
{
public:

	// Summary:
	//		��
	CGTHttpMessageHandler();
	
	// Summary:
	//		��
	virtual ~CGTHttpMessageHandler();

	// Summary:
	//		��ʼ��
	BOOL LPAPI Init(void);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

public:

	// Summary:
	//		���ü�����1
	virtual void LPAPI AddRef(void);

	// Summary:
	//		���ü�����1
	virtual UINT_32 LPAPI QueryRef(void);

	// Summary:
	//		�ͷţ�����ĸú���ʵ�ֽ�Ϊ�ͷ����ü��������ͷŶ���
	virtual void LPAPI Release(void);

public:

	// Summary:
	//		��
	virtual void LPAPI OnAccepted(ILPSocker* pSocker);

	// Summary:
	//		��
	virtual void LPAPI OnConnected(ILPSocker* pSocker);

	// Summary:
	//		��
	virtual void LPAPI OnConnectError(ILPConnector* pConnector, UINT_32 dwErrorNo);

	// Summary:
	//		��
	virtual void LPAPI OnMessage(ILPSocker* pSocker, const char* pcszBuf, UINT_32 dwSize);

	// Summary:
	//		��
	virtual void LPAPI OnDisconnected(ILPSocker* pSocker);

	// Summary:
	//		��
	virtual void LPAPI OnConnectDisconnected(ILPSocker* pSocker, ILPConnector* pConnector );

public:

	// Summary:
	//		���
	// Input:
	//		pLoopBuf�����������ݻ�����
	// Return:
	//		"<0"-��������"=0"-����������">0"-����������
	virtual INT_32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

public:

	void CloseAllHttpObject(void);

private:

	LPHttpObject* LPAPI _NewHttpObject(void);
	BOOL LPAPI _DelHttpObject(LPHttpObject* pHttpObject);

	BOOL LPAPI _ParseHttpMessage(ILPSocker * pSocker, const char * pcszBuf, UINT_32 dwSize);

private:

	UINT_32                  m_dwRef;

	typedef std::map<ILPSocker*, LPHttpObject*>  MAP_HTTP_OBJECT;
	MAP_HTTP_OBJECT               m_mapHttpObject;
	MAP_HTTP_OBJECT::iterator     m_iterHttpObject;

	LPObjPool<LPHttpObject>       m_oHttpObjectPool;

};






#endif