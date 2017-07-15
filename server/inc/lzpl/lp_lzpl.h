//****************************************************************************/
//    author: caihy
//    contact: 1003217929@qq.com
//    date: October 21, 2015
//    description: LZPL模块头文件
//
//****************************************************************************/
#ifndef _LP_LZPL_H_
#define _LP_LZPL_H_


#include "lp_base.h"
#include "lpi_logger.h"
#include "lpi_file.h"
#include "lpi_net.h"

#include "lp_dynamicarray.h"
#include "lp_simplelist.h"
#include "lp_stacklist.h"
#include "lp_baselist.h"
#include "lp_looplist.h"
#include "lp_string.h"
#include "lpi_buf.h"
#include "lp_system.h"
#include "lp_objpool.h"
#include "lp_processerror.h"
#include "lp_datamanager.h"
#include "lp_global.h"
#include "lp_singleton.h"
#include "lp_bt.h"
#include "lp_luascriptmgr.h"
#include "lpi_property.h"
#include "lpi_table.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//   LZPL模块类
class LP
{
public:

    // Summary:
    //   初始化LZPL默认日志控制对象
    //   注：使用层应当在使用LZPL组件之前调用此日志模块初始化函数
    // Input:
    //  stLogConfig: 日志配置
    // Return:
    //      TRUE-成功，FALSE-失败
    static BOOL LPAPI InitLzpl(LOG_CONFIG& stLogConfig);

    // Summary:
    //   反初始化LZPL模块
    static void LPAPI UnInitLzpl();

    // Summary:
    //      加载日志配置
    // Input:
    //      pszDir：日志配置文件的目录
    //      pszFileName：日志配置文件名
    // Return:
    //      TRUE-成功，FALSE-失败
    static BOOL LPAPI LoadLogConfig(const char* pszLogConfigDir, const char* pszLogConfigFileName, LOG_CONFIG& stLogConfig);

private:

    static void LPAPI GlobalAssert();

};



//end声明所处的名字空间
NS_LZPL_END

#endif