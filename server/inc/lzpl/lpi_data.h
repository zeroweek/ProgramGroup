//****************************************************************************/
//    author: caihy
//    date: Dec 1, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LPI_DATA_H_
#define _LPI_DATA_H_

#include "lp_base.h"
#include "lp_simplelist.h"
#include "lpi_identid.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN




// Summary:
//   数据类型
enum E_DataType
{
	eDataType_Invalid,
	eDataType_Int64,
	eDataType_Float,
	eDataType_Double,
	eDataType_String,
	eDataType_Object,
	eDataType_Total
};



// Summary:
//   数据基础类型
class DECLARE ILPData : public ILPObject
{
public:

	~ILPData() {}

	virtual E_DataType LPAPI GetType() const = 0;

	virtual BOOL LPAPI SetInt64(INT_64 value) = 0;
	virtual BOOL LPAPI SetFloat(FLOAT value) = 0;
	virtual BOOL LPAPI SetDouble(DOUBLE value) = 0;
	virtual BOOL LPAPI SetString(const std::string& value) = 0;

	inline virtual INT_64 LPAPI GetInt64() const = 0;
	virtual FLOAT LPAPI GetFloat() const = 0;
	virtual DOUBLE LPAPI GetDouble() const = 0;
	virtual const std::string& LPAPI GetString() const = 0;

public:

	static ILPData* LPAPI NewData(E_DataType eType);
	static void LPAPI DeleteData(ILPData* poData);
};





//end声明所处的名字空间
NS_LZPL_END

#endif