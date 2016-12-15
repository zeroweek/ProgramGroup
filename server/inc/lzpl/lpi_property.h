//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LPI_PROPERTY_H_
#define _LPI_PROPERTY_H_

#include "lp_base.h"
#include "lp_datalist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN




#define INVALID_PROPERTY                     (0)



// Summary:
//   属性回调接口定义
typedef BOOL(*pfunPropertyEvent)(const LPIDENTID&, const UINT_32&, const ILPDATALIST&, const ILPDATALIST&, const ILPDATALIST&);



// Summary:
//   属性接口
class DECLARE ILPProperty
{
public:

	virtual ~ILPProperty() {}

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType) = 0;
	virtual BOOL LPAPI UnInit() = 0;

	virtual UINT_32 LPAPI GetPropertyID() const  = 0;
	virtual const E_DataType GetType() const = 0;

	virtual BOOL LPAPI SetInt64(INT_64 value) = 0;
	virtual BOOL LPAPI SetFloat(FLOAT value) = 0;
	virtual BOOL LPAPI SetDouble(DOUBLE value) = 0;
	virtual BOOL LPAPI SetString(const std::string& value) = 0;

	virtual INT_64 LPAPI GetInt64() const = 0;
	virtual FLOAT LPAPI GetFloat() const = 0;
	virtual DOUBLE LPAPI GetDouble() const = 0;
	virtual const std::string& LPAPI GetString() const = 0;

	virtual BOOL LPAPI RegisterCallback(const pfunPropertyEvent& cb, INT_32 nPriority, const ILPDATALIST& vars) = 0;
};



// Summary:
//   属性工厂接口
class DECLARE ILPPropertyFactory
{
public:

	virtual INT_32 LPAPI GetPropertyInstanceCount() = 0;

	virtual ILPProperty* LPAPI NewPropertyArray(INT_32 nSize) = 0;
	virtual void LPAPI DeletePropertyArray(ILPProperty* & poProperty) = 0;

	virtual ILPProperty* LPAPI NewProperty(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType) = 0;
	virtual void LPAPI DeleteProperty(ILPProperty* & poProperty) = 0;
};



// Summary:
//   普通属性工厂
class DECLARE LPNormalPropertyFactory : public ILPPropertyFactory
{
public:

	virtual INT_32 LPAPI GetPropertyInstanceCount();
	
	virtual ILPProperty* LPAPI NewPropertyArray(INT_32 nSize);
	virtual void LPAPI DeletePropertyArray(ILPProperty* & poProperty);

	virtual ILPProperty* LPAPI NewProperty(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType);
	virtual void LPAPI DeleteProperty(ILPProperty* & poProperty);
};




//end声明所处的名字空间
NS_LZPL_END

#endif