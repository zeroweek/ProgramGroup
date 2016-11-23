//****************************************************************************/
//    author: caihy
//    date: January 25, 2016
//    description: 
//
//****************************************************************************/
#ifndef _NAV_MESH_CONVERTOR_H_
#define _NAV_MESH_CONVERTOR_H_

#include "grid_def.h"
#include "region_def.h"
#include "polygon_def.h"
#include "scene_def.h"
#include "lp_global.h"
#include "lp_lzpl.h"


#define DECIMAL_COUNT       (4)          //（目前保留4位，即十分之一毫米单位）
#define DECIMAL_MULT        (10000)
#define VERTEX_FMT          "v %.4lf %.4lf %.4lf\r\n"


// Summary:
//			将unity3d导出的NavMesh.obj导航三角形数据合并处理为多边形poly
class DECLARE CNavMeshConvertor
{
	SINGLETON_DECLARE(CNavMeshConvertor)

public:

	CNavMeshConvertor();
	~CNavMeshConvertor();

	BOOL Init(void);
	BOOL UnInit(void);

	BOOL StartConvert(int argc, char* argv[]);

private:

	void ShowUsage(void);

	void _ClearUp(void);
	BOOL _Load(const char* pcszInputFileName);
	BOOL _Convert(const char* pcszOutputFileName);

	BOOL _AddVertex(UINT_64 x, UINT_64 y, UINT_64 z);
	BOOL _AddTriangle(INT_32 a, INT_32 b, INT_32 c);

	POLYGON_DATA* _FindFirstNotAssignIDInRowList(void);
	BOOL _FindNeighborPolygonInRowList(lpIn POLYGON_DATA* pSrcPoly, lpOut POLYGON_DATA** ppNeighborPoly, lpOut UINT_32& dwNeighborPolyCount);
	BOOL _FindNeighborPolygonInSepList(lpIn LPBaseList* pList, lpIn POLYGON_DATA* pSrcPoly, lpOut POLYGON_DATA** ppNeighborPoly, lpOut UINT_32& dwNeighborPolyCount);
	BOOL _SeparatePolysToLists(POLYGON_DATA* pPolyData, INT_32& nPolyIDGenerator);

	BOOL _DoPolysMerge(POLYGON_DATA* pFromPoly, POLYGON_DATA* pToPoly);

	// 顶点浮点值转换为整数
	// @byDecimalCount	保留小数位数
	UINT_64 _F2UL(FLOAT f, UINT_8 byDecimalCount = DECIMAL_COUNT);

private:

	LPDynamicArray<UINT_64>  m_oVertexs;                 // NavMesh的顶点（vertex）信息
	LPDynamicArray<INT_32>   m_oTrianglePoints;          // NavMesh的所有三角形（triangle）信息

	LST_POLYGON_DATA         m_lstRawPolygonData;

	typedef std::map<INT_32, INT_32> MAP_ID_2_ID;

	MAP_ID_2_ID              m_mapPolyAlreadyFindNeighbor;
	LST_LST_POLYGON_DATA     m_lstSepListPolygonData;

	typedef std::map<POLYGON_DATA*, POLYGON_DATA*> MAP_POLY_POLY;
	MAP_POLY_POLY           m_mapFindNeighborInSepListExclu;
};



















#endif
