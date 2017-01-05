#include "lp_table.h"
#include "lp_processerror.h"




//begin声明所处的名字空间
NS_LZPL_BEGIN



LPTable::LPTable()
{
	++ms_dwTableCount;

	m_oOwner = LPIDENTID(0, 0);
	m_dwTableID = INVALID_TABLE_ID;
	m_oVarColType.Clear();
	m_oVarMakeIndexCol.Clear();
	m_poTableMap = nullptr;
	m_poCallbackList = nullptr;
}

LPTable::LPTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol)
{
	LPINT32 nResult = FALSE;

	++ms_dwTableCount;

	nResult = Init(oOwner, dwTableID, varColType, varMakeIndexCol);
	LOG_PROCESS_ERROR(nResult);

Exit0:
	return;
}

LPTable::~LPTable()
{
	UnInit();

	--ms_dwTableCount;
	
}

BOOL LPAPI LPTable::Init(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol)
{
	LPINT32 nResult = FALSE;

	m_oOwner = oOwner;
	m_dwTableID = dwTableID;
	m_oVarColType = varColType;
	m_oVarMakeIndexCol = varMakeIndexCol;

	//第0列只能是整形或字符串，并且必须创建单key索引
	LOG_PROCESS_ERROR(m_oVarColType.GetCount() > 0 && m_oVarMakeIndexCol.GetCount() == m_oVarColType.GetCount());
	LOG_PROCESS_ERROR(GetColType(0) == eDataType_Int64 || GetColType(0) == eDataType_String);
	LOG_PROCESS_ERROR(GetColMapType(0) == eTableMapType_MapInt64 || GetColMapType(0) == eTableMapType_MapString);

	nResult = InitTableMap();
	LOG_PROCESS_ERROR(nResult);

	m_poCallbackList = nullptr;

	return TRUE;
Exit0:

	UnInit();
	return FALSE;
}

BOOL LPAPI LPTable::UnInit()
{
	Clear();
	
	if (m_poCallbackList != nullptr)
	{
		SIMPLE_LIST_FOR_BEGIN((*m_poCallbackList))
		{
			LPTableCB* poTableCB = (LPTableCB*)ptNode;
			LPTableCB::DeleteTableCB(poTableCB);
		}
		SIMPLE_LIST_FOR_END
		(*m_poCallbackList).Clear();
		SAFE_DELETE(m_poCallbackList);
	}

	LOG_CHECK_ERROR(m_poTableMap != nullptr);
	for (LPUINT32 i = 0; m_poTableMap != nullptr && i < m_oVarMakeIndexCol.GetCount(); i++)
	{
		SAFE_DELETE(m_poTableMap[i].PIntMap);
		SAFE_DELETE(m_poTableMap[i].PStrMap);
		SAFE_DELETE(m_poTableMap[i].PIntMulmap);
		SAFE_DELETE(m_poTableMap[i].PStrMulmap);
	}

	SAFE_DELETE_SZ(m_poTableMap);
	return TRUE;
}

