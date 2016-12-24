#include "lp_property.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



ILPProperty* ILPProperty::m_poInvalidProperty = new LPProperty();

LZPL::LPProperty::LPProperty()
{
	++ms_dwPropertyCount;

	m_oOwner = LPIDENTID(0, 0);
	m_dwPropertyID = INVALID_PROPERTY_ID;

	m_poData = &ILPData::InvalidData();
	LOG_CHECK_ERROR(m_poData != nullptr);

	m_poCallbackList = nullptr;
}

LPProperty::LPProperty(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType)
{
	LPINT32 nResult = FALSE;

	++ms_dwPropertyCount;

	nResult = Init(oOwner, dwPropertyID, eDataType);
	LOG_PROCESS_ERROR(nResult);
	
Exit0:
	return;
}

LPProperty::~LPProperty()
{
	UnInit();

	--ms_dwPropertyCount;
}

BOOL LPAPI LZPL::LPProperty::Init(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType)
{
	LPINT32 nResult = FALSE;

	m_oOwner = oOwner;
	m_dwPropertyID = dwPropertyID;

	m_poData = ILPData::NewData(eDataType);
	LOG_PROCESS_ERROR(m_poData != nullptr);

	m_poCallbackList = nullptr;

	return TRUE;
Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);
	
	return FALSE;
}

BOOL LPAPI LPProperty::UnInit()
{
	if (m_poCallbackList != nullptr)
	{
		SIMPLE_LIST_FOR_BEGIN((*m_poCallbackList))
		{
			LPPropertyCB* poPropertyCB = (LPPropertyCB*)ptNode;
			LPPropertyCB::DeletePropertyCB(poPropertyCB);
		}
		SIMPLE_LIST_FOR_END
		(*m_poCallbackList).Clear();
		SAFE_DELETE(m_poCallbackList);
	}

	ILPData::DeleteData(m_poData);

	return TRUE;
}

BOOL LPAPI LPProperty::SetData(ILPData& poData)
{
	switch (poData.GetType())
	{
	case LZPL::eDataType_Int64:
		return SetInt64(poData.GetInt64());
	case LZPL::eDataType_Float:
		return SetFloat(poData.GetFloat());
	case LZPL::eDataType_Double:
		return SetDouble(poData.GetDouble());
	case LZPL::eDataType_String:
		return SetString(poData.GetString());
	case LZPL::eDataType_Object:
	case LZPL::eDataType_Invalid:
	case LZPL::eDataType_Total:
	default:
		LOG_PROCESS_ERROR(FALSE);
		LPASSERT(FALSE);
	}

Exit0:
	return FALSE;
}

