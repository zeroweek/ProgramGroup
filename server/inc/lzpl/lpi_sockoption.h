//****************************************************************************/
//    author: caihy
//    date: October 21, 2014
//    description: ����ѡ��ӿڼ���Ϣ�ṹ����
//
//****************************************************************************/
#ifndef _LPI_SOCK_OPTION_H_
#define _LPI_SOCK_OPTION_H_

#include "lp_base.h"

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		socketѡ����Ϣ�ṹ
struct SOCK_OPTION
{
	INT_32        nLevel;
	INT_32        nOptName;
	INT_32        nOptLen;
	const char*  pszOptValue;
};



// Summary:
//		����ѡ��ӿ���
class DECLARE ILPNetOption
{
public:

	// Summary:
	//		��
	virtual ~ILPNetOption(){}
};




//end�������������ֿռ�
NS_LZPL_END

#endif