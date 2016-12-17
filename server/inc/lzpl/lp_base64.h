//****************************************************************************/
//    author: caihy
//    date: December 28, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_BASE64_H_
#define _LP_BASE64_H_

#include "lp_global.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN


enum eBase64CodeType
{
	eBase64Code_None,
	eBase64Code_Standard,
	eBase64Code_Total
};



// Summary:
//		base64编码
// Input:
//		eType: 控制base64 table结尾两个字符以及填充字符的类型
DECLARE BOOL LPAPI lpBase64Encode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eBase64CodeType eType);

// Summary:
//		base64解码
// Input:
//		eType: 控制base64 table结尾两个字符以及填充字符的类型
DECLARE LPINT32 LPAPI lpBase64Decode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eBase64CodeType eType);







//end声明所处的名字空间
NS_LZPL_END

#endif