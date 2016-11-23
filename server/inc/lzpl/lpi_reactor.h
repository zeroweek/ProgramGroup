//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description: 
//
//        ��Ӧ��ILPReactor�ӿ�������
//
//****************************************************************************/
#ifndef _LPI_REACTOR_H_
#define _LPI_REACTOR_H_

#include "lpi_socker.h"
#include "lpi_eventhandler.h"

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		io��������ö��
enum e_IoOptType
{
	eIoOptType_None = 0,
	eIoOptType_Recv,
	eIoOptType_Send,
	eIoOptType_Accept,
	eIoOptType_Connect,
	eIoOptType_Max,
};


// Summary:
//		��
DECLARE const char * LPAPI lpGetIoOptTypeName(e_IoOptType eType);



// Summary��
//     ��Ӧ������ILPEventHandler���������������¼�����
class DECLARE ILPReactor
{
public:

	// Summary:
	//		��
	virtual ~ILPReactor(){}

	// Summary:
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler) = 0;

	// Summary��
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler) = 0;
};




//end�������������ֿռ�
NS_LZPL_END

#endif