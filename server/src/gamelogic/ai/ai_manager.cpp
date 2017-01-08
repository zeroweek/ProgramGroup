#include "ai_manager.h"
#include "lp_processerror.h"
#include "zlib.h"
#include "string_def.h"
#include "bt_manager.h"



const AI_DATA* g_pstLoadingNewAIData = NULL;

SINGLETON_IMPLEMENT(CAIManager)

CAIManager::CAIManager()
{
	memset(m_TemplateData, 0, sizeof(m_TemplateData));
	m_nCurUsingTemplateDataIndex = 0;
}

CAIManager::~CAIManager()
{
	UnInit();
}

BOOL CAIManager::Init(void)
{
	LPINT32 nResult = 0;

	m_nCurUsingTemplateDataIndex = 0;
	memset(m_TemplateData, 0, sizeof(m_TemplateData));

	nResult = Reload();
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CAIManager::UnInit(void)
{
	return TRUE;
}

BOOL CAIManager::Reload(void)
{
	LPINT32 nResult = 0;
	LPUINT64 qwStartTick = 0;
	CLoadAIData oLoadAIData;

	qwStartTick = lpGetTickCountEx();

	oLoadAIData.m_nFailCount = 0;
	oLoadAIData.m_nDataIndex = (m_nCurUsingTemplateDataIndex + 1) % 2;

	nResult = LPDataManger<AI_DATA>::Instance().Traverse(oLoadAIData);
	LOG_PROCESS_ERROR(nResult);

	m_nCurUsingTemplateDataIndex = oLoadAIData.m_nDataIndex;

	INF("read %d / %d ai template successfully (cost %d ms)",
		LPDataManger<AI_DATA>::Instance().GetDataCount() - oLoadAIData.m_nFailCount,
		LPDataManger<AI_DATA>::Instance().GetDataCount(), lpGetTickCountEx() - qwStartTick);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CAIManager::CLoadAIData::operator()(const AI_DATA* pAIData)
{
	LPINT32 nResult = 0;
	BOOL bLoaded = FALSE;
	BOOL bNewScript = FALSE;
	AI_TEMPLATE* pAITemplate = NULL;
	AI_TEMPLATE* pOldAITemplate = NULL;
	char szFilePath[MAX_PATH];

	LOG_PROCESS_ERROR(pAIData);
	LOG_PROCESS_ERROR(pAIData->nAIID >= 0 && pAIData->nAIID < AI_MAX_ID);
	
	pAITemplate = &Instance().m_TemplateData[m_nDataIndex][pAIData->nAIID];
	pOldAITemplate = &Instance().m_TemplateData[(m_nDataIndex + 1) % 2][pAIData->nAIID];

	pAITemplate->dwDataCRC = crc32(0, (const Bytef*)pAIData, (uInt)sizeof(AI_DATA));
	g_pstLoadingNewAIData = pAIData;

	snprintf(szFilePath, MAX_PATH, "%s/%s/main.lua", SCRIPT_AI_FLODER_NAME, pAIData->szScriptName);

	pAITemplate->poScript = LPLuaScriptMgr::Instance().FindScript(szFilePath);
	if (pAITemplate->poScript == NULL)
	{
		pAITemplate->poScript = LPLuaScriptMgr::Instance().NewScript(szFilePath);
		LOG_PROCESS_ERROR(pAITemplate->poScript);

		bNewScript = TRUE;
	}

	pAITemplate->dwScriptCRC = pAITemplate->poScript->GetCRC();

	if (pOldAITemplate->dwScriptCRC == pAITemplate->dwScriptCRC &&
		pOldAITemplate->dwDataCRC == pAITemplate->dwDataCRC)
	{
		for (LPINT32 nIndex = btetAIBegin; nIndex < btetAIEnd; ++nIndex)
		{
			if (pAITemplate->pTreeList[nIndex - btetAIBegin] != NULL)
			{
				CBTManager::Instance().DestroyBTTree(pAITemplate->pTreeList[nIndex - btetAIBegin]);
				pAITemplate->pTreeList[nIndex - btetAIBegin] = NULL;
			}

			pAITemplate->pTreeList[nIndex - btetAIBegin] = CBTManager::Instance().CopyBTTree(pOldAITemplate->pTreeList[nIndex - btetAIBegin]);
		}

		bLoaded = TRUE;
	}

	if (!bLoaded)
	{
		if (!bNewScript)
		{
			nResult = pAITemplate->poScript->Execute();
			LOG_PROCESS_ERROR(nResult);
		}

		for (LPINT32 nIndex = btetAIBegin; nIndex < btetAIEnd; ++nIndex)
		{
			if (pAITemplate->pTreeList[nIndex - btetAIBegin] != NULL)
			{
				CBTManager::Instance().DestroyBTTree(pAITemplate->pTreeList[nIndex - btetAIBegin]);
				pAITemplate->pTreeList[nIndex - btetAIBegin] = NULL;
			}

			if (pAITemplate->poScript->IsTableExist(g_BTEventName[nIndex]))
			{
				nResult = pAITemplate->poScript->CallFunction(LOAD_BT_TREE, "s:o", g_BTEventName[nIndex], &pAITemplate->pTreeList[nIndex - btetAIBegin]);
				if (FALSE == nResult || NULL == pAITemplate->pTreeList[nIndex - btetAIBegin])
				{
					FTL("AIID=%d, script=%s, tree=%s exist but load nothing", pAIData->nAIID, pAIData->szScriptName, g_BTEventName[nIndex]);
				}
				CBTManager::Instance().PrintBTTree(pAITemplate->pTreeList[nIndex - btetAIBegin], 0);
			}
		}
	}

	g_pstLoadingNewAIData = NULL;

	return TRUE;

Exit0:

	if (pAIData)
	{
		FTL("fail to generate ai %d template data", pAIData->nAIID);
	}

	m_nFailCount++;

	return TRUE;
}
