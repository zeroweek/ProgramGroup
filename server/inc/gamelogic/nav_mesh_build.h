//****************************************************************************/
//    author: caihy
//    date: January 15, 2016
//    description: 
//
//****************************************************************************/
#ifndef _NAV_MESH_BUILD_H_
#define _NAV_MESH_BUILD_H_

#include "lp_lzpl.h"
#include "nav_mesh.h"



// Summary:
//			ԭʼNavMesh.obj���ݼ�������
//		NavMesh.obj�Ǵ�Unity3d���������յ�������������
class DECLARE CNavMeshObjLoader
{
public:

	CNavMeshObjLoader();
	~CNavMeshObjLoader();

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL Load(const char* pcszFileName);

	inline const LPDynamicArray<FLOAT>& GetVertexs() const { return m_oVertexs; }
	inline const LPDynamicArray<INT_32>& GetTrianglePoints() const { return m_oTrianglePoints; }

	inline const char* GetFileName() const { return m_szFileName; }

private:

	BOOL _AddVertex(FLOAT x, FLOAT y, FLOAT z);
	BOOL _AddTriangle(INT_32 a, INT_32 b, INT_32 c);

	POLYGON_DATA* _FindFirstNotAssignIDInRowList(void);
	BOOL _FindNeighborPolygonInRowList(lpIn POLYGON_DATA* pSrcPoly, lpOut POLYGON_DATA** ppNeighborPoly, lpOut UINT_32& dwNeighborPolyCount);
	BOOL _SeparatePolysToLists(POLYGON_DATA* pPolyData, INT_32& nPolyIDGenerator);

	BOOL _DoPolysMerge(POLYGON_DATA* pFromPoly, POLYGON_DATA* pToPoly);

	BOOL _TransTriToPoly(void);


private:

	char                    m_szFileName[MAX_PATH];

	FLOAT                   m_fScale;
	LPDynamicArray<FLOAT>   m_oVertexs;                 // NavMesh�Ķ��㣨vertex����Ϣ
	LPDynamicArray<INT_32>  m_oTrianglePoints;          // NavMesh�����������Σ�triangle����Ϣ

	LST_POLYGON_DATA        m_lstRawPolygonData;
	typedef std::map<INT_32, INT_32> MAP_ID_2_ID;
	MAP_ID_2_ID             m_mapPolyAlreadyFindNeighbor;
	LST_LST_POLYGON_DATA    m_lstSepListPolygonData;
};






#endif