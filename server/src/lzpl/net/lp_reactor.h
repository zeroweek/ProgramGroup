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

public:

	// Summary��
	//		��
	// Input:
	//		bOneCompletionPortOneThread: �Ƿ񵥸���ɶ˿�ģʽ
	//			TRUE-һ����ɶ˿ڶ�Ӧ����̣߳�FALSE-�����ɶ˿ڣ�ÿ����ɶ˿ڶ�Ӧһ���߳�
	virtual BOOL LPAPI Init(BOOL bOneCompletionPortOneThread);
	// Summary��
	//		��     
	virtual BOOL LPAPI UnInit();

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

	// Summary��
	//     �̴߳����������߼�
	virtual void LPAPI OnExecute(LPINT32 nCompletionPortIndex);

public:

	// Summary��
	//     ��ɶ˿��̴߳�����
	static unsigned LPAPI ThreadFunc(void* pParam);

protected:

	// Summary��
	//		��ȡ��ǰ״̬
	virtual LPUINT32 LPAPI _GetState();

	// Summary��
	//		���õ�ǰ״̬
	virtual void LPAPI _SetState(LPUINT32 dwState);

protected:

	volatile atomic_uint    m_dwState;
};



// Summary��
//     windows IOCP��Ӧ��ʵ����
class DECLARE LPIocpReactor : public LPReactor
{
public:

	// Summary��
	//     ���캯��
	// Returns:
	//     ��
	LPIocpReactor();

	// Summary��
	//     ��������
	// Returns:
	//     ��
	~LPIocpReactor();

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
	//		��
	// Input:
	//		bOneCompletionPortOneThread: �Ƿ񵥸���ɶ˿�ģʽ
	//			TRUE-һ����ɶ˿ڶ�Ӧ����̣߳�FALSE-�����ɶ˿ڣ�ÿ����ɶ˿ڶ�Ӧһ���߳�
	virtual BOOL LPAPI Init(BOOL bOneCompletionPortOneThread);
	// Summary��
	//		��     
	virtual BOOL LPAPI UnInit();

	// Summary��
	//     �̴߳����������߼�
	virtual void LPAPI OnExecute(LPINT32 nCompletionPortIndex);

protected:

	LPINT32                  m_nCompletionPortCount;
	HANDLE*                  m_pCompletionPort;
	LPINT32                  m_nWorkerCountPerCompIo;
	HANDLE**                 m_ppWorkerArray;
};



//end�������������ֿռ�
NS_LZPL_END

#endif