//****************************************************************************/
//    author: caihy
//    date: November 14, 2015
//    description: 
//
//****************************************************************************/
#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "lp_lzpl.h"
#include "lp_bt.h"


using namespace LZPL;



// Summary:
//		нч
class DECLARE CGameLogic
{
	SINGLETON_DECLARE(CGameLogic)

public:

	BOOL LPAPI Init(void);
	BOOL LPAPI UnInit(void);

	BOOL LPAPI MainLoop(void);

	inline const char* GetConfigPath(void);

	inline BT_NODE* NewBTNode(void);
	inline BOOL DelBTNode(BT_NODE* pNode);

private:

	BOOL LPAPI _InitObjPool(void);
	BOOL LPAPI _InitScript(void);
	BOOL LPAPI _UnInitScript(void);
	BOOL LPAPI _InitSettingData(void);
	BOOL LPAPI _UnInitSettingData(void);

private:

	// Summary:
	//		none
	CGameLogic();

	// Summary:
	//		none
	virtual ~CGameLogic();

private:

	LPObjPool<BT_NODE>           m_oBTNodePool;

	char                            m_szConfigPath[MAX_PATH];
};



inline const char * CGameLogic::GetConfigPath(void)
{
	return m_szConfigPath;
}

inline BT_NODE* CGameLogic::NewBTNode(void)
{
	return m_oBTNodePool.Malloc();
}

inline BOOL CGameLogic::DelBTNode(BT_NODE* pNode)
{
	INT_32 nResult = 0;

	m_oBTNodePool.Free(pNode);

	return TRUE;
}













#endif