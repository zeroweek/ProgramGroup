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
	//		��
	CGSMessageHandler();
	
	// Summary:
	//		��
	virtual ~CGSMessageHandler();

	// Summary:
	//		��ʼ��
	BOOL LPAPI Init(void);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

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
	virtual void LPAPI OnConnectDisconnected(ILPSocker* pSocker, ILPConnector* pConnector);

public:

	// Summary:
	//		���
	// Input:
	//		pLoopBuf�����������ݻ�����
	// Return:
	//		"<0"-��������"=0"-����������">0"-����������
	virtual INT_32 LPAPI Parse(ILPLoopBuf* pLoopBuf);

public:

	BOOL DoGTRegister(ILPSocker* pSocker);
	void OnGTRegisterAck(ILPSocker* pSocker, const char* pcszBuf, UINT_32 dwSize);


public:

	void CloseAllSocker(void);

	UINT_32 GetSockerCount(void);

private:

	typedef std::map<UINT_32, ILPSocker*>  MAP_SOCKER;
	MAP_SOCKER                         m_mapSocker;
	MAP_SOCKER::iterator               m_iterSocker;

	typedef void (CGSMessageHandler::*pfunMessageCallback)(ILPSocker* pSocker, const char* pcszBuf, UINT_32 dwSize);
	pfunMessageCallback                m_MessageCallbackList[max_internal_message_count];
};






#endif