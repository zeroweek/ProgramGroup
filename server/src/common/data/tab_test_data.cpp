#include "tab_test_data.h"

void TAB_TEST_DATA::Print(TAB_TEST_DATA* pData)
{
    LOG_PROCESS_ERROR(pData);

    LPPRINTF("%d\t%s\t%s\t%s\t\n", pData->nId, pData->szName, pData->szBeginTime, pData->szEndTime);

Exit0:
    return;
}

BOOL TAB_TEST_DATA::ReadLine(LPINT32 & rnId, TAB_TEST_DATA * pData, LPTabFile * pTabFile, LPINT32 nRow)
{
    READ_DATA_START();

    READ_INT_DATA("ID", nId);
    READ_STR_DATA("Name", szName);
    READ_STR_DATA("BeginTime", szBeginTime);
    READ_STR_DATA("EndTime", szEndTime);

    READ_DATA_END(nId);
}
