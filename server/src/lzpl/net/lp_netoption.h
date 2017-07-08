//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_NET_OPTION_H_
#define _LP_NET_OPTION_H_

#include "lpi_sockoption.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      网络选项类
class DECLARE LPNetOption : public ILPNetOption
{
public:

    // Summary:
    //      none
    LPNetOption();

    // Summary:
    //      none
    virtual ~LPNetOption();

};



//end声明所处的名字空间
NS_LZPL_END

#endif