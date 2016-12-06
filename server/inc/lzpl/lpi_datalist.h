//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LPI_DATALIST_H_
#define _LPI_DATALIST_H_

#include "lp_base.h"
#include "lp_data.h"
#include "lp_simplelist.h"
#include "lpi_identid.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN




// Summary:
//   复合数据列表接口
class DECLARE ILPDATALIST
{
public:

	virtual ~ILPDATALIST() { }

	virtual void LPAPI Clear() = 0;
	virtual BOOL LPAPI IsEmpty() const = 0;
	virtual UINT_32 LPAPI GetCount() const = 0;
	virtual E_DataType LPAPI Type(const INT_32 nIndex) = 0;

	virtual BOOL LPAPI Add(const INT_64 value) = 0;
	virtual BOOL LPAPI Add(const FLOAT value) = 0;
	virtual BOOL LPAPI Add(const DOUBLE value) = 0;
	virtual BOOL LPAPI Add(const char* value) = 0;
	virtual BOOL LPAPI Add(const std::string& value) = 0;

	virtual INT_64 LPAPI Int64(const INT_32 nIndex) = 0;
	virtual FLOAT LPAPI Float(const INT_32 nIndex) = 0;
	virtual DOUBLE LPAPI Double(const INT_32 nIndex) = 0;
	virtual const std::string& LPAPI String(const INT_32 nIndex) = 0;

	inline BOOL LPAPI Compare(const INT_32 nPos, const ILPDATALIST& oSrc) const
	{
		LPASSERT(FALSE);
		return FALSE;
	}

	inline BOOL operator==(const ILPDATALIST& oSrc) const
	{
		if (oSrc.GetCount() == GetCount())
		{
			for (INT_32 i = 0; i < GetCount(); i++)
			{
				if (FALSE == Compare(i, oSrc))
				{
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	inline BOOL operator!=(const ILPDATALIST& oSrc)
	{
		return !(*this == oSrc);
	}

	inline ILPDATALIST& operator<<(const INT_8 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const UINT_8 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const INT_16 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const UINT_16 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const INT_32 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const UINT_32 value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const INT_64& value)
	{
		Add((INT_64)value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const FLOAT value)
	{
		Add(value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const DOUBLE value)
	{
		Add(value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const char* value)
	{
		Add(value);
		return *this;
	}

	inline ILPDATALIST& operator<<(const std::string& value)
	{
		Add(value);
		return *this;
	}
};








//end声明所处的名字空间
NS_LZPL_END

#endif