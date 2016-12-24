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



//begin�������������ֿռ�
NS_LZPL_BEGIN





// Summary:
//   ���ݱ���
// Attention:
//   1. ��0��ֻ�������λ��ַ��������ұ��봴����key����
//   2. ���д������������У��������޸�ֵ��ֻ��ɾ��
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
	//   remove�ɹ���ʧ�ܣ������iterator���ᱻ���óɶ�Ӧ��end
	virtual BOOL LPAPI Remove(Iterator& iter);
	virtual BOOL LPAPI Remove(IteratorVect& vectIter);

	// Summary:
	//   ��ձ�����
	// Attention:
	//   ���øú�����֮ǰ���洢��iterator����Ч��ǿ�з��ʽ��δ֪
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


	LPIDENTID                                       m_oOwner;           // ӵ����
	LPUINT32                                        m_dwTableID;        // ��ID
	LPDataList                                      m_oVarColType;      // ���е�����
	LPDataList                                      m_oVarMakeIndexCol; // ��Ҫ�����������У���0��Ĭ���������������������в������޸�ֵ

	LPNormalPropertyFactory                         m_oNormalPropertyFactory;

	TableMap*                                       m_poTableMap;
};








//end�������������ֿռ�
NS_LZPL_END

#endif