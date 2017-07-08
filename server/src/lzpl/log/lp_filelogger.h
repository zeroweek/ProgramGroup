//****************************************************************************/
//    author: caihy
//    date: June 10, 2015
//    description: 文件类型日志实现
//
//****************************************************************************/
#ifndef _LP_FILE_LOGGER_H_
#define _LP_FILE_LOGGER_H_

#include "lpi_logger.h"
#include "lp_system.h"
#include "lp_loopbuf.h"
#include "lp_lock.h"
#include "lp_thread.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      文件日志实现类
class LPFileLogger : public ILPFileLogger, public LPThread
{

public:

    // Summary:
    //      无
    LPFileLogger();

    // Summary:
    //      无
    virtual ~LPFileLogger();

    // Summary:
    //      引用计数加1
    virtual void LPAPI AddRef(void);

    // Summary:
    //      引用计数减1
    virtual LPUINT32 LPAPI QueryRef(void);

    // Summary:
    //      释放
    virtual void LPAPI Release();

    // Summary:
    //      记录日志
    // Input:
    //      pszLog：日志内容
    virtual void LPAPI LogText(const char *pszLog, LPUINT32 dwLen);

    // Summary:
    //      创建日志消息id
    // Input:
    //      pszLog：日志内容
    virtual LPUINT32 LPAPI CreateMsgId();

    // Summary:
    //   初始化
    // Input:
    //   stLogConfig：日志配置
    virtual BOOL LPAPI Init(LOG_CONFIG& stLogConfig);

    // Summary:
    //      无
    virtual void LPAPI UnInit();

private:

    // Summary:
    //      无
    void LPAPI _Flush();

    // Summary:
    //      无
    void LPAPI _CloseFile();

    // Summary:
    //      更新文件指针的位置
    void LPAPI _UpdateFilePointer();

    // Summary:
    //      获取文件路径
    const char* LPAPI _GetFilePath(LPTime& oTime);

    // Summary:
    //   打开或创建日志文件
    FILE* LPAPI _OpenOrCreateLogFile(LPTime& oTime);

private:

    static THREAD_FUNC_DECLARE(_ThreadProc)(void* pParam);

private:

    BOOL                     m_bRun;                       // 线程运行标记
    BOOL                     m_bInit;                      // 是否初始化标记
    volatile atomic_bool     m_bErrorLog;                  // 日志错误

    LPThread                 m_oThread;
    LPUINT32                  m_dwRef;                      // 本日志对象引用计数
    e_LogMode                m_eLogMode;                   // 日志记录模式
    LPUINT32                  m_dwOutputMask;                // 输出掩码

    char                     m_szDir[MAX_PATH];            // 目录路径
    char                     m_szFileName[MAX_FILE_NAME];  // 日志文件名
    char                     m_szFilePath[MAX_PATH];       // 文件路径

    FILE*                    m_fpFile;                     // 打开的文件句柄
    BOOL                     m_bDirty;                     // 是否有数据还没fflush到文件
    LPLoopBuf*               m_pLoopBuf;                   // 日志缓冲区
    LPUINT32                  m_dwSubFile;                  // 子文件编号（超过单个文件大小则需要分子文件记录）
    LPTime                   m_oLastTime;                  // 上次时间
    LPTime                   m_oCurTime;                   // 当前时间
    LPUINT32                  m_dwMsgId;                    // 消息id编号
    LOG_CONFIG               m_stLogConfig;                // 日志配置

};



//end声明所处的名字空间
NS_LZPL_END
#endif