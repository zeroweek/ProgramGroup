#include "lp_property.h"
#include "lp_processerror.h"
#include "lp_datalist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN




LPProperty::LPProperty(const LPIDENTID& oOwner, UINT_32 dwPropertyID, const std::string& strPropertyName, E_DataType eDataType)
{
	++ms_nPropertyCount;

	m_oOwner = oOwner;
	m_dwPropertyID = dwPropertyID;
	m_strPropertyName = strPropertyName;

	m_poData = ILPData::NewData(eDataType);
}


LPProperty::~LPProperty()
{
	ILPData::DeleteData(m_poData);
	--ms_nPropertyCount;
}

BOOL LPAPI LPProperty::SetInt64(INT_64 value)
{
	INT_32 nResult = FALSE;
	LPDATALIST oOldVals;
	LPDATALIST oNewVals;
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
	LPDATALIST oOldVals;
	LPDATALIST oNewVals;
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
	LPDATALIST oOldVals;
	LPDATALIST oNewVals;
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
	LPDATALIST oOldVals;
	LPDATALIST oNewVals;
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

const  std::string& LPAPI LPProperty::GetPropertyName() const
{
	return m_strPropertyName;
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

BOOL LPAPI LPProperty::RegisterCallback(const pfunPropertyEvent& cb, INT_32 nPriority, const ILPDATALIST& vars)
{
	INT_32 nResult = FALSE;

	BOOL bInsert = FALSE;

	LPPropertyCB oNewPropertyCB;
	oNewPropertyCB.m_nPriority = nPriority;
	oNewPropertyCB.m_pfPropertyCB = cb;

	for (BASE_LIST_NODE* pstNode = m_oCallbackList.Head().pstNext;
		pstNode != &m_oCallbackList.Rear();)
	{
		BASE_LIST_NODE* pstNextNode = pstNode->pstNext;

		LPPropertyCB* poPropertyCB = (LPPropertyCB*)pstNode;
		LOG_PROCESS_ERROR(poPropertyCB);

		if (oNewPropertyCB.m_nPriority >= poPropertyCB->m_nPriority)
		{
			nResult = m_oCallbackList.InsertBefore(pstNode, (BASE_LIST_NODE*)&oNewPropertyCB);
			LOG_PROCESS_ERROR(nResult);

			bInsert = TRUE;
			break;
		}

		pstNode = pstNextNode;
	}

	if (FALSE == bInsert)
	{
		nResult = m_oCallbackList.PushRear((BASE_LIST_NODE*)&oNewPropertyCB);
		LOG_PROCESS_ERROR(nResult);
	}

	return TRUE;
Exit0:
	return FALSE;
}

void LPAPI LZPL::LPProperty::OnEventHandler(const ILPDATALIST& oldVar, const ILPDATALIST& newVar)
{
	INT_32 nResult = FALSE;

	PROCESS_SUCCESS(m_oCallbackList.Size() == 0);
	
	for (BASE_LIST_NODE* pstNode = m_oCallbackList.Head().pstNext;
		pstNode != &m_oCallbackList.Rear();)
	{
		BASE_LIST_NODE* pstNextNode = pstNode->pstNext;

		LPPropertyCB* poPropertyCB = (LPPropertyCB*)pstNode;
		LOG_PROCESS_ERROR(poPropertyCB);

		nResult = poPropertyCB->m_pfPropertyCB(m_oOwner, m_dwPropertyID, m_strPropertyName, oldVar, newVar, LPDATALIST::NULLDataList());
		LOG_CHECK_ERROR_WITH_MSG(nResult, "[%d]%s", m_dwPropertyID, m_strPropertyName.c_str());

		pstNode = pstNextNode;
	}

Exit1:
Exit0:
	return;
}

INT_32 LPProperty::ms_nPropertyCount = 0;;


//end声明所处的名字空间


NS_LZPL_END