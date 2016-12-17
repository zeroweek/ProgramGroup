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


#ifndef LPINT8
	typedef int8_t               LPINT8;
#endif

#ifndef LPINT16
	typedef int16_t              LPINT16;
#endif

#ifndef LPINT32
	typedef int32_t              LPINT32;
#endif

#ifndef LPINT64
	typedef int64_t              LPINT64;
#endif

#ifndef LPUINT8
	typedef uint8_t              LPUINT8;
#endif

#ifndef LPUINT16
	typedef uint16_t             LPUINT16;
#endif

#ifndef LPUINT32
	typedef uint32_t             LPUINT32;
#endif

#ifndef LPUINT64
	typedef uint64_t              LPUINT64;
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
	typedef LPUINT32              SOCKET;
	typedef LPINT32               HANDLE
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