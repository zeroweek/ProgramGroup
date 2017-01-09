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
#include "lp_netdef.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary：
//     事件处理器，反应器的对外交互接口
class DECLARE ILPEventHandler
{
public:

	virtual ~ILPEventHandler(){}

	virtual HANDLE LPAPI GetHandle() = 0;

	virtual e_EventHandlerType LPAPI GetEventHandlerType() = 0;

	virtual void LPAPI OnNetEvent(BOOL bOperateRet, PER_IO_DATA* pstPerIoData) = 0;

public:

	static const char * LPAPI GetEventHandlerTypeName(e_EventHandlerType eType)
	{
		switch (eType)
		{
		case LZPL::eEventHandlerType_None:
			LOG_CHECK_ERROR(FALSE);
			return "eEventHandlerType_None";
			break;
		case LZPL::eEventHandlerType_Connector:
			return "eEventHandlerType_Connector";
			break;
		case LZPL::eEventHandlerType_Listener:
			return "eEventHandlerType_Listener";
			break;
		case LZPL::eEventHandlerType_Socker:
			return "eEventHandlerType_Socker";
			break;
		case LZPL::eEventHandlerType_Max:
			LOG_CHECK_ERROR(FALSE);
			return "eEventHandlerType_Max";
			break;
		default:
			LOG_PROCESS_ERROR(FALSE);
			break;
		}

	Exit0:
		return "*** Unknow e_EventHandlerType ***";
	}
};



//end声明所处的名字空间
NS_LZPL_END

#endif