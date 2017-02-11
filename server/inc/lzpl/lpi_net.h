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
//		io����ö��
enum e_IoType
{
	eIoType_None = 0,
	eIoType_CompletionPort = 1,
	eIoType_Epoll = 2,
	eIoType_Total
};



// tolua_begin
#pragma pack(push, 8)

// Summary:
//		
struct NET_CONFIG
{
	LPUINT32                       dwIoType;                    // io����
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
class DECLARE ILPNet
{
public:

	// Summary:
	//		��
	virtual ~ILPNet(){}

	// Summary:
	//		����ILPListener��������������
	//Input:
	//		pPacketParser����������
	//Return:
	//		����������
	virtual std::shared_ptr<ILPListener> LPAPI CreateListenerCtrl(ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		����ILPConnector����������
	//Input:
	//		pPacketParser����������
	//Return:
	//		����������
	virtual std::shared_ptr<ILPConnector> LPAPI CreateConnectorCtrl(ILPPacketParser* pPacketParser) = 0;

	// Summary:
	//		�������������
	//Input:
	//		nCount-ÿ�μ�⴦������������������-1��ʾ��������
	//Return:
	//		TRUE-������������ѱ�����FALSE-��ʣ��δ���������
	virtual BOOL LPAPI Run(LPINT32 nCount = -1) = 0;

public:

	// Summary:
	//  �����ʼ���ӿڣ�ʹ������ģ��֮ǰ�������
	static BOOL LPAPI GlobalInit();

	// Summary:
	//  ���練��ʼ���ӿڣ�����ʹ������ģ��ʱ�������
	static void LPAPI GlobalUnInit();

	// Summary:
	//  ���������������
	static std::shared_ptr<ILPNet> LPAPI CreateNetModule(ILPNetMessageHandler* pNetMessageHandler, NET_CONFIG* pNetConfig);



	// Summary:
	//  ɾ�������������
	static void LPAPI DeleteNetModule(std::shared_ptr<ILPNet>& poNet);
};



//end�������������ֿռ�
NS_LZPL_END

#endif