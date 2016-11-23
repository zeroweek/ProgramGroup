//****************************************************************************/
//    author: caihy
//    date: October 8, 2015
//    description: ��ͨ�ļ���ʵ��
//
//****************************************************************************/
#ifndef _LP_NORMAL_FILE_H_
#define _LP_NORMAL_FILE_H_

#include "lpi_file.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		��ͨ�ļ���
class DECLARE LPNormalFile : public ILPFile
{
public:

	// Summary:
	//		��
	LPNormalFile();

	// Summary:
	//		��
	virtual ~LPNormalFile();

	// Summary:
	//		��ʼ��
	BOOL LPAPI Init(const char* pcszFileName, const char* pcszMode);

	// Summary:
	//		��ʼ��
	BOOL LPAPI UnInit(void);

	// Summary:
	//		�ر��ļ�
	virtual void LPAPI Close(void);

	// Summary:
	//		��ȡ����
	// Input:
	//		pData�����ݻ�����
	//		dwLen������
	// Return:
	//		�ɹ���ȡ�ĳ��ȣ�������ɹ�������ļ�ĩβ����0
	virtual UINT_32 LPAPI Read(void* pData, UINT_32 dwLen);

	// Summary:
	//		д���ݣ������Զ�������ʽ���ļ����в��������������ı��ļ�
	// Input:
	//		pData�����ݻ�����
	//		dwLen������
	// Return:
	//		�ɹ�д��ĳ���
	virtual UINT_32 LPAPI Write(const void* pData, UINT_32 dwLen);

	// Summary:
	//		�ļ���ָ�붨λ
	// Input:
	//		nOffset�����׼λ�õ�ƫ����
	//		nOrigin����׼ԭλ��
	// Return:
	//		�ɹ���ȡ�ĳ��ȣ�������ɹ�������ļ�ĩβ����0
	virtual BOOL LPAPI Seek(INT_32 nOffset, INT_32 nOrigin);

	// Summary:
	//		�ļ�����ָ�뵱ǰλ��
	// Return:
	//		�ļ�����ָ�뵱ǰλ��
	virtual UINT_32 LPAPI Tell(void);

	// Summary:
	//		�Ƿ������ļ�β
	// Return:
	//		TRUE-�ǣ�FALSE-��
	virtual BOOL LPAPI Eof(void);

	// Summary:
	//		�ļ���С
	// Return:
	//		�ļ����ܴ�С
	virtual UINT_32 LPAPI Size(void);

private:

	FILE*           m_pFile;

};





//end�������������ֿռ�
NS_LZPL_END

#endif