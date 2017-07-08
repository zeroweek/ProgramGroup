#include "lp_http.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPHttp::LPHttp()
{
}

LPHttp::~LPHttp()
{
    UnInit();
}

BOOL LPAPI LPHttp::Init()
{

    return TRUE;
}

BOOL LPAPI LPHttp::UnInit(void)
{

    return TRUE;
}



//end声明所处的名字空间

NS_LZPL_END