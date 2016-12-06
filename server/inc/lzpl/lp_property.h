//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description: 
//
//****************************************************************************/
#ifndef _LP_PROPERTY_H_
#define _LP_PROPERTY_H_

#include "lp_base.h"
#include "lpi_property.h"
#include "lp_baselist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN




// Summary:
//		属性类
class DECLARE LPProperty : public ILPProperty
{
public:

	class DECLARE LPPropertyCB : public BASE_LIST_NODE
	{
	public:
		friend class LPProperty;
	public:
		LPPropertyCB()
		{
			m_nPriority = 0;
			m_pfPropertyCB = nullptr;
		}
		~LPPropertyCB()
		{

		}

	private:

		INT_32                        m_nPriority;
		pfunPropertyEvent             m_pfPropertyCB;
	};

public:

	static INT_32 ms_nPropertyCount;

public:

	LPProperty(const LPIDENTID& oOwner, UINT_32 dwPropertyID, const std::string& strPropertyName, E_DataType eDataType);
	~LPProperty();

	virtual UINT_32 LPAPI GetPropertyID() const;
	virtual const std::string& LPAPI GetPropertyName() const;
	virtual const E_DataType GetType() const;

	virtual BOOL LPAPI SetInt64(INT_64 value);
	virtual BOOL LPAPI SetFloat(FLOAT value);
	virtual BOOL LPAPI SetDouble(DOUBLE value);
	virtual BOOL LPAPI SetString(const std::string& value);

	virtual INT_64 LPAPI GetInt64() const;
	virtual FLOAT LPAPI GetFloat() const;
	virtual DOUBLE LPAPI GetDouble() const;
	virtual const std::string& LPAPI GetString() const;

	virtual BOOL LPAPI RegisterCallback(const pfunPropertyEvent& cb, INT_32 nPriority, const ILPDATALIST& vars);

protected:

	void LPAPI OnEventHandler(const ILPDATALIST& oldVar, const ILPDATALIST& newVar);

private:

	LPIDENTID                     m_oOwner;
	UINT_32                       m_dwPropertyID;
	std::string                   m_strPropertyName;
	ILPData*                      m_poData;

	LPBaseList                    m_oCallbackList;
};









//end声明所处的名字空间
NS_LZPL_END

#endif