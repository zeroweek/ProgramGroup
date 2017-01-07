//****************************************************************************/
//    author: caihy
//    date: December 28, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_URL_CODE_H_
#define _LP_URL_CODE_H_

#include "lp_global.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



enum eUrlCodeType
{
	eUrlCode_None,
	eUrlCode_Standard,
	eUrlCode_Total
};



// Summary:
//		url编码
// Input:
//		eType: 控制哪些特殊字符需要编码，哪些不需要编码
DECLARE BOOL lpUrlEncode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eUrlCodeType eType);

// Summary:
//		url解码
// Input:
//		eType: 控制哪些特殊字符需要编码，哪些不需要编码
DECLARE BOOL LPAPI lpUrlDecode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eUrlCodeType eType);



//end声明所处的名字空间
NS_LZPL_END

#endif