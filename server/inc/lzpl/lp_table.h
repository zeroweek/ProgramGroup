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
//   数据表类
// Attention:
//   1. 第0列只能是整形或字符串，并且必须创建单key索引
//   2. 所有创建了索引的列，都不能修改值，只能删除
class DECLARE LPTable : public ILPTable
{
public:

	static LPUINT32 ms_dwTableCount;

public:

	LPTable();
	LPTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol);
	~LPTable();

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol);
	virtual BOOL LPAPI UnInit();

	virtual LPUINT32 GetColCount() const { return m_oVarColType.GetCount(); }

	virtual LPUINT32 GetRecordCount() const;
	virtual E_DataType GetColType(const LPUINT32 nCol) const;
	virtual E_TableMapType GetColMapType(const LPUINT32 nCol) const;

	virtual BOOL AddRecord(const ILPDataList& var);

	virtual BOOL LPAPI SetInt64(Iterator& iter, const LPUINT32 nCol, LPINT64 value);
	virtual BOOL LPAPI SetFloat(Iterator& iter, const LPUINT32 nCol, FLOAT value);
	virtual BOOL LPAPI SetDouble(Iterator& iter, const LPUINT32 nCol, DOUBLE value);
	virtual BOOL LPAPI SetString(Iterator& iter, const LPUINT32 nCol, const std::string& value);

	virtual ILPData& LPAPI GetData(Iterator& iter, const LPUINT32 nCol) const;
	virtual LPINT64 LPAPI GetInt64(Iterator& iter, const LPUINT32 nCol) const;
	virtual FLOAT LPAPI GetFloat(Iterator& iter, const LPUINT32 nCol) const;
	virtual DOUBLE LPAPI GetDouble(Iterator& iter, const LPUINT32 nCol) const;
	virtual const std::string& LPAPI GetString(Iterator& iter, const LPUINT32 nCol) const;

	virtual BOOL LPAPI FindInt64(const LPUINT32 nCol, const LPINT64 value, IteratorVect& vectIterRet);
	virtual BOOL LPAPI FindFloat(const LPUINT32 nCol, const FLOAT value, IteratorVect& vectIterRet);
	virtual BOOL LPAPI FindDouble(const LPUINT32 nCol, const DOUBLE value, IteratorVect& vectIterRet);
	virtual BOOL LPAPI FindString(const LPUINT32 nCol, const std::string& value, IteratorVect& vectIterRet);

	// Summary:
	//   remove成功或失败，传入的iterator都会被设置成对应的end
	virtual BOOL LPAPI Remove(Iterator& iter);
	virtual BOOL LPAPI Remove(IteratorVect& vectIter);

	// Summary:
	//   清空表数据
	// Attention:
	//   调用该函数后，之前所存储的iterator都无效，强行访问结果未知
	virtual void LPAPI Clear();

private:

	BOOL InitTableMap();
	BOOL InsertIntoTableMap(PropertyVect& vectProperty);
	BOOL IsColCanUpdate(LPUINT32 nCol) { return GetColMapType(nCol) == eTableMapType_NotMap ? TRUE : FALSE; }

	void ResetRecordIterator(Iterator& iter);

	ILPProperty& GetProperty(Iterator& iter, const LPUINT32 nCol);
	ILPProperty& GetPropertyConst(Iterator& iter, const LPUINT32 nCol) const;

	BOOL LPAPI FindDataFromColMap(const LPUINT32 nCol, const ILPData& value, IteratorVect& vectIterRet);
	BOOL LPAPI FindDataFromDefaultMap(const LPUINT32 nCol, const ILPData& value, IteratorVect& vectIterRet);

	BOOL LPAPI RemoveFromDefaultMap(ILPData& poData);
	BOOL LPAPI RemoveFromNotDefaultMap(PropertyVect& vectProperty);

protected:


	LPIDENTID                                       m_oOwner;           // 拥有者
	LPUINT32                                        m_dwTableID;        // 表ID
	LPDataList                                      m_oVarColType;      // 各列的类型
	LPDataList                                      m_oVarMakeIndexCol; // 需要设置索引的列（第0列默认有索引），有索引的列不允许修改值

	LPNormalPropertyFactory                         m_oNormalPropertyFactory;

	TableMap*                                       m_poTableMap;
};








//end声明所处的名字空间
NS_LZPL_END

#endif