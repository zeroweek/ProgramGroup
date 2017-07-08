//****************************************************************************/
//    author: caihy
//    date: Nov 25, 2016
//    description:
//
//****************************************************************************/
#ifndef _LPI_IDENTID_H_
#define _LPI_IDENTID_H_

#include "lp_base.h"
#include "lp_string.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define MAX_IDENTID_LEN                     (64)



//#pragma pack(push, 1)

struct LPIDENTID
{
    LPINT64 nHead64;
    LPINT64 nData64;

    LPIDENTID()
    {
        nHead64 = 0;
        nData64 = 0;
    }

    LPIDENTID(LPINT64 nHeadData, LPINT64 nData)
    {
        nHead64 = nHeadData;
        nData64 = nData;
    }

    LPIDENTID(const LPIDENTID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;
    }

    LPIDENTID& operator=(const LPIDENTID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;

        return *this;
    }

    BOOL IsNull() const
    {
        return 0 == nData64 && 0 == nHead64;
    }

    BOOL operator == (const LPIDENTID& id) const
    {
        return this->nData64 == id.nData64 && this->nHead64 == id.nHead64;
    }

    BOOL operator != (const LPIDENTID& id) const
    {
        return this->nData64 != id.nData64 || this->nHead64 != id.nHead64;
    }

    BOOL operator < (const LPIDENTID& id) const
    {
        if(this->nHead64 == id.nHead64)
        {
            return this->nData64 < id.nData64;
        }

        return this->nHead64 < id.nHead64;
    }

    std::string ToString() const
    {
        return lpSerializeToString(MAX_IDENTID_LEN, FMT_I64 "-" FMT_I64, nHead64, nData64);
    }

    BOOL FromString(const std::string& strID)
    {
        size_t nStrLength = strID.length();
        size_t nPos = strID.find('-');
        if(nPos == std::string::npos)
        {
            return FALSE;
        }

        std::string strHead = strID.substr(0, nPos);
        std::string strData = "";
        if(nPos + 1 < nStrLength)
        {
            strData = strID.substr(nPos + 1, nStrLength - nPos);
        }

        try
        {
            nHead64 = atoll(strHead.c_str());
            nData64 = atoll(strData.c_str());

            return TRUE;
        }
        catch(...)
        {
            return FALSE;
        }
    }
};

//#pragma pack(pop)



//end声明所处的名字空间
NS_LZPL_END

#endif
