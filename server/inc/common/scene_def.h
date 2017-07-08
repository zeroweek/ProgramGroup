//****************************************************************************/
//    author: caihy
//    date: January 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _SCENE_DEF_H_
#define _SCENE_DEF_H_


//      坐标体系说明：x、z位平面坐标，y位高度坐标
//      场景定义说明： 场景必须定义在第一象限，即x、z坐标为正数;region的
//  分区，是为了将某些grid进行整体处理，作为scene的最小处理单元，避免对
//  scene的划分过细。



#define POINT_PER_CM            (10LL)                                 //最小坐标单位：毫米/mm
#define POINT_PER_METER         (POINT_PER_CM * 100)
#define GRID_WIDTH              (POINT_PER_CM * 50)                    // 格子宽度，单位/cm
#define REGION_WIDTH            (GRID_WIDTH * 4)                       // 区宽度（场景太大分区处理）
#define SCREEN_WIDTH            (GRID_WIDTH * 16)                      // 屏宽度，（9宫格的单位，9屏）
#define MAX_SCENE_WIDTH         (GRID_WIDTH * 2048)
#define GRID_PER_REGION         (REGION_WIDTH / GRID_WIDTH)
#define REGION_PER_SCREEN       (SCREEN_WIDTH / REGION_WIDTH)
#define MAX_REGION_PER_SCENE    (MAX_SCENE_WIDTH / REGION_WIDTH)
#define MAX_SCREEN_PER_SCENE    (MAX_SCENE_WIDTH / SCREEN_WIDTH)











#endif
