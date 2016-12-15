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
#include "lp_simplelist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN




// Summary:
//		属性类
class DECLARE LPProperty : public ILPProperty
{
public:

	class DECLARE LPPropertyCB : public SIMPLE_LIST_NODE
	{
	public:
		friend class LPProperty;

	public:

		LPPropertyCB()
		{
			m_nPriority = 0;
			m_pfPropertyCB = nullptr;
		}

		~LPPropertyCB() { }

		static LPPropertyCB* LPAPI NewPropertyCB(INT_32 nPriority, pfunPropertyEvent pfPropertyCB)
		{
			LPPropertyCB* poPropertyCB = new LPPropertyCB();
			poPropertyCB->m_nPriority = nPriority;
			poPropertyCB->m_pfPropertyCB = pfPropertyCB;

			return poPropertyCB;
		}

		static void LPAPI DeletePropertyCB(LPPropertyCB* poPropertyCB)
		{
			SAFE_DELETE(poPropertyCB);
		}

	private:

	private:

		INT_32                        m_nPriority;
		pfunPropertyEvent             m_pfPropertyCB;
	};

public:

	static INT_32 ms_nPropertyCount;

public:

	LPProperty();
	LPProperty(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType);
	~LPProperty();

	virtual BOOL LPAPI Init(const LPIDENTID& oOwner, UINT_32 dwPropertyID, E_DataType eDataType);
	virtual BOOL LPAPI UnInit();

	virtual UINT_32 LPAPI GetPropertyID() const;
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
	ILPData*                      m_poData;

	LPSimpleList*                 m_poCallbackList; //减小内存
};









//end声明所处的名字空间
NS_LZPL_END

#endif