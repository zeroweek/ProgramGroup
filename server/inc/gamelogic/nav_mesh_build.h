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
//			原始NavMesh.obj数据加载器，
//		NavMesh.obj是从Unity3d导出的最终导航可行走数据
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
	LPDynamicArray<FLOAT>   m_oVertexs;                 // NavMesh的顶点（vertex）信息
	LPDynamicArray<INT_32>  m_oTrianglePoints;          // NavMesh的所有三角形（triangle）信息

	LST_POLYGON_DATA        m_lstRawPolygonData;
	typedef std::map<INT_32, INT_32> MAP_ID_2_ID;
	MAP_ID_2_ID             m_mapPolyAlreadyFindNeighbor;
	LST_LST_POLYGON_DATA    m_lstSepListPolygonData;
};






#endif