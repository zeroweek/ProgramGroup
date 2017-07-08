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

E_DataType LPAPI LPDataList::Type(const LPINT32 nIndex) const
{
    LPINT32 i = 0;
    LOG_PROCESS_ERROR(nIndex < (LPINT32)m_oDataList.Size());

    SIMPLE_LIST_FOR_BEGIN(m_oDataList)
    {
        if(i == nIndex)
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

BOOL LPAPI LPDataList::AddData(const ILPData& oData)
{
    switch(oData.GetType())
    {
    case LZPL::eDataType_Int64:
        return Add(oData.GetInt64());
    case LZPL::eDataType_Float:
        return Add(oData.GetFloat());
    case LZPL::eDataType_Double:
        return Add(oData.GetDouble());
    case LZPL::eDataType_String:
        return Add(oData.GetString());
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

BOOL LPAPI LPDataList::Add(const LPINT64 value)
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

ILPData& LPAPI LPDataList::Data(const LPINT32 nIndex) const
{
    LPINT32 i = 0;
    LOG_PROCESS_ERROR(nIndex < (LPINT32)m_oDataList.Size());

    SIMPLE_LIST_FOR_BEGIN(m_oDataList)
    {
        if(i == nIndex)
        {
            DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
            LOG_PROCESS_ERROR(poDataListNode->poData);
            return *poDataListNode->poData;
        }

        ++i;
    }
    SIMPLE_LIST_FOR_END

Exit0:
    return ILPData::InvalidData();
}

LPINT64 LPAPI LPDataList::Int64(const LPINT32 nIndex) const
{
    LPINT32 i = 0;
    LOG_PROCESS_ERROR(nIndex < (LPINT32)m_oDataList.Size());

    SIMPLE_LIST_FOR_BEGIN(m_oDataList)
    {
        if(i == nIndex)
        {
            DATA_SIMPLE_LIST_NODE* poDataListNode = (DATA_SIMPLE_LIST_NODE*)ptNode;
            LOG_PROCESS_ERROR(poDataListNode->poData);
            return poDataListNode->poData->GetInt64();
        }

        ++i;
    }
    SIMPLE_LIST_FOR_END

Exit0:
    return 0;
}

FLOAT LPAPI LPDataList::Float(const LPINT32 nIndex) const
{
    LPINT32 i = 0;
    LOG_PROCESS_ERROR(nIndex < (LPINT32)m_oDataList.Size());

    SIMPLE_LIST_FOR_BEGIN(m_oDataList)
    {
        if(i == nIndex)
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

DOUBLE LPAPI LPDataList::Double(const LPINT32 nIndex) const
{
    LPINT32 i = 0;
    LOG_PROCESS_ERROR(nIndex < (LPINT32)m_oDataList.Size());

    SIMPLE_LIST_FOR_BEGIN(m_oDataList)
    {
        if(i == nIndex)
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

const std::string& LPAPI LPDataList::String(const LPINT32 nIndex) const
{
    LPINT32 i = 0;
    LOG_PROCESS_ERROR(nIndex < (LPINT32)m_oDataList.Size());

    SIMPLE_LIST_FOR_BEGIN(m_oDataList)
    {
        if(i == nIndex)
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

BOOL LPDataList::Append(const ILPDataList& oSrc, LPUINT32 dwStart, LPUINT32 dwCount)
{
    LPINT32 nResult = FALSE;
    LPUINT32 dwEnd = dwStart + dwCount;

    PROCESS_SUCCESS(dwCount == 0);
    LOG_PROCESS_ERROR(dwStart < oSrc.GetCount());
    LOG_PROCESS_ERROR(dwEnd <= oSrc.GetCount());

    for(LPUINT32 i = dwStart; i < dwEnd; i++)
    {
        nResult = AddData(oSrc.Data(i));
        LOG_PROCESS_ERROR(nResult);
    }

Exit1:
    return TRUE;
Exit0:
    return FALSE;
}

LPDataList& LPDataList::operator=(const LPDataList& oSrc)
{
    LPINT32 nResult = FALSE;

    Clear();
    nResult = Append(oSrc, 0, oSrc.GetCount());
    LOG_CHECK_ERROR(nResult);

    return *this;
}

LPDataList& LPDataList::operator=(const ILPDataList& oSrc)
{
    LPINT32 nResult = FALSE;

    Clear();
    nResult = Append(oSrc, 0, oSrc.GetCount());
    LOG_CHECK_ERROR(nResult);

    return *this;
}

BOOL LPDataList::Concat(const ILPDataList& oSrc)
{
    LPINT32 nResult = FALSE;

    nResult = Append(oSrc, 0, oSrc.GetCount());
    LOG_PROCESS_ERROR(nResult);

    return TRUE;
Exit0:
    return FALSE;
}




//end声明所处的名字空间
NS_LZPL_END