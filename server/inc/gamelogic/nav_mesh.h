//****************************************************************************/
//    author: caihy
//    date: January 15, 2016
//    description:
//
//****************************************************************************/
#ifndef _NAV_MESH_H_
#define _NAV_MESH_H_

#include "grid_def.h"
#include "region_def.h"
#include "polygon_def.h"
#include "scene_def.h"
#include "lp_global.h"



//      导航triangle数据先重新处理成polygon（指定最大顶点数）数据，polygon
//  优先级按顺时针、距离原点最近排列；

// polygon数据
// 方向处理：上下两个方向，由穿过poly中心点的与z轴平行的线指名
// 顶点存储：poly在xy屏幕的投影，距离原点最近的边位第一个边，然后顺时针存储顶点
// 顶点数据按照距离远点最近，倾斜角为正数，顺时针存储

// 格子数据信息
// 多个polygon同时覆盖一个格子，则距离原点最近的polygon引用



class NavMesh
{
public:

    //  Summary:
    //      求两个向量的向量积（交叉积）
    inline BOOL VCrossProduct(lpOut FLOAT* dst, lpIn const FLOAT* v1, lpIn const FLOAT* v2)
    {
        LOG_PROCESS_ERROR(dst);
        LOG_PROCESS_ERROR(v1);
        LOG_PROCESS_ERROR(v2);

        dst[0] = v1[1] * v2[2] - v1[2] * v2[1];
        dst[1] = v1[2] * v2[0] - v1[0] * v2[2];
        dst[2] = v1[0] * v2[1] - v1[1] * v2[0];

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      求两个向量的标量积
    inline BOOL VDotProduct(lpOut FLOAT& dst, lpIn const FLOAT* v1, lpIn const FLOAT* v2)
    {
        LOG_PROCESS_ERROR(v1);
        LOG_PROCESS_ERROR(v2);

        dst = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      求两个向量的向量和
    inline BOOL VAdd(lpOut FLOAT* dst, lpIn const FLOAT* v1, lpIn const FLOAT* v2, lpIn const FLOAT scale = 1.0f)
    {
        LOG_PROCESS_ERROR(dst);
        LOG_PROCESS_ERROR(v1);
        LOG_PROCESS_ERROR(v2);

        dst[0] = v1[0] + v2[0] * scale;
        dst[1] = v1[1] + v2[1] * scale;
        dst[2] = v1[2] + v2[2] * scale;

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      求两个向量的向量差
    inline BOOL VSub(lpOut FLOAT* dst, lpIn const FLOAT* v1, lpIn const FLOAT* v2)
    {
        LOG_PROCESS_ERROR(dst);
        LOG_PROCESS_ERROR(v1);
        LOG_PROCESS_ERROR(v2);

        dst[0] = v1[0] - v2[0];
        dst[1] = v1[1] - v2[1];
        dst[2] = v1[2] - v2[2];

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      取两个点中每个分量的最小值
    inline BOOL VMin(lpInOut FLOAT* mn, lpIn const FLOAT* v)
    {
        LOG_PROCESS_ERROR(mn);
        LOG_PROCESS_ERROR(v);

        mn[0] = lpMin(mn[0], v[0]);
        mn[1] = lpMin(mn[1], v[1]);
        mn[2] = lpMin(mn[2], v[2]);

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      取两个点中每个分量的最大值
    inline BOOL VMax(lpIn FLOAT* mx, lpIn const FLOAT* v)
    {
        LOG_PROCESS_ERROR(mx);
        LOG_PROCESS_ERROR(mx);

        mx[0] = lpMax(mx[0], v[0]);
        mx[1] = lpMax(mx[1], v[1]);
        mx[2] = lpMax(mx[2], v[2]);

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      比较两个点是否相同
    inline BOOL VIsEqual(lpIn FLOAT* v1, lpIn const FLOAT* v2)
    {
        LPINT32 nResult = 0;

        LOG_PROCESS_ERROR(v1);
        LOG_PROCESS_ERROR(v2);

        nResult = (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2]) ? TRUE : FALSE;

Exit0:
        return nResult;
    }

    //  Summary:
    //      点拷贝
    inline BOOL VCopy(lpOut FLOAT* dst, lpIn const FLOAT* v)
    {
        LOG_PROCESS_ERROR(dst);
        LOG_PROCESS_ERROR(v);

        dst[0] = v[0];
        dst[1] = v[1];
        dst[2] = v[2];

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      计算两个点的距离
    inline BOOL VDistance(lpOut FLOAT& dst, lpIn const FLOAT* v1, lpIn const FLOAT* v2)
    {
        FLOAT dx, dy, dz;

        LOG_PROCESS_ERROR(v1);
        LOG_PROCESS_ERROR(v2);

        dx = v2[0] - v1[0];
        dy = v2[1] - v1[1];
        dz = v2[2] - v1[2];
        dst = lpSqrt(dx * dx + dy * dy + dz * dz);

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      计算两个点的距离的平方值
    inline BOOL VDistanceSqr(lpOut FLOAT& dst, lpIn const FLOAT* v1, lpIn const FLOAT* v2)
    {
        FLOAT dx, dy, dz;

        LOG_PROCESS_ERROR(v1);
        LOG_PROCESS_ERROR(v2);

        dx = v2[0] - v1[0];
        dy = v2[1] - v1[1];
        dz = v2[2] - v1[2];
        dst =  dx * dx + dy * dy + dz * dz;

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      Normalizes the vector[(x, y, z)].
    inline BOOL VNormalize(lpInOut FLOAT* v)
    {
        FLOAT dst;

        LOG_PROCESS_ERROR(v);

        dst = 1.0f / lpSqrt(lpSqr(v[0]) + lpSqr(v[1]) + lpSqr(v[2]));
        v[0] *= dst;
        v[1] *= dst;
        v[2] *= dst;

        return TRUE;
Exit0:
        return FALSE;
    }

    //  Summary:
    //      计算一系列顶点的AABB碰撞盒（bounding box）
    BOOL CalBoundingBox(lpIn const FLOAT* pfVerts, lpIn int nVertCount, lpOut FLOAT* pfMin, lpOut FLOAT* pfMax);

private:
    NavMesh() {}
    ~NavMesh() {}

};











#endif