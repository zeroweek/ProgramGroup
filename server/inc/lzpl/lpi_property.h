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




#define INVALID_PROPERTY_ID                     (0)



// Summary:
//   属性接口
class DECLARE ILPProperty
{
public:

	// Summary:
	//   属性回调接口定义
	// Params:
	//   <拥有者,属性ID,旧值,新值,其他参数>
	typedef BOOL(*pfunPropertyEvent)(const LPIDENTID&, const LPUINT32&, const ILPDataList&, const ILPDataList&, const ILPDataList&);

public:

	virtual ~ILPProperty() {}

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType) = 0;
	virtual BOOL LPAPI UnInit() = 0;

	virtual LPUINT32 LPAPI GetPropertyID() const  = 0;
	virtual const E_DataType GetType() const = 0;

	virtual BOOL LPAPI SetData(ILPData& poData) = 0;
	virtual BOOL LPAPI SetInt64(LPINT64 value) = 0;
	virtual BOOL LPAPI SetFloat(FLOAT value) = 0;
	virtual BOOL LPAPI SetDouble(DOUBLE value) = 0;
	virtual BOOL LPAPI SetString(const std::string& value) = 0;

	virtual ILPData& LPAPI GetData() const = 0;
	virtual LPINT64 LPAPI GetInt64() const = 0;
	virtual FLOAT LPAPI GetFloat() const = 0;
	virtual DOUBLE LPAPI GetDouble() const = 0;
	virtual const std::string& LPAPI GetString() const = 0;

	virtual BOOL LPAPI RegisterCallback(const pfunPropertyEvent& cb, LPINT32 nPriority, const ILPDataList& oCBParams) = 0;

public:

	static ILPProperty& LPAPI InvalidProperty()
	{
		return *m_poInvalidProperty;
	}

private:
	static ILPProperty* m_poInvalidProperty;
};



// Summary:
//   属性工厂接口
class DECLARE ILPPropertyFactory
{
public:

	virtual LPUINT32 LPAPI GetPropertyInstanceCount() = 0;

	virtual ILPProperty** LPAPI NewPropertyArray(LPUINT32 dwSize) = 0;
	virtual void LPAPI DeletePropertyArray(ILPProperty** & poPropertyArray, LPUINT32 dwSize) = 0;

	virtual ILPProperty* LPAPI NewProperty(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType) = 0;
	virtual void LPAPI DeleteProperty(ILPProperty* & poProperty) = 0;
};



// Summary:
//   普通属性工厂
class DECLARE LPNormalPropertyFactory : public ILPPropertyFactory
{
public:

	virtual LPUINT32 LPAPI GetPropertyInstanceCount();

	virtual ILPProperty** LPAPI NewPropertyArray(LPUINT32 dwSize);
	virtual void LPAPI DeletePropertyArray(ILPProperty** & poPropertyArray, LPUINT32 dwSize);

	virtual ILPProperty* LPAPI NewProperty(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType);
	virtual void LPAPI DeleteProperty(ILPProperty* & poProperty);
};




//end声明所处的名字空间
NS_LZPL_END

#endif