//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: ��������ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_NET_H_
#define _LPI_NET_H_

#include "lpi_listener.h"
#include "lpi_connector.h"
#include "lpi_logger.h"
#include "lpi_packetparser.h"
#include "lpi_netmessagehandler.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		netģ�����ƶ���
const char LPNET_MODULENAME[] = "LPNet";



// Summary:
//		netģ��汾�Ŷ���
const MODULE_VERSION LPNET_MODULEVERSION = { 0, 1, 0, 1 };



// Summary:
//		io����ö��
enum e_IoType
{
	eIoType_None = 0,
	eIoType_CompletionPort = 1,
};



// tolua_begin
#pragma pack(push, 8)

// Summary:
//		
struct NET_CONFIG
{
	LPUINT32                       dwRecvBufSize;               // ���ջ������Ĵ�С
	LPUINT32                       dwSendBufSize;               // ���ͻ������Ĵ�С
	LPUINT32                       dwConnectCount;              // �������ӵ�����
	LPUINT32                       dwNetEventPoolSize;          // �����¼��ش�С
	LPUINT32                       dwNetEventListCount;         // �����¼������б����
	LPUINT32                       dwNetRecvEventBufSize;       // ��������¼���������С

};

#pragma pack(pop)
// tolua_end



// Summary:
//		��������ӿ���
class DECLARE ILPNet : public ILPBase
{
public:

	// Summary:
	//		��
	virtual ~ILPNet(){}

	// Summary:
	//		����ILPListener��������������
	//Input:
	//		eIoType������IO����
	//		pPacketParser����������
	//Return:
	//		����������
	virtual ILPListener* LPAPI CreateListenerCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		����ILPConnector����������
	//Input:
	//		eIoType������IO����
	//		pPacketParser����������
	//Return:
	//		����������
	virtual ILPConnector* LPAPI CreateConnectorCtrl(e_IoType eIoType, ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		�������������
	//Input:
	//		nCount-ÿ�μ�⴦������������������-1��ʾ��������
	//Return:
	//		TRUE-������������ѱ�����FALSE-��ʣ��δ���������
	virtual BOOL LPAPI Run(LPINT32 nCount = -1) = 0;

};



// Summary:
//		��ȡ�����������
// Return:
//		�ɹ������������ʧ�ܷ���NULL
DECLARE ILPNet* LPAPI lpCreateNetModule(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig);








//end�������������ֿռ�
NS_LZPL_END

#endif