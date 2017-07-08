//****************************************************************************/
//    author: caihy
//    date: January 15, 2016
//    description:
//
//****************************************************************************/
#ifndef _LP_FILE_LIST_H_
#define _LP_FILE_LIST_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      文件列表类
struct DECLARE FILE_LIST
{
    static const int MAX_FILE_LIST_COUNT = 256;

    LPUINT32      dwSize;
    char*        szFiles[MAX_FILE_LIST_COUNT];

    FILE_LIST()
    {
        dwSize = 0;
        memset(szFiles, 0, sizeof(char*) * MAX_FILE_LIST_COUNT);
    }
    ~FILE_LIST()
    {
        _CleanUp();
    }

    // Summary:
    //      扫描指定目录下的文件
    // Input:
    //      pcszExt: 扩展名，空字符串则扫描所有
    //      bFilenameWithDir: 扫描的文件名存储是否包括目录
    BOOL LPAPI ScanDirectory(const char* pcszDir, const char* pcszExt, BOOL bFilenameWithDir);

private:

    void _CleanUp(void);
    BOOL _FileListAdd(const char * pcszDir, const char* pcszPath, BOOL bFilenameWithDir);
    static int _Compare(const void* s1, const void* s2);
};



//end声明所处的名字空间
NS_LZPL_END

#endif