//****************************************************************************/
//    author: caihy
//    date: January 15, 2016
//    description:
//
//****************************************************************************/
#ifndef _LP_FILE_LIST_H_
#define _LP_FILE_LIST_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      �ļ��б���
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
    //      ɨ��ָ��Ŀ¼�µ��ļ�
    // Input:
    //      pcszExt: ��չ�������ַ�����ɨ������
    //      bFilenameWithDir: ɨ����ļ����洢�Ƿ����Ŀ¼
    BOOL LPAPI ScanDirectory(const char* pcszDir, const char* pcszExt, BOOL bFilenameWithDir);

private:

    void _CleanUp(void);
    BOOL _FileListAdd(const char * pcszDir, const char* pcszPath, BOOL bFilenameWithDir);
    static int _Compare(const void* s1, const void* s2);
};



//end�������������ֿռ�
NS_LZPL_END

#endif