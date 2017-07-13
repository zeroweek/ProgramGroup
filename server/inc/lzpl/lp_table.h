//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description:
//
//****************************************************************************/
#ifndef _LP_TABLE_H_
#define _LP_TABLE_H_

#include "lpi_table.h"
#include "lp_baselist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//   数据表类
// Attention:
//   1. 第0列只能是整形或字符串，并且必须创建单key索引
//   2. 所有创建了索引的列，都不能修改值，只能删除
class DECLARE LPTable : public ILPTable
{
public:

    static LPUINT32 ms_dwTableCount;

public:

    class DECLARE LPTableCB : public SIMPLE_LIST_NODE
    {
    public:
        friend class LPTable;

    public:

        LPTableCB()
        {
            m_nPriority = 0;
            m_pfTableCB = nullptr;
            m_poCBParams = nullptr;
        }

        ~LPTableCB() { }

        static LPTableCB* LPAPI NewTableCB(LPINT32 nPriority, pfunTableEvent pfTableCB, const ILPDataList& oCBParams)
        {
            LPTableCB* poTableCB = new LPTableCB();
            poTableCB->m_nPriority = nPriority;
            poTableCB->m_pfTableCB = pfTableCB;

            if(&oCBParams != &ILPDataList::NullDataList())
            {
                poTableCB->m_poCBParams = std::make_shared<LPDataList>();
                *poTableCB->m_poCBParams = oCBParams;
            }

            return poTableCB;
        }

        static void LPAPI DeleteTableCB(LPTableCB* & poTableCB)
        {
            SAFE_DELETE(poTableCB);
        }

    private:

        LPINT32                        m_nPriority;
        pfunTableEvent                 m_pfTableCB;
        lp_shared_ptr<ILPDataList>   m_poCBParams;
    };

public:

    LPTable();
    LPTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol);
    ~LPTable();

    virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol);
    virtual BOOL LPAPI UnInit();

    virtual LPUINT32 GetColCount() const
    {
        return m_oVarColType.GetCount();
    }

    virtual LPUINT32 GetRecordCount() const;
    virtual E_DataType GetColType(const LPUINT32 dwCol) const;
    virtual E_TableMapType GetColMapType(const LPUINT32 dwCol) const;

    virtual BOOL AddRecord(const ILPDataList& var);

    virtual BOOL LPAPI SetInt64(Iterator& iter, const LPUINT32 dwCol, LPINT64 value);
    virtual BOOL LPAPI SetFloat(Iterator& iter, const LPUINT32 dwCol, FLOAT value);
    virtual BOOL LPAPI SetDouble(Iterator& iter, const LPUINT32 dwCol, DOUBLE value);
    virtual BOOL LPAPI SetString(Iterator& iter, const LPUINT32 dwCol, const std::string& value);

    virtual ILPData& LPAPI GetData(Iterator& iter, const LPUINT32 dwCol) const;
    virtual LPINT64 LPAPI GetInt64(Iterator& iter, const LPUINT32 dwCol) const;
    virtual FLOAT LPAPI GetFloat(Iterator& iter, const LPUINT32 dwCol) const;
    virtual DOUBLE LPAPI GetDouble(Iterator& iter, const LPUINT32 dwCol) const;
    virtual const std::string& LPAPI GetString(Iterator& iter, const LPUINT32 dwCol) const;

    virtual BOOL LPAPI FindInt64(const LPUINT32 dwCol, const LPINT64 value, IteratorVect& vectIterRet);
    virtual BOOL LPAPI FindFloat(const LPUINT32 dwCol, const FLOAT value, IteratorVect& vectIterRet);
    virtual BOOL LPAPI FindDouble(const LPUINT32 dwCol, const DOUBLE value, IteratorVect& vectIterRet);
    virtual BOOL LPAPI FindString(const LPUINT32 dwCol, const std::string& value, IteratorVect& vectIterRet);

    // Summary:
    //   remove成功或失败，传入的iterator都会被设置成对应的end
    virtual BOOL LPAPI Remove(Iterator& iter);
    virtual BOOL LPAPI Remove(IteratorVect& vectIter);

    // Summary:
    //   清空表数据
    // Attention:
    //   调用该函数后，之前所存储的iterator都无效，强行访问结果未知
    virtual void LPAPI Clear();

    virtual BOOL LPAPI RegisterCallback(const pfunTableEvent& cb, LPINT32 nPriority, const ILPDataList& oCBParams);

protected:

    BOOL InitTableMap();
    BOOL InsertIntoTableMap(PropertyVect& vectProperty, Iterator& iterRet);
    BOOL IsColCanUpdate(LPUINT32 dwCol)
    {
        return GetColMapType(dwCol) == eTableMapType_NotMap ? TRUE : FALSE;
    }

    void ResetRecordIterator(Iterator& iter);

    ILPProperty& GetProperty(Iterator& iter, const LPUINT32 dwCol);
    ILPProperty& GetPropertyConst(Iterator& iter, const LPUINT32 dwCol) const;

    BOOL LPAPI FindDataFromColMap(const LPUINT32 dwCol, const ILPData& value, IteratorVect& vectIterRet);
    BOOL LPAPI FindDataFromDefaultMap(const LPUINT32 dwCol, const ILPData& value, IteratorVect& vectIterRet);

    BOOL LPAPI RemoveFromDefaultMap(ILPData& poData);
    BOOL LPAPI RemoveFromNotDefaultMap(PropertyVect& vectProperty);

    BOOL LPAPI IsCallbackListEmpty();
    void LPAPI OnEventHandler(const LPINT32& nOptType, const Iterator& iter, const LPUINT32& dwCol, const ILPDataList& oOldVar, const ILPDataList& oNewVar);

protected:

    LPIDENTID                                       m_oOwner;           // 拥有者
    LPUINT32                                        m_dwTableID;        // 表ID
    LPDataList                                      m_oVarColType;      // 各列的类型
    LPDataList                                      m_oVarMakeIndexCol; // 需要设置索引的列（第0列默认有索引），有索引的列不允许修改值

    TableMap*                                       m_poTableMap;
    LPSimpleList*                                   m_poCallbackList; //减小内存
};



//end声明所处的名字空间
NS_LZPL_END

#endif