//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description:
//
//        ��Ӧ��ILPReactor�ӿ�������
//
//****************************************************************************/
#ifndef _LPI_REACTOR_H_
#define _LPI_REACTOR_H_

#include "lp_netdef.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//�ṹ������
struct NET_CONFIG;



// Summary��
//     ��Ӧ������ILPEventHandler���������������¼�����
class DECLARE ILPReactor
{
public:

    // Summary:
    //      ��
    virtual ~ILPReactor() {}

    // Summary:
    //   ��
    virtual boost::asio::io_service& GetIoService(LPUINT32 dwId) = 0;

public:

    static lp_shared_ptr<ILPReactor> LPAPI NewReactor(NET_CONFIG& stNetConfig);
    static void LPAPI DeleteReactor(lp_shared_ptr<ILPReactor>& pReactor);
};



//end�������������ֿռ�
NS_LZPL_END

#endif