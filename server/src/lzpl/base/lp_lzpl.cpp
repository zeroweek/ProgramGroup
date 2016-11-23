#include "lp_lzpl.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



static void lpsGlobalAssert()
{
	LPASSERT(sizeof(INT_8) == 1);
	LPASSERT(sizeof(INT_16) == 2);
	LPASSERT(sizeof(INT_32) == 4);
	LPASSERT(sizeof(INT_64) == 8);
	LPASSERT(sizeof(UINT_8) == 1);
	LPASSERT(sizeof(UINT_16) == 2);
	LPASSERT(sizeof(UINT_32) == 4);
	LPASSERT(sizeof(UINT_64) == 8);
}

// Summary:
//   初始化LZPL默认日志控制对象
//   注：使用层应当在使用LZPL组件之前调用此日志模块初始化函数
// Input:
//  stLogConfig: 日志配置
// Return:
//		TRUE-成功，FALSE-失败
DECLARE BOOL LPAPI lpInitLzpl(LOG_CONFIG& stLogConfig)
{
	INT_32 nResult = 0;

	lpsGlobalAssert();

	nResult = LPTime::GlobalInit(stLogConfig.nTimezone);
	PRINTF_PROCESS_ERROR(nResult);

	nResult = lpInitLzplLoggerCtrl(stLogConfig);
	PRINTF_PROCESS_ERROR(nResult);

	return TRUE;

Exit0:
	return FALSE;
}


DECLARE void LPAPI lpUnInitLzpl()
{

}









//end声明所处的名字空间

NS_LZPL_END
