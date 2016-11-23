//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description: ��Ӧ���¼��������ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_EVENT_HANDLER_H_
#define _LPI_EVENT_HANDLER_H_

#include "lp_base.h"
#include "lp_processerror.h"

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		�¼�����������
enum e_EventHandlerType
{
	eEventHandlerType_None         = 0,
	eEventHandlerType_Connector    = 1,
	eEventHandlerType_Listener     = 2,
	eEventHandlerType_Socker       = 3,
	eEventHandlerType_Max
};


// Summary:
//		��
DECLARE const char * LPAPI lpGetEventHandlerTypeName(e_EventHandlerType eType);



// Summary��
//     �¼�����������Ӧ���Ķ��⽻���ӿ�
class DECLARE ILPEventHandler
{
public:

	// Summary:
	//		��
	virtual ~ILPEventHandler(){}

	// Summary:
	//		��ȡ���
	virtual HANDLE LPAPI GetHandle() = 0;

	// Summary:
	//		��ȡ�¼�����������
	virtual e_EventHandlerType LPAPI GetEventHandlerType() = 0;
};




//end�������������ֿռ�
NS_LZPL_END

#endif