#include "lp_property.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LZPL::LPProperty::LPProperty()
{
	++ms_dwPropertyCount;

	m_oOwner = LPIDENTID(0, 0);
	m_dwPropertyID = INVALID_PROPERTY_ID;

	m_poData = ILPData::InvalidData();
	LOG_CHECK_ERROR(m_poData != nullptr);

	m_poCallbackList = nullptr;
}

LPProperty::LPProperty(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType)
{
	++ms_dwPropertyCount;

	m_oOwner = oOwner;
	m_dwPropertyID = dwPropertyID;

	m_poData = ILPData::NewData(eDataType);
	LOG_CHECK_ERROR(m_poData != nullptr);

	m_poCallbackList = nullptr;
}

LPProperty::~LPProperty()
{
	UnInit();

	--ms_dwPropertyCount;
}

BOOL LPAPI LZPL::LPProperty::Init(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType)
{
	INT_32 nResult = FALSE;

	m_oOwner = oOwner;
	m_dwPropertyID = dwPropertyID;

	m_poData = ILPData::NewData(eDataType);
	LOG_PROCESS_ERROR(m_poData != nullptr);

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

BOOL LPAPI LPProperty::SetInt64(INT_64 value)
{
	INT_32 nResult = FALSE;
	LPDataList oOldVals;
	LPDataList oNewVals;
	INT_64 lOldValue = m_poData->GetInt64();

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
	INT_32 nResult = FALSE;
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
	INT_32 nResult = FALSE;
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
	INT_32 nResult = FALSE;
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

UINT_32 LPAPI LPProperty::GetPropertyID() const
{
	return m_dwPropertyID;
}

INT_64 LPAPI LPProperty::GetInt64() const
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

BOOL LPAPI LPProperty::RegisterCallback(const pfunPropertyEvent& cb, INT_32 nPriority, const ILPDataList& vars)
{
	INT_32 nResult = FALSE;
	BOOL bInsert = FALSE;
	LPPropertyCB* poNewPropertyCB = LPPropertyCB::NewPropertyCB(nPriority, cb);

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

void LPAPI LPProperty::OnEventHandler(const ILPDataList & oldVar, const ILPDataList & newVar)
{
	INT_32 nResult = FALSE;

	if (m_poCallbackList == nullptr)
	{
		return;
	}

	SIMPLE_LIST_FOR_BEGIN((*m_poCallbackList))
	{
		LPPropertyCB* poPropertyCB = (LPPropertyCB*)ptNode;
		nResult = poPropertyCB->m_pfPropertyCB(m_oOwner, m_dwPropertyID, oldVar, newVar, ILPDataList::NullDataList());
		LOG_CHECK_ERROR_WITH_MSG(nResult, "PropertyID[%d]", m_dwPropertyID);
	}
	SIMPLE_LIST_FOR_END

	return;
}

UINT_32 LPProperty::ms_dwPropertyCount = 0;;



UINT_32 LPAPI LPNormalPropertyFactory::GetPropertyInstanceCount()
{
	return LPProperty::ms_dwPropertyCount;
}

ILPProperty* LPAPI LPNormalPropertyFactory::NewPropertyArray(UINT_32 dwSize)
{
	return new LPProperty[dwSize];
}

void LPAPI LZPL::LPNormalPropertyFactory::DeletePropertyArray(ILPProperty* & poData)
{
	SAFE_DELETE_SZ(poData);
}

ILPProperty* LPAPI LPNormalPropertyFactory::NewProperty(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType)
{
	return new LPProperty(oOwner, dwPropertyID, eDataType);
}

void LPAPI LZPL::LPNormalPropertyFactory::DeleteProperty(ILPProperty* & poData)
{
	SAFE_DELETE(poData);
}

//end声明所处的名字空间


NS_LZPL_END