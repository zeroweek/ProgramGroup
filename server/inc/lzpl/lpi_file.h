//****************************************************************************/
//    author: caihy
//    date: October 8, 2015
//    description: �ļ��ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_FILE_H_
#define _LPI_FILE_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		�ļ��ӿ���
class DECLARE ILPFile
{
public:

	// Summary:
	//		��
	virtual ~ILPFile() {}

	// Summary:
	//		�ر��ļ�
	virtual void LPAPI Close() = 0;

	// Summary:
	//		��ȡ���ݣ���д�����л���������ʱһ��Ҫseek�����������ȡ��������eof����ȡ������Ϊ���룩
	// Input:
	//		pData�����ݻ�����
	//		dwLen������
	// Return:
	//		�ɹ���ȡ�ĳ��ȣ�������ɹ�������ļ�ĩβ����0
	virtual LPUINT32 LPAPI Read(void* pData, LPUINT32 dwLen) = 0;

	// Summary:
	//		д���ݣ������Զ�������ʽ���ļ����в��������������ı��ļ�
	// Input:
	//		pData�����ݻ�����
	//		dwLen������
	// Return:
	//		�ɹ�д��ĳ���
	virtual LPUINT32 LPAPI Write(const void* pData, LPUINT32 dwLen) = 0;

	// Summary:
	//		�ļ���ָ�붨λ
	// Input:
	//		nOffset�����׼λ�õ�ƫ����
	//		nOrigin����׼ԭλ��
	// Return:
	//		�ɹ���ȡ�ĳ��ȣ�������ɹ�������ļ�ĩβ����0
	virtual BOOL LPAPI Seek(LPINT32 nOffset, LPINT32 nOrigin) = 0;

	// Summary:
	//		�ļ�����ָ�뵱ǰλ��
	// Return:
	//		�ļ�����ָ�뵱ǰλ��
	virtual LPUINT32 LPAPI Tell(void) = 0;

	// Summary:
	//		�Ƿ����������ļ�Ϊ��ǣ�ֻ����һ�ζ�ȡ�����ļ�β�Ż����øñ�ǣ���һ�ζ�ȡ�����ݸոպõ����һ���ֽ��򲻻����ã�
	// Return:
	//		TRUE-�ǣ�FALSE-��
	virtual BOOL LPAPI Eof(void) = 0;

	// Summary:
	//		�ļ���С
	// Return:
	//		�ļ����ܴ�С
	virtual LPUINT32 LPAPI Size(void) = 0;

};



// Summary:
//		���ļ�
	// Input:
	//		pcszFileName���ļ���
	//		pcszMode���ļ���ģʽ
// Return:
//		�ļ�����
DECLARE ILPFile* LPAPI lpFileOpen(const char* pcszFileName, const char* pcszMode);



// Summary:
//		�ж��ļ��Ƿ����
// Input:
//		pcszFileName���ļ���
// Return:
//		�ļ�����
DECLARE BOOL LPAPI lpFileIsExist(const char* pcszFileName);





//end�������������ֿռ�
NS_LZPL_END

#endif