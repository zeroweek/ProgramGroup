//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LP_DATALIST_H_
#define _LP_DATALIST_H_

#include "lpi_datalist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN





// Summary:
//   表接口
class DECLARE LPDataList : public ILPDataList
{
public:

	struct DATA_SIMPLE_LIST_NODE : public SIMPLE_LIST_NODE
	{
		ILPData*     poData;

		DATA_SIMPLE_LIST_NODE()
		{
			poData = nullptr;
		}

		static DATA_SIMPLE_LIST_NODE* NewNode()
		{
			return new DATA_SIMPLE_LIST_NODE();
		}

		static void DeleteNode(DATA_SIMPLE_LIST_NODE* & ptNode)
		{
			SAFE_DELETE(ptNode);
		}
	};

public:

	LPDataList();
	virtual ~LPDataList();

	virtual void LPAPI Clear();
	virtual BOOL LPAPI IsEmpty() const;
	virtual UINT_32 LPAPI GetCount() const;
	virtual E_DataType LPAPI Type(const INT_32 nIndex) const;

	virtual BOOL LPAPI Add(const INT_64 value);
	virtual BOOL LPAPI Add(const FLOAT value);
	virtual BOOL LPAPI Add(const DOUBLE value);
	virtual BOOL LPAPI Add(const char* value);
	virtual BOOL LPAPI Add(const std::string& value);

	virtual INT_64 LPAPI Int64(const INT_32 nIndex) const;
	virtual FLOAT LPAPI Float(const INT_32 nIndex) const;
	virtual DOUBLE LPAPI Double(const INT_32 nIndex) const;
	virtual const std::string& LPAPI String(const INT_32 nIndex) const;

	virtual BOOL Concat(const ILPDataList& oSrc);
	virtual BOOL Append(const ILPDataList& oSrc, UINT_32 dwStart, UINT_32 dwCount);
	
	LPDataList& operator=(const LPDataList& oSrc);
	LPDataList& operator=(const ILPDataList& oSrc);

protected:

	LPSimpleList                  m_oDataList;
};








//end声明所处的名字空间
NS_LZPL_END

#endif