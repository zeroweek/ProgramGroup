//****************************************************************************/
//    author: caihy
//    date: September 18, 2015
//    description: 性能分析
//
//****************************************************************************/
#ifndef _LP_PROFILE_H_
#define _LP_PROFILE_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary;
//		none
DECLARE void LPAPI lpProfileStart(void);

// Summary;
//		none
DECLARE void LPAPI lpProfilePoint(LPINT32 nPid, LPINT32 nLine, const char* pcszMsg);

// Summary;
//		none
DECLARE void LPAPI lpProfileEnd(void);

#ifdef _WIN32
	#define PROFILE_START()\
		do\
		{\
			lpProfileStart();\
		}while(0)\

	#define PROFILE_POINT(_pid)\
		do\
		{\
			lpProfilePoint(_pid, __LINE__, \
				"[PROFILE %2d] %10I64u @ function "\
				__FUNCTION__\
				"() line %d file \""\
				__FILE__\
				"\"");\
		}while(0)

	#define PROFILE_END()\
		do\
		{\
			lpProfileEnd();\
		}while(0)

#else
	#define PROFILE_START
	#define PROFILE_POINT(_point_id)
	#define PROFILE_END
#endif



//end声明所处的名字空间
NS_LZPL_END

#endif