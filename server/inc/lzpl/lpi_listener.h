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
//		�������ӿ���
class DECLARE ILPListener
{
public:

	// Summary:
	//		��
	virtual ~ILPListener(){}

	// Summary:
	//		��ʼ����
	// Input:
	//		pcszIp: ip��ַ�ַ�������ʽ��192.168.1.1��
	//		dwPort: �����˿�
	//		bReUseAddr: �Ƿ��ظ����õ�ַ
	// Return:
	//		TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI Start(const char* pcszIp, UINT_32 dwPort, BOOL bReUseAddr) = 0;

	// Summary:
	//		ֹͣ����
	virtual void LPAPI Stop() = 0;

	// Summary:
	//		��ȡid
	virtual UINT_32 LPAPI GetId() = 0;

	// Summary:
	//		�ͷ�
	virtual void LPAPI Release() = 0;
};





//end�������������ֿռ�
NS_LZPL_END

#endif