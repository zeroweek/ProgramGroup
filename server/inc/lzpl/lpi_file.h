//****************************************************************************/
//    author: caihy
//    date: October 8, 2015
//    description: 文件接口定义
//
//****************************************************************************/
#ifndef _LPI_FILE_H_
#define _LPI_FILE_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//		文件接口类
class DECLARE ILPFile
{
public:

	// Summary:
	//		无
	virtual ~ILPFile() {}

	// Summary:
	//		关闭文件
	virtual void LPAPI Close() = 0;

	// Summary:
	//		读取数据（由写操作切换到读操作时一定要seek，否则继续读取不会碰到eof，读取的数据为乱码）
	// Input:
	//		pData：数据缓冲区
	//		dwLen：长度
	// Return:
	//		成功读取的长度，如果不成功或读到文件末尾返回0
	virtual LPUINT32 LPAPI Read(void* pData, LPUINT32 dwLen) = 0;

	// Summary:
	//		写数据，函数以二进制形式对文件进行操作，不局限于文本文件
	// Input:
	//		pData：数据缓冲区
	//		dwLen：长度
	// Return:
	//		成功写入的长度
	virtual LPUINT32 LPAPI Write(const void* pData, LPUINT32 dwLen) = 0;

	// Summary:
	//		文件读指针定位
	// Input:
	//		nOffset：与基准位置的偏移量
	//		nOrigin：基准原位置
	// Return:
	//		成功读取的长度，如果不成功或读到文件末尾返回0
	virtual BOOL LPAPI Seek(LPINT32 nOffset, LPINT32 nOrigin) = 0;

	// Summary:
	//		文件数据指针当前位置
	// Return:
	//		文件数据指针当前位置
	virtual LPUINT32 LPAPI Tell(void) = 0;

	// Summary:
	//		是否设置遇到文件为标记（只有上一次读取遇到文件尾才会设置该标记，上一次读取的数据刚刚好到最后一个字节则不会设置）
	// Return:
	//		TRUE-是，FALSE-否
	virtual BOOL LPAPI Eof(void) = 0;

	// Summary:
	//		文件大小
	// Return:
	//		文件的总大小
	virtual LPUINT32 LPAPI Size(void) = 0;

};



// Summary:
//		打开文件
	// Input:
	//		pcszFileName：文件名
	//		pcszMode：文件打开模式
// Return:
//		文件对象
DECLARE ILPFile* LPAPI lpFileOpen(const char* pcszFileName, const char* pcszMode);



// Summary:
//		判断文件是否存在
// Input:
//		pcszFileName：文件名
// Return:
//		文件对象
DECLARE BOOL LPAPI lpFileIsExist(const char* pcszFileName);





//end声明所处的名字空间
NS_LZPL_END

#endif