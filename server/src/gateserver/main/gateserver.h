//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_GATE_SERVER_H_
#define _LP_GATE_SERVER_H_

#include "lp_lzpl.h"
#include "common.h"
#include "server_def.h"
#include "gt_message_handler.h"
#include "gt_external_packet_parser.h"
#include "gt_internal_packet_parser.h"
#include "http_message_handler.h"


using namespace LZPL;



// Summary:
//		нч
class CGateServer
{
	SINGLETON_DECLARE(CGateServer)

public:

	BOOL LPAPI Init(void);
	BOOL LPAPI UnInit(void);

	BOOL LPAPI MainLoop(void);

	UINT_32 LPAPI GetServerState(void);
	void SetServerState(UINT_32 dwServerState);

	void Close(void);

private:

	// Summary:
	//		none
	CGateServer();

	// Summary:
	//		none
	virtual ~CGateServer();

private:

	ILPNet*                          m_pNet;
	ILPListener*                     m_pListener;
	ILPListener*                     m_pClientListener;
	CGTMessageHandler                m_oGTMessageHandler;
	CGTInternalPacketParser *        m_pGTInternalPacketParser;
	CGTExternalPacketParser *        m_pGTExternalPacketParser;
	CGTHttpMessageHandler            m_oGTHttpMessageHandler;

	UINT_32                          m_dwServerState;

};















#endif