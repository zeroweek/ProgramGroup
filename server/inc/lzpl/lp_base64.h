//****************************************************************************/
//    author: caihy
//    date: December 28, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_BASE64_H_
#define _LP_BASE64_H_

#include "lp_global.h"


//begin�������������ֿռ�
NS_LZPL_BEGIN


enum eBase64CodeType
{
	eBase64Code_None,
	eBase64Code_Standard,
	eBase64Code_Total
};



// Summary:
//		base64����
// Input:
//		eType: ����base64 table��β�����ַ��Լ�����ַ�������
DECLARE BOOL LPAPI lpBase64Encode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eBase64CodeType eType);

// Summary:
//		base64����
// Input:
//		eType: ����base64 table��β�����ַ��Լ�����ַ�������
DECLARE LPINT32 LPAPI lpBase64Decode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eBase64CodeType eType);







//end�������������ֿռ�
NS_LZPL_END

#endif