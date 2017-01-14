//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 基本头文件包含，以及一些基本类型或接口的定义
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

//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		LZPL组件版本结构定义，此结构描述一个组件的版本号信息
struct MODULE_VERSION
{
	LPUINT16 wMajorVer;
	LPUINT16 wMinorVer;
	LPUINT16 wCompatibleVer;
	LPUINT16 wBuildNo;
};



// Summary:
//   所有对象的基础接口定义
class DECLARE ILPObject
{
public:

	virtual LPAPI ~ILPObject(void){}

	virtual std::string LPAPI ToString() = 0;
};




// Summary:
//		所有组件对象的基础接口定义
class DECLARE ILPBase
{
public:

	// Summary:
	//		无
	virtual LPAPI ~ILPBase(void) {}

	// Summary:
	//		引用计数加1
	virtual void LPAPI AddRef(void) = 0;

	// Summary:
	//		引用计数减1
	virtual LPUINT32 LPAPI QueryRef(void) = 0;

	// Summary:
	//		释放对象
	virtual void LPAPI Release(void) = 0;

	// Summary:
	//		获取版本信息
	virtual MODULE_VERSION LPAPI GetVersion(void) = 0;

	// Summary:
	//		获取组件名称，每个组件都将重载此函数，已提供该组件的名称
	virtual const char* LPAPI GetModuleName(void) = 0;
};



// Summary:
//   通用状态定义
enum eCommonState
{
	eCommonState_NoInit,
	eCommonState_Initing,
	eCommonState_Inited,
	eCommonState_Close,
	eCommonState_UnIniting,
	eCommonState_UnInited,
};










//end声明所处的名字空间
NS_LZPL_END

#endif