//****************************************************************************/
//    author: caihy
//    date: September 18, 2015
//    description: 函数成功、错误返回处理
//
//****************************************************************************/
#ifndef _LP_PROCESSERROR_H_
#define _LP_PROCESSERROR_H_

#include "lpi_logger.h"
#include "lp_errorstring.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define PROCESS_ERROR(_condition_)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			goto Exit0;\
		}\
	}while(0)

#define PROCESS_SUCCESS(_condition_)\
	do\
	{\
		if(_condition_)\
		{\
			goto Exit1;\
		}\
	}while(0)

#define LOG_PROCESS_ERROR(_condition_)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			ERR("LOG_PROCESS_ERROR("#_condition_") at line %d in function %s() file \"%s\"",\
				__LINE__, __FUNCTION__, __FILE__);\
			goto Exit0;\
		}\
	}while(0)

#define LOG_PROCESS_ERROR_WITH_MSG(_condition_, _format_, ...)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			ERR("LOG_PROCESS_ERROR_WITH_MSG("#_condition_") "#_format_" at line %d in function %s() file \"%s\"",\
				##__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__);\
			goto Exit0;\
		}\
	}while(0)

#define LOG_CHECK_ERROR(_condition_)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			ERR("LOG_CHECK_ERROR("#_condition_") at line %d in function %s() file \"%s\"",\
				__LINE__, __FUNCTION__, __FILE__);\
		}\
	}while(0)

#define LOG_CHECK_ERROR_WITH_MSG(_condition_, _format_, ...)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			ERR("LOG_CHECK_ERROR_WITH_MSG("#_condition_") "#_format_" at line %d in function %s() file \"%s\"",\
				##__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__);\
		}\
	}while(0)

#define PROCESS_ERROR_RET_CODE(_condition_, _result_)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			nResult = _result_;\
			goto Exit0;\
		}\
	}while(0)

#define LOG_PROCESS_ERROR_RET_CODE(_condition_, _result_)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			nResult = _result_;\
			ERR("LOG_PROCESS_ERROR_RET_CODE("#_condition_") at line %d in function %s() file \"%s\"",\
				__LINE__, __FUNCTION__, __FILE__);\
			goto Exit0;\
		}\
	}while(0)

#define LOG_PROCESS_ERROR_RET_CODE_WITH_MSG(_condition_, _result_, _format_, ...)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			nResult = _result_;\
			ERR("LOG_PROCESS_ERROR_RET_CODE_WITH_MSG("#_condition_") "#_format_" at line %d in function %s() file \"%s\"",\
				##__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__);\
			goto Exit0;\
		}\
	}while(0)

#define PRINTF_PROCESS_ERROR(_condition_)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			lpSetConsoleColor(CTC_RED);\
			LPPRINTF("PRINTF_PROCESS_ERROR("#_condition_") at line %d in function %s() file \"%s\"\n",\
				__LINE__, __FUNCTION__, __FILE__);\
			lpSetConsoleColor();\
			goto Exit0;\
		}\
	}while(0)

#define PRINTF_PROCESS_ERROR_WITH_MSG(_condition_, _format_, ...)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			lpSetConsoleColor(CTC_RED);\
			LPPRINTF("PRINTF_PROCESS_ERROR_WITH_MSG("#_condition_") "#_format_" at line %d in function %s() file \"%s\"\n",\
				##__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__);\
			lpSetConsoleColor();\
			goto Exit0;\
		}\
	}while(0)

#define PRINTF_CHECK_ERROR(_condition_)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			lpSetConsoleColor(CTC_RED);\
			LPPRINTF("PRINTF_CHECK_ERROR("#_condition_") at line %d in function %s() file \"%s\"\n",\
				__LINE__, __FUNCTION__, __FILE__);\
			lpSetConsoleColor();\
		}\
	}while(0)

#define PRINTF_CHECK_ERROR_WITH_MSG(_condition_, _format_, ...)\
	do\
	{\
		if(_condition_)\
		{\
			\
		}\
		else\
		{\
			lpSetConsoleColor(CTC_RED);\
			LPPRINTF("PRINTF_CHECK_ERROR_WITH_MSG("#_condition_") "#_format_" at line %d in function %s() file \"%s\"\n",\
				##__VA_ARGS__, __LINE__, __FUNCTION__, __FILE__);\
			lpSetConsoleColor();\
		}\
	}while(0)



//end声明所处的名字空间
NS_LZPL_END

#endif