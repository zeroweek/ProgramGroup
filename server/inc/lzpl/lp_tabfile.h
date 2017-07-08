//****************************************************************************/
//    author: caihy
//    date: October 9, 2015
//    description: \t分隔符文件类实现
//
//****************************************************************************/
#ifndef _LP_TAB_FILE_H_
#define _LP_TAB_FILE_H_

#include "lpi_file.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      \t分隔符文件类
class DECLARE LPTabFile
{
public:

    // Summary:
    //      无
    LPTabFile();

    // Summary:
    //      无
    ~LPTabFile();

    // Summary:
    //      初始化
    // Input:
    //      nSkipLine: 从文件的=1+n行开始读取（排除字段名行），数据行row索引从0开始（row=0实际为文件的第2行）
    //      bZeroReplaceNull: 是否用"0"作为空值字段的值，TRUE-用"0"字符串，FALSE-用""空字符串
    BOOL LPAPI Init(const char* pcszFileName, LPINT32 nSkipLine, BOOL bZeroReplaceNull);

    // Summary:
    //      反始化
    BOOL LPAPI UnInit(void);

    // Summary:
    //      无
    LPINT32 LPAPI GetRowCount(void);

    // Summary:
    //      无
    LPINT32 LPAPI GetColCount(void);

    // Summary:
    //      无
    const char* LPAPI GetColName(LPINT32 nCol);

    BOOL LPAPI ReadData(LPINT32 nCol, LPINT32 nRow, LPINT32& rnValue);
    BOOL LPAPI ReadData(const char* pcszColName, int nRow, LPINT32& rnValue);
    BOOL LPAPI ReadData(LPINT32 nCol, LPINT32 nRow, char* pszValue, LPINT32 nSize);
    BOOL LPAPI ReadData(const char* pcszColName, int nRow, char* pszValue, LPINT32 nSize);

    LPUINT32 LPAPI GetCRC(void);

private:

    const char* LPAPI GetData(LPINT32 nCol, LPINT32 nRow);
    const char* LPAPI GetData(const char* pcszColName, LPINT32 nRow);

private:

    char*                    m_pData;
    size_t                   m_uSize;
    LPINT32                   m_nSkipLine;
    LPINT32                   m_nRowCount;
    LPINT32                   m_nColCount;
    BOOL                     m_bZeroReplaceNull;
    LPINT32*                  m_pOffsetTable;
    LPUINT32                  m_dwCRC32;

    typedef std::map<std::string, int, std::less<std::string> > MAP_NAME_2_COL;
    MAP_NAME_2_COL           m_mapName2Col;

};





//end声明所处的名字空间
NS_LZPL_END

#endif