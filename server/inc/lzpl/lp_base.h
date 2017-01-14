//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: ����ͷ�ļ��������Լ�һЩ�������ͻ�ӿڵĶ���
//
//****************************************************************************/
#ifndef _LP_BASE_H_
#define _LP_BASE_H_


#include "lp_def.h"
#include "lp_type.h"
#include "lp_singleton.h"


#if defined _WIN32
	//#pragma warning(push)
	#pragma warning ( disable : 4251)
	#pragma warning ( disable : 4018)
	#pragma warning( disable : 4200)
	//#pragma warning(pop)
#else
	#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wunused-value"
	#pragma GCC diagnostic ignored "-Wsign-compare"
	#pragma GCC diagnostic ignored "-Wdelete-incomplete"
	#pragma GCC diagnostic ignored "-Wunused-function"
	//#pragma GCC diagnostic ignored "-Wmultichar"
	#pragma GCC diagnostic ignored "-Wwrite-strings"
	//#pragma GCC diagnostic pop
#endif

using namespace std;
using namespace LZPL;

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		LZPL����汾�ṹ���壬�˽ṹ����һ������İ汾����Ϣ
struct MODULE_VERSION
{
	LPUINT16 wMajorVer;
	LPUINT16 wMinorVer;
	LPUINT16 wCompatibleVer;
	LPUINT16 wBuildNo;
};



// Summary:
//   ���ж���Ļ����ӿڶ���
class DECLARE ILPObject
{
public:

	virtual LPAPI ~ILPObject(void){}

	virtual std::string LPAPI ToString() = 0;
};




// Summary:
//		�����������Ļ����ӿڶ���
class DECLARE ILPBase
{
public:

	// Summary:
	//		��
	virtual LPAPI ~ILPBase(void) {}

	// Summary:
	//		���ü�����1
	virtual void LPAPI AddRef(void) = 0;

	// Summary:
	//		���ü�����1
	virtual LPUINT32 LPAPI QueryRef(void) = 0;

	// Summary:
	//		�ͷŶ���
	virtual void LPAPI Release(void) = 0;

	// Summary:
	//		��ȡ�汾��Ϣ
	virtual MODULE_VERSION LPAPI GetVersion(void) = 0;

	// Summary:
	//		��ȡ������ƣ�ÿ������������ش˺��������ṩ�����������
	virtual const char* LPAPI GetModuleName(void) = 0;
};



// Summary:
//   ͨ��״̬����
enum eCommonState
{
	eCommonState_NoInit,
	eCommonState_Initing,
	eCommonState_Inited,
	eCommonState_Close,
	eCommonState_UnIniting,
	eCommonState_UnInited,
};










//end�������������ֿռ�
NS_LZPL_END

#endif