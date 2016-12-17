#include "lp_table.h"
#include "lp_processerror.h"




//begin�������������ֿռ�
NS_LZPL_BEGIN



LPTable::LPTable()
{
	++ms_dwTableCount;

	m_oOwner = LPIDENTID(0, 0);
	m_dwTableID = INVALID_TABLE_ID;
	m_dwColCount = 0;
	m_oVarColType.Clear();

	m_oRecordList.Clear();
	m_poPropertyFactory = new LPNormalPropertyFactory();
	LOG_CHECK_ERROR(m_poPropertyFactory);
}

LPTable::LPTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, LPUINT32 nColCount, ILPDataList& varColType)
{
	++ms_dwTableCount;

	m_oOwner = oOwner;
	m_dwTableID = dwTableID;
	m_dwColCount = nColCount;

	m_oVarColType = varColType;

	m_oRecordList.Clear();
	m_poPropertyFactory = new LPNormalPropertyFactory();
	LOG_CHECK_ERROR(m_poPropertyFactory);
}

LPTable::~LPTable()
{
	UnInit();

	--ms_dwTableCount;
	
}

BOOL LPAPI LPTable::Init(const LPIDENTID& oOwner, LPUINT32 dwTableID, LPUINT32 nColCount, ILPDataList& varColType)
{
	m_oOwner = oOwner;
	m_dwTableID = dwTableID;
	m_dwColCount = nColCount;

	m_oVarColType = varColType;

	m_oRecordList.Clear();

	return TRUE;
}

BOOL LPAPI LPTable::UnInit()
{
	BASE_LIST_FOR_BEGIN(m_oRecordList)
	{
		ILPProperty* poPropertyArray = (ILPProperty*)ptNode;
		m_poPropertyFactory->DeletePropertyArray(poPropertyArray);
	}
	BASE_LIST_FOR_END
	m_oRecordList.Clear();

	SAFE_DELETE(m_poPropertyFactory);

	return TRUE;
}

LPUINT32 LPTable::GetColCount() const
{
	return m_dwColCount;
}

LPUINT32 LPTable::GetRecordCount() const
{
	return m_oRecordList.Size();
}

E_DataType LPTable::GetColType(const LPUINT32 nCol) const
{
	return eDataType_Invalid;
}

BOOL LPTable::AddRecord(const ILPDataList& var)
{
	return FALSE;
}

BOOL LPAPI LPTable::SetInt64(Iterator& iter, LPINT64 value)
{
	return FALSE;
}

BOOL LPAPI LPTable::SetFloat(Iterator& iter, FLOAT value)
{
	return FALSE;
}

BOOL LPAPI LPTable::SetDouble(Iterator& iter, DOUBLE value)
{
	return FALSE;
}

BOOL LPAPI LPTable::SetString(Iterator& iter, const std::string& value)
{
	return FALSE;
}

LPINT64 LPAPI LPTable::GetInt64(Iterator& iter) const
{
	return ZERO_INT;
}

FLOAT LPAPI LPTable::GetFloat(Iterator& iter) const
{
	return ZERO_FLOAT;
}

DOUBLE LPAPI LPTable::GetDouble(Iterator& iter) const
{
	return ZERO_DOUBLE;
}

const std::string& LPAPI LPTable::GetString(Iterator& iter) const
{
	return NULL_STR;
}

BOOL LPTable::FindInt64(const LPUINT32 nCol, const LPINT64 value, IteratorVect& vectIterRet)
{
	return FALSE;
}

BOOL LPTable::FindFloat(const LPUINT32 nCol, const FLOAT value, IteratorVect& vectIterRet)
{
	return FALSE;
}

BOOL LPTable::FindDouble(const LPUINT32 nCol, const DOUBLE value, IteratorVect& vectIterRet)
{
	return FALSE;
}

BOOL LPTable::FindString(const LPUINT32 nCol, const std::string& value, IteratorVect& vectIterRet)
{
	return FALSE;
}

BOOL LPTable::Remove(Iterator& iter)
{
	return FALSE;
}

BOOL LPTable::Remove(IteratorVect& vectIter)
{
	return FALSE;
}

void LPTable::Clear()
{

}

LPUINT32 LZPL::LPTable::ms_dwTableCount = 0;

//end�������������ֿռ�
NS_LZPL_END