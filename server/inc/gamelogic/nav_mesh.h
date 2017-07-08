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



//      ����triangle���������´����polygon��ָ����󶥵��������ݣ�polygon
//  ���ȼ���˳ʱ�롢����ԭ��������У�

// polygon����
// �������������������ɴ���poly���ĵ����z��ƽ�е���ָ��
// ����洢��poly��xy��Ļ��ͶӰ������ԭ������ı�λ��һ���ߣ�Ȼ��˳ʱ��洢����
// �������ݰ��վ���Զ���������б��Ϊ������˳ʱ��洢

// ����������Ϣ
// ���polygonͬʱ����һ�����ӣ������ԭ�������polygon����



class NavMesh
{
public:

    //  Summary:
    //      ���������������������������
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
    //      �����������ı�����
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
    //      ������������������
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
    //      ������������������
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
    //      ȡ��������ÿ����������Сֵ
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
    //      ȡ��������ÿ�����������ֵ
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
    //      �Ƚ��������Ƿ���ͬ
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
    //      �㿽��
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
    //      ����������ľ���
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
    //      ����������ľ����ƽ��ֵ
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
    //      ����һϵ�ж����AABB��ײ�У�bounding box��
    BOOL CalBoundingBox(lpIn const FLOAT* pfVerts, lpIn int nVertCount, lpOut FLOAT* pfMin, lpOut FLOAT* pfMax);

private:
    NavMesh() {}
    ~NavMesh() {}

};











#endif