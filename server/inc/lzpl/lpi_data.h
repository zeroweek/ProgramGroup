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
#include "lp_global.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN




// Summary:
//   ��������
enum E_DataType
{
	eDataType_Invalid = -1,
	eDataType_Int64,
	eDataType_Float,
	eDataType_Double,
	eDataType_String,
	eDataType_Object,
	eDataType_Total
};



// Summary:
//   ���ݻ�������
class DECLARE ILPData : public ILPObject
{
public:

	static LPUINT32 ms_dwDataCount;

public:

	~ILPData() {}

	virtual E_DataType LPAPI GetType() const = 0;

	virtual BOOL LPAPI SetInt64(LPINT64 value) = 0;
	virtual BOOL LPAPI SetFloat(FLOAT value) = 0;
	virtual BOOL LPAPI SetDouble(DOUBLE value) = 0;
	virtual BOOL LPAPI SetString(const std::string& value) = 0;

	virtual LPINT64 LPAPI GetInt64() const = 0;
	virtual FLOAT LPAPI GetFloat() const = 0;
	virtual DOUBLE LPAPI GetDouble() const = 0;
	virtual const std::string& LPAPI GetString() const = 0;
	
	inline BOOL operator==(const ILPData& oSrc) const
	{
		LOG_CHECK_ERROR(GetType() == oSrc.GetType());
		switch (oSrc.GetType())
		{
		case LZPL::eDataType_Int64:
			return GetInt64() == oSrc.GetInt64();
		case LZPL::eDataType_Float:
			return lpIsZeroFloat(GetFloat() - oSrc.GetFloat());
		case LZPL::eDataType_Double:
			return lpIsZeroDouble(GetDouble() - oSrc.GetDouble());
		case LZPL::eDataType_String:
			return GetString() == oSrc.GetString();
		case LZPL::eDataType_Invalid:
			return GetType() == oSrc.GetType();
		case LZPL::eDataType_Object:
		case LZPL::eDataType_Total:
		default:
			LOG_CHECK_ERROR(FALSE);
			LPASSERT(FALSE);
			break;
		}

		return FALSE;
	}

public:

	static ILPData& LPAPI InvalidData()
	{
		return *m_poInvalidData;
	}

	static ILPData* LPAPI NewData(E_DataType eType);
	static void LPAPI DeleteData(ILPData* & poData);

private:
	static ILPData* m_poInvalidData;
};



//end�������������ֿռ�
NS_LZPL_END

#endif