//****************************************************************************/
//    author: caihy
//    date: December 21, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_HTTP_H_
#define _LP_HTTP_H_

#include "lp_base.h"
#include "lpi_socker.h"
#include "curl/curl.h"


//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		http���Ӷ�����
class DECLARE LPHttp
{
public:

	// Summary:
	//		��
	LPHttp();

	// Summary:
	//		��
	~LPHttp();

	// Summary:
	//		��ʼ��
	BOOL LPAPI Init(void);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

};












//end�������������ֿռ�
NS_LZPL_END

#endif