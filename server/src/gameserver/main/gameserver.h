//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_GAME_SERVER_H_
#define _LP_GAME_SERVER_H_

#include "lp_lzpl.h"
#include "common.h"
#include "server_def.h"
#include "gs_message_handler.h"
#include "gs_internal_packet_parser.h"
#include "gs_external_packet_parser.h"


using namespace LZPL;



// Summary:
//		нч
class CGameServer
{
	SINGLETON_DECLARE(CGameServer)

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
	CGameServer();

	// Summary:
	//		none
	virtual ~CGameServer();

private:

	BOOL _InitDB(void);

private:

	ILPNet*                          m_pNet;
	ILPConnector*                    m_pConnector;
	ILPListener*                     m_pListener;
	CGSMessageHandler                m_oGSInternalMessageHandler;
	CGSInternalPacketParser*         m_pGSInternalPacketParser;
	CGSExternalPacketParser*         m_pGSExternalPacketParser;

	UINT_32                          m_dwServerState;

};















#endif