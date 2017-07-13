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
//      нч
class CGateServer
{
    SINGLETON_DECLARE(CGateServer)

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
    CGateServer();

    // Summary:
    //      none
    virtual ~CGateServer();

private:

    lp_shared_ptr<ILPNet>                 m_pNet;
    lp_shared_ptr<ILPListener>            m_pListener;
    lp_shared_ptr<ILPListener>            m_pClientListener;
    lp_shared_ptr<ILPConnector>           m_pConnector;
    lp_shared_ptr<CGTMessageHandler>        m_pGTMessageHandler;
    lp_shared_ptr<CGTInternalPacketParser>  m_pGTInternalPacketParser;
    lp_shared_ptr<CGTExternalPacketParser>  m_pGTExternalPacketParser;
    lp_shared_ptr<CGTHttpMessageHandler>    m_pGTHttpMessageHandler;

    LPUINT32                                m_dwServerState;

};















#endif