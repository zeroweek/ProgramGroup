//****************************************************************************/
//    author: caihy
//    date: January 19, 2016
//    description: 
//
//****************************************************************************/
#ifndef _POLYGON_DEF_H_
#define _POLYGON_DEF_H_

#include "lp_base.h"
#include "lp_baselist.h"


#define MIN_VERTEX_PER_POLYGON              (3)
#define MAX_VERTEX_PER_POLYGON              (6)
#define MIN_EDGE_PER_POLYGON                (MIN_VERTEX_PER_POLYGON)
#define MAX_EDGE_PER_POLYGON                (MAX_VERTEX_PER_POLYGON)
#define MIN_NEIGHBOR_PER_POLYGON            (MIN_EDGE_PER_POLYGON)
#define MAX_NEIGHBOR_PER_POLYGON            (MAX_EDGE_PER_POLYGON)
#define MAX_POLYGON_COUNT                   (65535)
#define INVALID_POLYGON_ID                  (0)


//#pragma pack(push, 1)

struct POLYGON_DATA : public BASE_LIST_NODE
{
	INT_32              nID;
	UINT_8              byVertCount;
	INT_32              anVertIndex[MAX_VERTEX_PER_POLYGON];
	UINT_8              byNeighborCount;
	INT_32              anNeighborPoly[MAX_NEIGHBOR_PER_POLYGON];
};
typedef std::list<POLYGON_DATA*> LST_POLYGON_DATA;
typedef std::list<LPBaseList> LST_LST_POLYGON_DATA;

//#pragma pack(pop)








#endif