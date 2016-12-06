//****************************************************************************/
//    author: caihy
//    date: December 28, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_GLOBAL_H_
#define _LP_GLOBAL_H_

#include "lp_base.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN



//	Summary:
//		将数字转换为对应的16进制字符
DECLARE UINT_8 LPAPI lpNumToHex(UINT_8 n, BOOL bCapital);


//	Summary:
//		将16进制字符转换为对应的数字
DECLARE UINT_8 LPAPI lpHexToNum(UINT_8 h);



//	Summary:
//		Swaps the values of the two parameters.
template<class T> inline DECLARE void LPAPI lpSwap(T& a, T& b) { T t = a; a = b; b = t; }

//	Summary:
//		Returns the minimum of two values.
template<class T> inline DECLARE T LPAPI lpMin(T a, T b) { return a < b ? a : b; }

//	Summary:
//		Returns the maximum of two values.
template<class T> inline DECLARE T LPAPI lpMax(T a, T b) { return a > b ? a : b; }

//	Summary:
//		Returns the absolute value.
template<class T> inline DECLARE T LPAPI lpAbs(T a) { return a < 0 ? -a : a; }

//	Summary:
//		Returns the square of the value.
template<class T> inline DECLARE T LPAPI lpSqr(T a) { return a*a; }

//	Summary:
//		Returns the square root of the value.
inline DECLARE float LPAPI lpSqrt(float x) { return sqrtf(x); }

//	Summary:
//		Clamps the value to the specified range.
template<class T> inline DECLARE T LPAPI lpClamp(T v, T _min, T _max) { return v < _min ? _min : (v > _max ? _max : v); }


inline BOOL lpIsZeroFloat(const FLOAT fValue, FLOAT epsilon = 1e-6)
{
	return std::abs(fValue) <= epsilon ? TRUE : FALSE;
}

inline BOOL lpIsZeroDouble(const DOUBLE dValue, DOUBLE epsilon = 1e-15)
{
	return std::abs(dValue) <= epsilon ? TRUE : FALSE;
}









//end声明所处的名字空间
NS_LZPL_END

#endif