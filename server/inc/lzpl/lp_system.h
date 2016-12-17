//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: ϵͳ�ӿڶ��壬��������ϵͳ���ԣ��ṩ��������ϵͳ�İ����ӿڣ���
//        ���д���ϵͳ���Ե���ش��붼���е�**sysimpl.*
//
//****************************************************************************/
#ifndef _LP_SYSTEM_H_
#define _LP_SYSTEM_H_

#include "lp_base.h"
#include "lp_time.h"

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		˯�ߺ�������λ����
// Input:
//		dwMicSeconds��˯�ߵĺ�����
DECLARE void LPAPI lpSleep(LPUINT32 dwMicSeconds);



// Summary:
//		�ر��׽���
DECLARE LPINT32 LPAPI lpShutDown(SOCKET s, LPINT32 nHowTo);



// Summary:
//		�ر��׽���
DECLARE LPINT32 LPAPI lpCloseSocket(SOCKET s);



// Summary:
//		�رվ��
DECLARE LPINT32 LPAPI lpCloseHandle(HANDLE h);



// Summary:
//		�ر��׽���
DECLARE void LPAPI lpCancelIoEx(SOCKET s);



// Summary:
//		��ȡϵͳ��ǰʱ��
// Input:
//		stTime[output]��ʱ��ṹ��
DECLARE void LPAPI lpGetTimeEx(TIME_VALUE& stTime);



// Summary:
//		����Ŀ¼
// Input:
//		pszDir��Ŀ¼ȫ·��
// Return:
//		TRUE-�ɹ���FALSE-ʧ��
DECLARE BOOL LPAPI lpCreateDirectory(const char* pszDir);



// Summary:
//		��ȡ����·��
// Return:
//		����·��
DECLARE const char* LPAPI lpGetExePath();



// Summary:
//		��ȡ������Ŀ¼
// Return:
//		���Ŀ¼
DECLARE const char* LPAPI lpGetExeDir();



// Summary:
//		��ȡ�����ļ�����������չ����
// Return:
//		�����ļ���
DECLARE const char* LPAPI lpGetExeFileName();



// Summary:
//		��ȡ�������������������չ����
// Return:
//		���������
DECLARE const char* LPAPI lpGetExeProcessName();



// Summary:
//		��ȡ����Ŀ¼
// Return:
//		����Ŀ¼
DECLARE const char* LPAPI lpGetWorkingDir();



// Summary:
//		��ȡִ��·��
// Return:
//		ִ��·��
DECLARE LPUINT64 LPAPI lpGetTickCountEx();



// Summary:
//		��ȡϵͳ�������еĵδ���
DECLARE LPUINT64 LPAPI lpRdtsc(void);



// Summary:
//		���õ�����
DECLARE void LPAPI lpINT3(void);



// Summary:
//		��ȡ��ǰ����id
DECLARE LPUINT32 LPAPI lpGetCurrentProcessId();



// Summary:
//		��ȡ��ǰ�߳�id
DECLARE LPUINT32 LPAPI lpGetCurrentThreadId();



// Summary:
//		��ȡ��һ�δ�����
DECLARE LPINT32 LPAPI lpGetLastError();



//end�������������ֿռ�

NS_LZPL_END

#endif

