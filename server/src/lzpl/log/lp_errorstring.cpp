#include "lp_errorstring.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



DECLARE const char* lpGetErrorString(LPUINT32 dwErrorString)
{
	switch (dwErrorString)
	{
	case LZPL::eErrorString_Unknow:
		return "*** eErrorString_Unknow ***";
	case LZPL::eErrorString_Fail:
		return "ʧ��";
	case LZPL::eErrorString_Success:
		return "�ɹ�";
	case LZPL::eErrorString_Socket_AcceptFail:
		return "acceptʧ��";
	case LZPL::eErrorString_Socket_ConnectFail:
		return "connectʧ��";
	case LZPL::eErrorString_Socket_SendBufOverFlow:
		return "���ͻ��������";
	case LZPL::eErrorString_Socket_PostRecvFail:
		return "ִ���첽���ղ���ʧ��";
	case LZPL::eErrorString_Socket_PostSendFail:
		return "ִ���첽���Ͳ���ʧ��";
	case LZPL::eErrorString_Socket_ReactorErrorEvent:
		return "��Ӧ�������¼�";
	case LZPL::eErrorString_Socket_RecvError:
		return "�������ݴ���";
	case LZPL::eErrorString_Total:
		return "eErrorString_Total";
	default:
		break;
	}

	return "*** invalid error string ***";
}



//end�������������ֿռ�

NS_LZPL_END
