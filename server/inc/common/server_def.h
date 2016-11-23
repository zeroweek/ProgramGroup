//****************************************************************************/
//    author: caihy
//    date: November 09, 2015
//    description: 
//
//****************************************************************************/
#ifndef _SERVER_DEF_H_
#define _SERVER_DEF_H_



// Summory:
//		服务器状态
enum eServerState
{
	eServerState_Invalid,
	eServerState_Initing,
	eServerState_SelfReady,
	eServerState_AllReady,
	eServerState_Closing,
	eServerState_Closed,
	eServerState_UnIniting,
	eServerState_UnInited,
	eServerState_Total
};



// Summory:
//		服务器类型
enum eServerType
{
	eServerType_Invalid,

	eServerType_Gd,
	eServerType_Ns,
	eServerType_Ls,
	eServerType_Gt,
	eServerType_Db,
	eServerType_Ms,
	eServerType_Gs,

	eServerType_Total
};







#endif