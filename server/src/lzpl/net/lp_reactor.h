//****************************************************************************/
//    author: caihy
//    date: October 29, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_REACTOR_H_
#define _LP_REACTOR_H_

#include "lpi_reactor.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//������
class LPNetImpl;



// Summary:
//		sql�����̲߳���
struct REACTOR_THREAD_PARAM
{
	ILPReactor*       pReactorImpl;
	LPINT32           nCompletionPortIndex;
};



// Summary��
//     windowsƽ̨��Ӧ����
class DECLARE LPReactorIocpImpl : public ILPReactor
{
public:

	// Summary��
	//     ���캯��
	// Returns:
	//     ��
	LPReactorIocpImpl();

	// Summary��
	//     ��������
	// Returns:
	//     ��
	~LPReactorIocpImpl();

public:

	// Summary:
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler);

	// Summary��
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler);

public:

	// Summary��
	//     ��ɶ˿��̴߳�����
	static unsigned LPAPI ThreadFunc(void* pParam);

	// Summary��
	//		��
	// Input:
	//		bOneCompletionPortOneThread: �Ƿ񵥸���ɶ˿�ģʽ
	//			TRUE-һ����ɶ˿ڶ�Ӧ����̣߳�FALSE-�����ɶ˿ڣ�ÿ����ɶ˿ڶ�Ӧһ���߳�
	BOOL LPAPI Init(BOOL bOneCompletionPortOneThread);
	// Summary��
	//		��     
	BOOL LPAPI UnInit();

	// Summary��
	//     �̴߳����������߼�
	void LPAPI OnExecute(LPINT32 nCompletionPortIndex);

private:

	// Summary��
	//		��ȡ��ǰ״̬
	LPUINT32 LPAPI _GetState();

	// Summary��
	//		���õ�ǰ״̬
	void LPAPI _SetState(LPUINT32 dwState);

protected:

	BOOL	                m_bInit;
	volatile atomic_uint    m_dwState;

	LPINT32                  m_nCompletionPortCount;
	HANDLE*                  m_pCompletionPort;
	LPINT32                  m_nWorkerCountPerCompIo;
	HANDLE**                 m_ppWorkerArray;
};



//end�������������ֿռ�
NS_LZPL_END

#endif