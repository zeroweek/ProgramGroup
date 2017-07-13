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
//      нч
class CGameServer
{
    SINGLETON_DECLARE(CGameServer)

public:

    BOOL LPAPI Init(void);
    BOOL LPAPI UnInit(void);

    BOOL LPAPI MainLoop(void);

    LPUINT32 LPAPI GetServerState(void);
    void SetServerState(LPUINT32 dwServerState);

    void Close(void);

private:

    // Summary:
    //      none
    CGameServer();

    // Summary:
    //      none
    virtual ~CGameServer();

private:

    BOOL InitDB(void);

private:

    lp_shared_ptr<ILPNet>                 m_pNet;
    lp_shared_ptr<ILPConnector>           m_pConnector;
    lp_shared_ptr<ILPListener>            m_pListener;
    lp_shared_ptr<CGSMessageHandler>        m_pGSInternalMessageHandler;
    lp_shared_ptr<CGSInternalPacketParser>  m_pGSInternalPacketParser;
    lp_shared_ptr<CGSExternalPacketParser>  m_pGSExternalPacketParser;

    LPUINT32                                m_dwServerState;

};















#endif