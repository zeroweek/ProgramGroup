//****************************************************************************/
//    author: caihy
//    date: October 9, 2015
//    description: \t�ָ����ļ���ʵ��
//
//****************************************************************************/
#ifndef _LP_TAB_FILE_H_
#define _LP_TAB_FILE_H_

#include "lpi_file.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      \t�ָ����ļ���
class DECLARE LPTabFile
{
public:

    // Summary:
    //      ��
    LPTabFile();

    // Summary:
    //      ��
    ~LPTabFile();

    // Summary:
    //      ��ʼ��
    // Input:
    //      nSkipLine: ���ļ���=1+n�п�ʼ��ȡ���ų��ֶ����У���������row������0��ʼ��row=0ʵ��Ϊ�ļ��ĵ�2�У�
    //      bZeroReplaceNull: �Ƿ���"0"��Ϊ��ֵ�ֶε�ֵ��TRUE-��"0"�ַ�����FALSE-��""���ַ���
    BOOL LPAPI Init(const char* pcszFileName, LPINT32 nSkipLine, BOOL bZeroReplaceNull);

    // Summary:
    //      ��ʼ��
    BOOL LPAPI UnInit(void);

    // Summary:
    //      ��
    LPINT32 LPAPI GetRowCount(void);

    // Summary:
    //      ��
    LPINT32 LPAPI GetColCount(void);

    // Summary:
    //      ��
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





//end�������������ֿռ�
NS_LZPL_END

#endif