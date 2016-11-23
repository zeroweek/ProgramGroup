//****************************************************************************/
//    author: caihy
//    date: December 3, 2015
//    description: 
//
//****************************************************************************/
#ifndef _AIT_MANAGER_H_
#define _AIT_MANAGER_H_

#include "ai_data.h"
#include "bt_event.h"

using namespace LZPL;



struct AI_TEMPLATE
{
	LPLuaScript*      poScript;
	BT_NODE*          pTreeList[btetAIEnd - btetAIBegin];
	UINT_32           dwScriptCRC;
	UINT_32           dwDataCRC;

};

class CAIManager
{
	SINGLETON_DECLARE(CAIManager)

public:

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL Reload(void);

	inline const AI_TEMPLATE* GetTemplate(INT_32 nAIID);

private:

	struct CLoadAIData
	{
		INT_32    m_nFailCount;
		INT_32    m_nDataIndex;
		BOOL operator()(const AI_DATA* pAIData);
	};

private:

	CAIManager();
	~CAIManager();

private:

	AI_TEMPLATE           m_TemplateData[2][AI_MAX_ID];
	INT_32                   m_nCurUsingTemplateDataIndex;
};

inline const AI_TEMPLATE* CAIManager::GetTemplate(INT_32 nAIID)
{
	return NULL;
}






#endif