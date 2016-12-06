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




#define INVALID_PROPERTY                     (0)



// Summary:
//   ���Իص��ӿڶ���
typedef BOOL(*pfunPropertyEvent)(const LPIDENTID&, const UINT_32&, const std::string&, const ILPDATALIST&, const ILPDATALIST&, const ILPDATALIST&);



// Summary:
//   ���Խӿ�
class DECLARE ILPProperty
{
public:

	virtual ~ILPProperty() {}

	virtual UINT_32 LPAPI GetPropertyID() const  = 0;
	virtual const std::string& LPAPI GetPropertyName() const  = 0;
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








//end�������������ֿռ�
NS_LZPL_END

#endif