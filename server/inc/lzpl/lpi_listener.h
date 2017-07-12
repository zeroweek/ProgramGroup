//****************************************************************************/
//    author: caihy
//    date: May 29, 2015
//    description: �������ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_LISTENER_H_
#define _LPI_LISTENER_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      �������ӿ���
class DECLARE ILPListener
{
public:

    virtual ~ILPListener() {}

    // Summary:
    //   ��ʼ����
    // Input:
    //   strIP: ip��ַ�ַ�������ʽ��192.168.1.1��"0"��ʾ�����ַ��
    //   dwPort: �����˿�
    //   bReUseAddr: �Ƿ��ظ����õ�ַ
    // Return:
    //   TRUE-�ɹ���FALSE-ʧ��
    virtual BOOL LPAPI Start(const std::string& strIP, LPUINT32 dwPort, BOOL bReUseAddr) = 0;

    // Summary:
    //      ֹͣ����
    virtual void LPAPI Close() = 0;

    // Summary:
    //      ��ȡid
    virtual LPUINT32 LPAPI GetId() = 0;

    // Summary:
    //      ��ȡip�ַ���
    virtual std::string& LPAPI GetIp() = 0;

    // Summary:
    //      ��ȡport
    virtual LPUINT16 LPAPI GetPort() = 0;
};



//end�������������ֿռ�
NS_LZPL_END

#endif