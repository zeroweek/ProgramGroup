//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LPI_DATALIST_H_
#define _LPI_DATALIST_H_

#include "lp_base.h"
#include "lpi_data.h"
#include "lp_simplelist.h"
#include "lpi_identid.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define PRINTF_DATA_LIST(__print_type, __datalist__) \
	for (UINT_32 i = 0; i < ##__datalist__.GetCount(); ++i)\
	{\
		E_DataType eDataType = ##__datalist__.Type(i);\
		switch (eDataType)\
		{\
			break;\
		case LZPL::eDataType_Int64:\
			##__print_type(FMT_I64, ##__datalist__.Int64(i));\
			break;\
		case LZPL::eDataType_Float:\
			##__print_type(FMT_FLT, ##__datalist__.Float(i));\
			break;\
		case LZPL::eDataType_Double:\
			##__print_type(FMT_DBE, ##__datalist__.Double(i));\
			break;\
		case LZPL::eDataType_String:\
			##__print_type(FMT_STR, ##__datalist__.String(i).c_str());\
			break;\
		case LZPL::eDataType_Object:\
		case LZPL::eDataType_Invalid:\
		case LZPL::eDataType_Total:\
		default:\
			LOG_CHECK_ERROR(FALSE);\
			break;\
		}\
	}



// Summary:
//   复合数据列表接口
class DECLARE ILPDataList
{
public:

	static ILPDataList* ms_poNullDataList;
	static const ILPDataList& NullDataList()
	{
		return *ms_poNullDataList;
	}

public:

	virtual ~ILPDataList() { }

	virtual void LPAPI Clear() = 0;
	virtual BOOL LPAPI IsEmpty() const = 0;
	virtual UINT_32 LPAPI GetCount() const = 0;
	virtual E_DataType LPAPI Type(const INT_32 nIndex) const = 0;

	virtual BOOL LPAPI Add(const INT_64 value) = 0;
	virtual BOOL LPAPI Add(const FLOAT value) = 0;
	virtual BOOL LPAPI Add(const DOUBLE value) = 0;
	virtual BOOL LPAPI Add(const char* value) = 0;
	virtual BOOL LPAPI Add(const std::string& value) = 0;

	virtual INT_64 LPAPI Int64(const INT_32 nIndex) const = 0;
	virtual FLOAT LPAPI Float(const INT_32 nIndex) const = 0;
	virtual DOUBLE LPAPI Double(const INT_32 nIndex) const = 0;
	virtual const std::string& LPAPI String(const INT_32 nIndex) const = 0;

	virtual BOOL Concat(const ILPDataList& oSrc) = 0;
	virtual BOOL Append(const ILPDataList& oSrc, UINT_32 dwStart, UINT_32 dwCount) = 0;

	inline BOOL LPAPI Compare(const UINT_32 nPos, const ILPDataList& oSrc) const
	{
		LPASSERT(FALSE);
		return FALSE;
	}

	//ILPDataList& operator=(const ILPDataList& oSrc)
	//{
	//	Clear();
	//	Append(oSrc, 0, oSrc.GetCount());

	//	return *this;
	//}

	//inline BOOL operator==(const ILPDataList& oSrc) const
	//{
	//	if (oSrc.GetCount() == GetCount())
	//	{
	//		for (INT_32 i = 0; i < GetCount(); i++)
	//		{
	//			if (FALSE == Compare(i, oSrc))
	//			{
	//				return FALSE;
	//			}
	//		}
	//	}

	//	return TRUE;
	//}

	//inline BOOL operator!=(const ILPDataList& oSrc)
	//{
	//	return !(*this == oSrc);
	//}

	inline ILPDataList& operator<<(const INT_8 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDataList& operator<<(const UINT_8 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDataList& operator<<(const INT_16 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDataList& operator<<(const UINT_16 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDataList& operator<<(const INT_32 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDataList& operator<<(const UINT_32 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDataList& operator<<(const INT_64& value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDataList& operator<<(const FLOAT value)
	{
		Add(value);
		return *this;
	}

	inline ILPDataList& operator<<(const DOUBLE value)
	{
		Add(value);
		return *this;
	}

	inline ILPDataList& operator<<(const char* value)
	{
		Add(value);
		return *this;
	}

	inline ILPDataList& operator<<(const std::string& value)
	{
		Add(value);
		return *this;
	}
};








//end声明所处的名字空间
NS_LZPL_END

#endif