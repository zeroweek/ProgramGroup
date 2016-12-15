//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LPI_TABLE_H_
#define _LPI_TABLE_H_

#include "lpi_property.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN





// Summary:
//   表接口
class DECLARE ILPTable
{
public:

	virtual ~ILPTable() {}

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, UINT_32 dwTableID) = 0;
	virtual BOOL LPAPI UnInit() = 0;

};



// Summary:
//   talbe工厂接口
class DECLARE ILPTableFactory
{
public:

	virtual INT_32 LPAPI GetTableInstanceCount() = 0;

	virtual ILPTable* LPAPI NewTableArray(INT_32 nSize) = 0;
	virtual void LPAPI DeleteTableArray(ILPTable** poTable) = 0;

	virtual ILPTable* LPAPI NewTable(const LPIDENTID& oOwner, UINT_32 dwTableID) = 0;
	virtual void LPAPI DeleteTable(ILPTable** poTable) = 0;
};








//end声明所处的名字空间
NS_LZPL_END

#endif