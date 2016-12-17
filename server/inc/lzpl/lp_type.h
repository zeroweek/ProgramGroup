//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 定义基本的全局数据类型
//
//****************************************************************************/
#ifndef _LP_TYPE_H_
#define _LP_TYPE_H_

#include "lp_def.h"
#include <stdint.h>


#ifndef INT_8
	typedef int8_t               INT_8;
#endif

#ifndef INT_16
	typedef int16_t              INT_16;
#endif

#ifndef INT_32
	typedef int32_t              INT_32;
#endif

#ifndef INT_64
	typedef int64_t              INT_64;
#endif

#ifndef UINT_8
	typedef uint8_t              UINT_8;
#endif

#ifndef UINT_16
	typedef uint16_t             UINT_16;
#endif

#ifndef UINT_32
	typedef uint32_t             UINT_32;
#endif

#ifndef UINT_64
	typedef uint64_t              UINT_64;
#endif

#ifndef FLOAT
	typedef float                FLOAT;
#endif

#ifndef DOUBLE
	typedef double               DOUBLE;
#endif

#ifndef BYTE
	typedef unsigned char BYTE;
#endif


#ifndef _WIN32
	typedef UINT_32              SOCKET;
	typedef INT_32               HANDLE
#endif



#ifdef _WIN32
	//#define FMT_I64 "%I64"
	//#define FMT_U64 "%I64u"
	//#define FMT_64X "%I64x"
	#define FMT_I64 "%lld"
	#define FMT_U64 "%llu"
	#define FMT_64X "%llx"
	#define FMT_FLT "%f"
	#define FMT_DBE "%lf"
	#define FMT_STR "%s"
#else
	#define FMT_I64 "%ll"
	#define FMT_U64 "%llu"
	#define FMT_64X "%llx"
	#define FMT_FLT "%f"
	#define FMT_DBE "%lf"
	#define FMT_STR "%s"
#endif




//begin声明所处的名字空间
NS_LZPL_BEGIN







//end声明所处的名字空间
NS_LZPL_END

#endif