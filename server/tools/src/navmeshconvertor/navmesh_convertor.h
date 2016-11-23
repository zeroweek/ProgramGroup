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


#define DECIMAL_COUNT       (4)          //��Ŀǰ����4λ����ʮ��֮һ���׵�λ��
#define DECIMAL_MULT        (10000)
#define VERTEX_FMT          "v %.4lf %.4lf %.4lf\r\n"


// Summary:
//			��unity3d������NavMesh.obj�������������ݺϲ�����Ϊ�����poly
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

	// ���㸡��ֵת��Ϊ����
	// @byDecimalCount	����С��λ��
	UINT_64 _F2UL(FLOAT f, UINT_8 byDecimalCount = DECIMAL_COUNT);

private:

	LPDynamicArray<UINT_64>  m_oVertexs;                 // NavMesh�Ķ��㣨vertex����Ϣ
	LPDynamicArray<INT_32>   m_oTrianglePoints;          // NavMesh�����������Σ�triangle����Ϣ

	LST_POLYGON_DATA         m_lstRawPolygonData;

	typedef std::map<INT_32, INT_32> MAP_ID_2_ID;

	MAP_ID_2_ID              m_mapPolyAlreadyFindNeighbor;
	LST_LST_POLYGON_DATA     m_lstSepListPolygonData;

	typedef std::map<POLYGON_DATA*, POLYGON_DATA*> MAP_POLY_POLY;
	MAP_POLY_POLY           m_mapFindNeighborInSepListExclu;
};



















#endif
