//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 定义一些全局用的宏
//
//    preprocessor: 
//      _DEBUG：调试C运行库自动定义该宏，非调试C运行库则没有定义。
//      WIN32/WIN64: 
//		  WIN32，Windows.h中有定义或是在预处理器自己定义；
//		  WIN64，在预处理器自己定义；
//      _WIN32/_WIN64: 
//		  _WIN32，由编译器定义，x86和x64的程序都会有定义；（由此可以判断系统）
//		  _WIN64，由编译器定义，x64的程序才有定义；（由此可以判断WINDOWS位数）
//
//****************************************************************************/
#ifndef _LP_DEF_H_
#define _LP_DEF_H_


#ifdef _WIN32
#   define _WINSOCK_DEPRECATED_NO_WARNINGS
#   include <WinSock2.h>
#   include <windows.h>
#   include <process.h>
#else
#endif

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <time.h>
#include <atomic>
#include <thread>
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <wchar.h>



//命名空间宏定义: LZPL 
#ifndef NS_LZPL_BEGIN
#   define NS_LZPL_BEGIN                                   namespace LZPL { 
#endif

#ifndef NS_LZPL_END      
#   define NS_LZPL_END                                     }
#endif

#define LPASSERT                                           assert
#define LPPRINTF                                           printf

//LPAPI宏定义
#if defined _WIN32
#   define LPAPI				                	
//#   define LPAPI				                           __stdcall	
#else
#   define LPAPI	
#endif

//DECLARE宏定义
#if defined _WIN32
#   if defined DLL_IMPORT 
#       define DECLARE
#   elif defined DLL_EXPORT
#       define DECLARE			                           __declspec(dllexport)
#   else
#       define DECLARE			      
#   endif
#else
#   define DECLARE   
#endif


//函数传递方向标识符
#define lpIn
#define lpOut
#define lpInOut

//调试输出函数定义
#if defined _WIN32
#   define LP_OUTPUT_DEBUG_STRING(_msg_)                   OutputDebugString(_msg_)
#else
#   define LP_OUTPUT_DEBUG_STRING                          ()
#endif

#ifdef _WIN32
#   define __TRY__                                         __try 
#   define __EXCEPT__                                      __except (lpDUMPExceptionFilter(GetExceptionInformation()))
#else
#   define __TRY__                             
#   define __EXCEPT__                          
#endif

#define SAFE_DELETE(p)                                     if(NULL!=p){delete p; p=NULL;}  
#define SAFE_DELETE_SZ(p)                                  if(NULL!=p){delete [] p; p=NULL;}
#define SAFE_FREE(p)                                       if(NULL!=p){free(p);p=NULL;}


#define IF_NULL_RETURN(p)                                  if(NULL == p){return;}
#define IF_NULL_BREAK(p)                                   if(NULL == p){break;}
#define IF_NULL_CONTINUE(p)                                if(NULL == p){continue;}
#define IF_NULL_RETURN_VALUE(p, v)                         if(NULL == p){return v;}
#define IF_TRUE_RETURN(e)                                  if(e){return;}
#define IF_TRUE_BREAK(e)                                   if(e){break;}
#define IF_TRUE_CONTINUE(e)                                if(e){continue;}
#define IF_FALSE_RETURN(e)                                 if(!e){return;}
#define IF_FALSE_BREAK(e)                                  if(!e){break;}
#define IF_FALSE_CONTINUE(e)                               if(!e){continue;}
#define IF_TRUE_RETURN_VALUE(e, v)                         if(e){return v;}
#define IF_FALSE_RETURN_VALUE(e, v)                        if(!e){return v;}

//文件名字最大长度、路径最大长度宏定义
#define MAX_FILE_NAME                                      (256)
#if defined _WIN32	
#   ifndef MAX_PATH
#       define MAX_PATH                                    PATH_MAX
#   endif
#else
#endif



//通用宏
#define COMMON_NAME_LEN                                    (256)


#define KB                                                 (1024)
#define MB                                                 (1024 * 1024)


#define ZERO_FLOAT                                         (0.0f)
#define ZERO_DOUBLE                                        (0.0)

#define MAX_INT64_LEN                                      (32)
#define MAX_FLOAT_LEN                                      (32)
#define MAX_DOUBLE_LEN                                     (32)


//网络相关宏
#define IP_LEN                                             (64)                 //定义存储ip字符串的长度
#define DELAY_CLOSE_SOCKET_TIME                            (3000)               //socker关闭连接延迟时间，单位毫秒/ms
#define DELAY_RELEASE_SOCKER_TIME                          (1500)               //socker释放延迟时间，单位毫秒/ms
#define INVALID_SOCKER_ID                                  (0)                  //无效的socker id，每一个连接socker都会分配一个id
#define INFINITE_VALUE                                     (0xffffffff)	        //无穷值

#ifndef _WIN32
#   define INVALID_SOCKET                                  (NULL)
#endif

#if defined _WIN32
#   define SOCK_RECE                                       (0x00)
#   define SOCK_SEND                                       (0x01)
#   define SOCK_BOTH                                       (0x02)
#else
#   define SOCK_RECE                                       (0x00)
#   define SOCK_SEND                                       (0x01)
#   define SOCK_BOTH                                       (0x02)
#endif


//消息相关
#define MAX_PACKET_LEN                                     (65535)              //最大数据包长度






#endif