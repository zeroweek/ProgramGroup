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



//begin�������������ֿռ�
NS_LZPL_BEGIN




#define INVALID_PROPERTY_ID                     (0)



// Summary:
//   ���Իص��ӿڶ���
typedef BOOL(*pfunPropertyEvent)(const LPIDENTID&, const LPUINT32&, const ILPDataList&, const ILPDataList&, const ILPDataList&);



// Summary:
//   ���Խӿ�
class DECLARE ILPProperty
{
public:

	virtual ~ILPProperty() {}

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType) = 0;
	virtual BOOL LPAPI UnInit() = 0;

	virtual LPUINT32 LPAPI GetPropertyID() const  = 0;
	virtual const E_DataType GetType() const = 0;

	virtual BOOL LPAPI SetInt64(LPINT64 value) = 0;
	virtual BOOL LPAPI SetFloat(FLOAT value) = 0;
	virtual BOOL LPAPI SetDouble(DOUBLE value) = 0;
	virtual BOOL LPAPI SetString(const std::string& value) = 0;

	virtual LPINT64 LPAPI GetInt64() const = 0;
	virtual FLOAT LPAPI GetFloat() const = 0;
	virtual DOUBLE LPAPI GetDouble() const = 0;
	virtual const std::string& LPAPI GetString() const = 0;

	virtual BOOL LPAPI RegisterCallback(const pfunPropertyEvent& cb, LPINT32 nPriority, const ILPDataList& vars) = 0;
};



// Summary:
//   ���Թ����ӿ�
class DECLARE ILPPropertyFactory
{
public:

	virtual LPUINT32 LPAPI GetPropertyInstanceCount() = 0;

	virtual ILPProperty* LPAPI NewPropertyArray(LPUINT32 dwSize) = 0;
	virtual void LPAPI DeletePropertyArray(ILPProperty* & poProperty) = 0;

	virtual ILPProperty* LPAPI NewProperty(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType) = 0;
	virtual void LPAPI DeleteProperty(ILPProperty* & poProperty) = 0;
};



// Summary:
//   ��ͨ���Թ���
class DECLARE LPNormalPropertyFactory : public ILPPropertyFactory
{
public:

	virtual LPUINT32 LPAPI GetPropertyInstanceCount();
	
	virtual ILPProperty* LPAPI NewPropertyArray(LPUINT32 dwSize);
	virtual void LPAPI DeletePropertyArray(ILPProperty* & poProperty);

	virtual ILPProperty* LPAPI NewProperty(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType);
	virtual void LPAPI DeleteProperty(ILPProperty* & poProperty);
};




//end�������������ֿռ�
NS_LZPL_END

#endif