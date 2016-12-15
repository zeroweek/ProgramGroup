//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LPI_TABLE_H_
#define _LPI_TABLE_H_

#include "lpi_property.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN





// Summary:
//   ��ӿ�
class DECLARE ILPTable
{
public:

	virtual ~ILPTable() {}

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, UINT_32 dwTableID) = 0;
	virtual BOOL LPAPI UnInit() = 0;

};



// Summary:
//   talbe�����ӿ�
class DECLARE ILPTableFactory
{
public:

	virtual INT_32 LPAPI GetTableInstanceCount() = 0;

	virtual ILPTable* LPAPI NewTableArray(INT_32 nSize) = 0;
	virtual void LPAPI DeleteTableArray(ILPTable** poTable) = 0;

	virtual ILPTable* LPAPI NewTable(const LPIDENTID& oOwner, UINT_32 dwTableID) = 0;
	virtual void LPAPI DeleteTable(ILPTable** poTable) = 0;
};








//end�������������ֿռ�
NS_LZPL_END

#endif