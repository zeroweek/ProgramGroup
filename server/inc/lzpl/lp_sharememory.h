//****************************************************************************/
//    author: caihy
//    date: February 22, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LP_SHARE_MEMORY_H_
#define _LP_SHARE_MEMORY_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



#define SHARE_MEMORY_FLAG              (0x1234abcd)



struct SHARE_MEM_HEADER
{
	INT_32     nFlag;
	UINT_64    qwSize;
};



// Summary:
//		��Ϣ������
class DECLARE LPShareMemory
{
public:

	static BOOL IsExisting(const char* pcszName);

public:

	// Summary:
	//		��
	LPShareMemory();

	// Summary:
	//		��
	~LPShareMemory();

	// Summary:
	//		��ʼ��
	BOOL LPAPI Init(const char* pcszName, UINT_64 qwSize);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		�����ڴ�����
	const char* LPAPI Name(void);

	// Summary:
	//		�Ƿ����½���
	BOOL LPAPI IsNew(void);

	// Summary:
	//		�����ڴ��С
	UINT_64 LPAPI Size(void);

	// Summary:
	//		�����ڴ�
	char* LPAPI Mem(void);

	// Summary:
	//		����handle
	HANDLE LPAPI Handle(void);

private:

	char                     m_szName[COMMON_NAME_LEN];
	BOOL                     m_bNew;
	UINT_64                  m_qwSize;
	char*                    m_pszShareMem;
	HANDLE                   m_hHandle;
};













//end�������������ֿռ�
NS_LZPL_END

#endif