//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description: 
//
//        反应器ILPReactor接口声明。
//
//****************************************************************************/
#ifndef _LPI_REACTOR_H_
#define _LPI_REACTOR_H_

#include "lpi_socker.h"
#include "lpi_eventhandler.h"
#include "lp_netdef.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary：
//     反应器，与ILPEventHandler交互，负责网络事件处理
class DECLARE ILPReactor
{
public:

	// Summary:
	//		无
	virtual ~ILPReactor(){}

	// Summary:
	//     注册ILPEventHandler
	// Returns:
	//     TRUE-成功，FALSE-失败
	virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler) = 0;

	// Summary：
	//     注销ILPEventHandler
	// Returns:
	//     TRUE-成功，FALSE-失败
	virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler) = 0;

public:

	static const char * LPAPI GetIoOptTypeName(e_IoOptType eType)
	{
		switch (eType)
		{
		case LZPL::eIoOptType_None:
			LOG_CHECK_ERROR(FALSE);
			return "eIoOptType_None";
			break;
		case LZPL::eIoOptType_Recv:
			return "eIoOptType_Recv";
			break;
		case LZPL::eIoOptType_Send:
			return "eIoOptType_Send";
			break;
		case LZPL::eIoOptType_Accept:
			return "eIoOptType_Accept";
			break;
		case LZPL::eIoOptType_Connect:
			return "eIoOptType_Connect";
			break;
		case LZPL::eIoOptType_Max:
			LOG_CHECK_ERROR(FALSE);
			return "eIoOptType_Max";
			break;
		default:
			LOG_PROCESS_ERROR(FALSE);
			break;
		}

	Exit0:
		return "*** unknow e_IoOptType ***";
	}
};



//end声明所处的名字空间
NS_LZPL_END

#endif