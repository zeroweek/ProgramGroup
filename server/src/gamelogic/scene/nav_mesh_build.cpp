#include "nav_mesh_build.h"
#include "lpi_file.h"
#include "lp_processerror.h"



CNavMeshObjLoader::CNavMeshObjLoader()
{
	memset(m_szFileName, 0, sizeof(m_szFileName));
	m_fScale = 0.0f;
	m_lstRawPolygonData.clear();
	m_mapPolyAlreadyFindNeighbor.clear();
	m_lstSepListPolygonData.clear();
}

CNavMeshObjLoader::~CNavMeshObjLoader()
{
	UnInit();
}

BOOL CNavMeshObjLoader::Init(void)
{
	LPINT32 nResult = 0;

	m_fScale = 1.0f;

	nResult = m_oVertexs.Init(100);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oTrianglePoints.Init(100);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	UnInit();
	return FALSE;
}

BOOL CNavMeshObjLoader::UnInit(void)
{
	LPINT32 nResult = 0;

	nResult = m_oVertexs.UnInit();
	LOG_CHECK_ERROR(nResult);

	nResult = m_oTrianglePoints.UnInit();
	LOG_CHECK_ERROR(nResult);

	return TRUE;
}

BOOL CNavMeshObjLoader::_AddVertex(FLOAT x, FLOAT y, FLOAT z)
{
	LPINT32 nResult = 0;

	nResult = m_oVertexs.Push(x*m_fScale);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oVertexs.Push(y*m_fScale);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oVertexs.Push(z*m_fScale);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:

	return FALSE;
}

POLYGON_DATA* CNavMeshObjLoader::_FindFirstNotAssignIDInRowList(void)
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

