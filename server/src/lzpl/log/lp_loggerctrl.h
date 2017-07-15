//****************************************************************************/
//    author: caihy
//    date: June 10, 2015
//    description: 日志操作类实现
//
//****************************************************************************/
#ifndef _LP_LOGGER_CTRL_H_
#define _LP_LOGGER_CTRL_H_

#include "lpi_logger.h"
#include "lp_lock.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      日志控制类
class DECLARE LPLoggerCtrl : public ILPLoggerCtrl
{
public:

    // Summary:
    //      无
    LPLoggerCtrl();

    // Summary:
    //      无
    virtual ~LPLoggerCtrl();

    // Summary:
    //      初始化
    // Input:
    //      pLogger：底层使用的日志对象
    //      pszPrefix：日志信息前缀，空则传入NULL
    //      dwLogLevelConfig：日志等级配置
    virtual BOOL LPAPI Init(lp_shared_ptr<ILPLogger> pLogger, LOG_CONFIG& stLogConfig);

    // Summary:
    //      无
    virtual BOOL LPAPI UnInit(void);

    // Summary:
    //      lua信息
    virtual void LPAPI Lua(const char* format, ...);

    // Summary:
    //      致命错误信息
    virtual void LPAPI Fatal(const char* format, ...);

    // Summary:
    //      一般性错信息
    virtual void LPAPI Error(const char* format, ...);

    // Summary:
    //      警告
    virtual void LPAPI Warn(const char* format, ...);

    // Summary:
    //      重要信息
    virtual void LPAPI Important(const char* format, ...);

    // Summary:
    //      普通信息
    virtual void LPAPI Info(const char* format, ...);

    // Summary:
    //      调试信息
    virtual void LPAPI Debug(const char* format, ...);

private:

    // Summary:
    //      无
    void LPAPI _Log(e_LogLevel eLogLevel, const char* format, va_list argptr);

    // Summary:
    //      无
    const char* LPAPI _GetLogLevelPrompt(e_LogLevel eLogLevel);

private:

    lp_shared_ptr<ILPLogger>            m_pLogger;                             // 底层使用的日志对象
    BOOL                                m_bLogLock;                            // 是否使用锁
    LPLock                              m_oLock;                               // 锁
    LPUINT32                            m_dwLogLevelConfig;                    // 日志等级配置，控制各个等级日志的开关
    LPUINT32                            m_dwOutputMask;
    char                                m_szModulePrefix[COMMON_NAME_LEN];     // 日志模块前缀，表明是哪个模块记录的日志信息
    LOG_CONFIG                          m_stLogConfig;                         // 日志配置
};



//end声明所处的名字空间
NS_LZPL_END
#endif