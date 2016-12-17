//****************************************************************************/
//    author: caihy
//    date: Dec 1, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LP_DATA_H_
#define _LP_DATA_H_

#include "lpi_data.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN





// Summary:
//   Invalid
class DECLARE LPDataInvalid : public ILPData
{
public:

	LPDataInvalid(){}
	~LPDataInvalid(){}

	virtual std::string LPAPI ToString() { return NULL_STR; }

	virtual E_DataType LPAPI GetType() const { return eDataType_Invalid; }

	virtual BOOL LPAPI SetInt64(LPINT64 value)
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return FALSE;
	}

	virtual BOOL LPAPI SetFloat(FLOAT value)
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return FALSE;
	}

	virtual BOOL LPAPI SetDouble(DOUBLE value)
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return FALSE;
	}

	virtual BOOL LPAPI SetString(const std::string& value)
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return FALSE;
	}


	virtual LPINT64 LPAPI GetInt64() const 
	{ 
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_INT; 
	}

	virtual FLOAT LPAPI GetFloat() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_FLOAT; 
	}

	virtual DOUBLE LPAPI GetDouble() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_DOUBLE; 
	}

	virtual const std::string& LPAPI GetString() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return NULL_STR; 
	}

private:

	LPINT64                        m_lData;
};



// Summary:
//   LPINT64
class DECLARE LPDataInt64 : public ILPData
{
public:

	LPDataInt64();
	~LPDataInt64();

	virtual std::string LPAPI ToString();

	virtual E_DataType LPAPI GetType() const { return eDataType_Int64; }

	virtual BOOL LPAPI SetInt64(LPINT64 value);
	virtual BOOL LPAPI SetFloat(FLOAT value);
	virtual BOOL LPAPI SetDouble(DOUBLE value);
	virtual BOOL LPAPI SetString(const std::string& value);

	virtual LPINT64 LPAPI GetInt64() const 
	{ 
		return m_lData; 
	}

	virtual FLOAT LPAPI GetFloat() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_FLOAT; 
	}

	virtual DOUBLE LPAPI GetDouble() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_DOUBLE; 
	}

	virtual const std::string& LPAPI GetString() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return NULL_STR; 
	}

private:

	LPINT64                        m_lData;
};



// Summary:
//   FLOAT
class DECLARE LPDataFloat : public ILPData
{
public:

	LPDataFloat();
	~LPDataFloat();

	virtual std::string LPAPI ToString();

	virtual E_DataType LPAPI GetType() const { return eDataType_Float; }

	virtual BOOL LPAPI SetInt64(LPINT64 value);
	virtual BOOL LPAPI SetFloat(FLOAT value);
	virtual BOOL LPAPI SetDouble(DOUBLE value);
	virtual BOOL LPAPI SetString(const std::string& value);

	virtual LPINT64 LPAPI GetInt64() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_INT; 
	}

	virtual FLOAT LPAPI GetFloat() const 
	{ 
		return m_fData; 
	}

	virtual DOUBLE LPAPI GetDouble() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_DOUBLE; 
	}

	virtual const std::string& LPAPI GetString() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return NULL_STR; 
	}

private:

	FLOAT                         m_fData;
};



// Summary:
//   DOUBLE
class DECLARE LPDataDouble : public ILPData
{
public:

	LPDataDouble();
	~LPDataDouble();

	virtual std::string LPAPI ToString();

	virtual E_DataType LPAPI GetType() const { return eDataType_Double; }

	virtual BOOL LPAPI SetInt64(LPINT64 value);
	virtual BOOL LPAPI SetFloat(FLOAT value);
	virtual BOOL LPAPI SetDouble(DOUBLE value);
	virtual BOOL LPAPI SetString(const std::string& value);

	virtual LPINT64 LPAPI GetInt64() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_INT; 
	}

	virtual FLOAT LPAPI GetFloat() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return ZERO_FLOAT; 
	}

	virtual DOUBLE LPAPI GetDouble() const 
	{ 
		return m_dData; 
	}

	virtual const std::string& LPAPI GetString() const 
	{
		LOG_CHECK_ERROR(FALSE);
		LPASSERT(FALSE);
		return NULL_STR; 
	}

private:

	DOUBLE                        m_dData;
};



// Summary:
//   string
class DECLARE LPDataString : public ILPData
{
public:

	LPDataString();
	~LPDataString();

	virtual std::string LPAPI ToString();

	virtual E_DataType LPAPI GetType() const { return eDataType_String; }

	virtual BOOL LPAPI SetInt64(LPINT64 value);
	virtual BOOL LPAPI SetFloat(FLOAT value);
	virtual BOOL LPAPI SetDouble(DOUBLE value);
	virtual BOOL LPAPI SetString(const std::string& value);

	virtual LPINT64 LPAPI GetInt64() const { return ZERO_INT; }
	virtual FLOAT LPAPI GetFloat() const { return ZERO_FLOAT; }
	virtual DOUBLE LPAPI GetDouble() const { return ZERO_DOUBLE; }
	virtual const std::string& LPAPI GetString() const { return m_strData; }

private:

	std::string                   m_strData;
};







//end声明所处的名字空间
NS_LZPL_END

#endif