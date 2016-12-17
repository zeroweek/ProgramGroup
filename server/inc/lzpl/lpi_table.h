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



#define INVALID_TABLE_ID                     (0)




// Summary:
//   表接口
class DECLARE ILPTable
{
public:

	struct DECLARE TABLE_INDEX
	{
		std::map<LPINT64, std::vector<ILPProperty*>>::iterator iterInt64ToProVect;
		std::map<std::string, std::vector<ILPProperty*>>::iterator iterStringToProVect;

		std::multimap<LPINT64, std::vector<ILPProperty*>>::iterator muliterInt64ToProVect;
		std::multimap<std::string, std::vector<ILPProperty*>>::iterator muliterStringToProVect;
	};

	typedef TABLE_INDEX Iterator;
	typedef std::vector<TABLE_INDEX> IteratorVect;

public:

	virtual ~ILPTable() {}

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwTableID, LPUINT32 nColCount, ILPDataList& varColType) = 0;
	virtual BOOL LPAPI UnInit() = 0;

	virtual LPUINT32 GetColCount() const = 0;
	virtual LPUINT32 GetRecordCount() const = 0;
	virtual E_DataType GetColType(const LPUINT32 nCol) const = 0;

	virtual BOOL AddRecord(const ILPDataList& var) = 0;

	virtual BOOL LPAPI SetInt64(Iterator& iter, LPINT64 value) = 0;
	virtual BOOL LPAPI SetFloat(Iterator& iter, FLOAT value) = 0;
	virtual BOOL LPAPI SetDouble(Iterator& iter, DOUBLE value) = 0;
	virtual BOOL LPAPI SetString(Iterator& iter, const std::string& value) = 0;

	virtual LPINT64 LPAPI GetInt64(Iterator& iter) const = 0;
	virtual FLOAT LPAPI GetFloat(Iterator& iter) const = 0;
	virtual DOUBLE LPAPI GetDouble(Iterator& iter) const = 0;
	virtual const std::string& LPAPI GetString(Iterator& iter) const = 0;

	virtual BOOL FindInt64(const LPUINT32 nCol, const LPINT64 value, IteratorVect& vectIterRet) = 0;
	virtual BOOL FindFloat(const LPUINT32 nCol, const FLOAT value, IteratorVect& vectIterRet) = 0;
	virtual BOOL FindDouble(const LPUINT32 nCol, const DOUBLE value, IteratorVect& vectIterRet) = 0;
	virtual BOOL FindString(const LPUINT32 nCol, const std::string& value, IteratorVect& vectIterRet) = 0;

	virtual BOOL Remove(Iterator& iter) = 0;
	virtual BOOL Remove(IteratorVect& vectIter) = 0;

	virtual void Clear() = 0;
};



// Summary:
//   talbe工厂接口
class DECLARE ILPTableFactory
{
public:

	virtual LPINT32 LPAPI GetTableInstanceCount() = 0;

	virtual ILPTable* LPAPI NewTableArray(LPINT32 nSize) = 0;
	virtual void LPAPI DeleteTableArray(ILPTable* & poTableArray) = 0;

	virtual ILPTable* LPAPI NewTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, LPUINT32 nColCount, ILPDataList& varColType) = 0;
	virtual void LPAPI DeleteTable(ILPTable* & poTable) = 0;
};








//end声明所处的名字空间
NS_LZPL_END

#endif