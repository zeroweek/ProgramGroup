//****************************************************************************/
//    author: caihy
//    date: October 17, 2014
//    description: �������ӿ��ඨ��
//
//****************************************************************************/
#ifndef _LPI_CONNECTOR_H_
#define _LPI_CONNECTOR_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      �������ӿ���
class DECLARE ILPConnector
{
public:

    // Summary:
    //      ��
    virtual ~ILPConnector() {}

    // Summary:
    //      ��ʼ����
    // Input:
    //      strIP: ip��ַ�ַ�������ʽ��192.168.1.1��
    //      dwPort: ���ӵĶ˿�
    //      bReconnect: �Ƿ��Զ����������û�������Զ���������Ҫ�ڶϿ��ص��ֶ����������ӿ�
    virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReconnect) = 0;

    // Summary:
    //      ֹͣ����
    // Return:
    //      TRUE-�ɹ���FALSE-ʧ��
    virtual void LPAPI Close() = 0;

    // Summary:
    //      ��ȡid
    virtual LPUINT32 LPAPI GetId() = 0;

    // Summary:
    //      ����
    virtual BOOL LPAPI Reconnect() = 0;

    // Summary:
    //      ��ȡ�Զ����ӵ�ip�ַ���
    virtual std::string& LPAPI GetIp() = 0;

    // Summary:
    //      ��ȡ�Զ����ӵ�port
    virtual LPUINT16 LPAPI GetPort() = 0;
};



//end�������������ֿռ�
NS_LZPL_END

#endif