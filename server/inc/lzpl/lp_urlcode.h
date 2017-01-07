//****************************************************************************/
//    author: caihy
//    date: December 28, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_URL_CODE_H_
#define _LP_URL_CODE_H_

#include "lp_global.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



enum eUrlCodeType
{
	eUrlCode_None,
	eUrlCode_Standard,
	eUrlCode_Total
};



// Summary:
//		url����
// Input:
//		eType: ������Щ�����ַ���Ҫ���룬��Щ����Ҫ����
DECLARE BOOL lpUrlEncode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eUrlCodeType eType);

// Summary:
//		url����
// Input:
//		eType: ������Щ�����ַ���Ҫ���룬��Щ����Ҫ����
DECLARE BOOL LPAPI lpUrlDecode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eUrlCodeType eType);



//end�������������ֿռ�
NS_LZPL_END

#endif