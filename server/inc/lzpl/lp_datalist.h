//****************************************************************************/
//    author: caihy
//    date: Nov 24, 2016
//    description:
//
//****************************************************************************/
#ifndef _LP_DATALIST_H_
#define _LP_DATALIST_H_

#include "lpi_datalist.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN





// Summary:
//   ��ӿ�
class DECLARE LPDataList : public ILPDataList
{
public:

    struct DATA_SIMPLE_LIST_NODE : public SIMPLE_LIST_NODE
    {
        ILPData*     poData;

        DATA_SIMPLE_LIST_NODE()
        {
            poData = nullptr;
        }

        static DATA_SIMPLE_LIST_NODE* NewNode()
        {
            return new DATA_SIMPLE_LIST_NODE();
        }

        static void DeleteNode(DATA_SIMPLE_LIST_NODE* & ptNode)
        {
            SAFE_DELETE(ptNode);
        }
    };

public:

    LPDataList();
    virtual ~LPDataList();

    virtual void LPAPI Clear();
    virtual BOOL LPAPI IsEmpty() const
    {
        return (m_oDataList.Size() == 0);
    }
    virtual LPUINT32 LPAPI GetCount() const
    {
        return m_oDataList.Size();
    }
    virtual E_DataType LPAPI Type(const LPINT32 nIndex) const;

    virtual BOOL LPAPI AddData(const ILPData& oData);
    virtual BOOL LPAPI Add(const LPINT64 value);
    virtual BOOL LPAPI Add(const FLOAT value);
    virtual BOOL LPAPI Add(const DOUBLE value);
    virtual BOOL LPAPI Add(const char* value);
    virtual BOOL LPAPI Add(const std::string& value);


    virtual ILPData& LPAPI Data(const LPINT32 nIndex) const;
    virtual LPINT64 LPAPI Int64(const LPINT32 nIndex) const;
    virtual FLOAT LPAPI Float(const LPINT32 nIndex) const;
    virtual DOUBLE LPAPI Double(const LPINT32 nIndex) const;
    virtual const std::string& LPAPI String(const LPINT32 nIndex) const;

    virtual BOOL Concat(const ILPDataList& oSrc);
    virtual BOOL Append(const ILPDataList& oSrc, LPUINT32 dwStart, LPUINT32 dwCount);

    LPDataList& operator=(const LPDataList& oSrc);
    LPDataList& operator=(const ILPDataList& oSrc);

protected:

    LPSimpleList                  m_oDataList;
};








//end�������������ֿռ�
NS_LZPL_END

#endif