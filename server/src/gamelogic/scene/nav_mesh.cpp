#include "nav_mesh.h"


static void __test_nav_mesh(void)
{
    return;
}

BOOL NavMesh::CalBoundingBox(lpIn const FLOAT* pfVerts, lpIn int nVertCount, lpOut FLOAT* pfMin, lpOut FLOAT* pfMax)
{
    LPINT32 nResult = 0;
    const float* pfVert = NULL;

    nResult = VCopy(pfMin, pfVerts);
    LOG_PROCESS_ERROR(nResult);

    nResult = VCopy(pfMax, pfVerts);
    LOG_PROCESS_ERROR(nResult);

    for(int i = 1; i < nVertCount; ++i)
    {
        pfVert = &pfVerts[i * 3];
        nResult = VMin(pfMin, pfVert);
        LOG_PROCESS_ERROR(nResult);

        nResult = VMax(pfMax, pfVert);
        LOG_PROCESS_ERROR(nResult);
    }

    return TRUE;
Exit0:
    return FALSE;
}
