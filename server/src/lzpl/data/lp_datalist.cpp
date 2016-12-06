#include "lp_datalist.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN


LPDATALIST::LPDATALIST()
{

}

LPDATALIST::~LPDATALIST()
{
	Clear();
}

void LPAPI LPDATALIST::Clear()
{
	for (SIMPLE_LIST_NODE* pstNode = m_oDataList.Head(); pstNode != nullptr;)
	{
		SIMPLE_LIST_NODE* pstNextNode = pstNode->pstNext;

		DATA_SIMPLE_LIST_NODE* ptDataListNode = (DATA_SIMPLE_LIST_NODE*)pstNode;
		LOG_CHECK_ERROR(ptDataListNode->poData);
		ILPData::DeleteData(ptDataListNode->poData);
		DATA_SIMPLE_LIST_NODE::DeleteNode(ptDataListNode);

		pstNode = pstNextNode;
	}

	m_oDataList.Clear();
}

BOOL LPAPI LPDATALIST::IsEmpty() const
{
	return (m_oDataList.Size() == 0);
}

UINT_32 LPAPI LPDATALIST::GetCount() const
{
	return m_oDataList.Size();
}

E_DataType LPAPI LPDATALIST::Type(const INT_32 nIndex)
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	for (SIMPLE_LIST_NODE* pstNode = m_oDataList.Head();pstNode != nullptr; ++i)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)pstNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetType();
		}

		pstNode = pstNode->pstNext;
	}

Exit0:
	return eDataType_Invalid;
}

BOOL LPAPI LPDATALIST::Add(const INT_64 value)
{
	ILPData* poData = nullptr;
	DATA_SIMPLE_LIST_NODE* ptDataListNode = DATA_SIMPLE_LIST_NODE::NewNode();

	LOG_PROCESS_ERROR(ptDataListNode);

	poData = ILPData::NewData(eDataType_Int64);
	LOG_PROCESS_ERROR(poData);
	poData->SetInt64(value);

	ptDataListNode->poData = poData;

	m_oDataList.Append(ptDataListNode);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPDATALIST::Add(const FLOAT value)
{
	ILPData* poData = nullptr;
	DATA_SIMPLE_LIST_NODE* ptDataListNode = DATA_SIMPLE_LIST_NODE::NewNode();

	LOG_PROCESS_ERROR(ptDataListNode);

	poData = ILPData::NewData(eDataType_Float);
	LOG_PROCESS_ERROR(poData);
	poData->SetFloat(value);

	ptDataListNode->poData = poData;

	m_oDataList.Append(ptDataListNode);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPDATALIST::Add(const DOUBLE value)
{
	ILPData* poData = nullptr;
	DATA_SIMPLE_LIST_NODE* ptDataListNode = DATA_SIMPLE_LIST_NODE::NewNode();

	LOG_PROCESS_ERROR(ptDataListNode);

	poData = ILPData::NewData(eDataType_Double);
	LOG_PROCESS_ERROR(poData);
	poData->SetDouble(value);

	ptDataListNode->poData = poData;

	m_oDataList.Append(ptDataListNode);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPDATALIST::Add(const char* value)
{
	ILPData* poData = nullptr;
	DATA_SIMPLE_LIST_NODE* ptDataListNode = DATA_SIMPLE_LIST_NODE::NewNode();

	LOG_PROCESS_ERROR(ptDataListNode);

	poData = ILPData::NewData(eDataType_String);
	LOG_PROCESS_ERROR(poData);
	poData->SetString(std::string(value));

	ptDataListNode->poData = poData;

	m_oDataList.Append(ptDataListNode);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPDATALIST::Add(const std::string& value)
{
	ILPData* poData = nullptr;
	DATA_SIMPLE_LIST_NODE* ptDataListNode = DATA_SIMPLE_LIST_NODE::NewNode();

	LOG_PROCESS_ERROR(ptDataListNode);

	poData = ILPData::NewData(eDataType_String);
	LOG_PROCESS_ERROR(poData);
	poData->SetString(value);

	ptDataListNode->poData = poData;

	m_oDataList.Append(ptDataListNode);

	return TRUE;
Exit0:
	return FALSE;
}

INT_64 LPAPI LPDATALIST::Int64(const INT_32 nIndex)
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	for (SIMPLE_LIST_NODE* pstNode = m_oDataList.Head(); pstNode != nullptr; ++i)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)pstNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetInt64();
		}

		pstNode = pstNode->pstNext;
	}

Exit0:
	return ZERO_INT;
}

FLOAT LPAPI LPDATALIST::Float(const INT_32 nIndex)
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	for (SIMPLE_LIST_NODE* pstNode = m_oDataList.Head(); pstNode != nullptr; ++i)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)pstNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetFloat();
		}

		pstNode = pstNode->pstNext;
	}

Exit0:
	return ZERO_FLOAT;
}

DOUBLE LPAPI LPDATALIST::Double(const INT_32 nIndex)
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	for (SIMPLE_LIST_NODE* pstNode = m_oDataList.Head(); pstNode != nullptr; ++i)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)pstNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetDouble();
		}

		pstNode = pstNode->pstNext;
	}

Exit0:
	return ZERO_DOUBLE;
}

const std::string& LPAPI LPDATALIST::String(const INT_32 nIndex)
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	for (SIMPLE_LIST_NODE* pstNode = m_oDataList.Head(); pstNode != nullptr; ++i)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)pstNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetString();
		}

		pstNode = pstNode->pstNext;
	}

Exit0:
	return NULL_STR;
}

const LPDATALIST LPDATALIST::m_oNull = LPDATALIST();


//end声明所处的名字空间
NS_LZPL_END