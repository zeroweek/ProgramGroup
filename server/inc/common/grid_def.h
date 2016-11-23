//****************************************************************************/
//    author: caihy
//    date: January 18, 2016
//    description: 
//
//****************************************************************************/
#ifndef _GRID_DEF_H_
#define _GRID_DEF_H_



// 
enum eGridPkType
{
	egptInvalid = -1,
	egptNormal,
	egptSafe,
	egptTotal
};


#pragma pack(push, 1)

struct GRID_DATA
{
	unsigned            byNextGrid    : 7;     // 高度方向上的下一个格子的索引
	unsigned            bInPoly       : 1;
	unsigned            bBorder       : 1;
	unsigned            byPkType      : 2;
	unsigned            byReserved1   : 21;
	unsigned            byReserved2   : 32;
};

struct GRID_INSTANCE_DATA
{
	unsigned short      byCharRef    : 2;
	unsigned short      byDropRef    : 2;
	unsigned short      byReserved1  : 4;
	unsigned short      byReserved2  : 8;
};

#pragma pack(pop)







#endif
