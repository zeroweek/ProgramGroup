//****************************************************************************/
//    author: caihy
//    date: Jan 9, 2017
//    description:
//
//****************************************************************************/
#ifndef _LPI_LISTENER_IMPL_H_
#define _LPI_LISTENER_IMPL_H_

#include "lpi_listener.h"
#include "lpi_packetparser.h"
#include "lp_netdef.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//类声明
class LPNetImpl;
class ILPReactor;



// Summary:
//      监听器接口类
class DECLARE ILPListenerImpl : public ILPListener, public lp_enable_shared_from_this<ILPListenerImpl>
{
public:

    virtual ~ILPListenerImpl() {}

    // Summary：
    //      无
    // Input:
    //      pNetImpl:
    //      pPacketParser: 消息包解析对象
    //      dwId: 监听器id
    virtual BOOL LPAPI Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId) = 0;

public:

    static std::shared_ptr<ILPListenerImpl> LPAPI NewListenerImpl();
    static void LPAPI DeleteListenerImpl(std::shared_ptr<ILPListenerImpl>& pListener);
};

typedef std::map<LPUINT32, std::shared_ptr<ILPListenerImpl>> MAP_LISTENER;



//end声明所处的名字空间
NS_LZPL_END

#endif