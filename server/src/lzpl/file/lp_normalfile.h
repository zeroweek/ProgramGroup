//****************************************************************************/
//    author: caihy
//    date: October 8, 2015
//    description: 普通文件类实现
//
//****************************************************************************/
#ifndef _LP_NORMAL_FILE_H_
#define _LP_NORMAL_FILE_H_

#include "lpi_file.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		普通文件类
class DECLARE LPNormalFile : public ILPFile
{
public:

	// Summary:
	//		无
	LPNormalFile();

	// Summary:
	//		无
	virtual ~LPNormalFile();

	// Summary:
	//		初始化
	BOOL LPAPI Init(const char* pcszFileName, const char* pcszMode);

	// Summary:
	//		反始化
	BOOL LPAPI UnInit(void);

	// Summary:
	//		关闭文件
	virtual void LPAPI Close(void);

	// Summary:
	//		读取数据
	// Input:
	//		pData：数据缓冲区
	//		dwLen：长度
	// Return:
	//		成功读取的长度，如果不成功或读到文件末尾返回0
	virtual LPUINT32 LPAPI Read(void* pData, LPUINT32 dwLen);

	// Summary:
	//		写数据，函数以二进制形式对文件进行操作，不局限于文本文件
	// Input:
	//		pData：数据缓冲区
	//		dwLen：长度
	// Return:
	//		成功写入的长度
	virtual LPUINT32 LPAPI Write(const void* pData, LPUINT32 dwLen);

	// Summary:
	//		文件读指针定位
	// Input:
	//		nOffset：与基准位置的偏移量
	//		nOrigin：基准原位置
	// Return:
	//		成功读取的长度，如果不成功或读到文件末尾返回0
	virtual BOOL LPAPI Seek(LPINT32 nOffset, LPINT32 nOrigin);

	// Summary:
	//		文件数据指针当前位置
	// Return:
	//		文件数据指针当前位置
	virtual LPUINT32 LPAPI Tell(void);

	// Summary:
	//		是否遇到文件尾
	// Return:
	//		TRUE-是，FALSE-否
	virtual BOOL LPAPI Eof(void);

	// Summary:
	//		文件大小
	// Return:
	//		文件的总大小
	virtual LPUINT32 LPAPI Size(void);

private:

	FILE*           m_pFile;

};





//end声明所处的名字空间
NS_LZPL_END

#endif