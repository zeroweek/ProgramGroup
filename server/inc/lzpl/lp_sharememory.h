//****************************************************************************/
//    author: caihy
//    date: February 22, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LP_SHARE_MEMORY_H_
#define _LP_SHARE_MEMORY_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define SHARE_MEMORY_FLAG              (0x1234abcd)



struct SHARE_MEM_HEADER
{
	INT_32     nFlag;
	UINT_64    qwSize;
};



// Summary:
//		消息解析类
class DECLARE LPShareMemory
{
public:

	static BOOL IsExisting(const char* pcszName);

public:

	// Summary:
	//		无
	LPShareMemory();

	// Summary:
	//		无
	~LPShareMemory();

	// Summary:
	//		初始化
	BOOL LPAPI Init(const char* pcszName, UINT_64 qwSize);

	// Summary:
	//		反始化
	BOOL LPAPI UnInit(void);

	// Summary:
	//		共享内存名字
	const char* LPAPI Name(void);

	// Summary:
	//		是否是新建的
	BOOL LPAPI IsNew(void);

	// Summary:
	//		共享内存大小
	UINT_64 LPAPI Size(void);

	// Summary:
	//		共享内存
	char* LPAPI Mem(void);

	// Summary:
	//		关联handle
	HANDLE LPAPI Handle(void);

private:

	char                     m_szName[COMMON_NAME_LEN];
	BOOL                     m_bNew;
	UINT_64                  m_qwSize;
	char*                    m_pszShareMem;
	HANDLE                   m_hHandle;
};













//end声明所处的名字空间
NS_LZPL_END

#endif