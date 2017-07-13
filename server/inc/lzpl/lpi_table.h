//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description:
//
//****************************************************************************/
#ifndef _LPI_TABLE_H_
#define _LPI_TABLE_H_

#include "lpi_property.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define INVALID_TABLE_ID                     (0)



// Summary:
//
enum DECLARE E_TableOptType
{
    eTableOptType_Invalid = -1,
    eTableOptType_Add,
    eTableOptType_Del,
    eTableOptType_Update,
    eTableOptType_Clear,
    eTableOptType_Total
};



// Summary:
//   map类型
enum DECLARE E_TableMapType
{
    eTableMapType_Invalid = -1,
    eTableMapType_NotMap,
    eTableMapType_MapInt64,
    eTableMapType_MapString,
    eTableMapType_MulmapInt64,
    eTableMapType_MulmapString,
    eTableMapType_Total
};



// Summary:
//   表接口
class DECLARE ILPTable
{
public:

    typedef ILPProperty** PropertyVect;
    typedef std::map<LPINT64, PropertyVect> RecordIntKeyMap;
    typedef std::map<std::string, PropertyVect> RecordStrKeyMap;
    typedef std::multimap<LPINT64, PropertyVect> RecordIntKeyMulmap;
    typedef std::multimap<std::string, PropertyVect> RecordStrKeyMulmap;

    struct DECLARE TABLE_MAP
    {
        RecordIntKeyMap* PIntMap;
        RecordStrKeyMap* PStrMap;

        RecordIntKeyMulmap* PIntMulmap;
        RecordStrKeyMulmap* PStrMulmap;
    };

    // Summary:
    //   索引结构
    // 注意：
    //   1. 如果不使用指针，不确定默认值（不同map的iterator不能进行比较）
    //   2. 如果不使用只能指针，则map查找或遍历返回的是临时变量
    struct DECLARE TABLE_INDEX
    {
        LPUINT32 MapIndexCol;

        lp_shared_ptr<RecordIntKeyMap::iterator> PIntMapIter;
        lp_shared_ptr<RecordStrKeyMap::iterator> PStrMapIter;

        lp_shared_ptr<RecordIntKeyMulmap::iterator> PIntMulmapIter;
        lp_shared_ptr<RecordStrKeyMulmap::iterator> PStrMulmapIter;
    };

    typedef TABLE_MAP TableMap;
    typedef TABLE_INDEX Iterator;
    typedef std::vector<TABLE_INDEX> IteratorVect;

    // Summary:
    //   表回调接口定义
    // Params:
    //   <拥有者,表ID,操作类型,列,旧值,新值,其他参数>
    typedef BOOL(*pfunTableEvent)(const LPIDENTID&, const LPUINT32&, const LPINT32&, const Iterator&, const LPUINT32&, const ILPDataList&, const ILPDataList&, const ILPDataList&);

public:

    virtual ~ILPTable() {}

    virtual BOOL LPAPI Init(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol) = 0;
    virtual BOOL LPAPI UnInit() = 0;

    virtual LPUINT32 GetColCount() const = 0;
    virtual LPUINT32 GetRecordCount() const = 0;
    virtual E_DataType GetColType(const LPUINT32 dwCol) const = 0;

    virtual BOOL AddRecord(const ILPDataList& var) = 0;

    virtual BOOL LPAPI SetInt64(Iterator& iter, const LPUINT32 dwCol, LPINT64 value) = 0;
    virtual BOOL LPAPI SetFloat(Iterator& iter, const LPUINT32 dwCol, FLOAT value) = 0;
    virtual BOOL LPAPI SetDouble(Iterator& iter, const LPUINT32 dwCol, DOUBLE value) = 0;
    virtual BOOL LPAPI SetString(Iterator& iter, const LPUINT32 dwCol, const std::string& value) = 0;

    virtual ILPData& LPAPI GetData(Iterator& iter, const LPUINT32 dwCol) const = 0;
    virtual LPINT64 LPAPI GetInt64(Iterator& iter, const LPUINT32 dwCol) const = 0;
    virtual FLOAT LPAPI GetFloat(Iterator& iter, const LPUINT32 dwCol) const = 0;
    virtual DOUBLE LPAPI GetDouble(Iterator& iter, const LPUINT32 dwCol) const = 0;
    virtual const std::string& LPAPI GetString(Iterator& iter, const LPUINT32 dwCol) const = 0;

    virtual BOOL LPAPI FindInt64(const LPUINT32 dwCol, const LPINT64 value, IteratorVect& vectIterRet) = 0;
    virtual BOOL LPAPI FindFloat(const LPUINT32 dwCol, const FLOAT value, IteratorVect& vectIterRet) = 0;
    virtual BOOL LPAPI FindDouble(const LPUINT32 dwCol, const DOUBLE value, IteratorVect& vectIterRet) = 0;
    virtual BOOL LPAPI FindString(const LPUINT32 dwCol, const std::string& value, IteratorVect& vectIterRet) = 0;

    // Summary:
    //   remove成功或失败，传入的iterator都会被设置成对应的end
    virtual BOOL LPAPI Remove(Iterator& iter) = 0;
    virtual BOOL LPAPI Remove(IteratorVect& vectIter) = 0;

    // Summary:
    //   清空表数据
    // Attention:
    //   调用该函数后，之前所存储的iterator都无效，强行访问结果未知
    virtual void LPAPI Clear() = 0;

    virtual BOOL LPAPI RegisterCallback(const pfunTableEvent& cb, LPINT32 nPriority, const ILPDataList& oCBParams) = 0;
};



// Summary:
//   talbe工厂接口
class DECLARE ILPTableFactory
{
public:

    virtual LPINT32 LPAPI GetTableInstanceCount() = 0;

    virtual ILPTable** LPAPI NewTableArray(LPUINT32 dwSize) = 0;
    virtual void LPAPI DeleteTableArray(ILPTable** & poTableArray, LPUINT32 dwSize) = 0;

    virtual ILPTable* LPAPI NewTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol) = 0;
    virtual void LPAPI DeleteTable(ILPTable* & poTable) = 0;
};



// Summary:
//   普通talbe工厂
class DECLARE LPNormalTableFactory : public ILPTableFactory
{
public:

    virtual LPINT32 LPAPI GetTableInstanceCount();

    virtual ILPTable** LPAPI NewTableArray(LPUINT32 dwSize);
    virtual void LPAPI DeleteTableArray(ILPTable** & poTableArray, LPUINT32 dwSize);

    virtual ILPTable* LPAPI NewTable(const LPIDENTID& oOwner, LPUINT32 dwTableID, const ILPDataList& varColType, const ILPDataList& varMakeIndexCol);
    virtual void LPAPI DeleteTable(ILPTable* & poTable);

public:

    static ILPTableFactory& Factory()
    {
        return m_oNormalTableFactory;
    }

private:

    static LPNormalTableFactory m_oNormalTableFactory;
};



//end声明所处的名字空间
NS_LZPL_END

#endif