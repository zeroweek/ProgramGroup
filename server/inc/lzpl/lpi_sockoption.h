//****************************************************************************/
//    author: caihy
//    date: October 21, 2014
//    description: 网络选项接口及信息结构定义
//
//****************************************************************************/
#ifndef _LPI_SOCK_OPTION_H_
#define _LPI_SOCK_OPTION_H_

#include "lp_base.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		socket选项信息结构
struct SOCK_OPTION
{
	LPINT32        nLevel;
	LPINT32        nOptName;
	LPINT32        nOptLen;
	const char*  pszOptValue;
};



// Summary:
//		网络选项接口类
class DECLARE ILPNetOption
{
public:

	// Summary:
	//		无
	virtual ~ILPNetOption(){}
};




//end声明所处的名字空间
NS_LZPL_END

#endif