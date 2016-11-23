//****************************************************************************/
//    author: caihy
//    date: November 14, 2015
//    description: 
//
//****************************************************************************/
#ifndef _BT_MANAGER_H_
#define _BT_MANAGER_H_

#include "lp_lzpl.h"

using namespace LZPL;


#define BT_ACTION_NAME_LEN                 (128)

#define CREATE_NODE(_parent_, _type_, _param0_, _param1_, _param2_, _param3_) \
	CBTManager::Instance().CreateBTNode(_parent_, _type_, 0, #_type_, _param0_, _param1_, _param2_, _param3_, 0)

#define CREATE_NODE_WITH_TAG(_parent_, _type_, _tag_, _param0_, _param1_, _param2_, _param3_) \
	CBTManager::Instance().CreateBTNode(_parent_, _type_, _tag_, #_type_, _param0_, _param1_, _param2_, _param3_, 0)


class CSceneObject;
class CCharacter;
class CNpc;
class CScene;

enum eBTOwnerType
{
	btotInvalid,

	btotSceneObject,
	btotCharacter,
	btotNpc,
	btotScene,

	btotTotal,

	btotSceneObjectBit = 1 << btotSceneObject,
	btotCharacterBit = 1 << btotCharacter,
	btotNpcBit = 1 << btotNpc,
	btotSceneBit = 1 << btotScene,

	btotAll = btotSceneObjectBit | btotCharacterBit | btotNpcBit | btotSceneBit
};

enum eBTNodeTag
{
	btntgInvalid,

	btntgHead,

	btntgTotal
};

enum eBTRunningType
{
	btrtInvalid,

	btrtWait,

	btrtTotal
};

#pragma pack(push, 1)

struct BT_CUSTOM_CTRL : BT_CTRL
{
	unsigned int   eEventType     : 6;
	unsigned int   eOwnerType     : 2;
	unsigned int   eRunningType   : 8;
	unsigned int   uReserved      : 16;
	INT_32         nWaitFrame;
	UINT_32        dwRoleId;

	union
	{
		void*             poOwner;
		CSceneObject*     poSceneObject;
		CCharacter*       poCharacter;
		CNpc*             poNpc;
		CScene*           poScene;
	};

	LPLuaScript*       poScript;
	BASE_LIST_NODE     ListNode;
};

#pragma pack(pop)

typedef INT_32(*pfunBtActionFunc)(BT_CUSTOM_CTRL* pCtrl, INT_32 nParam[]);

class CBTManager
{
	SINGLETON_DECLARE(CBTManager)

public:

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL Reload(void);

	BT_NODE* CreateBTNode(BT_NODE* pParent, INT_32 nType, INT_32 nTag, const char* pcszName,
		INT_32 nParam0, INT_32 nParam1, INT_32 nParam2, INT_32 nParam3, INT_32 nFlag);
	BOOL DestroyBTTree(BT_NODE* pRoot, INT_32 nLayer = 0);
	BT_NODE* CopyBTTree(BT_NODE* pRoot);
	void PrintBTTree(BT_NODE* pRoot, INT_32 nLayer);

	INT_32 FindBTActionByName(const char* pcszName);
	INT_32 RegisterBTAction(const char* pcszName);
	const char* GetBTActionName(INT_32 nActionId);

	BOOL CallBTAction(INT_32 nBTNodeType, INT_32 nParam[]);
	BOOL TestBTAction(void* pObj, INT_32 nBTNodeType, INT_32 nParam[]);

	UINT_32 GetCtrlRoleId();

private:

	CBTManager();
	~CBTManager();

	static INT_32 _BTNodeFunc(BT_CTRL* pCtrl, BT_NODE* pNode);

private:

	struct ACTION_INFO
	{
		INT_32               nActionId;
		char                 szName[BT_ACTION_NAME_LEN];
		pfunBtActionFunc     pfnCallback;
		INT_16               eOwnerMask;
		INT_16               bIsLuaFunc;
	};
	static ACTION_INFO           m_ActionInfoList[btntTotal];

	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strncmp(s1, s2, MAX_FILE_NAME) < 0;
		}
	};
	typedef std::map<const char*, ACTION_INFO*, ltstr>  MAP_ACTION_NAME_2_INFO;
	MAP_ACTION_NAME_2_INFO          m_mapActionName2Info;

	INT_32                          m_nActionIdGenerator;
	BT_CUSTOM_CTRL*              m_pCurrentCtrl;
};



#endif