BOOL LPAPI LPProperty::SetInt64(LPINT64 value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	LPINT64 lOldValue = m_poData->GetInt64();

	nResult = m_poData->SetInt64(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << lOldValue;
	oNewVals << m_poData->GetInt64();
	OnEventHandler(oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPProperty::SetFloat(FLOAT value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	FLOAT fOldValue = m_poData->GetFloat();

	nResult = m_poData->SetFloat(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << fOldValue;
	oNewVals << m_poData->GetFloat();
	OnEventHandler(oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPProperty::SetDouble(DOUBLE value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	DOUBLE dOldValue = m_poData->GetDouble();

	nResult = m_poData->SetDouble(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << dOldValue;
	oNewVals << m_poData->GetDouble();
	OnEventHandler(oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPProperty::SetString(const std::string& value)
{
	LPINT32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	std::string strOldValue = m_poData->GetString();

	nResult = m_poData->SetString(value);
	LOG_PROCESS_ERROR(nResult);

	oOldVals << strOldValue;
	oNewVals << m_poData->GetString();
	OnEventHandler(oOldVals, oNewVals);

	return TRUE;
Exit0:
	return FALSE;
}

const E_DataType LPProperty::GetType() const
{
	return m_poData->GetType();
}

LPUINT32 LPAPI LPProperty::GetPropertyID() const
{
	return m_dwPropertyID;
}

ILPData& LPAPI LPProperty::GetData() const
{
	return *m_poData;
}

LPINT64 LPAPI LPProperty::GetInt64() const
{
	return m_poData->GetInt64();
}

FLOAT LPAPI LPProperty::GetFloat() const
{
	return m_poData->GetFloat();
}

DOUBLE LPAPI LPProperty::GetDouble() const
{
	return m_poData->GetDouble();
}

const std::string& LPAPI LPProperty::GetString() const
{
	return m_poData->GetString();
}

BOOL LPAPI LPProperty::RegisterCallback(const pfunPropertyEvent& cb, LPINT32 nPriority, const ILPDataList& oCBParams)
{
	LPINT32 nResult = FALSE;
	BOOL bInsert = FALSE;
	LPPropertyCB* poNewPropertyCB = LPPropertyCB::NewPropertyCB(nPriority, cb, oCBParams);

	if (m_poCallbackList == nullptr)
	{
		m_poCallbackList = new LPSimpleList();
		LOG_PROCESS_ERROR(m_poCallbackList);
	}

	SIMPLE_LIST_FOR_BEGIN((*m_poCallbackList))
	{
		LPPropertyCB* poPropertyCB = (LPPropertyCB*)ptNode;
		if (poNewPropertyCB->m_nPriority >= poPropertyCB->m_nPriority)
		{
			nResult = (*m_poCallbackList).InsertBefore(poNewPropertyCB, ptNode);
			LOG_PROCESS_ERROR(nResult);

			bInsert = TRUE;
			break;
		}
	}
	SIMPLE_LIST_FOR_END

	if (FALSE == bInsert)
	{
		nResult = (*m_poCallbackList).Append(poNewPropertyCB);
		LOG_PROCESS_ERROR(nResult);
	}

	return TRUE;
Exit0:
	return FALSE;
}

void LPAPI LPProperty::OnEventHandler(const ILPDataList & oOldVar, const ILPDataList & oNewVar)
{
	LPINT32 nResult = FALSE;

	if (m_poCallbackList == nullptr)
	{
		return;
	}

	SIMPLE_LIST_FOR_BEGIN((*m_poCallbackList))
	{
		LPPropertyCB* poPropertyCB = (LPPropertyCB*)ptNode;
		nResult = poPropertyCB->m_pfPropertyCB(m_oOwner, m_dwPropertyID, oOldVar, oNewVar, 
			poPropertyCB->m_poCBParams != nullptr ? *poPropertyCB->m_poCBParams : ILPDataList::NullDataList());
		LOG_CHECK_ERROR_WITH_MSG(nResult, "PropertyID[%d]", m_dwPropertyID);
	}
	SIMPLE_LIST_FOR_END

	return;
}

LPUINT32 LPProperty::ms_dwPropertyCount = 0;;



LPUINT32 LPAPI LPNormalPropertyFactory::GetPropertyInstanceCount()
{
	return LPProperty::ms_dwPropertyCount;
}

ILPProperty** LPAPI LPNormalPropertyFactory::NewPropertyArray(LPUINT32 dwSize)
{
	ILPProperty** poPropertyArray = new ILPProperty*[dwSize];
	for (LPUINT32 i = 0; i < dwSize; ++i)
	{
		poPropertyArray[i] = new LPProperty();
	}
	return poPropertyArray;
}

void LPAPI LPNormalPropertyFactory::DeletePropertyArray(ILPProperty** & poPropertyArray, LPUINT32 dwSize)
{
	LOG_PROCESS_ERROR(poPropertyArray != nullptr);
	for (LPUINT32 i = 0; i < dwSize; ++i)
	{
		SAFE_DELETE(poPropertyArray[i]);
	}
	SAFE_DELETE_SZ(poPropertyArray);

Exit0:
	return;
}

ILPProperty* LPAPI LPNormalPropertyFactory::NewProperty(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType)
{
	return new LPProperty(oOwner, dwPropertyID, eDataType);
}

void LPAPI LPNormalPropertyFactory::DeleteProperty(ILPProperty* & poProperty)
{
	SAFE_DELETE(poProperty);
}

//end声明所处的名字空间


NS_LZPL_END