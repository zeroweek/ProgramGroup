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
#endif

using namespace std;
using namespace LZPL;

//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		LZPL����汾�ṹ���壬�˽ṹ����һ������İ汾����Ϣ
struct MODULE_VERSION
{
	UINT_16 wMajorVer;
	UINT_16 wMinorVer;
	UINT_16 wCompatibleVer;
	UINT_16 wBuildNo;
};



// Summary:
//		���ж���Ļ����ӿڶ���
class DECLARE ILPObject
{
public:

	// Summary:
	//		��
	virtual LPAPI ~ILPObject(void){}

	// Summary:
	//		���ü�����1
	virtual void LPAPI AddRef(void) = 0;

	// Summary:
	//		���ü�����1
	virtual UINT_32 LPAPI QueryRef(void) = 0;

	// Summary:
	//		�ͷŶ���
	virtual void LPAPI Release(void) = 0;
};




// Summary:
//		�����������Ļ����ӿڶ���
class DECLARE ILPBase : public ILPObject
{
public:

	// Summary:
	//		��
	virtual LPAPI ~ILPBase(void){}

	// Summary:
	//		��ȡ�汾��Ϣ
	virtual MODULE_VERSION LPAPI GetVersion(void) = 0;

	// Summary:
	//		��ȡ������ƣ�ÿ������������ش˺��������ṩ�����������
	virtual const char* LPAPI GetModuleName(void) = 0;
};










//end�������������ֿռ�
NS_LZPL_END

#endif