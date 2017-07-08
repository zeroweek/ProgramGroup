#include "ai_data.h"



void AI_DATA::Print(AI_DATA* pData)
{
    LOG_PROCESS_ERROR(pData);

    LPPRINTF("%d\t%s\t%s\t\n", pData->nAIID, pData->szAIDesc, pData->szScriptName);

Exit0:
    return;
}

BOOL AI_DATA::ReadLine(LPINT32 & rnId, AI_DATA * pData, LPTabFile * pTabFile, LPINT32 nRow)
{
    READ_DATA_START();

    READ_INT_DATA("AIID", nAIID);
    READ_STR_DATA("AIDesc", szAIDesc);
    READ_STR_DATA("ScriptName", szScriptName);

    READ_DATA_END(nAIID);
}
