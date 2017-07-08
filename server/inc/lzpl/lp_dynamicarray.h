//****************************************************************************/
//    author: caihy
//    date: January 18, 2016
//    description: 动态数组
//
//****************************************************************************/
#ifndef _LP_DYNAMIC_ARRAY_H_
#define _LP_DYNAMIC_ARRAY_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      动态数组类，用于number类型
template <class T>
class DECLARE LPDynamicArray
{
public:

    LPDynamicArray();
    ~LPDynamicArray();

    BOOL LPAPI Init(LPUINT32 dwCap);
    BOOL LPAPI UnInit(void);

    inline BOOL LPAPI Push(T data);

    BOOL LPAPI ReSize(LPUINT32 dwSize);
    inline LPUINT32 LPAPI Size(void);

    inline const T& LPAPI operator[](LPUINT32 i) const;
    inline T& LPAPI operator[](LPUINT32 i);

private:

    BOOL LPAPI _CheckCapacity(LPUINT32 dwSize);

    LPDynamicArray(const LPDynamicArray&);
    LPDynamicArray& operator=(const LPDynamicArray&);


private:

    T*          m_pData;
    LPUINT32     m_dwSize;
    LPUINT32     m_dwCap;
};

template<class T>
inline LPDynamicArray<T>::LPDynamicArray()
{
    m_pData = NULL;
    m_dwSize = 0;
    m_dwCap = 0;
}

template<class T>
inline LPDynamicArray<T>::~LPDynamicArray()
{
    LPINT32 nResult = 0;

    nResult = UnInit();
    LOG_CHECK_ERROR(nResult);
}

template<class T>
inline BOOL LPAPI LPDynamicArray<T>::Init(LPUINT32 dwCap)
{
    LPINT32 nResult = 0;

    nResult = _CheckCapacity(dwCap);
    LOG_PROCESS_ERROR(nResult);

    nResult = ReSize(0);
    LOG_PROCESS_ERROR(nResult);

    return TRUE;
Exit0:
    return FALSE;
}

template<class T>
inline BOOL LPAPI LPDynamicArray<T>::UnInit(void)
{
    SAFE_DELETE_SZ(m_pData);
    m_dwSize = 0;
    m_dwCap = 0;

    return TRUE;
}

template<class T>
inline BOOL LPAPI LPDynamicArray<T>::Push(T data)
{
    LPINT32 nResult = 0;

    nResult = ReSize(m_dwSize + 1);
    LOG_PROCESS_ERROR(nResult);

    m_pData[m_dwSize - 1] = data;

    return TRUE;
Exit0:
    return FALSE;
}

template<class T>
inline LPUINT32 LPAPI LPDynamicArray<T>::Size(void)
{
    return m_dwSize;
}

template<class T>
inline const T& LPAPI LPDynamicArray<T>::operator[](LPUINT32 i) const
{
    return m_pData[i];
}

template<class T>
inline T& LPAPI LPDynamicArray<T>::operator[](LPUINT32 i)
{
    return m_pData[i];
}

template<class T>
inline BOOL LPAPI LPDynamicArray<T>::_CheckCapacity(LPUINT32 dwCap)
{
    T* pNewArray = NULL;

    if(dwCap > m_dwCap)
    {
        if(0 == m_dwCap)
        {
            m_dwCap = dwCap;
        }

        while(m_dwCap < dwCap)
        {
            m_dwCap *= 2;
        }

        pNewArray = new T[m_dwCap * sizeof(T)];
        LOG_PROCESS_ERROR(pNewArray);
        memset(pNewArray, 0, m_dwCap * sizeof(T));

        if(m_dwSize)
        {
            memcpy(pNewArray, m_pData, m_dwSize * sizeof(T));
        }
        if(m_pData)
        {
            SAFE_DELETE_SZ(m_pData);
        }
        m_pData = pNewArray;
    }

    return TRUE;
Exit0:
    return FALSE;
}

template<class T>
BOOL LPAPI LPDynamicArray<T>::ReSize(LPUINT32 dwSize)
{
    LPINT32 nResult = 0;
    T* pNewArray = NULL;

    nResult = _CheckCapacity(dwSize);
    LOG_PROCESS_ERROR(nResult);

    m_dwSize = dwSize;

    return TRUE;
Exit0:
    return FALSE;
}






//end声明所处的名字空间
NS_LZPL_END

#endif