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
//      io����ö��
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
    LPUINT32                       dwRecvBufSize;               // ���ջ������Ĵ�С
    LPUINT32                       dwSendBufSize;               // ���ͻ������Ĵ�С
    LPUINT32                       dwConnectCount;              // �������ӵ�����
    LPUINT32                       dwNetEventPoolSize;          // �����¼��ش�С
    LPUINT32                       dwNetEventListCount;         // �����¼������б����

};

#pragma pack(pop)
// tolua_end



// Summary:
//      ��������ӿ���
class DECLARE ILPNet
{
public:

    // Summary:
    //      ��
    virtual ~ILPNet() {}

    // Summary:
    //      ����ILPListener��������������
    //Input:
    //      pPacketParser����������
    //Return:
    //      ����������
    virtual lp_shared_ptr<ILPListener> LPAPI CreateListenerCtrl(lp_shared_ptr<ILPPacketParser> pPacketParser) = 0;

    // Summary:
    //      ����ILPConnector����������
    //Input:
    //      pPacketParser����������
    //Return:
    //      ����������
    virtual lp_shared_ptr<ILPConnector> LPAPI CreateConnectorCtrl(lp_shared_ptr<ILPPacketParser> pPacketParser) = 0;

    // Summary:
    //      �������������
    //Input:
    //      nCount-ÿ�μ�⴦������������������-1��ʾ��������
    //Return:
    //      TRUE-������������ѱ�����FALSE-��ʣ��δ���������
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
    static lp_shared_ptr<ILPNet> LPAPI CreateNetModule(lp_shared_ptr<ILPNetMessageHandler> pNetMessageHandler, const NET_CONFIG& tNetConfig);



    // Summary:
    //  ɾ�������������
    static void LPAPI DeleteNetModule(lp_shared_ptr<ILPNet>& poNet);
};



//end�������������ֿռ�
NS_LZPL_END

#endif