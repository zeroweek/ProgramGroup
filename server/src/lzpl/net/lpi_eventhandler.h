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
#include "lp_netdef.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary��
//     �¼�����������Ӧ���Ķ��⽻���ӿ�
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



//end�������������ֿռ�
NS_LZPL_END

#endif