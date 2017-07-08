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
//      属性类
class DECLARE LPProperty : public ILPProperty
{
public:

    static LPUINT32 ms_dwPropertyCount;

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
            m_poCBParams = nullptr;
        }

        ~LPPropertyCB() { }

        static LPPropertyCB* LPAPI NewPropertyCB(LPINT32 nPriority, pfunPropertyEvent pfPropertyCB, const ILPDataList& oCBParams)
        {
            LPPropertyCB* poPropertyCB = new LPPropertyCB();
            poPropertyCB->m_nPriority = nPriority;
            poPropertyCB->m_pfPropertyCB = pfPropertyCB;

            if(&oCBParams != &ILPDataList::NullDataList())
            {
                poPropertyCB->m_poCBParams = std::make_shared<LPDataList>();
                *poPropertyCB->m_poCBParams = oCBParams;
            }

            return poPropertyCB;
        }

        static void LPAPI DeletePropertyCB(LPPropertyCB* & poPropertyCB)
        {
            SAFE_DELETE(poPropertyCB);
        }

    private:

        LPINT32                        m_nPriority;
        pfunPropertyEvent              m_pfPropertyCB;
        std::shared_ptr<ILPDataList>   m_poCBParams;
    };

public:

    LPProperty();
    LPProperty(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType);
    ~LPProperty();

    virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwPropertyID, E_DataType eDataType);
    virtual BOOL LPAPI UnInit();

    virtual LPUINT32 LPAPI GetPropertyID() const;
    virtual const E_DataType GetType() const;

    virtual BOOL LPAPI SetData(ILPData& poData);
    virtual BOOL LPAPI SetInt64(LPINT64 value);
    virtual BOOL LPAPI SetFloat(FLOAT value);
    virtual BOOL LPAPI SetDouble(DOUBLE value);
    virtual BOOL LPAPI SetString(const std::string& value);

    virtual ILPData& LPAPI GetData() const;
    virtual LPINT64 LPAPI GetInt64() const;
    virtual FLOAT LPAPI GetFloat() const;
    virtual DOUBLE LPAPI GetDouble() const;
    virtual const std::string& LPAPI GetString() const;

    virtual BOOL LPAPI RegisterCallback(const pfunPropertyEvent& cb, LPINT32 nPriority, const ILPDataList& oCBParams);

protected:

    void LPAPI OnEventHandler(const ILPDataList& oOldVar, const ILPDataList& oNewVar);

private:

    LPIDENTID                     m_oOwner;
    LPUINT32                       m_dwPropertyID;
    ILPData*                      m_poData;

    LPSimpleList*                 m_poCallbackList; //减小内存
};









//end声明所处的名字空间
NS_LZPL_END

#endif