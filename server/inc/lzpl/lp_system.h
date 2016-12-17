//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 系统接口定义，意在屏蔽系统特性，提供不依赖于系统的包裹接口，将
//        所有带有系统特性的相关代码都集中到**sysimpl.*
//
//****************************************************************************/
#ifndef _LP_SYSTEM_H_
#define _LP_SYSTEM_H_

#include "lp_base.h"
#include "lp_time.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		睡眠函数，单位毫秒
// Input:
//		dwMicSeconds：睡眠的毫秒数
DECLARE void LPAPI lpSleep(LPUINT32 dwMicSeconds);



// Summary:
//		关闭套接字
DECLARE LPINT32 LPAPI lpShutDown(SOCKET s, LPINT32 nHowTo);



// Summary:
//		关闭套接字
DECLARE LPINT32 LPAPI lpCloseSocket(SOCKET s);



// Summary:
//		关闭句柄
DECLARE LPINT32 LPAPI lpCloseHandle(HANDLE h);



// Summary:
//		关闭套接字
DECLARE void LPAPI lpCancelIoEx(SOCKET s);



// Summary:
//		获取系统当前时间
// Input:
//		stTime[output]：时间结构体
DECLARE void LPAPI lpGetTimeEx(TIME_VALUE& stTime);



// Summary:
//		创建目录
// Input:
//		pszDir：目录全路径
// Return:
//		TRUE-成功，FALSE-失败
DECLARE BOOL LPAPI lpCreateDirectory(const char* pszDir);



// Summary:
//		获取程序路径
// Return:
//		程序路径
DECLARE const char* LPAPI lpGetExePath();



// Summary:
//		获取程序存放目录
// Return:
//		存放目录
DECLARE const char* LPAPI lpGetExeDir();



// Summary:
//		获取程序文件名（包括扩展名）
// Return:
//		程序文件名
DECLARE const char* LPAPI lpGetExeFileName();



// Summary:
//		获取程序进程名（不包括扩展名）
// Return:
//		程序进程名
DECLARE const char* LPAPI lpGetExeProcessName();



// Summary:
//		获取工作目录
// Return:
//		工作目录
DECLARE const char* LPAPI lpGetWorkingDir();



// Summary:
//		获取执行路径
// Return:
//		执行路径
DECLARE LPUINT64 LPAPI lpGetTickCountEx();



// Summary:
//		获取系统启动运行的滴答数
DECLARE LPUINT64 LPAPI lpRdtsc(void);



// Summary:
//		调用调试器
DECLARE void LPAPI lpINT3(void);



// Summary:
//		获取当前进程id
DECLARE LPUINT32 LPAPI lpGetCurrentProcessId();



// Summary:
//		获取当前线程id
DECLARE LPUINT32 LPAPI lpGetCurrentThreadId();



// Summary:
//		获取上一次错误码
DECLARE LPINT32 LPAPI lpGetLastError();



//end声明所处的名字空间

NS_LZPL_END

#endif

