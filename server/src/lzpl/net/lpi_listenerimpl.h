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



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;
class ILPReactor;



// Summary:
//      �������ӿ���
class DECLARE ILPListenerImpl : public ILPListener, public lp_enable_shared_from_this<ILPListenerImpl>
{
public:

    virtual ~ILPListenerImpl() {}

    // Summary��
    //      ��
    // Input:
    //      pNetImpl:
    //      pPacketParser: ��Ϣ����������
    //      dwId: ������id
    virtual BOOL LPAPI Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId) = 0;

public:

    static std::shared_ptr<ILPListenerImpl> LPAPI NewListenerImpl();
    static void LPAPI DeleteListenerImpl(std::shared_ptr<ILPListenerImpl>& pListener);
};

typedef std::map<LPUINT32, std::shared_ptr<ILPListenerImpl>> MAP_LISTENER;



//end�������������ֿռ�
NS_LZPL_END

#endif