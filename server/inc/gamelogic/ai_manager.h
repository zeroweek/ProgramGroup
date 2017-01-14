//****************************************************************************/
//    author: caihy
//    date: December 3, 2015
//    description: 
//
//****************************************************************************/
#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_

#include "ai_data.h"
#include "bt_event.h"

using namespace LZPL;



struct AI_TEMPLATE
{
	LPLuaScript*      poScript;
	BT_NODE*          pTreeList[btetAIEnd - btetAIBegin];
	LPUINT32           dwScriptCRC;
	LPUINT32           dwDataCRC;

};

class CAIManager
{
	SINGLETON_DECLARE(CAIManager)

public:

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL Reload(void);

	inline const AI_TEMPLATE* GetTemplate(LPINT32 nAIID);

private:

	struct CLoadAIData
	{
		LPINT32    m_nFailCount;
		LPINT32    m_nDataIndex;
		BOOL operator()(const AI_DATA* pAIData);
	};

private:

	CAIManager();
	~CAIManager();

private:

	AI_TEMPLATE           m_TemplateData[2][AI_MAX_ID];
	LPINT32                   m_nCurUsingTemplateDataIndex;
};

inline const AI_TEMPLATE* CAIManager::GetTemplate(LPINT32 nAIID)
{
	return NULL;
}






#endif