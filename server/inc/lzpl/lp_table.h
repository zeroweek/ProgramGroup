//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LP_TABLE_H_
#define _LP_TABLE_H_

#include "lpi_table.h"
#include "lp_baselist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN





// Summary:
//		表类
class DECLARE LPTable : public ILPTable
{
public:

	static LPUINT32 ms_dwTableCount;

public:

	LPTable();
	LPTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, LPUINT32 nColCount, ILPDataList& varColType);
	~LPTable();

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwTableID, LPUINT32 nColCount, ILPDataList& varColType);
	virtual BOOL LPAPI UnInit();

	virtual LPUINT32 GetColCount() const;
	virtual LPUINT32 GetRecordCount() const;
	virtual E_DataType GetColType(const LPUINT32 nCol) const;

	virtual BOOL AddRecord(const ILPDataList& var);

	virtual BOOL LPAPI SetInt64(Iterator& iter, LPINT64 value);
	virtual BOOL LPAPI SetFloat(Iterator& iter, FLOAT value);
	virtual BOOL LPAPI SetDouble(Iterator& iter, DOUBLE value);
	virtual BOOL LPAPI SetString(Iterator& iter, const std::string& value);

	virtual LPINT64 LPAPI GetInt64(Iterator& iter) const;
	virtual FLOAT LPAPI GetFloat(Iterator& iter) const;
	virtual DOUBLE LPAPI GetDouble(Iterator& iter) const;
	virtual const std::string& LPAPI GetString(Iterator& iter) const;

	virtual BOOL FindInt64(const LPUINT32 nCol, const LPINT64 value, IteratorVect& vectIterRet);
	virtual BOOL FindFloat(const LPUINT32 nCol, const FLOAT value, IteratorVect& vectIterRet);
	virtual BOOL FindDouble(const LPUINT32 nCol, const DOUBLE value, IteratorVect& vectIterRet);
	virtual BOOL FindString(const LPUINT32 nCol, const std::string& value, IteratorVect& vectIterRet);

	virtual BOOL Remove(Iterator& iter);
	virtual BOOL Remove(IteratorVect& vectIter);

	virtual void Clear();

protected:


	LPIDENTID                     m_oOwner;
	LPUINT32                       m_dwTableID;
	LPUINT32                       m_dwColCount;
	LPDataList                    m_oVarColType;



	LPBaseList                    m_oRecordList;
	ILPPropertyFactory*           m_poPropertyFactory;
};








//end声明所处的名字空间
NS_LZPL_END

#endif