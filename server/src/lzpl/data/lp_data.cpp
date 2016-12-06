#include "lp_data.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN




ILPData* ILPData::NewData(E_DataType eType)
{
	switch (eType)
	{
	case LZPL::eDataType_Int64:
		return new LPDataInt64();
	case LZPL::eDataType_Float:
		return new LPDataFloat();
	case LZPL::eDataType_Double:
		return new LPDataDouble();
	case LZPL::eDataType_String:
		return new LPDataString();
	case LZPL::eDataType_Object:
	case LZPL::eDataType_Invalid:
	case LZPL::eDataType_Total:
	default:
		break;
	}

	return new LPDataInvalid();
}

void ILPData::DeleteData(ILPData* poData)
{
	SAFE_DELETE(poData);
}

LPDataInt64::LPDataInt64()
{
	m_lData = ZERO_INT;
}

LPDataInt64::~LPDataInt64()
{

}

std::string LPAPI LPDataInt64::ToString()
{
	return lpSerializeToString(MAX_INT64_LEN, FMT_I64, m_lData);
}

BOOL LPAPI LPDataInt64::SetInt64(INT_64 value)
{
	m_lData = value;
	return TRUE;
}

BOOL LPAPI LPDataInt64::SetFloat(FLOAT value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataInt64::SetDouble(DOUBLE value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataInt64::SetString(const std::string& value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}



LPDataFloat::LPDataFloat()
{
	m_fData = ZERO_FLOAT;
}

LPDataFloat::~LPDataFloat()
{

}

std::string LPAPI LPDataFloat::ToString()
{
	return lpSerializeToString(MAX_FLOAT_LEN, FMT_FLT, m_fData);
}

BOOL LPAPI LPDataFloat::SetInt64(INT_64 value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataFloat::SetFloat(FLOAT value)
{
	m_fData = value;
	return TRUE;
}

BOOL LPAPI LPDataFloat::SetDouble(DOUBLE value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataFloat::SetString(const std::string& value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}



LPDataDouble::LPDataDouble()
{
	m_dData = ZERO_DOUBLE;
}

LPDataDouble::~LPDataDouble()
{

}

std::string LPAPI LPDataDouble::ToString()
{
	return lpSerializeToString(MAX_DOUBLE_LEN, FMT_DBE, m_dData);
}

BOOL LPAPI LPDataDouble::SetInt64(INT_64 value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataDouble::SetFloat(FLOAT value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataDouble::SetDouble(DOUBLE value)
{
	m_dData = value;
	return TRUE;
}

BOOL LPAPI LPDataDouble::SetString(const std::string& value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}



LPDataString::LPDataString()
{
	//m_strData = NULL_STR;
}

LPDataString::~LPDataString()
{

}

std::string LPAPI LPDataString::ToString()
{
	return lpSerializeToString(MAX_DOUBLE_LEN, "%s", m_strData.c_str());
}

BOOL LPAPI LPDataString::SetInt64(INT_64 value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataString::SetFloat(FLOAT value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataString::SetDouble(DOUBLE value)
{
	LOG_CHECK_ERROR(FALSE);
	LPASSERT(FALSE);
	return FALSE;
}

BOOL LPAPI LPDataString::SetString(const std::string& value)
{
	m_strData = value;
	return TRUE;
}



//end声明所处的名字空间
NS_LZPL_END