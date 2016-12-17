//****************************************************************************/
//    author: caihy
//    date: October 20, 2014
//    description: 
//
//****************************************************************************/
#ifndef _LPI_SOCKER_H_
#define _LPI_SOCKER_H_

#include "lpi_sockoption.h"
#include "lp_processerror.h"


//begin�������������ֿռ�
NS_LZPL_BEGIN



//****************************************************************************/
//	�Զ���socket������ö��
//
//		1. ���ӽ���ǰ�Ĵ����¼���socket��connect��bind��listen��accept���ش���
//		2. ���ӽ����¼���connect��accept���سɹ�
//		3. ���ӱ����еĳɹ��¼���read��write���سɹ�
//		3. ���ӱ����еĴ����¼���read��write���ط�����Ͽ�����
//		4. ���ӶϿ��¼���read��write��������Ͽ�
//
//****************************************************************************/
enum e_SockErrCode
{
	eSockErrCode_Fail                        = 0,    // ʧ��
	eSockErrCode_Success                     = 1,    // �ɹ�
	eSockErrCode_AcceptFail                  = 2,    // ����ʧ��
	eSockErrCode_ConnectFail                 = 3,    // ����ʧ��
	eSockErrCode_SendBufOverflow             = 4,    // ���ͻ��������
	eSockErrCode_PostRecvFail                = 5,    // post�첽���ղ���ʧ��
	eSockErrCode_PostSendFail                = 6,    // post�첽����ʧ��
	eSockErrCode_ReactorErrorEvent           = 7,    // ��Ӧ�������¼�
	eSockErrCode_RecvError                   = 8,    // �������ݴ���
	eSockErrCode_Max                                 // ��
};



// Summary:
//		sock��������Ϣ�ṹ��
struct SOCK_ERR_CODE
{
	e_SockErrCode   eSockErrCode;  // �Զ��������
	LPUINT32         dwParam;       // ���Ӳ��������㶨λ
	LPUINT32         dwErrno;       // ϵͳ�����루ע���Ƿ�����GetLastError()��WSAGetLastError()��

	SOCK_ERR_CODE(e_SockErrCode _eSockErrCode, LPUINT32 _dwParam, LPUINT32 _dwErrno)
	{
		eSockErrCode = _eSockErrCode;
		dwParam      = _dwParam;
		dwErrno      = _dwErrno;
	}

	const char* LPAPI GetDescription();
};



// Summary:
//		�ײ�socket�����װ�ӿ���
class DECLARE ILPSocker
{
public:

	// Summary:
	//		��
	virtual ~ILPSocker(){}

	// Summary:
	//		��ȡ�ײ�sock�ľ��
	// Return:
	//		���صײ�sock���
	virtual SOCKET LPAPI GetSock() = 0;

	// Summary:
	//		��ȡsock id
	// Return:
	//		����sock id
	virtual LPUINT32 LPAPI GetSockerId() = 0;

	// Summary:
	//		��ȡ�Զ����ӵ�ip
	virtual LPUINT32 LPAPI GetRemoteIp() = 0;

	// Summary:
	//		��ȡ�Զ����ӵ�ip�ַ���
	virtual const char* LPAPI GetRemoteIpStr() = 0;

	// Summary:
	//		��ȡ�Զ����ӵ�port
	virtual LPUINT16 LPAPI GetRemotePort() = 0;

	// Summary:
	//		��ȡ�������ӵ�ip
	virtual LPUINT32 LPAPI GetLocalIp() = 0;

	// Summary:
	//		��ȡ�������ӵ�ip�ַ���
	virtual const char* LPAPI GetLocalIpStr() = 0;

	// Summary:
	//		��ȡ�������ӵ�port
	virtual LPUINT16 LPAPI GetLocalPort() = 0;

	// Summary:
	//		�жϵ�ǰ��socket�Ƿ�������״̬
	// Return:
	//		TRUE-���ӣ�FALSE-�Ͽ�
	virtual BOOL LPAPI IsConnect() = 0;

	// Summary:
	//		�ж��Ƿ񱻶��ر�
	// Return:
	//		TRUE-�ǣ�FALSE-��
	virtual BOOL LPAPI IsPassiveClose() = 0;

	// Summary:
	//		��������
	// Input:
	//		pData������
	//		dwLen������
	virtual BOOL LPAPI Send(const char* pData, LPUINT32 dwLen) = 0;

	// Summary:
	//		�ر����ӣ������������رջ��Ǳ����رգ���ͳһ���ô˽ӿڹر��Ѿ�������socker
	virtual void LPAPI Close(SOCK_ERR_CODE stSockErrCode, BOOL bPassiveClose) = 0;
};




//end�������������ֿռ�
NS_LZPL_END

#endif