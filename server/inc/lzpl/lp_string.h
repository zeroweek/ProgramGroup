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



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//   ȫ�ֱ���
extern std::string NULL_STR;



// Summary:
//		�����ַ����������ʼ��
template <class T>
inline DECLARE void LPAPI lpFastZeroCharArray(T& dst) throw()
{
	// Use static_cast to ensure the function only allow character arrays
	(static_cast<char[sizeof(dst)]>(dst));
	
	dst[0] = '\0';
	dst[sizeof(dst) - 1] = '\0';
}



// Summary:
//   �����ַ��������ʼ��
inline DECLARE void LPAPI lpStrFastZero(char* pStr)
{
	if (NULL != pStr)
	{
		*pStr = '\0';
		return;
	}
}



// Summary:
//		��ȡ�ַ����ĳ��ȣ�������NULL������������ֵ��󲻳���ָ������
inline DECLARE LPUINT32 LPAPI lpStrNLen(const char* pStr, LPUINT32 tMaxLen)
{
	for (LPUINT32 i = 0; i < tMaxLen; ++i)
	{
		if ('\0' == *pStr)
		{
			return i;
		}
		++pStr;
	}

	return tMaxLen;
}



// Summary:
//		·���ַ����е�'\\'��'/'�ַ�ͳһ����Ϊ����ϵͳ��Ĭ�ϵ�·���ָ���
DECLARE void LPAPI lpPathFilter(char* pszPath, LPUINT32 dwMaxLen);



// Summary:
//		�����ַ�����scr����null�ַ�����������dst��֤��null�ַ���β��
// Input:
//		size: ��࿽���ַ�����������null�ַ���
// Return:
//		����ʵ�ʿ������ַ�������������null�ַ���
DECLARE LPUINT64 LPAPI lpStrCpyN(char* dst, const char* src, LPUINT64 size);



// Summary:
//		�����ַ�����scr����null�ַ�����������dst��֤��null�ַ���β��
// Input:
//		size: ���Ӻ�dst�ַ�������ַ�����������null�ַ���
// Return:
//		�������Ӻ�dst�ַ������ַ�������������null�ַ���
DECLARE LPUINT64 LPAPI lpStrCatN(char* dst, const char* src, LPUINT64 size);



// Summary:
//   �ַ��������ö��
enum e_CharCheckType
{
	eCharCheckType_None       = 0,
	eCharCheckType_Digit      = 1, //����Ƿ�������
	eCharCheckType_Alpha      = 2, //����Ƿ�����ĸ
	eCharCheckType_Alnum      = 3, //����Ƿ����ֺ���ĸ�����
	eCharCheckType_Integer    = 4, //����Ƿ�������������0��������
	eCharCheckType_Max
};


// Summary:
//   ����ַ����Ƿ�ָ������
// Input:
//   pcszSrc���������ַ���
//   nMaxLen���������ַ������������-1��ʾ����'\0'Ϊֹ���������0�������ַ�������
//   bIsEmptyMeet��TRUE-�ַ���Ϊ��ʱ���㣬FALSE-�ַ���Ϊ��ʱ������
// Return:
//   TRUE-���㣬FALSE-������
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
//   �Զ����ַ�����
//   ȱ�ٸ�ֵ������������Ϊleft-value
class DECLARE LPString
{
public:

	LPString(LPUINT32 dwSize = SIZE_512, bool bAutoExtend = true);
	virtual ~LPString();

	void LPAPI Reset();

	virtual LPUINT32 LPAPI GetSize() { return m_dwDataSize; }
	virtual LPUINT32 LPAPI GetFreeSize();

	virtual BOOL LPAPI Read(LPString& oDstStr, LPUINT32 dwDstStartPos, LPUINT32 dwSrcStartPos, LPUINT32 dwReadLen);
	virtual BOOL LPAPI Read(char* pszDst, LPUINT32 dwDstStartPos, LPUINT32 dwDstMaxLen, LPUINT32 dwStartPos, LPUINT32 dwReadLen, BOOL bNullTerminate);

	virtual BOOL LPAPI Append(const char c);
	virtual BOOL LPAPI Append(const char* pszSrc, LPUINT32 dwStartPos, LPUINT32 dwLen);
	virtual BOOL LPAPI Append(LPString& oStr, LPUINT32 dwStartPos, LPUINT32 dwLen);

	// Summary:
	//   ��ָ��λ�ÿ�ʼ�����ַ���
	// Input:
	//   dwDstStartPos��Ŀ���ַ�������ʼ����λ�ã���ֵ����С�ڵ���Ŀ���ַ�������Ч���ݴ�С
	virtual BOOL LPAPI Insert(LPUINT32 dwDstStartPos, LPString& oSrcStr, LPUINT32 dwSrcStartPos, LPUINT32 dwLen);
	virtual BOOL LPAPI Insert(LPUINT32 dwDstStartPos, const char* pszSrc, LPUINT32 dwSrcStartPos, LPUINT32 dwLen);

	virtual char* LPAPI GetData();

private:

	// Summary:
	//   ��ʼ��
	// Input:
	//   dwSize: ��ʼ��������������������ֹ���������ݴ�ʱ��������֤��512�ı���
	//   bAutoExtend: �Ƿ��Զ���չ
	BOOL LPAPI Init(LPUINT32 dwSize, bool bAutoExtend);
	BOOL LPAPI UnInit();

	virtual LPUINT32 LPAPI GetCapacity() { return m_dwCapacity; }
	virtual BOOL LPAPI CheckExtend(LPUINT32 dwWillAddSize);

private:

	char*                   m_pBuf;          // �������ڴ�

	bool                    m_bAutoExtend;   // �Զ�����
	LPUINT32                 m_dwDataSize;    // ���ݴ�С
	LPUINT32                 m_dwCapacity;    // ������������������1�ֽ���ֹ����
};



// Summary:
//   ���л��ַ���
// Input:
//   nMaxLen: ����ַ�����󳤶ȣ�������null��ֹ��
DECLARE std::string LPAPI lpSerializeToString(LPUINT32 nMaxLen, const char * format, ...);






//end�������������ֿռ�
NS_LZPL_END

#endif