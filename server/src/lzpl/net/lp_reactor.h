//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description:
//
//****************************************************************************/
#ifndef _LP_REACTOR_H_
#define _LP_REACTOR_H_

#include "lpi_reactor.h"
#include "lpi_net.h"
#include "lp_thread.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//      sql�����̲߳���
struct REACTOR_THREAD_PARAM
{
    ILPReactor*       pReactor;
    LPINT32           nIndex;

    REACTOR_THREAD_PARAM()
    {
        pReactor = nullptr;
        nIndex = -1;
    }
};



// Summary��
//     ������Ӧ��
class DECLARE LPReactor : public ILPReactor
{
public:

    // Summary��
    //     ���캯��
    // Returns:
    //     ��
    LPReactor();

    // Summary��
    //     ��������
    // Returns:
    //     ��
    ~LPReactor();

    // Summary��
    //      ��
    virtual BOOL LPAPI Init(NET_CONFIG& stNetConfig);
    // Summary��
    //      ��
    virtual BOOL LPAPI UnInit();

    // Summary:
    //   ��
    virtual boost::asio::io_service& GetIoService(LPUINT32 dwId);

    // Summary��
    //     �̴߳����������߼�
    virtual void LPAPI OnExecute(LPUINT32 nIndex);

    // Summary��
    //     ��ɶ˿��̴߳�����
    virtual void ThreadFunc(LPUINT32 nIndex);

protected:

    // Summary��
    //      ��ȡ��ǰ״̬
    virtual LPUINT32 LPAPI GetState();

    // Summary��
    //      ���õ�ǰ״̬
    virtual void LPAPI SetState(LPUINT32 dwState);

protected:

    volatile atomic_uint                            m_dwState;
    NET_CONFIG                                      m_stNetConfig;

    LPUINT32                                        m_nIoServiceNum;
    std::vector<lp_shared_ptr<asio::io_service>>    m_vecIoService;
    std::vector<lp_shared_ptr<std::thread>>         m_vecThread;
};



//end�������������ֿռ�
NS_LZPL_END

#endif