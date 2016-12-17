#include "lpi_logger.h"
#include "lp_system.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



#define MAX_PROFILE_POINT_ID_COUNT		(16)
#define MAX_PROFILE_POINT_COUNT			(1024)


// Summary;
//		分析点结构体
struct PROFILE_POINT
{
	LPUINT64        qwTime;
	LPINT32         nLine;
	const char*    pcszMsg;
};

LPINT32 g_nProfileIndex[MAX_PROFILE_POINT_COUNT];
PROFILE_POINT g_stProfileTable[MAX_PROFILE_POINT_COUNT][MAX_PROFILE_POINT_ID_COUNT];

DECLARE void LPAPI lpProfileStart(void)
{
	for (LPINT32 nPid = 0; nPid < MAX_PROFILE_POINT_ID_COUNT; ++nPid)
	{
		g_nProfileIndex[nPid] = 0;
		for (LPINT32 nIndex = 0; nIndex < MAX_PROFILE_POINT_COUNT; ++nIndex)
		{
			g_stProfileTable[nPid][nIndex].qwTime = 0;
			g_stProfileTable[nPid][nIndex].nLine = 0;
			g_stProfileTable[nPid][nIndex].pcszMsg = NULL;
		}
	}
}

DECLARE void LPAPI lpProfilePoint(LPINT32 nPid, LPINT32 nLine, const char* pcszMsg)
{
	g_stProfileTable[nPid][g_nProfileIndex[nPid]].qwTime = lpRdtsc();
	g_stProfileTable[nPid][g_nProfileIndex[nPid]].nLine = nLine;
	g_stProfileTable[nPid][g_nProfileIndex[nPid]++].pcszMsg = pcszMsg;
}

DECLARE void LPAPI lpProfileEnd(void)
{
	for (LPINT32 nPid = 0; nPid < MAX_PROFILE_POINT_ID_COUNT; ++nPid)
	{
		if (g_nProfileIndex[nPid] > 0)
		{
			DBG("[PROFILE %2d]", nPid);
			DBG(g_stProfileTable[nPid][0].pcszMsg, nPid, 0LL, g_stProfileTable[nPid][0].nLine);
		}

		for (LPINT32 nIndex = 1; nIndex < g_nProfileIndex[nPid]; ++nIndex)
		{
			DBG(g_stProfileTable[nPid][0].pcszMsg, nPid,
				g_stProfileTable[nPid][nIndex].qwTime - g_stProfileTable[nPid][nIndex - 1].qwTime,
				g_stProfileTable[nPid][nIndex].nLine);
		}
	}
}

//end声明所处的名字空间
NS_LZPL_END
