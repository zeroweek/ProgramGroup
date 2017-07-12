//****************************************************************************/
//    author: caihy
//    date: Jan 9, 2017
//    description:
//
//****************************************************************************/
#ifndef _LPI_CONNECTOR_IMPL_H_
#define _LPI_CONNECTOR_IMPL_H_

#include "lpi_connector.h"
#include "lpi_packetparser.h"
#include "lp_netdef.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;
class ILPReactor;



// Summary:
//      �������ӿ���
class DECLARE ILPConnectorImpl : public ILPConnector, public lp_enable_shared_from_this<ILPConnectorImpl>
{
public:

    // Summary:
    //      ��
    virtual ~ILPConnectorImpl() {}

    // Summary��
    //      ��
    // Input:
    //      pNetImpl:
    //      pPacketParser: ��Ϣ����������
    //      dwId: ������id
    virtual BOOL LPAPI Init(lp_shared_ptr<LPNetImpl> pNetImpl, lp_shared_ptr<ILPReactor> pReactor, lp_shared_ptr<ILPPacketParser> pPacketParser, LPUINT32 dwId) = 0;

    // Summary:
    //      ���ӹرջص�
    virtual void LPAPI OnConnectionClose() = 0;

public:

    static std::shared_ptr<ILPConnectorImpl> LPAPI NewConnectorImpl(LPUINT32 dwIoType);
    static void LPAPI DeleteConnectorImpl(std::shared_ptr<ILPConnectorImpl>& pConnector);
};

typedef std::map<LPUINT32, std::shared_ptr<ILPConnectorImpl>> MAP_CONNECTOR;



//end�������������ֿռ�
NS_LZPL_END

#endif