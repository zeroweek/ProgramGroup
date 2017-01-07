//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: ����һЩȫ���õĺ�
//
//    preprocessor: 
//      _DEBUG������C���п��Զ�����ú꣬�ǵ���C���п���û�ж��塣
//      WIN32/WIN64: 
//		  WIN32��Windows.h���ж��������Ԥ�������Լ����壻
//		  WIN64����Ԥ�������Լ����壻
//      _WIN32/_WIN64: 
//		  _WIN32���ɱ��������壬x86��x64�ĳ��򶼻��ж��壻���ɴ˿����ж�ϵͳ��
//		  _WIN64���ɱ��������壬x64�ĳ�����ж��壻���ɴ˿����ж�WINDOWSλ����
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



//�����ռ�궨��: LZPL 
#ifndef NS_LZPL_BEGIN
#   define NS_LZPL_BEGIN                                   namespace LZPL { 
#endif

#ifndef NS_LZPL_END      
#   define NS_LZPL_END                                     }
#endif

#define LPASSERT                                           assert
#define LPPRINTF                                           printf

//LPAPI�궨��
#if defined _WIN32
#   define LPAPI				                	
//#   define LPAPI				                           __stdcall	
#else
#   define LPAPI	
#endif

//DECLARE�궨��
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


//�������ݷ����ʶ��
#define lpIn
#define lpOut
#define lpInOut

//���������������
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

//�ļ�������󳤶ȡ�·����󳤶Ⱥ궨��
#define MAX_FILE_NAME                                      (256)
#if defined _WIN32	
#   ifndef MAX_PATH
#       define MAX_PATH                                    PATH_MAX
#   endif
#else
#endif



//ͨ�ú�
#define COMMON_NAME_LEN                                    (256)


#define KB                                                 (1024)
#define MB                                                 (1024 * 1024)


#define ZERO_FLOAT                                         (0.0f)
#define ZERO_DOUBLE                                        (0.0)

#define MAX_INT64_LEN                                      (32)
#define MAX_FLOAT_LEN                                      (32)
#define MAX_DOUBLE_LEN                                     (32)


//������غ�
#define IP_LEN                                             (64)                 //����洢ip�ַ����ĳ���
#define DELAY_CLOSE_SOCKET_TIME                            (3000)               //socker�ر������ӳ�ʱ�䣬��λ����/ms
#define DELAY_RELEASE_SOCKER_TIME                          (1500)               //socker�ͷ��ӳ�ʱ�䣬��λ����/ms
#define INVALID_SOCKER_ID                                  (0)                  //��Ч��socker id��ÿһ������socker�������һ��id
#define INFINITE_VALUE                                     (0xffffffff)	        //����ֵ

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


//��Ϣ���
#define MAX_PACKET_LEN                                     (65535)              //������ݰ�����






#endif