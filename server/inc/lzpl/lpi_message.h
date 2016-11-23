//****************************************************************************/
//    author: caihy
//    date: October 19, 2015
//    description: ��Ϣ�ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_MESSAGE_H_
#define _LPI_MESSAGE_H_

#include "lpi_messageserializer.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		��Ϣ�ӿ���
class DECLARE ILPMessage
{
public:

	// Summary:
	//		��
	virtual ~ILPMessage() {}

public:

	virtual BOOL LPAPI Serialize(ILPMessageSerializer* pMessageSerializer) = 0;
	virtual BOOL LPAPI UnSerialize(ILPMessageSerializer* pMessageSerializer) = 0;

};







//end�������������ֿռ�
NS_LZPL_END

#endif