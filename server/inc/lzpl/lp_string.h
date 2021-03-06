//****************************************************************************/
//    author: caihy
//    date: June 10, 2015
//    description:
//
//****************************************************************************/
#ifndef _LP_STRING_H_
#define _LP_STRING_H_

#include <string.h>
#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//   全局变量
extern std::string nullstr;



// Summary:
//      快速字符数组清零初始化
template <class T>
inline DECLARE void LPAPI lpFastZeroCharArray(T& dst) throw()
{
    // Use static_cast to ensure the function only allow character arrays
#   ifdef _WIN32
    {
        (static_cast<char[sizeof(dst)]>(dst));
    }
#   endif

    dst[0] = '\0';
    dst[sizeof(dst) - 1] = '\0';
}



// Summary:
//   快速字符串清零初始化
inline DECLARE void LPAPI lpStrFastZero(char* pStr)
{
    if(NULL != pStr)
    {
        *pStr = '\0';
        return;
    }
}



// Summary:
//      获取字符串的长度，不包括NULL结束符，返回值最大不超过指定长度
inline DECLARE LPUINT32 LPAPI lpStrNLen(const char* pStr, LPUINT32 tMaxLen)
{
    for(LPUINT32 i = 0; i < tMaxLen; ++i)
    {
        if('\0' == *pStr)
        {
            return i;
        }
        ++pStr;
    }

    return tMaxLen;
}



// Summary:
//      路径字符串中的'\\'和'/'字符统一处理为运行系统所默认的路径分隔符
DECLARE void LPAPI lpPathFilter(char* pszPath, LPUINT32 dwMaxLen);



// Summary:
//      拷贝字符串，scr遇到null字符结束拷贝（dst保证以null字符结尾）
// Input:
//      size: 最多拷贝字符个数（包括null字符）
// Return:
//      返回实际拷贝的字符个数（不包括null字符）
DECLARE LPUINT64 LPAPI lpStrCpyN(char* dst, const char* src, LPUINT64 size);



// Summary:
//      连接字符串，scr遇到null字符结束拷贝（dst保证以null字符结尾）
// Input:
//      size: 连接后，dst字符串最大字符个数（包括null字符）
// Return:
//      返回连接后，dst字符串的字符个数（不包括null字符）
DECLARE LPUINT64 LPAPI lpStrCatN(char* dst, const char* src, LPUINT64 size);



// Summary:
//   字符检测类型枚举
enum e_CharCheckType
{
    eCharCheckType_None       = 0,
    eCharCheckType_Digit      = 1, //检测是否都是数字
    eCharCheckType_Alpha      = 2, //检测是否都是字母
    eCharCheckType_Alnum      = 3, //检测是否数字和字母的组合
    eCharCheckType_Integer    = 4, //检测是否整数（负数、0、正数）
    eCharCheckType_Max
};


// Summary:
//   检测字符串是否指定类型
// Input:
//   pcszSrc：待检测的字符串
//   nMaxLen：最大检测的字符数，如果等于-1表示遇到'\0'为止，如果等于0则当做空字符串处理
//   bIsEmptyMeet：TRUE-字符串为空时满足，FALSE-字符串为空时不满足
// Return:
//   TRUE-满足，FALSE-不满足
DECLARE BOOL LPAPI lpStrCharCheck(const char* pcszSrc, e_CharCheckType eCharCheckType, LPINT32 nMaxLen, BOOL bIsEmptyMeet);



#define SIZE_16              (16)
#define SIZE_32              (32)
#define SIZE_64              (64)
#define SIZE_128             (128)
#define SIZE_256             (256)
#define SIZE_512             (512)
#define SIZE_1_KB            (1 * KB)
#define SIZE_2_KB            (2 * KB)
#define SIZE_4_KB            (4 * KB)
#define SIZE_8_KB            (8 * KB)
#define SIZE_10_KB           (10 * KB)



// Summary:
//   自定义字符串类
//   缺少赋值函数，不能作为left-value
class DECLARE LPString
{
public:

    LPString(LPUINT32 dwSize = SIZE_512, bool bAutoExtend = true);
    virtual ~LPString();

    void LPAPI Reset();

    virtual LPUINT32 LPAPI GetSize()
    {
        return m_dwDataSize;
    }
    virtual LPUINT32 LPAPI GetFreeSize();

    virtual BOOL LPAPI Read(LPString& oDstStr, LPUINT32 dwDstStartPos, LPUINT32 dwSrcStartPos, LPUINT32 dwReadLen);
    virtual BOOL LPAPI Read(char* pszDst, LPUINT32 dwDstStartPos, LPUINT32 dwDstMaxLen, LPUINT32 dwStartPos, LPUINT32 dwReadLen, BOOL bNullTerminate);

    virtual BOOL LPAPI Append(const char c);
    virtual BOOL LPAPI Append(const char* pszSrc, LPUINT32 dwStartPos, LPUINT32 dwLen);
    virtual BOOL LPAPI Append(LPString& oStr, LPUINT32 dwStartPos, LPUINT32 dwLen);

    // Summary:
    //   从指定位置开始插入字符串
    // Input:
    //   dwDstStartPos：目的字符串的起始插入位置，该值必须小于等于目标字符串的有效数据大小
    virtual BOOL LPAPI Insert(LPUINT32 dwDstStartPos, LPString& oSrcStr, LPUINT32 dwSrcStartPos, LPUINT32 dwLen);
    virtual BOOL LPAPI Insert(LPUINT32 dwDstStartPos, const char* pszSrc, LPUINT32 dwSrcStartPos, LPUINT32 dwLen);

    virtual char* LPAPI GetData();

private:

    // Summary:
    //   初始化
    // Input:
    //   dwSize: 初始化缓冲区容量（包括终止符），数据大时，尽量保证是512的倍数
    //   bAutoExtend: 是否自动扩展
    BOOL LPAPI Init(LPUINT32 dwSize, bool bAutoExtend);
    BOOL LPAPI UnInit();

    virtual LPUINT32 LPAPI GetCapacity()
    {
        return m_dwCapacity;
    }
    virtual BOOL LPAPI CheckExtend(LPUINT32 dwWillAddSize);

private:

    char*                   m_pBuf;          // 缓冲区内存

    bool                    m_bAutoExtend;   // 自动扩充
    LPUINT32                 m_dwDataSize;    // 数据大小
    LPUINT32                 m_dwCapacity;    // 缓存区容量（不包含1字节终止符）
};



// Summary:
//   序列化字符串
// Input:
//   nMaxLen: 输出字符串最大长度，不包含null终止符
DECLARE std::string LPAPI lpSerializeToString(LPUINT32 nMaxLen, const char * format, ...);



// Summary:
//   序列化字符串，会再缓冲区后附加终止符
// Input:
//   pszBuf: 输出缓冲区
//   nBufLen: 输出字符串最大长度，包含null终止符
DECLARE BOOL LPAPI lpSerializeToCString(char* pszBuf, LPUINT32 nBufLen, const char * format, ...);



//end声明所处的名字空间
NS_LZPL_END

#endif