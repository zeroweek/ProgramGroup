#include "lp_datalist.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



ILPDataList* ILPDataList::ms_poNullDataList = new LPDataList();

LPDataList::LPDataList()
{

}

LPDataList::~LPDataList()
{
	Clear();
}

void LPAPI LPDataList::Clear()
{
	SIMPLE_LIST_FOR_BEGIN(m_oDataList)
	{
		DATA_SIMPLE_LIST_NODE* ptDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
		LOG_CHECK_ERROR(ptDataListNode->poData);
		ILPData::DeleteData(ptDataListNode->poData);
		DATA_SIMPLE_LIST_NODE::DeleteNode(ptDataListNode);
	}
	SIMPLE_LIST_FOR_END

	m_oDataList.Clear();
}

BOOL LPAPI LPDataList::IsEmpty() const
{
	return (m_oDataList.Size() == 0);
}

UINT_32 LPAPI LPDataList::GetCount() const
{
	return m_oDataList.Size();
}

E_DataType LPAPI LPDataList::Type(const INT_32 nIndex) const
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	SIMPLE_LIST_FOR_BEGIN(m_oDataList)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetType();
		}

		++i;
	}
	SIMPLE_LIST_FOR_END

Exit0:
	return eDataType_Invalid;
}

BOOL LPAPI LPDataList::Add(const INT_64 value)
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

BOOL LPAPI LPDataList::Add(const FLOAT value)
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

BOOL LPAPI LPDataList::Add(const DOUBLE value)
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

BOOL LPAPI LPDataList::Add(const char* value)
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

BOOL LPAPI LPDataList::Add(const std::string& value)
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

INT_64 LPAPI LPDataList::Int64(const INT_32 nIndex) const
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	SIMPLE_LIST_FOR_BEGIN(m_oDataList)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetInt64();
		}

		++i;
	}
	SIMPLE_LIST_FOR_END

Exit0:
	return ZERO_INT;
}

FLOAT LPAPI LPDataList::Float(const INT_32 nIndex) const
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	SIMPLE_LIST_FOR_BEGIN(m_oDataList)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetFloat();
		}

		++i;
	}
	SIMPLE_LIST_FOR_END

Exit0:
	return ZERO_FLOAT;
}

DOUBLE LPAPI LPDataList::Double(const INT_32 nIndex) const
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	SIMPLE_LIST_FOR_BEGIN(m_oDataList)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetDouble();
		}

		++i;
	}
	SIMPLE_LIST_FOR_END

Exit0:
	return ZERO_DOUBLE;
}

const std::string& LPAPI LPDataList::String(const INT_32 nIndex) const
{
	LOG_PROCESS_ERROR(nIndex < m_oDataList.Size());

	INT_32 i = 0;
	SIMPLE_LIST_FOR_BEGIN(m_oDataList)
	{
		if (i == nIndex)
		{
			DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
			LOG_PROCESS_ERROR(poDataListNode->poData);
			return poDataListNode->poData->GetString();
		}

		++i;
	}
	SIMPLE_LIST_FOR_END

Exit0:
	return NULL_STR;
}

BOOL LPDataList::Append(const ILPDataList& oSrc, UINT_32 dwStart, UINT_32 dwCount)
{
	INT_32 nResult = FALSE;
	UINT_32 dwEnd = dwStart + dwCount;

	LOG_PROCESS_ERROR(dwStart < oSrc.GetCount());
	LOG_PROCESS_ERROR(dwEnd <= oSrc.GetCount());

	for (UINT_32 i = dwStart; i < dwEnd; i++)
	{
		E_DataType eDataType = oSrc.Type(i);
		switch (eDataType)
		{
			break;
		case LZPL::eDataType_Int64:
			Add(oSrc.Int64(i));
			break;
		case LZPL::eDataType_Float:
			Add(oSrc.Float(i));
			break;
		case LZPL::eDataType_Double:
			Add(oSrc.Double(i));
			break;
		case LZPL::eDataType_String:
			Add(oSrc.String(i));
			break;
		case LZPL::eDataType_Object:
		case LZPL::eDataType_Invalid:
		case LZPL::eDataType_Total:
		default:
			LOG_CHECK_ERROR(FALSE);
			break;
		}
	}

	return TRUE;
Exit0:
	return FALSE;
}

LPDataList& LPDataList::operator=(const LPDataList& oSrc)
{
	Clear();
	Append(oSrc, 0, oSrc.GetCount());

	return *this;
}

LPDataList& LPDataList::operator=(const ILPDataList& oSrc)
{
	Clear();
	Append(oSrc, 0, oSrc.GetCount());

	return *this;
}

BOOL LPDataList::Concat(const ILPDataList& oSrc)
{
	INT_32 nResult = FALSE;
	UINT_32 dwStart = 0;
	UINT_32 dwEnd = oSrc.GetCount();

	for (UINT_32 i = dwStart; i < dwEnd; i++)
	{
		E_DataType eDataType = oSrc.Type(i);
		switch (eDataType)
		{
			break;
		case LZPL::eDataType_Int64:
			Add(oSrc.Int64(i));
			break;
		case LZPL::eDataType_Float:
			Add(oSrc.Float(i));
			break;
		case LZPL::eDataType_Double:
			Add(oSrc.Double(i));
			break;
		case LZPL::eDataType_String:
			Add(oSrc.String(i));
			break;
		case LZPL::eDataType_Object:
		case LZPL::eDataType_Invalid:
		case LZPL::eDataType_Total:
		default:
			LOG_CHECK_ERROR(FALSE);
			break;
		}
	}

	return TRUE;
}




//end声明所处的名字空间
NS_LZPL_END