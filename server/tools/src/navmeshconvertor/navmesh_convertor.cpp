#include "navmesh_convertor.h"
#include "lpi_file.h"
#include "lp_processerror.h"
#include "common.h"
#include "lp_dump.h"
#include "lp_filelist.h"



SINGLETON_IMPLEMENT(CNavMeshConvertor)

CNavMeshConvertor::CNavMeshConvertor()
{
	m_lstRawPolygonData.clear();
	m_mapPolyAlreadyFindNeighbor.clear();
	m_lstSepListPolygonData.clear();
	m_mapFindNeighborInSepListExclu.clear();
}

CNavMeshConvertor::~CNavMeshConvertor()
{
	UnInit();
}

BOOL CNavMeshConvertor::Init(void)
{
	INT_32 nResult = 0;

	nResult = m_oVertexs.Init(100);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oTrianglePoints.Init(100);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;

Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL CNavMeshConvertor::UnInit(void)
{
	INT_32 nResult = 0;

	_ClearUp();

	nResult = m_oVertexs.UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = m_oTrianglePoints.UnInit();
	LOG_CHECK_ERROR(nResult);

	return TRUE;
}

BOOL CNavMeshConvertor::_AddVertex(UINT_64 x, UINT_64 y, UINT_64 z)
{
	INT_32 nResult = 0;

	nResult = m_oVertexs.Push(x);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oVertexs.Push(y);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oVertexs.Push(z);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	return FALSE;
}

POLYGON_DATA* CNavMeshConvertor::_FindFirstNotAssignIDInRowList(void)
{
	POLYGON_DATA* pPolygonData = NULL;
	POLYGON_DATA* pResultPolyPolygonData = NULL;
	LST_POLYGON_DATA::iterator itr;

	for (itr = m_lstRawPolygonData.begin(); itr != m_lstRawPolygonData.end(); ++itr)
	{
		pPolygonData = (*itr);
		LOG_PROCESS_ERROR(pPolygonData);

		if (pPolygonData->nID == INVALID_POLYGON_ID)
		{
			pResultPolyPolygonData = pPolygonData;
			break;
		}
	}

Exit0:
	return pResultPolyPolygonData;
}

BOOL CNavMeshConvertor::_FindNeighborPolygonInRowList(lpIn POLYGON_DATA* pSrcPoly, lpOut POLYGON_DATA** ppNeighborPoly, lpOut UINT_32& dwNeighborPolyCount)
{
	INT_32 nResult = 0;
	INT_32 nEqualVertCount = 0;
	POLYGON_DATA* pPolygonData = NULL;
	LST_POLYGON_DATA::iterator itr;

	LOG_PROCESS_ERROR(pSrcPoly);
	LOG_PROCESS_ERROR(ppNeighborPoly);

	dwNeighborPolyCount = 0;

	for (itr = m_lstRawPolygonData.begin(); itr != m_lstRawPolygonData.end(); ++itr)
	{
		pPolygonData = (*itr);
		LOG_PROCESS_ERROR(pPolygonData);

		if (pPolygonData == pSrcPoly)
		{
			continue;
		}

		nEqualVertCount = 0;
		for (UINT_8 byIndexDst = 0; byIndexDst < pPolygonData->byVertCount; ++byIndexDst)
		{
			for (UINT_8 byIndexSrc = 0; byIndexSrc < pSrcPoly->byVertCount; ++byIndexSrc)
			{
				if (pPolygonData->anVertIndex[byIndexDst] == pSrcPoly->anVertIndex[byIndexSrc])
				{
					++nEqualVertCount;
				}
			}
		}

		LOG_PROCESS_ERROR(nEqualVertCount <= 2);
		if (nEqualVertCount == 2 && dwNeighborPolyCount < MAX_NEIGHBOR_PER_POLYGON)
		{
			ppNeighborPoly[dwNeighborPolyCount++] = pPolygonData;
		}

		LOG_PROCESS_ERROR(dwNeighborPolyCount <= MAX_NEIGHBOR_PER_POLYGON);
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CNavMeshConvertor::_FindNeighborPolygonInSepList(lpIn LPBaseList * pList, lpIn POLYGON_DATA * pSrcPoly, lpOut POLYGON_DATA ** ppNeighborPoly, lpOut UINT_32 & dwNeighborPolyCount)
{
	INT_32 nResult = 0;
	INT_32 nEqualVertCount = 0;
	POLYGON_DATA* pPolygonData = NULL;
	MAP_POLY_POLY::iterator fiter;

	LOG_PROCESS_ERROR(pList);
	LOG_PROCESS_ERROR(pSrcPoly);
	LOG_PROCESS_ERROR(ppNeighborPoly);

	dwNeighborPolyCount = 0;

	pPolygonData = (POLYGON_DATA*)pList->Head().pstNext;
	while (pPolygonData && pPolygonData != &pList->Rear())
	{
		if (pPolygonData == pSrcPoly)
		{
			pPolygonData = (POLYGON_DATA*)pPolygonData->pstNext;
			continue;
		}

		fiter = m_mapFindNeighborInSepListExclu.find(pPolygonData);
		if (fiter != m_mapFindNeighborInSepListExclu.end())
		{
			pPolygonData = (POLYGON_DATA*)pPolygonData->pstNext;
			continue;
		}

		nEqualVertCount = 0;
		for (UINT_8 byIndexDst = 0; byIndexDst < pPolygonData->byVertCount; ++byIndexDst)
		{
			for (UINT_8 byIndexSrc = 0; byIndexSrc < pSrcPoly->byVertCount; ++byIndexSrc)
			{
				if (pPolygonData->anVertIndex[byIndexDst] == pSrcPoly->anVertIndex[byIndexSrc])
				{
					++nEqualVertCount;
				}
			}
		}

		LOG_PROCESS_ERROR(nEqualVertCount <= 2);
		if (nEqualVertCount == 2 && dwNeighborPolyCount < MAX_NEIGHBOR_PER_POLYGON)
		{
			ppNeighborPoly[dwNeighborPolyCount++] = pPolygonData;
		}

		LOG_PROCESS_ERROR(dwNeighborPolyCount <= MAX_NEIGHBOR_PER_POLYGON);

		pPolygonData = (POLYGON_DATA*)pPolygonData->pstNext;
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CNavMeshConvertor::_SeparatePolysToLists(POLYGON_DATA* pPolyData, INT_32& nPolyIDGenerator)
{
	INT_32 nResult = 0;
	UINT_32 dwNeighborPolyCount = 0;
	POLYGON_DATA* pNeighborPoly[MAX_NEIGHBOR_PER_POLYGON];
	LPBaseList* pLstPolygonData = NULL;

	MAP_ID_2_ID::iterator fit;
	std::pair<MAP_ID_2_ID::iterator, bool> InsRet;
	BOOL bPolyAlreadyFindNeighbor = FALSE;

	LOG_PROCESS_ERROR(pPolyData);
	LOG_PROCESS_ERROR(pPolyData->byVertCount >= MIN_VERTEX_PER_POLYGON);

	memset(pNeighborPoly, 0, sizeof(pNeighborPoly));

	if (pPolyData->nID == INVALID_POLYGON_ID)
	{
		pPolyData->nID = ++nPolyIDGenerator;

		LOG_PROCESS_ERROR(!m_lstSepListPolygonData.empty());

		// 放到列表中去
		pLstPolygonData = &m_lstSepListPolygonData.back();
		LOG_PROCESS_ERROR(pLstPolygonData);
		pLstPolygonData->PushRear(pPolyData);
	}

	fit = m_mapPolyAlreadyFindNeighbor.find(pPolyData->nID);
	if (fit != m_mapPolyAlreadyFindNeighbor.end())
	{
		bPolyAlreadyFindNeighbor = TRUE;
	}

	PROCESS_SUCCESS(bPolyAlreadyFindNeighbor);

	InsRet = m_mapPolyAlreadyFindNeighbor.insert(make_pair(pPolyData->nID, pPolyData->nID));
	LOG_PROCESS_ERROR(InsRet.second);

	nResult = _FindNeighborPolygonInRowList(pPolyData, pNeighborPoly, dwNeighborPolyCount);
	LOG_PROCESS_ERROR(nResult);

	for (UINT_32 dwIndex = 0; dwIndex < dwNeighborPolyCount; ++dwIndex)
	{
		if (pNeighborPoly[dwIndex]->nID != INVALID_POLYGON_ID)
		{
			continue;
		}
		nResult = _SeparatePolysToLists(pNeighborPoly[dwIndex], nPolyIDGenerator);
		LOG_PROCESS_ERROR(nResult);
	}

Exit1:
	return TRUE;
Exit0:
	return FALSE;
}

BOOL CNavMeshConvertor::_DoPolysMerge(POLYGON_DATA* pFromPoly, POLYGON_DATA* pToPoly)
{
	INT_32 nResult = 0;
	INT_32 nEqualVertCount = 0;

	BOOL bRemove = FALSE;
	UINT_8 byTempIndex = 0;
	UINT_8 byFromBeMergeIndex = 0;
	UINT_8 abyFromEqualIndex[2] = { 0 };
	UINT_8 abyToEqualIndex[2] = { 0 };
	UINT_8 byRemoveCount = 0;
	UINT_8 abyRemoveIndex[MAX_VERTEX_PER_POLYGON] = { 0 }; 

	UINT_8 byMergeVertsCount = 0;
	INT_32 anMergeVerts[MAX_VERTEX_PER_POLYGON];   // 存储合并后的顶点
	UINT_8 byCalVertsCount = 0;
	INT_32 anCalVerts[MAX_VERTEX_PER_POLYGON * 2]; // 用于计算的顶点数组

	DOUBLE k, b; // 直线方程的斜率k和常数b
	DOUBLE dVertCalRet2, dVertCalRet3;
	INT_64 lVertCalRet, lVertCalRet1, lVertCalRet2, lVertCalRet3;
	INT_64 x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3;
	INT_64 v0x, v0y, v0z, v1x, v1y, v1z, v2x, v2y, v2z;

	LOG_PROCESS_ERROR(pFromPoly);
	LOG_PROCESS_ERROR(pToPoly);

	// 注意：在此需要保证即将被合并的pFromPoly都是三角形
	//
	// 若被合并的pFromPoly是三角形，则只需将不共边的那个点放在
	//	pToPoly中共边的两个顶点之间，即可保证合并后的顶点在pToPoly
	//	中是按顺时针或逆时针顺序存储的。
	LOG_PROCESS_ERROR(pFromPoly->byVertCount == 3);

	// 检测顶点是否满足合并条件
	PROCESS_ERROR(pFromPoly->byVertCount + pToPoly->byVertCount - 2 <= MAX_VERTEX_PER_POLYGON);
	PROCESS_ERROR(pFromPoly->byVertCount + pToPoly->byVertCount - 2 >= MIN_VERTEX_PER_POLYGON);
	for (UINT_8 byIndexTo = 0; byIndexTo < pToPoly->byVertCount; ++byIndexTo)
	{
		for (UINT_8 byIndexFrom = 0; byIndexFrom < pFromPoly->byVertCount; ++byIndexFrom)
		{
			if (pFromPoly->anVertIndex[byIndexFrom] == pToPoly->anVertIndex[byIndexTo])
			{
				LOG_PROCESS_ERROR(nEqualVertCount < 2);
				abyFromEqualIndex[nEqualVertCount] = byIndexFrom;
				abyToEqualIndex[nEqualVertCount] = byIndexTo;
				++nEqualVertCount;
			}
		}
	}
	PROCESS_ERROR(nEqualVertCount == 2);

	// 找出pFromPoly中要被合并的点，将其存储在pToPoly共边的两个顶点之间
	//	被合并的是三角形，只有三个索引下标：0、1、2（互斥的） 
	byFromBeMergeIndex = 0 + 1 + 2 - abyFromEqualIndex[0] - abyFromEqualIndex[1];
	LOG_PROCESS_ERROR(byFromBeMergeIndex >= 0 && byFromBeMergeIndex <= 2);
	if (abyToEqualIndex[0] > abyToEqualIndex[1])
	{
		byTempIndex = abyToEqualIndex[0];
		abyToEqualIndex[0] = abyToEqualIndex[1];
		abyToEqualIndex[1] = byTempIndex;
	}

	byMergeVertsCount = pToPoly->byVertCount;
	memcpy(anMergeVerts, pToPoly->anVertIndex, sizeof(pToPoly->anVertIndex[0]) * byMergeVertsCount);
	if (abyToEqualIndex[0] == 0 && abyToEqualIndex[1] == byMergeVertsCount - 1)
	{
		anMergeVerts[byMergeVertsCount++] = pFromPoly->anVertIndex[byFromBeMergeIndex];
	}
	else
	{
		for (UINT_8 byIndex = pToPoly->byVertCount; byIndex - 1 >= 0 && byIndex > abyToEqualIndex[1]; --byIndex)
		{
			anMergeVerts[byIndex] = anMergeVerts[byIndex - 1];
		}
		anMergeVerts[abyToEqualIndex[1]] = pFromPoly->anVertIndex[byFromBeMergeIndex];
		++byMergeVertsCount;
	}

	// 判断所有点共面
	for (UINT_8 byIndex = 0; byIndex + 3 < byMergeVertsCount; ++byIndex)
	{
		LOG_PROCESS_ERROR(anMergeVerts[byIndex + 0] * 3 + 2 < m_oVertexs.Size());
		LOG_PROCESS_ERROR(anMergeVerts[byIndex + 1] * 3 + 2 < m_oVertexs.Size());
		LOG_PROCESS_ERROR(anMergeVerts[byIndex + 2] * 3 + 2 < m_oVertexs.Size());
		LOG_PROCESS_ERROR(anMergeVerts[byIndex + 3] * 3 + 2 < m_oVertexs.Size());

		x0 = m_oVertexs[anMergeVerts[byIndex + 0] * 3 + 0];
		y0 = m_oVertexs[anMergeVerts[byIndex + 0] * 3 + 1];
		z0 = m_oVertexs[anMergeVerts[byIndex + 0] * 3 + 2];

		x1 = m_oVertexs[anMergeVerts[byIndex + 1] * 3 + 0];
		y1 = m_oVertexs[anMergeVerts[byIndex + 1] * 3 + 1];
		z1 = m_oVertexs[anMergeVerts[byIndex + 1] * 3 + 2];

		x2 = m_oVertexs[anMergeVerts[byIndex + 2] * 3 + 0];
		y2 = m_oVertexs[anMergeVerts[byIndex + 2] * 3 + 1];
		z2 = m_oVertexs[anMergeVerts[byIndex + 2] * 3 + 2];

		x3 = m_oVertexs[anMergeVerts[byIndex + 3] * 3 + 0];
		y3 = m_oVertexs[anMergeVerts[byIndex + 3] * 3 + 1];
		z3 = m_oVertexs[anMergeVerts[byIndex + 3] * 3 + 2];

		v0x = x0 - x3;
		v0y = y0 - y3;
		v0z = z0 - z3;

		v1x = x1 - x3;
		v1y = y1 - y3;
		v1z = z1 - z3;

		v2x = x2 - x3;
		v2y = y2 - y3;
		v2z = z2 - z3;

		lVertCalRet = v0x * v1y * v2z + v0y * v1z * v2x + v0z * v1x * v2y
			- v0z * v1y * v2x - v0y * v1x * v2z - v0x * v1z * v2y;
		//if (vertCalRet == 0.00001)
		//{
		//	LPPRINTF("vertCalRet=%lld\n", lVertCalRet);
		//}

		PROCESS_ERROR(lVertCalRet == 0);
	}

	// 去掉多余的点（一个边上超过两个顶点）
	byRemoveCount = 0;
	byCalVertsCount = byMergeVertsCount;
	memcpy(anCalVerts, anMergeVerts, sizeof(anCalVerts[0]) * byCalVertsCount);
	anCalVerts[byCalVertsCount++] = anCalVerts[0];
	anCalVerts[byCalVertsCount++] = anCalVerts[1];
	for (UINT_8 byIndex = 0; byIndex + 2 < byCalVertsCount; ++byIndex)
	{
		//设三维中的三点为P1,P2,P3;
		//P1到P2的向量为:V1=P2-P1；
		//P1到P3的向量为:V2=P3-P1;
		//如果P1,P2,P3在同一直线，则V1与V2的夹角为0或PI,所以
		//V1与V2的叉积的模为:|V1xV2|=0;
		//即有：
		//V1.Y*V2.Z-V1.Z*V2.Y=0,
		//V1.Z*V2.X-V1.X*V2.Z=0,
		//V1.X*V2.Y-V1.Y*V2.X=0.

		x0 = m_oVertexs[anCalVerts[byIndex + 0] * 3 + 0];
		y0 = m_oVertexs[anCalVerts[byIndex + 0] * 3 + 1];
		z0 = m_oVertexs[anCalVerts[byIndex + 0] * 3 + 2];

		x1 = m_oVertexs[anCalVerts[byIndex + 1] * 3 + 0];
		y1 = m_oVertexs[anCalVerts[byIndex + 1] * 3 + 1];
		z1 = m_oVertexs[anCalVerts[byIndex + 1] * 3 + 2];

		x2 = m_oVertexs[anCalVerts[byIndex + 2] * 3 + 0];
		y2 = m_oVertexs[anCalVerts[byIndex + 2] * 3 + 1];
		z2 = m_oVertexs[anCalVerts[byIndex + 2] * 3 + 2];

		v0x = x0 - x1;
		v0y = y0 - y1;
		v0z = z0 - z1;

		v1x = x2 - x1;
		v1y = y2 - y1;
		v1z = z2 - z1;

		lVertCalRet1 = v0y * v1z - v0z * v1y;
		lVertCalRet2 = v0z * v1x - v0x * v1z;
		lVertCalRet3 = v0x * v1y - v0y * v1x;

		//浮点数计算误差0.00005
		if (lVertCalRet1 == 0 && lVertCalRet2 == 0 && lVertCalRet3 == 0)
		{
			if (byIndex + 1 >= byMergeVertsCount)
			{
				abyRemoveIndex[byRemoveCount++] = byIndex + 1 - byMergeVertsCount;
			}
			else
			{
				abyRemoveIndex[byRemoveCount++] = byIndex + 1;
			}

			//LPPRINTF("lVertCalRet1=%lld\n", lVertCalRet1);
			//LPPRINTF("lVertCalRet2=%lld\n", lVertCalRet2);
			//LPPRINTF("lVertCalRet3=%lld\n", lVertCalRet3);
			//LPPRINTF("\n");
		}
	}

	if (byRemoveCount > 0)
	{
		byCalVertsCount = byMergeVertsCount;
		LOG_PROCESS_ERROR(byCalVertsCount - byRemoveCount >= MIN_VERTEX_PER_POLYGON);
		memcpy(anCalVerts, anMergeVerts, sizeof(anCalVerts[0]) * byCalVertsCount);
		byMergeVertsCount = 0;
		for (UINT_8 byIndex = 0; byIndex < byCalVertsCount; ++byIndex)
		{
			bRemove = FALSE;
			for (UINT_8 j = 0; j < byRemoveCount; ++j)
			{
				if (byIndex == abyRemoveIndex[j])
				{
					bRemove = TRUE;
					break;
				}
			}

			if (!bRemove)
			{
				anMergeVerts[byMergeVertsCount++] = anCalVerts[byIndex];
			}
		}
		LOG_PROCESS_ERROR(byMergeVertsCount == byCalVertsCount - byRemoveCount);
	}

	// 判断是否为凸多边形
	//	前面已经证明为共面，即可直接使用x、z平面的投影进行判断
	//	计算出直线方程的斜率k和常数b，判断所有点都在同一边
	if (byMergeVertsCount > 3)
	{
		byCalVertsCount = byMergeVertsCount;
		memcpy(anCalVerts, anMergeVerts, sizeof(anCalVerts[0]) * byCalVertsCount);
		anCalVerts[byCalVertsCount++] = anCalVerts[0];
		anCalVerts[byCalVertsCount++] = anCalVerts[1];
		anCalVerts[byCalVertsCount++] = anCalVerts[2];
		for (UINT_8 byIndex = 0; byIndex < byMergeVertsCount; ++byIndex)
		{
			x0 = m_oVertexs[anCalVerts[byIndex + 0] * 3 + 0];
			z0 = m_oVertexs[anCalVerts[byIndex + 0] * 3 + 2];
			x1 = m_oVertexs[anCalVerts[byIndex + 1] * 3 + 0];
			z1 = m_oVertexs[anCalVerts[byIndex + 1] * 3 + 2];
			x2 = m_oVertexs[anCalVerts[byIndex + 2] * 3 + 0];
			z2 = m_oVertexs[anCalVerts[byIndex + 2] * 3 + 2];
			if (x1 != x0)
			{
				k = (DOUBLE)(z1 - z0) / (DOUBLE)(x1 - x0);
				b = ((DOUBLE)(z0 + z1) - (DOUBLE)(x0 + x1) * k) / 2;
				dVertCalRet2 = k * x2 + b;
			}
			for (UINT_8 i = 3; i < byCalVertsCount; ++i)
			{
				x3 = m_oVertexs[anCalVerts[i] * 3 + 0];
				z3 = m_oVertexs[anCalVerts[i] * 3 + 2];

				if (x1 != x0)
				{
					dVertCalRet3 = k * x3 + b;
					if ((DOUBLE)z2 > dVertCalRet2 && (DOUBLE)z3 < dVertCalRet3 ||
						(DOUBLE)z2 < dVertCalRet2 && (DOUBLE)z3 > dVertCalRet3)
					{
						PROCESS_ERROR(FALSE);
					}
				}
				else
				{
					if (x2 > x1 && x3 < x1 || x2 < x1 && x3 > x1)
					{
						PROCESS_ERROR(FALSE);
					}
				}
			}
		}
	}
	//x0 = m_oVertexs[anCalVerts[0] * 3 + 0];
	//z0 = m_oVertexs[anCalVerts[0] * 3 + 2];
	//x1 = m_oVertexs[anCalVerts[1] * 3 + 0];
	//z1 = m_oVertexs[anCalVerts[1] * 3 + 2];
	//x2 = m_oVertexs[anCalVerts[2] * 3 + 0];
	//z2 = m_oVertexs[anCalVerts[2] * 3 + 2];
	//if (x1 != x0)
	//{
	//	k = (z1 - z0) / (x1 - x0);
	//	b = (z0 * x1 - z1 * x0) / (x1 - x0);
	//}
	//vertCalRet1 = k * x2 + b;
	//for (UINT_8 byIndex = 3; byIndex < byCalVertsCount; ++byIndex)
	//{
	//	x3 = m_oVertexs[anCalVerts[byIndex] * 3 + 0];
	//	z3 = m_oVertexs[anCalVerts[byIndex] * 3 + 2];

	//	if (x1 != x0)
	//	{
	//		vertCalRet2 = k * x3 + b;
	//		if (z2 > vertCalRet1 && z3 < vertCalRet2)
	//		{
	//			PROCESS_ERROR(FALSE);
	//		}
	//	}
	//	else
	//	{
	//		if (x2 > x1 && x3 < x1 || x2 < x1 && x3 > x1)
	//		{
	//			PROCESS_ERROR(FALSE);
	//		}
	//	}
	//}

	// 更新pToPoly为最终的顶点数据
	pToPoly->byVertCount = byMergeVertsCount;
	memcpy(pToPoly->anVertIndex, anMergeVerts, sizeof(pToPoly->anVertIndex[0]) * pToPoly->byVertCount);

	return TRUE;
Exit0:
	return FALSE;
}

UINT_64 CNavMeshConvertor::_F2UL(FLOAT f, UINT_8 byDecimalCount)
{
	UINT_64 qwResult = 0;

	for (UINT_8 i = 0; i < byDecimalCount; ++i)
	{
		f *= 10;
	}

	qwResult = (UINT_64)f;

	return qwResult;
}

BOOL CNavMeshConvertor::_Convert(const char* pcszOutputFileName)
{
	INT_32 nResult = 0;
	UINT_32 dwTotalPolyCount = 0;
	INT_32 nPolyIDGenerator = INVALID_POLYGON_ID;
	POLYGON_DATA* pPolyData = NULL;
	POLYGON_DATA* pNextPolyData = NULL;
	LST_LST_POLYGON_DATA::iterator llitr;
	LPBaseList* pBaseList = NULL;
	UINT_32 dwNeighborPolyCount = 0;
	POLYGON_DATA* pNeighborPoly[MAX_NEIGHBOR_PER_POLYGON];
	std::pair<MAP_POLY_POLY::iterator, bool> InsRet;

	DOUBLE x, y, z;

	ILPFile* pFile = NULL;
	INT_32 nLen = 0;
	char szBuf[512];

	LOG_PROCESS_ERROR(pcszOutputFileName);


	// 1. tri列表转换成poly列表
	for (INT_32 nIndex = 0; nIndex + 2 < m_oTrianglePoints.Size(); nIndex += 3)
	{
		pPolyData = new POLYGON_DATA();
		LOG_PROCESS_ERROR(pPolyData);

		memset(pPolyData, 0, sizeof(POLYGON_DATA));
		pPolyData->nID = INVALID_POLYGON_ID;
		pPolyData->byVertCount = 3;
		pPolyData->anVertIndex[0] = m_oTrianglePoints[nIndex + 0];
		pPolyData->anVertIndex[1] = m_oTrianglePoints[nIndex + 1];
		pPolyData->anVertIndex[2] = m_oTrianglePoints[nIndex + 2];

		m_lstRawPolygonData.push_back(pPolyData);
	}


	// 2. 分离poly，将相邻的poly分到相同的列表
	nPolyIDGenerator = INVALID_POLYGON_ID;
	pPolyData = _FindFirstNotAssignIDInRowList();
	LOG_PROCESS_ERROR(pPolyData);
	while (pPolyData)
	{
		// 创建新列表
		m_lstSepListPolygonData.push_back(LPBaseList());

		nResult = _SeparatePolysToLists(pPolyData, nPolyIDGenerator);
		LOG_PROCESS_ERROR(nResult);

		pPolyData = _FindFirstNotAssignIDInRowList();
	}

	dwTotalPolyCount = 0;
	for (llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	{
		dwTotalPolyCount += llitr->Size();
	}
	LOG_PROCESS_ERROR(dwTotalPolyCount == (UINT_32)m_lstRawPolygonData.size());


	// 3. 合并poly，相邻的poly尽可能多的合并（限制顶点数量之内）
	// 从第一个编号的tri为基础，逐一合并后面的tri，如果满足条件则归为同一个poly；
	// 当遇到不满足条件的tri时，合并操作结束，前面满足合并条件的tri归为一个
	// poly，接着接续选择下一个未被合并的tri进行下一个poly的合并操作
	//dwTotalPolyCount = 0;
	//for (llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	//{
	//	pBaseList = &(*llitr);
	//	if (pBaseList->Size() <= 1)
	//	{
	//		dwTotalPolyCount += pBaseList->Size();
	//		continue;
	//	}

	//	pPolyData = (POLYGON_DATA*)pBaseList->Head().pstNext;
	//	LOG_PROCESS_ERROR(pPolyData);
	//	pNextPolyData = (POLYGON_DATA*)pPolyData->pstNext;
	//	while (pNextPolyData && pNextPolyData != &pBaseList->Rear())
	//	{
	//		if (_DoPolysMerge(pNextPolyData, pPolyData))
	//		{
	//			nResult = pBaseList->Remove(pNextPolyData);
	//			LOG_PROCESS_ERROR(nResult);
	//		}
	//		else
	//		{
	//			pPolyData = pNextPolyData;
	//		}

	//		pNextPolyData = (POLYGON_DATA*)pPolyData->pstNext;
	//	}

	//	dwTotalPolyCount += pBaseList->Size();
	//}
	dwTotalPolyCount = 0;
	for (llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	{
		pBaseList = &(*llitr);
		if (pBaseList->Size() <= 1)
		{
			dwTotalPolyCount += pBaseList->Size();
			continue;
		}

		pPolyData = (POLYGON_DATA*)pBaseList->Head().pstNext;
		LOG_PROCESS_ERROR(pPolyData);
		pNextPolyData = (POLYGON_DATA*)pPolyData->pstNext;
		while (pNextPolyData && pNextPolyData != &pBaseList->Rear())
		{
			if (_DoPolysMerge(pNextPolyData, pPolyData))
			{
				nResult = pBaseList->Remove(pNextPolyData);
				LOG_PROCESS_ERROR(nResult);

				pNextPolyData = (POLYGON_DATA*)pPolyData->pstNext;
				continue;
			}

			nResult = _FindNeighborPolygonInSepList(pBaseList, pPolyData, pNeighborPoly, dwNeighborPolyCount);
			LOG_PROCESS_ERROR(nResult);
			while (dwNeighborPolyCount > 0)
			{
				for (UINT_32 dwIndex = 0; dwIndex < dwNeighborPolyCount; ++dwIndex)
				{
					if (pNeighborPoly[dwIndex]->byVertCount == 3 &&  _DoPolysMerge(pNeighborPoly[dwIndex], pPolyData))
					{
						nResult = pBaseList->Remove(pNeighborPoly[dwIndex]);
						LOG_PROCESS_ERROR(nResult);
					}
					else
					{
						InsRet = m_mapFindNeighborInSepListExclu.insert(make_pair(pNeighborPoly[dwIndex], pNeighborPoly[dwIndex]));
						LOG_PROCESS_ERROR(InsRet.second);
					}
				}

				nResult = _FindNeighborPolygonInSepList(pBaseList, pPolyData, pNeighborPoly, dwNeighborPolyCount);
				LOG_PROCESS_ERROR(nResult);
			}

			m_mapFindNeighborInSepListExclu.clear();

			pPolyData = pNextPolyData;
			pNextPolyData = (POLYGON_DATA*)pPolyData->pstNext;
		}

		dwTotalPolyCount += pBaseList->Size();
	}


	// 4. poly编号：对最终的poly进行编号
	nPolyIDGenerator = INVALID_POLYGON_ID;
	for (llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	{
		pBaseList = &(*llitr);
		pPolyData = (POLYGON_DATA*)pBaseList->Head().pstNext;
		while (pPolyData && pPolyData != &pBaseList->Rear())
		{
			pPolyData->nID = ++nPolyIDGenerator;
			pPolyData = (POLYGON_DATA*)pPolyData->pstNext;
		}

	}
	LOG_PROCESS_ERROR(dwTotalPolyCount == (UINT_32)(nPolyIDGenerator));


	// 5. 将最终的poly数据保存到文件
	pFile = lpFileOpen(pcszOutputFileName, "wb");
	LOG_PROCESS_ERROR(pFile);

	for (UINT_32 i = 0; i + 2 < m_oVertexs.Size(); i += 3)
	{
		x = (DOUBLE)m_oVertexs[i + 0];
		x /= DECIMAL_MULT;
		y = (DOUBLE)m_oVertexs[i + 1];
		y /= DECIMAL_MULT;
		z = (DOUBLE)m_oVertexs[i + 2];
		z /= DECIMAL_MULT;
		nLen = sprintf_s(szBuf, sizeof(szBuf) - 1, VERTEX_FMT, x, y, z);
		LOG_PROCESS_ERROR(nLen > 0);
		nResult = (UINT_32)pFile->Write(szBuf, (UINT_32)nLen);
		LOG_PROCESS_ERROR(nResult == nLen);
	}
	nResult = (UINT_32)pFile->Write("\r\n", 2);
	LOG_PROCESS_ERROR(nResult == 2);

	for (llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	{
		nLen = (INT_32)sizeof("a start a new area\r\n") - 1;
		nResult = (UINT_32)pFile->Write("a start a new area\r\n", (UINT_32)nLen);
		LOG_PROCESS_ERROR(nResult == nLen);

		pBaseList = &(*llitr);
		pPolyData = (POLYGON_DATA*)pBaseList->Head().pstNext;
		while (pPolyData && pPolyData != &pBaseList->Rear())
		{
			nLen = 0;
			nResult = sprintf_s(szBuf + nLen, sizeof(szBuf) - 1 - nLen, "f");
			LOG_PROCESS_ERROR(nResult > 0);
			nLen += nResult;
			for (UINT_8 i = 0; i < pPolyData->byVertCount; i++)
			{
				nResult = sprintf_s(szBuf + nLen, sizeof(szBuf) - 1 - nLen, " %d/%d/%d", pPolyData->anVertIndex[i] + 1, pPolyData->anVertIndex[i] + 1, pPolyData->anVertIndex[i] + 1);
				LOG_PROCESS_ERROR(nResult > 0);
				nLen += nResult;
			}
			nResult = sprintf_s(szBuf + nLen, sizeof(szBuf) - 1 - nLen, "\r\n");
			LOG_PROCESS_ERROR(nResult > 0);
			nLen += nResult;
			nResult = (UINT_32)pFile->Write(szBuf, (UINT_32)nLen);
			LOG_PROCESS_ERROR(nResult == nLen);

			pPolyData = (POLYGON_DATA*)pPolyData->pstNext;
		}

		nLen = (INT_32)sizeof("\r\n") - 1;
		nResult = (UINT_32)pFile->Write("\r\n", (UINT_32)nLen);
		LOG_PROCESS_ERROR(nResult == nLen);
	}

	pFile->Close();
	pFile = NULL;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CNavMeshConvertor::_AddTriangle(INT_32 a, INT_32 b, INT_32 c)
{
	INT_32 nResult = 0;

	nResult = m_oTrianglePoints.Push(a);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oTrianglePoints.Push(b);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oTrianglePoints.Push(c);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

static char* _ParseRow(char* pszBuf, char* pszBufEnd, char* pszRow, INT_32 nMaxRowLen)
{
	char c = 0;
	INT_32 nIndex = 0;
	BOOL bStart = true;
	BOOL bDone = false;

	LOG_PROCESS_ERROR(pszBuf);
	LOG_PROCESS_ERROR(pszBufEnd);
	LOG_PROCESS_ERROR(pszRow);

	while (!bDone && pszBuf < pszBufEnd)
	{
		c = *pszBuf;
		++pszBuf;

		switch (c)
		{
			// multirow
		case '\\':
			break;
		case '\n':
			if (bStart)
			{
				break;
			}
			else
			{
				bDone = true;
			}
			break;
		case '\r':
			break;
		case '\t':
		case ' ':
			if (bStart)
			{
				break;
			}
		default:
			bStart = false;
			pszRow[nIndex++] = c;
			if (nIndex >= nMaxRowLen - 1)
			{
				bDone = true;
			}
			break;
		}
	}
	pszRow[nIndex] = '\0';

Exit0:
	return pszBuf;
}

static INT_32 _ParseFace(char* pszRow, INT_32* pnFaceData, INT_32 nMaxFacePoint, INT_32 nVertCount)
{
	INT_32 nIndex = 0;
	INT_32 nVertIndex = 0;
	char* pStr = NULL;

	LOG_PROCESS_ERROR(pszRow);
	LOG_PROCESS_ERROR(pnFaceData);
	LOG_PROCESS_ERROR(nMaxFacePoint > 0);

	while (*pszRow != '\0')
	{
		// Skip initial white space
		while (*pszRow != '\0' && (*pszRow == ' ' || *pszRow == '\t'))
		{
			++pszRow;
		}

		pStr = pszRow;

		// Find vertex delimiter and terminated the string there for conversion.
		while (*pszRow != '\0' && *pszRow != ' ' && *pszRow != '\t')
		{
			if (*pszRow == '/')
			{
				*pszRow = '\0';
			}
			++pszRow;
		}
		if (*pStr == '\0')
		{
			continue;
		}

		nVertIndex = atoi(pStr);
		LOG_PROCESS_ERROR(nVertIndex >= 1);

		pnFaceData[nIndex++] = nVertIndex - 1;

		LOG_CHECK_ERROR(nIndex < nMaxFacePoint);
		PROCESS_SUCCESS(nIndex >= nMaxFacePoint);
	}

Exit1:
Exit0:
	return nIndex;
}

BOOL CNavMeshConvertor::StartConvert(int argc, char * argv[])
{
	INT_32 nResult = 0;
	BOOL bConvertAll = FALSE;
	FILE_LIST fileList;
	char szSrcPath[MAX_PATH];
	char szDstPath[MAX_PATH];

	if (argc < 2)
	{
		ShowUsage();
		LOG_PROCESS_ERROR(FALSE);
	}
	if (strcmp(argv[1], "-all") == 0)
	{
		bConvertAll = TRUE;
	}
	else
	{
		bConvertAll = FALSE;
		if (argc < 3)
		{
			ShowUsage();
			LOG_PROCESS_ERROR(FALSE);
		}
	}

	if (bConvertAll)
	{
		nResult = fileList.ScanDirectory("scene_src", ".obj", FALSE);
		LOG_PROCESS_ERROR(nResult);

		for (INT_32 i = 0; i < fileList.dwSize; ++i)
		{
			sprintf_s(szSrcPath, sizeof(szSrcPath) - 1, "scene_src/%s", fileList.szFiles[i]);
			lpPathFilter(szSrcPath, sizeof(szSrcPath));
			nResult = _Load(szSrcPath);
			if (!nResult)
			{
				LPPRINTF("*** load file %s fail\n", szSrcPath);
				ERR("*** load file %s fail", szSrcPath);
				LOG_PROCESS_ERROR(FALSE);
			}

			sprintf_s(szDstPath, sizeof(szDstPath) - 1, "scene_dst/cvt_%s", fileList.szFiles[i]);
			lpPathFilter(szDstPath, sizeof(szDstPath));
			nResult = _Convert(szDstPath);
			if (!nResult)
			{
				LPPRINTF("*** convert to file %s fail\n", szDstPath);
				ERR("*** convert to file %s fail", szDstPath);
				LOG_PROCESS_ERROR(FALSE);
			}
		}
	}
	else
	{
		sprintf_s(szSrcPath, sizeof(szSrcPath) - 1, "scene_src/%s", argv[1]);
		lpPathFilter(szSrcPath, sizeof(szSrcPath));
		nResult = _Load(szSrcPath);
		if (!nResult)
		{
			LPPRINTF("*** load file %s fail\n", szSrcPath);
			ERR("*** load file %s fail", szSrcPath);
			LOG_PROCESS_ERROR(FALSE);
		}

		sprintf_s(szDstPath, sizeof(szDstPath) - 1, "scene_dst/cvt_%s", argv[2]);
		lpPathFilter(szDstPath, sizeof(szDstPath));
		nResult = _Convert(szDstPath);
		if (!nResult)
		{
			LPPRINTF("*** convert to file %s fail\n", szDstPath);
			ERR("*** convert to file %s fail", szDstPath);
			LOG_PROCESS_ERROR(FALSE);
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

void CNavMeshConvertor::ShowUsage(void)
{
	LPPRINTF("\n");
	LPPRINTF("usage:\n");
	LPPRINTF("\t1. xxx -all\n");
	LPPRINTF("\t2. xxx src.obj dst.obj\n");
	LPPRINTF("\n");
}

void CNavMeshConvertor::_ClearUp(void)
{
	POLYGON_DATA* pPolygonData = NULL;
	LPBaseList* pBaseList = NULL;

	m_oVertexs.ReSize(0);
	m_oTrianglePoints.ReSize(0);
	m_mapPolyAlreadyFindNeighbor.clear();

	for (LST_POLYGON_DATA::iterator itr = m_lstRawPolygonData.begin();
			itr != m_lstRawPolygonData.end(); ++itr)
	{
		pPolygonData = (*itr);
		LOG_CHECK_ERROR(pPolygonData);

		SAFE_DELETE(pPolygonData);
	}
	m_lstRawPolygonData.clear();
	
	for (LST_LST_POLYGON_DATA::iterator llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	{
		pBaseList = &(*llitr);
		pBaseList->Clear();
	}
	m_lstSepListPolygonData.clear();
}

BOOL CNavMeshConvertor::_Load(const char* pcszInputFileName)
{
	INT_32 nResult = 0;

	UINT_32 dwFileSize = 0;
	ILPFile* pFile = NULL;
	char* pszFileData = NULL;

	char* pSrc = NULL;
	char* pSrcEnd = NULL;
	char szRow[512];

	FLOAT x = 0, y = 0, z = 0;

	INT_32 szFacePoint[MAX_VERTEX_PER_POLYGON], nVert = 0, a = 0, b = 0, c = 0;

	LOG_PROCESS_ERROR(pcszInputFileName);

	_ClearUp();

	pFile = lpFileOpen(pcszInputFileName, "rb");
	LOG_PROCESS_ERROR(pFile);

	dwFileSize = pFile->Size();
	LOG_PROCESS_ERROR(dwFileSize > 0);

	pszFileData = new char[dwFileSize];
	LOG_PROCESS_ERROR(pszFileData);
	memset(pszFileData, 0, dwFileSize);

	nResult = pFile->Read(pszFileData, dwFileSize);
	LOG_PROCESS_ERROR(nResult > 0);

	pFile->Close();
	pFile = NULL;

	pSrc = pszFileData;
	pSrcEnd = pszFileData + dwFileSize;
	while (pSrc < pSrcEnd)
	{
		// parse one szRow
		szRow[0] = '\0';
		pSrc = _ParseRow(pSrc, pSrcEnd, szRow, sizeof(szRow) / sizeof(char));

		// skip comments
		if (szRow[0] == '#') continue;

		// parse vertex pos
		if (szRow[0] == 'v' && szRow[1] != 'n' && szRow[1] != 't')
		{
			sscanf_s(szRow + 1, "%f %f %f", &x, &y, &z);
			nResult = _AddVertex(_F2UL(x), _F2UL(y), _F2UL(z));
			LOG_PROCESS_ERROR(nResult);
		}

		// parse faces
		if (szRow[0] == 'f')
		{
			nVert = _ParseFace(szRow + 1, szFacePoint, MAX_VERTEX_PER_POLYGON, m_oVertexs.Size());
			LOG_PROCESS_ERROR(nVert == 3);
			for (INT_32 i = 2; i < nVert; ++i)
			{
				a = szFacePoint[0];
				b = szFacePoint[i - 1];
				c = szFacePoint[i];
				LOG_PROCESS_ERROR(a >= 0 && a < m_oVertexs.Size() || b >= 0 && b < m_oVertexs.Size() || c >= 0 && c < m_oVertexs.Size());
				nResult = _AddTriangle(a, b, c);
				LOG_PROCESS_ERROR(nResult);
			}
		}
	}

	LOG_PROCESS_ERROR(m_oTrianglePoints.Size() % 3 == 0);

	SAFE_DELETE_SZ(pszFileData);

	return TRUE;
Exit0:

	if (pFile)
	{
		pFile->Close();
		pFile = NULL;
	}
	SAFE_DELETE_SZ(pszFileData);

	return FALSE;
}


int main(int argc, char* argv[])
{
	INT_32 nResult = 0;
	char szDir[MAX_PATH];
	BOOL bConvertAll = FALSE;

	RedefinedConsole();

	// 使用LZPL，必须在程序运行时先初始化
	lpStrCpyN(szDir, lpGetWorkingDir(), MAX_PATH);
	lpStrCatN(szDir, "\\log", MAX_PATH);
#ifdef _DEBUG
	nResult = lpLZPLInitLoggerCtrl(szDir, lpGetExeProcessName(), eLogMode_DayDivide, TRUE, NULL, eLogLevel_All, eOutputType_File | eOutputType_Debug | eOutputType_Console);
#else
	nResult = lpLZPLInitLoggerCtrl(szDir, lpGetExeProcessName(), eLogMode_DayDivide, TRUE, NULL, eLogLevel_All, eOutputType_File | eOutputType_Console);
#endif
	LOG_PROCESS_ERROR(nResult);

	nResult = LPDumpMgr::Instance().Init(NULL, "log");
	LOG_PROCESS_ERROR(nResult);

	nResult = CNavMeshConvertor::Instance().Init();
	LOG_PROCESS_ERROR(nResult);

	//IMP("start success !");

	__TRY__
	{
		nResult = CNavMeshConvertor::Instance().StartConvert(argc, argv);
		if (!nResult)
		{
			ERR("*** navmesh convert error !");
			LOG_PROCESS_ERROR(FALSE);
		}
		else
		{
			IMP("navmesh convert success !");
		}
	}
		__EXCEPT__
	{

	}

	PROCESS_SUCCESS(TRUE);

Exit0:

Exit1:

	CNavMeshConvertor::Instance().UnInit();

	//IMP("exit success !");

	LPPRINTF("\nEnter any key to exit !\n");
	getchar();

	return 0;
}