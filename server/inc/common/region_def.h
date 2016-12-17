//****************************************************************************/
//    author: caihy
//    date: January 18, 2016
//    description: 
//
//****************************************************************************/
#ifndef _REGION_DEF_H_
#define _REGION_DEF_H_

#include "lp_base.h"
#include "grid_def.h"
#include "scene_def.h"



#pragma pack(push, 1)

struct REGION_DATA
{
	LPUINT16             wRegionX;
	LPUINT16             wRegionY;

	                    //�����ϵĸ��ӵ����������Ӹ߶ȷ����ϻ�������next���ӣ�
	LPUINT16             GroundGrid[GRID_PER_REGION][GRID_PER_REGION];
	LPUINT16             wGridCount;

	GRID_DATA           GridData[0];
};

#pragma pack(pop)








#endif