BOOL CNavMeshObjLoader::_FindNeighborPolygonInRowList(lpIn POLYGON_DATA* pSrcPoly, lpOut POLYGON_DATA** ppNeighborPoly, lpOut LPUINT32& dwNeighborPolyCount)
{
	LPINT32 nResult = 0;
	LPINT32 nEqualVertCount = 0;
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
		for (LPUINT8 byIndexDst = 0; byIndexDst < pPolygonData->byVertCount; ++byIndexDst)
		{
			for (LPUINT8 byIndexSrc = 0; byIndexSrc < pSrcPoly->byVertCount; ++byIndexSrc)
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

BOOL CNavMeshObjLoader::_SeparatePolysToLists(POLYGON_DATA* pPolyData, LPINT32& nPolyIDGenerator)
{
	LPINT32 nResult = 0;
	LPUINT32 dwNeighborPolyCount = 0;
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

		// �ŵ��б���ȥ
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

	for (LPUINT32 dwIndex = 0; dwIndex < dwNeighborPolyCount; ++dwIndex)
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

BOOL CNavMeshObjLoader::_DoPolysMerge(POLYGON_DATA* pFromPoly, POLYGON_DATA* pToPoly)
{
	LPINT32 nResult = 0;
	LPINT32 nEqualVertCount = 0;

	BOOL bRemove = FALSE;
	LPUINT8 byTempIndex = 0;
	LPUINT8 byFromBeMergeIndex = 0;
	LPUINT8 abyFromVertEqualIndex[2] = { 0 };
	LPUINT8 abyToVertEqualIndex[2] = { 0 };
	LPUINT8 byRemoveCount = 0;
	LPUINT8 abyRemoveVertIndex[MAX_VERTEX_PER_POLYGON] = { 0 };

	LPUINT8 byMergeVertCount = 0;
	LPINT32 anMergeVertIndex[MAX_VERTEX_PER_POLYGON];
	LPUINT8 byCalVertCount = 0;
	LPINT32 anCalVertIndex[MAX_VERTEX_PER_POLYGON * 2];

	DOUBLE k, b;
	DOUBLE vertCalRet, vertCalRet1, vertCalRet2, vertCalRet3;
	FLOAT x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3;
	FLOAT v0x, v0y, v0z, v1x, v1y, v1z, v2x, v2y, v2z;

	LOG_PROCESS_ERROR(pFromPoly);
	LOG_PROCESS_ERROR(pToPoly);

	// ע�⣺�ڴ���Ҫ��֤�������ϲ���pFromPoly����������
	//     �����ϲ���pFromPoly�������Σ���ֻ�轫�����ߵ��Ǹ������
	// pToPoly�й��ߵ���������֮�䣬���ɱ�֤�ϲ���Ķ�����pToPoly
	// ���ǰ�˳ʱ�����ʱ��˳��洢�ġ�
	LOG_PROCESS_ERROR(pFromPoly->byVertCount == 3);

	// ��ⶥ���Ƿ�����ϲ�����
	PROCESS_ERROR(pFromPoly->byVertCount + pToPoly->byVertCount - 2 <= MAX_VERTEX_PER_POLYGON);
	PROCESS_ERROR(pFromPoly->byVertCount + pToPoly->byVertCount - 2 >= MIN_VERTEX_PER_POLYGON);
	for (LPUINT8 byIndexTo = 0; byIndexTo < pToPoly->byVertCount; ++byIndexTo)
	{
		for (LPUINT8 byIndexFrom = 0; byIndexFrom < pFromPoly->byVertCount; ++byIndexFrom)
		{
			if (pFromPoly->anVertIndex[byIndexFrom] == pToPoly->anVertIndex[byIndexTo])
			{
				LOG_PROCESS_ERROR(nEqualVertCount < 2);
				abyFromVertEqualIndex[nEqualVertCount] = byIndexFrom;
				abyToVertEqualIndex[nEqualVertCount] = byIndexTo;
				++nEqualVertCount;
			}
		}
	}
	PROCESS_ERROR(nEqualVertCount == 2);

	// �ҳ�������ض���
	// �ҳ�pFromPoly��Ҫ���ϲ��ĵ㣬����洢��pToPoly���ߵ���������֮��
	byFromBeMergeIndex = 0 + 1 + 2 - abyFromVertEqualIndex[0] - abyFromVertEqualIndex[1];
	LOG_PROCESS_ERROR(byFromBeMergeIndex >= 0 && byFromBeMergeIndex <= 2);
	if (abyToVertEqualIndex[0] > abyToVertEqualIndex[1])
	{
		byTempIndex = abyToVertEqualIndex[0];
		abyToVertEqualIndex[0] = abyToVertEqualIndex[1];
		abyToVertEqualIndex[1] = byTempIndex;
	}

	byMergeVertCount = pToPoly->byVertCount;
	memcpy(anMergeVertIndex, pToPoly->anVertIndex, sizeof(pToPoly->anVertIndex[0]) * byMergeVertCount);
	if (abyToVertEqualIndex[0] == 0 && abyToVertEqualIndex[1] == byMergeVertCount - 1)
	{
		anMergeVertIndex[byMergeVertCount++] = pFromPoly->anVertIndex[byFromBeMergeIndex];
	}
	else
	{
		for (LPUINT8 byIndex = pToPoly->byVertCount; byIndex - 1 >= 0 && byIndex > abyToVertEqualIndex[1]; --byIndex)
		{
			anMergeVertIndex[byIndex] = anMergeVertIndex[byIndex - 1];
		}
		anMergeVertIndex[abyToVertEqualIndex[1]] = pFromPoly->anVertIndex[byFromBeMergeIndex];
		++byMergeVertCount;
	}

	// �ж����е㹲��
	for (LPUINT8 byIndex = 0; byIndex + 3 < byMergeVertCount; ++byIndex)
	{
		LOG_PROCESS_ERROR(anMergeVertIndex[byIndex + 0] * 3 + 2 < m_oVertexs.Size());
		LOG_PROCESS_ERROR(anMergeVertIndex[byIndex + 1] * 3 + 2 < m_oVertexs.Size());
		LOG_PROCESS_ERROR(anMergeVertIndex[byIndex + 2] * 3 + 2 < m_oVertexs.Size());
		LOG_PROCESS_ERROR(anMergeVertIndex[byIndex + 3] * 3 + 2 < m_oVertexs.Size());

		x0 = m_oVertexs[anMergeVertIndex[byIndex + 0] * 3 + 0];
		y0 = m_oVertexs[anMergeVertIndex[byIndex + 0] * 3 + 1];
		z0 = m_oVertexs[anMergeVertIndex[byIndex + 0] * 3 + 2];

		x1 = m_oVertexs[anMergeVertIndex[byIndex + 1] * 3 + 0];
		y1 = m_oVertexs[anMergeVertIndex[byIndex + 1] * 3 + 1];
		z1 = m_oVertexs[anMergeVertIndex[byIndex + 1] * 3 + 2];

		x2 = m_oVertexs[anMergeVertIndex[byIndex + 2] * 3 + 0];
		y2 = m_oVertexs[anMergeVertIndex[byIndex + 2] * 3 + 1];
		z2 = m_oVertexs[anMergeVertIndex[byIndex + 2] * 3 + 2];

		x3 = m_oVertexs[anMergeVertIndex[byIndex + 3] * 3 + 0];
		y3 = m_oVertexs[anMergeVertIndex[byIndex + 3] * 3 + 1];
		z3 = m_oVertexs[anMergeVertIndex[byIndex + 3] * 3 + 2];

		v0x = x0 - x3;
		v0y = y0 - y3;
		v0z = z0 - z3;

		v1x = x1 - x3;
		v1y = y1 - y3;
		v1z = z1 - z3;

		v2x = x2 - x3;
		v2y = y2 - y3;
		v2z = z2 - z3;

		vertCalRet = v0x * v1y * v2z + v0y * v1z * v2x + v0z * v1x * v2y
				- v0z * v1y * v2x - v0y * v1x * v2z - v0x * v1z * v2y;
		//if (vertCalRet >= 0.00001)
		//{
		//	LPPRINTF("vertCalRet=%lf\n", vertCalRet);
		//}
		//�ų����
		PROCESS_ERROR(vertCalRet < 0.00005);

	}

	// ȥ������ĵ㣨һ�����ϳ����������㣩
	byRemoveCount = 0;
	byCalVertCount = byMergeVertCount;
	memcpy(anCalVertIndex, anMergeVertIndex, sizeof(anCalVertIndex[0]) * byCalVertCount);
	anCalVertIndex[byCalVertCount++] = anCalVertIndex[0];
	anCalVertIndex[byCalVertCount++] = anCalVertIndex[1];
	for (LPUINT8 byIndex = 0; byIndex + 2 < byCalVertCount; ++byIndex)
	{
		//����ά�е�����ΪP1,P2,P3;
		//P1��P2������Ϊ:V1=P2-P1��
		//P1��P3������Ϊ:V2=P3-P1;
		//���P1,P2,P3��ͬһֱ�ߣ���V1��V2�ļн�Ϊ0��PI,����
		//V1��V2�Ĳ����ģΪ:|V1xV2|=0;
		//���У�
		//V1.Y*V2.Z-V1.Z*V2.Y=0,
		//V1.Z*V2.X-V1.X*V2.Z=0,
		//V1.X*V2.Y-V1.Y*V2.X=0.

		x0 = m_oVertexs[anCalVertIndex[byIndex + 0] * 3 + 0];
		y0 = m_oVertexs[anCalVertIndex[byIndex + 0] * 3 + 1];
		z0 = m_oVertexs[anCalVertIndex[byIndex + 0] * 3 + 2];

		x1 = m_oVertexs[anCalVertIndex[byIndex + 1] * 3 + 0];
		y1 = m_oVertexs[anCalVertIndex[byIndex + 1] * 3 + 1];
		z1 = m_oVertexs[anCalVertIndex[byIndex + 1] * 3 + 2];

		x2 = m_oVertexs[anCalVertIndex[byIndex + 2] * 3 + 0];
		y2 = m_oVertexs[anCalVertIndex[byIndex + 2] * 3 + 1];
		z2 = m_oVertexs[anCalVertIndex[byIndex + 2] * 3 + 2];

		v0x = x0 - x1;
		v0y = y0 - y1;
		v0z = z0 - z1;

		v1x = x2 - x1;
		v1y = y2 - y1;
		v1z = z2 - z1;

		vertCalRet1 = v0y * v1z - v0z * v1y;
		vertCalRet2 = v0z * v1x - v0x * v1z;
		vertCalRet3 = v0x * v1y - v0y * v1x;
		if (-0.00005 < vertCalRet1 && vertCalRet1 < 0.00005 &&
			-0.00005 < vertCalRet2 && vertCalRet2 < 0.00005 &&
			-0.00005 < vertCalRet3 && vertCalRet3 < 0.00005)
		{
			if (byIndex + 1 >= byMergeVertCount)
			{
				abyRemoveVertIndex[byRemoveCount++] = byIndex + 1 - byMergeVertCount;
			}
			else
			{
				abyRemoveVertIndex[byRemoveCount++] = byIndex + 1;
			}
			LPPRINTF("vertCalRet1=%lf\n", vertCalRet1);
			LPPRINTF("vertCalRet2=%lf\n", vertCalRet2);
			LPPRINTF("vertCalRet3=%lf\n", vertCalRet3);
			LPPRINTF("\n");
		}
	}

	if (byRemoveCount > 0)
	{
		byCalVertCount = byMergeVertCount;
		LOG_PROCESS_ERROR(byCalVertCount - byRemoveCount >= MIN_VERTEX_PER_POLYGON);
		memcpy(anCalVertIndex, anMergeVertIndex, sizeof(anCalVertIndex[0]) * byCalVertCount);
		byMergeVertCount = 0;
		for (LPUINT8 byIndex = 0; byIndex < byCalVertCount; ++byIndex)
		{
			bRemove = FALSE;
			for (LPUINT8 j = 0; j < byRemoveCount; ++j)
			{
				if (byIndex == abyRemoveVertIndex[j])
				{
					bRemove = TRUE;
					break;
				}
			}

			if (!bRemove)
			{
				anMergeVertIndex[byMergeVertCount++] = anCalVertIndex[byIndex];
			}
		}
		LOG_PROCESS_ERROR(byMergeVertCount == byCalVertCount - byRemoveCount);
	}

	// �ж��Ƿ�Ϊ͹�����
	// ǰ���Ѿ�֤��Ϊ���棬����ֱ��ʹ��x��zƽ���ͶӰ�����ж�
	// �����ֱ�߷��̵�б��k�ͳ���b���ж����е㶼��ͬһ��
	x0 = m_oVertexs[anMergeVertIndex[0] * 3 + 0];
	z0 = m_oVertexs[anMergeVertIndex[0] * 3 + 2];
	x1 = m_oVertexs[anMergeVertIndex[1] * 3 + 0];
	z1 = m_oVertexs[anMergeVertIndex[1] * 3 + 2];
	x2 = m_oVertexs[anMergeVertIndex[2] * 3 + 0];
	z2 = m_oVertexs[anMergeVertIndex[2] * 3 + 2];
	if (x1 != x0)
	{
		k = (z1 - z0) / (x1 - x0);
		b = (z0 * x1 - z1 * x0) / (x1 - x0);
	}
	vertCalRet1 = k * x2 + b;
	for (LPUINT8 byIndex = 3; byIndex < byMergeVertCount; ++byIndex)
	{
		x3 = m_oVertexs[anMergeVertIndex[byIndex] * 3 + 0];
		z3 = m_oVertexs[anMergeVertIndex[byIndex] * 3 + 2];

		if (x1 != x0)
		{
			vertCalRet2 = k * x3 + b;
			if (z2 > vertCalRet1 && z3 < vertCalRet2)
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

	// ����pToPolyΪ���յĶ�������
	pToPoly->byVertCount = byMergeVertCount;
	memcpy(pToPoly->anVertIndex, anMergeVertIndex, sizeof(pToPoly->anVertIndex[0]) * pToPoly->byVertCount);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CNavMeshObjLoader::_TransTriToPoly(void)
{
	LPINT32 nResult = 0;
	LPUINT32 dwTotalPolyCount = 0;
	LPINT32 nPolyIDGenerator = INVALID_POLYGON_ID;
	POLYGON_DATA* pPolyData = NULL;
	POLYGON_DATA* pNextPolyData = NULL;
	LST_LST_POLYGON_DATA::iterator llitr;
	LPBaseList* pBaseList = NULL;

	ILPFile* pFile = NULL;
	LPINT32 nLen = 0;
	char szBuf[512];


	// 1. tri�б�ת����poly�б�
	for (LPINT32 nIndex = 0; nIndex + 2 < m_oTrianglePoints.Size(); nIndex += 3)
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


	// 2. ����poly�������ڵ�poly�ֵ���ͬ���б�
	nPolyIDGenerator = INVALID_POLYGON_ID;
	pPolyData = _FindFirstNotAssignIDInRowList();
	LOG_PROCESS_ERROR(pPolyData);
	while (pPolyData)
	{
		// �������б�
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
	LOG_PROCESS_ERROR(dwTotalPolyCount == (LPUINT32)m_lstRawPolygonData.size());


	// 3. �ϲ�poly�����ڵ�poly�����ܶ�ĺϲ������ƶ�������֮�ڣ�
	// �ӵ�һ����ŵ�triΪ��������һ�ϲ������tri����������������Ϊͬһ��poly��
	// ������������������triʱ���ϲ�����������ǰ������ϲ�������tri��Ϊһ��
	// poly�����Ž���ѡ����һ��δ���ϲ���tri������һ��poly�ĺϲ�����
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
		pNextPolyData = (POLYGON_DATA*)pPolyData->pstNext;
		while (pNextPolyData && pNextPolyData != &pBaseList->Rear())
		{
			if (_DoPolysMerge(pNextPolyData, pPolyData))
			{
				nResult = pBaseList->Remove(pNextPolyData);
				LOG_PROCESS_ERROR(nResult);
				SAFE_DELETE(pNextPolyData);
			}
			else
			{
				pPolyData = pNextPolyData;
			}

			pNextPolyData = (POLYGON_DATA*)pPolyData->pstNext;
		}

		dwTotalPolyCount += pBaseList->Size();
	}


	// 4. poly��ţ������յ�poly���б��
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
	LOG_PROCESS_ERROR(dwTotalPolyCount == (LPUINT32)(nPolyIDGenerator));

	
	// 5. �����յ�poly���ݱ��浽�ļ�
	pFile = lpFileOpen("config/scene/new_navmesh.obj", "wb");
	LOG_PROCESS_ERROR(pFile);

	for (LPUINT32 i = 0; i + 2 < m_oVertexs.Size(); i+=3)
	{
		nLen = sprintf_s(szBuf, sizeof(szBuf) - 1, "v %f %f %f\r\n", m_oVertexs[i + 0], m_oVertexs[i + 1], m_oVertexs[i + 2]);
		LOG_PROCESS_ERROR(nLen > 0);
		nResult = (LPUINT32)pFile->Write(szBuf, (LPUINT32)nLen);
		LOG_PROCESS_ERROR(nResult == nLen);
	}
	nResult = (LPUINT32)pFile->Write("\r\n", 2);
	LOG_PROCESS_ERROR(nResult == 2);

	for (llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	{
		pBaseList = &(*llitr);
		pPolyData = (POLYGON_DATA*)pBaseList->Head().pstNext;
		while (pPolyData && pPolyData != &pBaseList->Rear())
		{
			nLen = 0;
			nResult = sprintf_s(szBuf + nLen, sizeof(szBuf) - 1 - nLen, "f");
			LOG_PROCESS_ERROR(nResult > 0);
			nLen += nResult;
			for (LPUINT8 i = 0; i < pPolyData->byVertCount; i++)
			{
				nResult = sprintf_s(szBuf + nLen, sizeof(szBuf) - 1 - nLen, " %d/%d/%d", pPolyData->anVertIndex[i] + 1, pPolyData->anVertIndex[i] + 1, pPolyData->anVertIndex[i] + 1);
				LOG_PROCESS_ERROR(nResult > 0);
				nLen += nResult;
			}
			nResult = sprintf_s(szBuf + nLen, sizeof(szBuf) - 1 - nLen, "\r\n");
			LOG_PROCESS_ERROR(nResult > 0);
			nLen += nResult;
			nResult = (LPUINT32)pFile->Write(szBuf, (LPUINT32)nLen);
			LOG_PROCESS_ERROR(nResult == nLen);

			pPolyData = (POLYGON_DATA*)pPolyData->pstNext;
		}
	}

	pFile->Close();
	pFile = NULL;

	for (llitr = m_lstSepListPolygonData.begin(); llitr != m_lstSepListPolygonData.end(); ++llitr)
	{
		pBaseList = &(*llitr);
		pPolyData = (POLYGON_DATA*)pBaseList->Head().pstNext;
		while (pPolyData && pPolyData != &pBaseList->Rear())
		{
			pBaseList->Remove(pPolyData);
			SAFE_DELETE(pPolyData);
			pPolyData = (POLYGON_DATA*)pBaseList->Head().pstNext;
		}

	}


	return TRUE;
Exit0:
	return FALSE;
}

BOOL CNavMeshObjLoader::_AddTriangle(LPINT32 a, LPINT32 b, LPINT32 c)
{
	LPINT32 nResult = 0;

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

static char* _ParseRow(char* pszBuf, char* pszBufEnd, char* pszRow, LPINT32 nMaxRowLen)
{
	char c = 0;
	LPINT32 nIndex = 0;
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

static LPINT32 _ParseFace(char* pszRow, LPINT32* pnFaceData, LPINT32 nMaxFacePoint, LPINT32 nVertCount)
{
	LPINT32 nIndex = 0;
	LPINT32 nVertIndex = 0;
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

BOOL CNavMeshObjLoader::Load(const char* pcszFileName)
{
	LPINT32 nResult = 0;

	LPUINT32 dwFileSize = 0;
	ILPFile* pFile = NULL;
	char* pszFileData = NULL;

	char* pSrc = NULL;
	char* pSrcEnd = NULL;
	char szRow[512];

	FLOAT x = 0, y = 0, z = 0;

	LPINT32 szFacePoint[MAX_VERTEX_PER_POLYGON], nVert = 0, a = 0, b = 0, c = 0;

	FLOAT* pV0 = NULL;
	FLOAT* pV1 = NULL;
	FLOAT* pV2 = NULL;
	FLOAT* pNormal = NULL;

	LOG_PROCESS_ERROR(pcszFileName);

	pFile = lpFileOpen(pcszFileName, "rb");
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
			nResult = _AddVertex(x, y, z);
			LOG_PROCESS_ERROR(nResult);
		}

		// parse faces
		if (szRow[0] == 'f')
		{
			nVert = _ParseFace(szRow + 1, szFacePoint, MAX_VERTEX_PER_POLYGON, m_oVertexs.Size());
			LOG_PROCESS_ERROR(nVert == 3);
			for (LPINT32 i = 2; i < nVert; ++i)
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

	nResult = _TransTriToPoly();
	LOG_PROCESS_ERROR(nResult);

	SAFE_DELETE_SZ(pszFileData);
	lpStrCpyN(m_szFileName, pcszFileName, MAX_PATH);

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