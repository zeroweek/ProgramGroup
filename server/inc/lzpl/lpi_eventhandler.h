//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description: 反应器事件处理器接口定义
//
//****************************************************************************/
#ifndef _LPI_EVENT_HANDLER_H_
#define _LPI_EVENT_HANDLER_H_

#include "lp_base.h"
#include "lp_processerror.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		事件处理器类型
enum e_EventHandlerType
{
	eEventHandlerType_None         = 0,
	eEventHandlerType_Connector    = 1,
	eEventHandlerType_Listener     = 2,
	eEventHandlerType_Socker       = 3,
	eEventHandlerType_Max
};


// Summary:
//		无
DECLARE const char * LPAPI lpGetEventHandlerTypeName(e_EventHandlerType eType);



// Summary：
//     事件处理器，反应器的对外交互接口
class DECLARE ILPEventHandler
{
public:

	// Summary:
	//		无
	virtual ~ILPEventHandler(){}

	// Summary:
	//		获取句柄
	virtual HANDLE LPAPI GetHandle() = 0;

	// Summary:
	//		获取事件处理器类型
	virtual e_EventHandlerType LPAPI GetEventHandlerType() = 0;
};




//end声明所处的名字空间
NS_LZPL_END

#endif