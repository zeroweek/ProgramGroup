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
//		\t�ָ����ļ���
class DECLARE LPTabFile
{
public:

	// Summary:
	//		��
	LPTabFile();

	// Summary:
	//		��
	~LPTabFile();

	// Summary:
	//		��ʼ��
	// Input:
	//		nSkipLine: ���ļ���=1+n�п�ʼ��ȡ���ų��ֶ����У���������row������0��ʼ��row=0ʵ��Ϊ�ļ��ĵ�2�У�
	//		bZeroReplaceNull: �Ƿ���"0"��Ϊ��ֵ�ֶε�ֵ��TRUE-��"0"�ַ�����FALSE-��""���ַ���
	BOOL LPAPI Init(const char* pcszFileName, INT_32 nSkipLine, BOOL bZeroReplaceNull);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		��
	INT_32 LPAPI GetRowCount(void);

	// Summary:
	//		��
	INT_32 LPAPI GetColCount(void);

	// Summary:
	//		��
	const char* LPAPI GetColName(INT_32 nCol);

	BOOL LPAPI ReadData(INT_32 nCol, INT_32 nRow, INT_32& rnValue);
	BOOL LPAPI ReadData(const char* pcszColName, int nRow, INT_32& rnValue);
	BOOL LPAPI ReadData(INT_32 nCol, INT_32 nRow, char* pszValue, INT_32 nSize);
	BOOL LPAPI ReadData(const char* pcszColName, int nRow, char* pszValue, INT_32 nSize);

	UINT_32 LPAPI GetCRC(void);

private:

	const char* LPAPI GetData(INT_32 nCol, INT_32 nRow);
	const char* LPAPI GetData(const char* pcszColName, INT_32 nRow);

private:

	char*                    m_pData;
	size_t                   m_uSize;
	INT_32                   m_nSkipLine;
	INT_32                   m_nRowCount;
	INT_32                   m_nColCount;
	BOOL                     m_bZeroReplaceNull;
	INT_32*                  m_pOffsetTable;
	UINT_32                  m_dwCRC32;

	typedef std::map<std::string, int, std::less<std::string> > MAP_NAME_2_COL;
	MAP_NAME_2_COL           m_mapName2Col;

};





//end�������������ֿռ�
NS_LZPL_END

#endif