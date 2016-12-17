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
//		�������ӿ���
class DECLARE ILPConnector
{
public:

	// Summary:
	//		��
	virtual ~ILPConnector(){}

	// Summary:
	//		��ʼ����
	// Input:
	//		pcszIp: ip��ַ�ַ�������ʽ��192.168.1.1��
	//		dwPort: ���ӵĶ˿�
	//		bReconnect: �Ƿ��Զ����������û�������Զ���������Ҫ�ڶϿ��ص��ֶ����������ӿ�
	virtual BOOL LPAPI Start(const char* pcszIp, LPUINT32 dwPort, BOOL bReconnect) = 0;

	// Summary:
	//		ֹͣ����
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual void LPAPI Stop() = 0;

	// Summary:
	//		��ȡid
	virtual LPUINT32 LPAPI GetId() = 0;

	// Summary:
	//		����
	virtual BOOL LPAPI Reconnect() = 0;

	// Summary:
	//		�ͷ�
	virtual void LPAPI Release() = 0;
};





//end�������������ֿռ�
NS_LZPL_END

#endif