LPUINT32 LPTable::GetRecordCount() const
{
	LOG_PROCESS_ERROR(m_poTableMap != nullptr);
	LOG_PROCESS_ERROR(m_oVarColType.GetCount() > 0 &&m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());

	switch (GetColMapType(0))
	{
	case LZPL::eTableMapType_MapInt64:
			{
				LOG_PROCESS_ERROR(m_poTableMap[0].PIntMap != nullptr);
				return (LPUINT32)m_poTableMap[0].PIntMap->size();
			}
	case LZPL::eTableMapType_MapString:
			{
				LOG_PROCESS_ERROR(m_poTableMap[0].PStrMap != nullptr);
				return (LPUINT32)m_poTableMap[0].PStrMap->size();
			}
	case LZPL::eTableMapType_NotMap:
	case LZPL::eTableMapType_MulmapInt64:
	case LZPL::eTableMapType_MulmapString:
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

Exit0:
	return 0;
}

E_DataType LPTable::GetColType(const LPUINT32 dwCol) const
{
	LOG_PROCESS_ERROR(dwCol < m_oVarColType.GetCount());
	LOG_PROCESS_ERROR(m_oVarColType.Type(dwCol) == eDataType_Int64);
	
	return (E_DataType)m_oVarColType.Int64(dwCol);
Exit0:
	return eDataType_Invalid;
}

E_TableMapType LPTable::GetColMapType(const LPUINT32 dwCol) const
{
	LOG_PROCESS_ERROR(dwCol < m_oVarMakeIndexCol.GetCount());
	LOG_PROCESS_ERROR(m_oVarMakeIndexCol.Type(dwCol) == eDataType_Int64);

	return (E_TableMapType)m_oVarMakeIndexCol.Int64(dwCol);
Exit0:
	return eTableMapType_Invalid;
}

BOOL LPTable::AddRecord(const ILPDataList& var)
{
	LPINT32 nResult = FALSE;
	Iterator recordIteratorRet;
	PropertyVect vectProperty = nullptr;
	LPUINT32 dwColCount = m_oVarColType.GetCount();

	LOG_PROCESS_ERROR(var.GetCount() == dwColCount);
	LOG_PROCESS_ERROR(dwColCount > 0 && dwColCount == m_oVarMakeIndexCol.GetCount());
	
	vectProperty = LPNormalPropertyFactory::Factory().NewPropertyArray(dwColCount);
	LOG_PROCESS_ERROR(vectProperty != nullptr);
	for (LPUINT32 i = 0; i < dwColCount; ++i)
	{
		LOG_PROCESS_ERROR(vectProperty[i] != nullptr);
		nResult = vectProperty[i]->Init(m_oOwner, i + 1, GetColType(i));
		LOG_PROCESS_ERROR(nResult);

		nResult = vectProperty[i]->SetData(var.Data(i));
		LOG_PROCESS_ERROR(nResult);
	}

	nResult = InsertIntoTableMap(vectProperty, recordIteratorRet);
	LOG_PROCESS_ERROR(nResult);

	OnEventHandler(eTableOptType_Add, recordIteratorRet, 0, ILPDataList::NullDataList(), var);

	return TRUE;
Exit0:

	LPNormalPropertyFactory::Factory().DeletePropertyArray(vectProperty, dwColCount);
	return FALSE;
}

BOOL LPAPI LPTable::SetInt64(Iterator& iter, const LPUINT32 dwCol, LPINT64 value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	ILPProperty& pProperty = GetProperty(iter, dwCol);
	LPINT64 oldValue = pProperty.GetInt64();

	LOG_PROCESS_ERROR(IsColCanUpdate(dwCol));

	nResult = pProperty.SetInt64(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << oldValue;
	oNewVals << pProperty.GetInt64();
	OnEventHandler(eTableOptType_Update, iter, dwCol, oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::SetFloat(Iterator& iter, const LPUINT32 dwCol, FLOAT value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	ILPProperty& pProperty = GetProperty(iter, dwCol);
	FLOAT oldValue = pProperty.GetFloat();

	nResult = pProperty.SetFloat(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << oldValue;
	oNewVals << pProperty.GetFloat();
	OnEventHandler(eTableOptType_Update, iter, dwCol, oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::SetDouble(Iterator& iter, const LPUINT32 dwCol, DOUBLE value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	ILPProperty& pProperty = GetProperty(iter, dwCol);
	DOUBLE oldValue = pProperty.GetDouble();

	nResult = pProperty.SetDouble(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << oldValue;
	oNewVals << pProperty.GetDouble();
	OnEventHandler(eTableOptType_Update, iter, dwCol, oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::SetString(Iterator& iter, const LPUINT32 dwCol, const std::string& value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	ILPProperty& pProperty = GetProperty(iter, dwCol);
	std::string oldValue = pProperty.GetString();

	LOG_PROCESS_ERROR(IsColCanUpdate(dwCol));

	nResult = pProperty.SetString(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << oldValue;
	oNewVals << pProperty.GetString();
	OnEventHandler(eTableOptType_Update, iter, dwCol, oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

ILPData& LPAPI LPTable::GetData(Iterator& iter, const LPUINT32 dwCol) const
{
	return GetPropertyConst(iter, dwCol).GetData();
}

LPINT64 LPAPI LPTable::GetInt64(Iterator& iter, const LPUINT32 dwCol) const
{
	return GetPropertyConst(iter, dwCol).GetInt64();
}

FLOAT LPAPI LPTable::GetFloat(Iterator& iter, const LPUINT32 dwCol) const
{
	return GetPropertyConst(iter, dwCol).GetFloat();
}

DOUBLE LPAPI LPTable::GetDouble(Iterator& iter, const LPUINT32 dwCol) const
{
	return GetPropertyConst(iter, dwCol).GetDouble();
}

const std::string& LPAPI LPTable::GetString(Iterator& iter, const LPUINT32 dwCol) const
{
	return GetPropertyConst(iter, dwCol).GetString();
}

BOOL LPAPI LPTable::FindInt64(const LPUINT32 dwCol, const LPINT64 value, IteratorVect& vectIterRet)
{
	LPINT32 nResult = FALSE;

	vectIterRet.clear();

	ILPData* poData = ILPData::NewData(eDataType_Int64);
	LOG_PROCESS_ERROR(poData != nullptr);
	nResult = poData->SetInt64(value);
	LOG_PROCESS_ERROR(nResult);

	nResult = FindDataFromColMap(dwCol, *poData, vectIterRet);
	LOG_PROCESS_ERROR(nResult);

	nResult = TRUE;

Exit0:
	ILPData::DeleteData(poData);
	return nResult;
}

BOOL LPAPI LPTable::FindFloat(const LPUINT32 dwCol, const FLOAT value, IteratorVect& vectIterRet)
{
	LPINT32 nResult = FALSE;

	vectIterRet.clear();

	ILPData* poData = ILPData::NewData(eDataType_Float);
	LOG_PROCESS_ERROR(poData != nullptr);
	nResult = poData->SetFloat(value);
	LOG_PROCESS_ERROR(nResult);

	nResult = FindDataFromColMap(dwCol, *poData, vectIterRet);
	LOG_PROCESS_ERROR(nResult);

	nResult = TRUE;

Exit0:
	ILPData::DeleteData(poData);
	return nResult;
}

BOOL LPAPI LPTable::FindDouble(const LPUINT32 dwCol, const DOUBLE value, IteratorVect& vectIterRet)
{
	LPINT32 nResult = FALSE;

	vectIterRet.clear();

	ILPData* poData = ILPData::NewData(eDataType_Double);
	LOG_PROCESS_ERROR(poData != nullptr);
	nResult = poData->SetDouble(value);
	LOG_PROCESS_ERROR(nResult);

	nResult = FindDataFromColMap(dwCol, *poData, vectIterRet);
	LOG_PROCESS_ERROR(nResult);

	nResult = TRUE;

Exit0:
	ILPData::DeleteData(poData);
	return nResult;
}

BOOL LPAPI LPTable::FindString(const LPUINT32 dwCol, const std::string& value, IteratorVect& vectIterRet)
{
	LPINT32 nResult = FALSE;

	vectIterRet.clear();

	ILPData* poData = ILPData::NewData(eDataType_String);
	LOG_PROCESS_ERROR(poData != nullptr);
	nResult = poData->SetString(value);
	LOG_PROCESS_ERROR(nResult);

	nResult = FindDataFromColMap(dwCol, *poData, vectIterRet);
	LOG_PROCESS_ERROR(nResult);

	nResult = TRUE;

Exit0:
	ILPData::DeleteData(poData);
	return nResult;
}

BOOL LPAPI LPTable::Remove(Iterator& iter)
{
	LPINT32 nResult = FALSE;
	LPDataList oVals;
	PropertyVect vectProperty = nullptr;
	
	LOG_PROCESS_ERROR(m_poTableMap != nullptr);

	switch (GetColMapType(iter.MapIndexCol))
	{
	case LZPL::eTableMapType_MapInt64:
		{
			LOG_PROCESS_ERROR(m_poTableMap[iter.MapIndexCol].PIntMap != nullptr);
			LOG_PROCESS_ERROR(iter.PIntMapIter != nullptr);
			vectProperty = (*iter.PIntMapIter)->second;
		}
		break;
	case LZPL::eTableMapType_MapString:
		{
			LOG_PROCESS_ERROR(m_poTableMap[iter.MapIndexCol].PStrMap != nullptr);
			LOG_PROCESS_ERROR(iter.PStrMapIter != nullptr);
			vectProperty = (*iter.PStrMapIter)->second;
		}
		break;
	case LZPL::eTableMapType_MulmapInt64:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(iter.MapIndexCol != 0);

			LOG_PROCESS_ERROR(m_poTableMap[iter.MapIndexCol].PIntMulmap != nullptr);
			LOG_PROCESS_ERROR(iter.PIntMulmapIter != nullptr);
			vectProperty = (*iter.PIntMulmapIter)->second;
		}
		break;
	case LZPL::eTableMapType_MulmapString:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(iter.MapIndexCol != 0);

			LOG_PROCESS_ERROR(m_poTableMap[iter.MapIndexCol].PStrMulmap != nullptr);
			LOG_PROCESS_ERROR(iter.PStrMulmapIter != nullptr);
			vectProperty = (*iter.PStrMulmapIter)->second;
		}
		break;
	case LZPL::eTableMapType_NotMap:
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

	LOG_PROCESS_ERROR(vectProperty != nullptr);
	if (IsCallbackListEmpty() == FALSE)
	{
		for (LPUINT32 i = 0; i < GetColCount(); i++)
		{
			nResult = oVals.AddData(vectProperty[i]->GetData());
			LOG_PROCESS_ERROR(nResult);
		}
		OnEventHandler(eTableOptType_Del, iter, 0, oVals, ILPDataList::NullDataList());
	}

	nResult = RemoveFromNotDefaultMap(vectProperty);
	LOG_CHECK_ERROR(nResult);

	ILPData& poData = vectProperty[0]->GetData();
	nResult = RemoveFromDefaultMap(poData);
	LOG_CHECK_ERROR(nResult);

	ResetRecordIterator(iter);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::Remove(IteratorVect& vectIter)
{
	LPINT32 nResult = FALSE;

	for (size_t i = 0; i < vectIter.size(); i++)
	{
		nResult = Remove(vectIter[i]);
		LOG_CHECK_ERROR(nResult);
	}

	return TRUE;
}

void LPAPI LPTable::Clear()
{
	LOG_PROCESS_ERROR(m_poTableMap != nullptr);
	LOG_PROCESS_ERROR(m_oVarColType.GetCount() > 0 && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());

	switch (GetColMapType(0))
	{
	case LZPL::eTableMapType_MapInt64:
		{
			LOG_PROCESS_ERROR(m_poTableMap[0].PIntMap != nullptr);
			RecordIntKeyMap::iterator doit;
			RecordIntKeyMap::iterator it = m_poTableMap[0].PIntMap->begin();
			for ( ; it != m_poTableMap[0].PIntMap->end(); )
			{
				doit = it;
				++it;

				LPNormalPropertyFactory::Factory().DeletePropertyArray(doit->second, m_oVarColType.GetCount());
				m_poTableMap[0].PIntMap->erase(doit);
			}
			m_poTableMap[0].PIntMap->clear();
		}
		break;
	case LZPL::eTableMapType_MapString:
		{
			LOG_PROCESS_ERROR(m_poTableMap[0].PStrMap != nullptr);
			RecordStrKeyMap::iterator doit;
			RecordStrKeyMap::iterator it = m_poTableMap[0].PStrMap->begin();
			for ( ; it != m_poTableMap[0].PStrMap->end(); )
			{
				doit = it;
				++it;

				LPNormalPropertyFactory::Factory().DeletePropertyArray(doit->second, m_oVarColType.GetCount());
				m_poTableMap[0].PStrMap->erase(doit);
			}
			m_poTableMap[0].PStrMap->clear();
		}
		break;
	case LZPL::eTableMapType_NotMap:
	case LZPL::eTableMapType_MulmapInt64:
	case LZPL::eTableMapType_MulmapString:
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

	for (LPUINT32 i = 0; i < m_oVarMakeIndexCol.GetCount(); i++)
	{
		if (m_poTableMap[i].PIntMap != nullptr) { m_poTableMap[i].PIntMap->clear(); }
		if (m_poTableMap[i].PStrMap != nullptr) { m_poTableMap[i].PStrMap->clear(); }
		if (m_poTableMap[i].PIntMulmap != nullptr) { m_poTableMap[i].PIntMulmap->clear(); }
		if (m_poTableMap[i].PStrMulmap != nullptr) { m_poTableMap[i].PStrMulmap->clear(); }
	}

Exit0:
	return;
}

BOOL LPAPI LPTable::RegisterCallback(const pfunTableEvent& cb, LPINT32 nPriority, const ILPDataList& oCBParams)
{
	LPINT32 nResult = FALSE;
	BOOL bInsert = FALSE;
	LPTableCB* poNewTableCB = LPTableCB::NewTableCB(nPriority, cb, oCBParams);

	if (m_poCallbackList == nullptr)
	{
		m_poCallbackList = new LPSimpleList();
		LOG_PROCESS_ERROR(m_poCallbackList);
	}

	SIMPLE_LIST_FOR_BEGIN((*m_poCallbackList))
	{
		LPTableCB* poTableCB = (LPTableCB*)ptNode;
		if (poNewTableCB->m_nPriority >= poTableCB->m_nPriority)
		{
			nResult = (*m_poCallbackList).InsertBefore(poNewTableCB, ptNode);
			LOG_PROCESS_ERROR(nResult);

			bInsert = TRUE;
			break;
		}
	}
	SIMPLE_LIST_FOR_END

	if (bInsert == FALSE)
	{
		nResult = (*m_poCallbackList).Append(poNewTableCB);
		LOG_PROCESS_ERROR(nResult);
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPTable::InitTableMap()
{
	LOG_PROCESS_ERROR(m_oVarColType.GetCount() > 0 && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());
	m_poTableMap = new TableMap[m_oVarColType.GetCount()];
	LOG_PROCESS_ERROR(m_poTableMap != nullptr);

	for (LPUINT32 i = 0; i < m_oVarColType.GetCount(); ++i)
	{
		m_poTableMap[i].PIntMap = nullptr;
		m_poTableMap[i].PIntMulmap = nullptr;
		m_poTableMap[i].PStrMap = nullptr;
		m_poTableMap[i].PStrMulmap = nullptr;

		switch (GetColMapType(i))
		{
		case LZPL::eTableMapType_NotMap:
			//第0列必须有索引
			LOG_PROCESS_ERROR(i != 0);
			break;
		case LZPL::eTableMapType_MapInt64:
			{
				m_poTableMap[i].PIntMap = new RecordIntKeyMap();
				LOG_PROCESS_ERROR(m_poTableMap[i].PIntMap != nullptr);
			}
			break;
		case LZPL::eTableMapType_MapString:
			{
				m_poTableMap[i].PStrMap = new RecordStrKeyMap();
				LOG_PROCESS_ERROR(m_poTableMap[i].PStrMap != nullptr);
			}
			break;
		case LZPL::eTableMapType_MulmapInt64:
			{
				//第0列为单key索引
				LOG_PROCESS_ERROR(i != 0);
				m_poTableMap[i].PIntMulmap = new RecordIntKeyMulmap();
				LOG_PROCESS_ERROR(m_poTableMap[i].PIntMulmap != nullptr);
			}
			break;
		case LZPL::eTableMapType_MulmapString:
			{
				//第0列为单key索引
				LOG_PROCESS_ERROR(i != 0);
				m_poTableMap[i].PStrMulmap = new RecordStrKeyMulmap();
				LOG_PROCESS_ERROR(m_poTableMap[i].PStrMulmap != nullptr);
			}
			break;
		case LZPL::eTableMapType_Invalid:
		case LZPL::eTableMapType_Total:
		default:
			LOG_CHECK_ERROR(FALSE);
			LPASSERT(FALSE);
			break;
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPTable::InsertIntoTableMap(PropertyVect& vectProperty, Iterator& recordIteratorRet)
{
	LPINT32 nResult = FALSE;

	LOG_PROCESS_ERROR(m_poTableMap != nullptr);
	LOG_PROCESS_ERROR(m_oVarColType.GetCount() > 0 && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());

	for (LPUINT32 i = 0; i < m_oVarMakeIndexCol.GetCount(); ++i)
	{
		switch (GetColMapType(i))
		{
		case LZPL::eTableMapType_NotMap:
			//第0列必须有索引
			LOG_PROCESS_ERROR(i != 0);
			break;
		case LZPL::eTableMapType_MapInt64:
			{
				LOG_PROCESS_ERROR(m_poTableMap[i].PIntMap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_Int64);
				std::pair<RecordIntKeyMap::iterator, bool> InsRet = m_poTableMap[i].PIntMap->insert(std::make_pair(vectProperty[i]->GetInt64(), vectProperty));
				LOG_PROCESS_ERROR(InsRet.second == true);

				//返回第0列的索引
				if (i == 0)
				{
					ResetRecordIterator(recordIteratorRet);
					recordIteratorRet.MapIndexCol = 0;
					recordIteratorRet.PIntMapIter = std::make_shared<RecordIntKeyMap::iterator>();
					*recordIteratorRet.PIntMapIter = InsRet.first;
				}
			}
			break;
		case LZPL::eTableMapType_MapString:
			{
				LOG_PROCESS_ERROR(m_poTableMap[i].PStrMap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_String);
				std::pair<RecordStrKeyMap::iterator, bool> InsRet = m_poTableMap[i].PStrMap->insert(std::make_pair(vectProperty[i]->GetString(), vectProperty));
				LOG_PROCESS_ERROR(InsRet.second == true);

				//返回第0列的索引
				if (i == 0)
				{
					ResetRecordIterator(recordIteratorRet);
					recordIteratorRet.MapIndexCol = 0;
					recordIteratorRet.PStrMapIter = std::make_shared<RecordStrKeyMap::iterator>();
					*recordIteratorRet.PStrMapIter = InsRet.first;
				}
			}
			break;
		case LZPL::eTableMapType_MulmapInt64:
			{
				//第0列为单key索引
				LOG_PROCESS_ERROR(i != 0);
				LOG_PROCESS_ERROR(m_poTableMap[i].PIntMulmap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_Int64);
				m_poTableMap[i].PIntMulmap->insert(std::make_pair(vectProperty[i]->GetInt64(), vectProperty));
			}
			break;
		case LZPL::eTableMapType_MulmapString:
			{
				//第0列为单key索引
				LOG_PROCESS_ERROR(i != 0);
				LOG_PROCESS_ERROR(m_poTableMap[i].PStrMulmap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_String);
				m_poTableMap[i].PStrMulmap->insert(std::make_pair(vectProperty[i]->GetString(), vectProperty));
			}
			break;
		case LZPL::eTableMapType_Invalid:
		case LZPL::eTableMapType_Total:
		default:
			LOG_CHECK_ERROR(FALSE);
			LPASSERT(FALSE);
			break;
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

void LPTable::ResetRecordIterator(Iterator& iter)
{
	iter.MapIndexCol = 0;
	iter.PIntMapIter = nullptr;
	iter.PIntMulmapIter = nullptr;
	iter.PStrMapIter = nullptr;
	iter.PStrMulmapIter = nullptr;
}

ILPProperty& LPTable::GetProperty(Iterator& iter, const LPUINT32 dwCol)
{
	LOG_PROCESS_ERROR(iter.MapIndexCol < m_oVarColType.GetCount());
	LOG_PROCESS_ERROR(dwCol < m_oVarColType.GetCount() && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());

	switch (GetColMapType(iter.MapIndexCol))
	{
	case LZPL::eTableMapType_MapInt64:
		{
			LOG_PROCESS_ERROR(iter.PIntMapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PIntMapIter)->second != nullptr);
			return *((*iter.PIntMapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_MapString:
		{
			LOG_PROCESS_ERROR(iter.PStrMapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PStrMapIter)->second != nullptr);
			return *((*iter.PStrMapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_MulmapInt64:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(iter.MapIndexCol != 0);
			LOG_PROCESS_ERROR(iter.PIntMulmapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PIntMulmapIter)->second != nullptr);
			return *((*iter.PIntMulmapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_MulmapString:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(iter.MapIndexCol != 0);
			LOG_PROCESS_ERROR(iter.PStrMulmapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PStrMulmapIter)->second != nullptr);
			return *((*iter.PStrMulmapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_NotMap:
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

Exit0:
	return ILPProperty::InvalidProperty();
}

ILPProperty& LPTable::GetPropertyConst(Iterator& iter, const LPUINT32 dwCol) const
{
	LOG_PROCESS_ERROR(iter.MapIndexCol < m_oVarColType.GetCount());
	LOG_PROCESS_ERROR(dwCol < m_oVarColType.GetCount() && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());

	switch (GetColMapType(iter.MapIndexCol))
	{
	case LZPL::eTableMapType_MapInt64:
		{
			LOG_PROCESS_ERROR(iter.PIntMapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PIntMapIter)->second != nullptr);
			return *((*iter.PIntMapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_MapString:
		{
			LOG_PROCESS_ERROR(iter.PStrMapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PStrMapIter)->second != nullptr);
			return *((*iter.PStrMapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_MulmapInt64:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(iter.MapIndexCol != 0);
			LOG_PROCESS_ERROR(iter.PIntMulmapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PIntMulmapIter)->second != nullptr);
			return *((*iter.PIntMulmapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_MulmapString:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(iter.MapIndexCol != 0);
			LOG_PROCESS_ERROR(iter.PStrMulmapIter != nullptr);
			LOG_PROCESS_ERROR((*iter.PStrMulmapIter)->second != nullptr);
			return *((*iter.PStrMulmapIter)->second[dwCol]);
		}
		break;
	case LZPL::eTableMapType_NotMap:
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

Exit0:
	return ILPProperty::InvalidProperty();
}

BOOL LPAPI LPTable::FindDataFromColMap(const LPUINT32 dwCol, const ILPData& value, IteratorVect& vectIterRet)
{
	LPINT32 nResult = FALSE;
	Iterator recordIterator;

	LOG_PROCESS_ERROR(m_poTableMap != nullptr);
	LOG_PROCESS_ERROR(dwCol < m_oVarColType.GetCount() && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());
	LOG_PROCESS_ERROR(value.GetType() == GetColType(dwCol));

	switch (GetColMapType(dwCol))
	{
	case LZPL::eTableMapType_NotMap:
		{
			//第0列必须有索引
			LOG_PROCESS_ERROR(dwCol != 0);
			nResult = FindDataFromDefaultMap(dwCol, value, vectIterRet);
			LOG_PROCESS_ERROR(nResult);
		}
		break;
	case LZPL::eTableMapType_MapInt64:
		{
			LOG_PROCESS_ERROR(m_poTableMap[dwCol].PIntMap != nullptr);
			LOG_PROCESS_ERROR(GetColType(dwCol) == eDataType_Int64);
			RecordIntKeyMap::iterator fit = m_poTableMap[dwCol].PIntMap->find(value.GetInt64());
			if (fit != m_poTableMap[dwCol].PIntMap->end())
			{
				ResetRecordIterator(recordIterator);
				recordIterator.MapIndexCol = dwCol;
				recordIterator.PIntMapIter = std::make_shared<RecordIntKeyMap::iterator>();
				*recordIterator.PIntMapIter = fit;
				vectIterRet.push_back(recordIterator);
			}
		}
		break;
	case LZPL::eTableMapType_MapString:
		{
			LOG_PROCESS_ERROR(m_poTableMap[dwCol].PStrMap != nullptr);
			LOG_PROCESS_ERROR(GetColType(dwCol) == eDataType_String);
			RecordStrKeyMap::iterator fit = m_poTableMap[dwCol].PStrMap->find(value.GetString());
			if (fit != m_poTableMap[dwCol].PStrMap->end())
			{
				ResetRecordIterator(recordIterator);
				recordIterator.MapIndexCol = dwCol;
				recordIterator.PStrMapIter = std::make_shared<RecordStrKeyMap::iterator>();
				*recordIterator.PStrMapIter = fit;
				vectIterRet.push_back(recordIterator);
			}
		}
		break;
	case LZPL::eTableMapType_MulmapInt64:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(dwCol != 0);
			LOG_PROCESS_ERROR(m_poTableMap[dwCol].PIntMulmap != nullptr);
			LOG_PROCESS_ERROR(GetColType(dwCol) == eDataType_Int64);
			std::pair<RecordIntKeyMulmap::iterator, RecordIntKeyMulmap::iterator> retFits = m_poTableMap[dwCol].PIntMulmap->equal_range(value.GetInt64());
			for (RecordIntKeyMulmap::iterator it = retFits.first; it != retFits.second; ++it)
			{
				ResetRecordIterator(recordIterator);
				recordIterator.MapIndexCol = dwCol;
				recordIterator.PIntMulmapIter = std::make_shared<RecordIntKeyMulmap::iterator>();
				*recordIterator.PIntMulmapIter = it;
				vectIterRet.push_back(recordIterator);
			}
		}
		break;
	case LZPL::eTableMapType_MulmapString:
		{
			//第0列为单key索引
			LOG_PROCESS_ERROR(dwCol != 0);
			LOG_PROCESS_ERROR(m_poTableMap[dwCol].PStrMulmap != nullptr);
			LOG_PROCESS_ERROR(GetColType(dwCol) == eDataType_String);
			std::pair<RecordStrKeyMulmap::iterator, RecordStrKeyMulmap::iterator> retFits = m_poTableMap[dwCol].PStrMulmap->equal_range(value.GetString());
			for (RecordStrKeyMulmap::iterator it = retFits.first; it != retFits.second; ++it)
			{
				ResetRecordIterator(recordIterator);
				recordIterator.MapIndexCol = dwCol;
				recordIterator.PStrMulmapIter = std::make_shared<RecordStrKeyMulmap::iterator>();
				*recordIterator.PStrMulmapIter = it;
				vectIterRet.push_back(recordIterator);
			}
		}
		break;
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::FindDataFromDefaultMap(const LPUINT32 dwCol, const ILPData& value, IteratorVect& vectIterRet)
{
	Iterator recordIterator;

	LOG_PROCESS_ERROR(m_poTableMap != nullptr);
	LOG_PROCESS_ERROR(dwCol < m_oVarColType.GetCount() && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());
	LOG_PROCESS_ERROR(value.GetType() == GetColType(dwCol));

	switch (GetColMapType(0))
	{
	case LZPL::eTableMapType_MapInt64:
		{
			LOG_PROCESS_ERROR(m_poTableMap[0].PIntMap != nullptr);
			for (RecordIntKeyMap::iterator it = m_poTableMap[0].PIntMap->begin(); it != m_poTableMap[0].PIntMap->end(); ++it)
			{
				if (value == it->second[dwCol]->GetData())
				{
					ResetRecordIterator(recordIterator);
					recordIterator.MapIndexCol = 0;
					recordIterator.PIntMapIter = std::make_shared<RecordIntKeyMap::iterator>();
					*recordIterator.PIntMapIter = it;
					vectIterRet.push_back(recordIterator);
				}
			}
		}
		break;
	case LZPL::eTableMapType_MapString:
		{
			LOG_PROCESS_ERROR(m_poTableMap[0].PStrMap != nullptr);
			for (RecordStrKeyMap::iterator it = m_poTableMap[0].PStrMap->begin(); it != m_poTableMap[0].PStrMap->end(); ++it)
			{
				if (value == it->second[dwCol]->GetData())
				{
					ResetRecordIterator(recordIterator);
					recordIterator.MapIndexCol = 0;
					recordIterator.PStrMapIter = std::make_shared<RecordStrKeyMap::iterator>();
					*recordIterator.PStrMapIter = it;
					vectIterRet.push_back(recordIterator);
				}
			}
		}
		break;
	case LZPL::eTableMapType_MulmapInt64:
	case LZPL::eTableMapType_MulmapString:
	case LZPL::eTableMapType_NotMap:
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::RemoveFromDefaultMap(ILPData& poData)
{
	LOG_PROCESS_ERROR(m_poTableMap != nullptr);
	LOG_PROCESS_ERROR(m_oVarColType.GetCount() > 0 && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());
	LOG_PROCESS_ERROR(poData.GetType() == GetColType(0));

	switch (GetColMapType(0))
	{
	case LZPL::eTableMapType_MapInt64:
		{
			LOG_PROCESS_ERROR(m_poTableMap[0].PIntMap != nullptr);
			LOG_PROCESS_ERROR(GetColType(0) == eDataType_Int64);
			RecordIntKeyMap::iterator fit = m_poTableMap[0].PIntMap->find(poData.GetInt64());
			LOG_PROCESS_ERROR(fit != m_poTableMap[0].PIntMap->end());
			LPNormalPropertyFactory::Factory().DeletePropertyArray(fit->second, m_oVarColType.GetCount());
			m_poTableMap[0].PIntMap->erase(fit);
		}
		break;
	case LZPL::eTableMapType_MapString:
		{
			LOG_PROCESS_ERROR(m_poTableMap[0].PStrMap != nullptr);
			LOG_PROCESS_ERROR(GetColType(0) == eDataType_String);
			RecordStrKeyMap::iterator fit = m_poTableMap[0].PStrMap->find(poData.GetString());
			LOG_PROCESS_ERROR(fit != m_poTableMap[0].PStrMap->end());
			LPNormalPropertyFactory::Factory().DeletePropertyArray(fit->second, m_oVarColType.GetCount());
			m_poTableMap[0].PStrMap->erase(fit);
		}
		break;
	case LZPL::eTableMapType_MulmapInt64:
	case LZPL::eTableMapType_MulmapString:
	case LZPL::eTableMapType_NotMap:
	case LZPL::eTableMapType_Invalid:
	case LZPL::eTableMapType_Total:
	default:
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		break;
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::RemoveFromNotDefaultMap(PropertyVect& vectProperty)
{
	LPINT32 nResult = FALSE;

	LOG_PROCESS_ERROR(vectProperty != nullptr);
	LOG_PROCESS_ERROR(m_poTableMap != nullptr);
	LOG_PROCESS_ERROR(m_oVarColType.GetCount() > 0 && m_oVarColType.GetCount() == m_oVarMakeIndexCol.GetCount());

	for (LPUINT32 i = 1; i < m_oVarMakeIndexCol.GetCount(); ++i)
	{
		switch (GetColMapType(i))
		{
		case LZPL::eTableMapType_NotMap:
			//第0列必须有索引
			LOG_PROCESS_ERROR(i != 0);
			break;
		case LZPL::eTableMapType_MapInt64:
			{
				LOG_PROCESS_ERROR(m_poTableMap[i].PIntMap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_Int64);
				m_poTableMap[i].PIntMap->erase(vectProperty[i]->GetInt64());
			}
			break;
		case LZPL::eTableMapType_MapString:
			{
				LOG_PROCESS_ERROR(m_poTableMap[i].PStrMap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_String);
				m_poTableMap[i].PStrMap->erase(vectProperty[i]->GetString());
			}
			break;
		case LZPL::eTableMapType_MulmapInt64:
			{
				//第0列为单key索引
				LOG_PROCESS_ERROR(i != 0);
				LOG_PROCESS_ERROR(m_poTableMap[i].PIntMulmap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_Int64);

				std::pair<RecordIntKeyMulmap::iterator, RecordIntKeyMulmap::iterator> retFits = m_poTableMap[i].PIntMulmap->equal_range(vectProperty[i]->GetInt64());
				RecordIntKeyMulmap::iterator doit;
				for (RecordIntKeyMulmap::iterator it = retFits.first; it != retFits.second;)
				{
					doit = it;
					++it;
					if (doit->second == vectProperty)
					{
						m_poTableMap[i].PIntMulmap->erase(doit);
					}
				}
			}
			break;
		case LZPL::eTableMapType_MulmapString:
			{
				//第0列为单key索引
				LOG_PROCESS_ERROR(i != 0);
				LOG_PROCESS_ERROR(m_poTableMap[i].PStrMulmap != nullptr);
				LOG_PROCESS_ERROR(vectProperty[i]->GetType() == eDataType_String);

				std::pair<RecordStrKeyMulmap::iterator, RecordStrKeyMulmap::iterator> retFits = m_poTableMap[i].PStrMulmap->equal_range(vectProperty[i]->GetString());
				RecordStrKeyMulmap::iterator doit;
				for (RecordStrKeyMulmap::iterator it = retFits.first; it != retFits.second;)
				{
					doit = it;
					++it;
					if (doit->second == vectProperty)
					{
						m_poTableMap[i].PStrMulmap->erase(doit);
					}
				}
			}
			break;
		case LZPL::eTableMapType_Invalid:
		case LZPL::eTableMapType_Total:
		default:
			LOG_CHECK_ERROR(FALSE);
			LPASSERT(FALSE);
			break;
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPTable::IsCallbackListEmpty()
{
	return m_poCallbackList == nullptr;
}

void LPAPI LPTable::OnEventHandler(const LPINT32& nOptType, const Iterator& iter, const LPUINT32& dwCol, const ILPDataList& oOldVar, const ILPDataList& oNewVar)
{
	LPINT32 nResult = FALSE;

	if (m_poCallbackList == nullptr)
	{
		return;
	}

	SIMPLE_LIST_FOR_BEGIN((*m_poCallbackList))
	{
		LPTableCB* poTableCB = (LPTableCB*)ptNode;
		nResult = poTableCB->m_pfTableCB(m_oOwner, m_dwTableID, nOptType, iter, dwCol, oOldVar, oNewVar, 
			poTableCB->m_poCBParams != nullptr ? *poTableCB->m_poCBParams : ILPDataList::NullDataList());
		LOG_CHECK_ERROR_WITH_MSG(nResult, "TableID[%d]", m_dwTableID);
	}
	SIMPLE_LIST_FOR_END

	return;
}

LPUINT32 LPTable::ms_dwTableCount = 0;



LPINT32 LPAPI LPNormalTableFactory::GetTableInstanceCount()
{
	return LPTable::ms_dwTableCount;
}

ILPTable** LPAPI LPNormalTableFactory::NewTableArray(LPUINT32 dwSize)
{
	ILPTable** poTableArray = new ILPTable*[dwSize];
	for (LPUINT32 i = 0; i < dwSize; ++i)
	{
		poTableArray[i] = new LPTable();
	}
	return poTableArray;
}

void LPAPI LPNormalTableFactory::DeleteTableArray(ILPTable** & poTableArray, LPUINT32 dwSize)
{
	LOG_PROCESS_ERROR(poTableArray != nullptr);
	for (LPUINT32 i = 0; i < dwSize; ++i)
	{
		SAFE_DELETE(poTableArray[i]);
	}
	SAFE_DELETE_SZ(poTableArray);

Exit0:
	return;
}

ILPTable* LPAPI LPNormalTableFactory::NewTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol)
{
	return new LPTable(oOwner, dwTableID, varColType, varMakeIndexCol);
}

void LPAPI LPNormalTableFactory::DeleteTable(ILPTable* & poTable)
{
	SAFE_DELETE(poTable);
}

LPNormalTableFactory LPNormalTableFactory::m_oNormalTableFactory;

//end声明所处的名字空间
NS_LZPL_END