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


//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		http连接对象类
class DECLARE LPHttp
{
public:

	// Summary:
	//		无
	LPHttp();

	// Summary:
	//		无
	~LPHttp();

	// Summary:
	//		初始化
	BOOL LPAPI Init(void);

	// Summary:
	//		反始化
	BOOL LPAPI UnInit(void);

};












//end声明所处的名字空间
NS_LZPL_END

#endif