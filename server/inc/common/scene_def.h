//****************************************************************************/
//    author: caihy
//    date: January 18, 2016
//    description:
//
//****************************************************************************/
#ifndef _SCENE_DEF_H_
#define _SCENE_DEF_H_


//      ������ϵ˵����x��zλƽ�����꣬yλ�߶�����
//      ��������˵���� �������붨���ڵ�һ���ޣ���x��z����Ϊ����;region��
//  ��������Ϊ�˽�ĳЩgrid�������崦����Ϊscene����С����Ԫ�������
//  scene�Ļ��ֹ�ϸ��



#define POINT_PER_CM            (10LL)                                 //��С���굥λ������/mm
#define POINT_PER_METER         (POINT_PER_CM * 100)
#define GRID_WIDTH              (POINT_PER_CM * 50)                    // ���ӿ�ȣ���λ/cm
#define REGION_WIDTH            (GRID_WIDTH * 4)                       // ����ȣ�����̫���������
#define SCREEN_WIDTH            (GRID_WIDTH * 16)                      // ����ȣ���9����ĵ�λ��9����
#define MAX_SCENE_WIDTH         (GRID_WIDTH * 2048)
#define GRID_PER_REGION         (REGION_WIDTH / GRID_WIDTH)
#define REGION_PER_SCREEN       (SCREEN_WIDTH / REGION_WIDTH)
#define MAX_REGION_PER_SCENE    (MAX_SCENE_WIDTH / REGION_WIDTH)
#define MAX_SCREEN_PER_SCENE    (MAX_SCENE_WIDTH / SCREEN_WIDTH)











#endif
