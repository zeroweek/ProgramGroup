//****************************************************************************/
//    author: caihy
//    date: June 10, 2015
//    description: 日志组件接口定义
//
//****************************************************************************/
#ifndef _LPI_LOGGER_H_
#define _LPI_LOGGER_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      日志级别定义
enum e_LogLevel
{
    eLogLevel_Off       = 0x00000000,   // 关闭所有
    eLogLevel_Debug     = 0x00000001,   // 调试日志
    eLogLevel_Info      = 0x00000002,   // 普通信息
    eLogLevel_Warn      = 0x00000004,   // 警告信息
    eLogLevel_Important = 0x00000008,   // 重要信息
    eLogLevel_Error     = 0x00000010,   // 一般性错误
    eLogLevel_Fatal     = 0x00000020,   // 致命错误
    eLogLevel_Lua       = 0x00000040,   // lua信息
    eLogLevel_All       = 0x0fffffff    // 打开所有
};



// Summary:
//      日记记录模式：针对文件类型日志
enum e_LogMode
{
    eLogMode_None            = 0,      // none
    eLogMode_SingleFile      = 1,      // 记录在单个文件
    eLogMode_DayDivide       = 2,      // 按天分开记在不同文件
    eLogMode_MonthDivide     = 3,      // 按月分开记在不同文件
    eLogMode_HourDivide      = 4,      // 按小时分开记在不同文件
    eLogMode_Max
};



// Summary:
//      日记输出级别定义
enum e_OutputType
{
    eOutputType_Off       = 0,            // none
    eOutputType_File      = 0x00000001,   // 输出到文件
    eOutputType_Console   = 0x00000002,   // 输出到控制台
    eOutputType_Debug     = 0x00000004,   // 调试输出
    eOutputType_All       = 0x0fffffff    // all
};



// Summary:
//      日志类型
enum e_LogType
{
    eLogType_None            = 0,      // none
    eLogType_File            = 1,      // 文件日志，日志记录在文件
    eLogType_Net             = 2,      // 网络日志，日志发送到网络
    eLogType_Mysql           = 3,      // 数据库日志，日志存储到数据库
    eLogType_Max
};



// Summary:
//   控制台前景颜色
// Color Define:
//   0 = 黑色      8 = 灰色
//   1 = 蓝色      9 = 淡蓝色
//   2 = 绿色      A = 淡绿色
//   3 = 浅绿色    B = 淡浅绿色
//   4 = 红色      C = 淡红色
//   5 = 紫色      D = 淡紫色
//   6 = 黄色      E = 淡黄色
//   7 = 白色      F = 亮白色
enum e_ConsoleTextColor
{
#   ifdef _WIN32
    CTC_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
    CTC_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CTC_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    CTC_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    CTC_PURPLE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    CTC_CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    CTC_GRAY = FOREGROUND_INTENSITY,
    CTC_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    CTC_HIGH_WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    CTC_BLACK = 0
#   else
    CTC_RED = 0,
    CTC_GREEN = 0,
    CTC_BLUE = 0,
    CTC_YELLOW = 0,
    CTC_PURPLE = 0,
    CTC_CYAN = 0,
    CTC_GRAY = 0,
    CTC_WHITE = 0,
    CTC_HIGH_WHITE = 0,
    CTC_BLACK = 0
#   endif
};



// Summary:
//   控制台背景颜色
// Color Define:
//   颜色同前景颜色一样，只不过左移16位
enum e_ConsoleBackGroundColor
{
#   ifdef _WIN32
    CBC_RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
    CBC_GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
    CBC_BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
    CBC_YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
    CBC_PURPLE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
    CBC_CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
    CBC_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
    CBC_HIGH_WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
    CBC_BLACK = 0
#   else
    CBC_RED = 0,
    CBC_GREEN = 0,
    CBC_BLUE = 0,
    CBC_YELLOW = 0,
    CBC_PURPLE = 0,
    CBC_CYAN = 0,
    CBC_WHITE = 0,
    CBC_HIGH_WHITE = 0,
    CBC_BLACK = 0
#   endif
};



// Summary:
//   设置控制台输出颜色
DECLARE void LPAPI lpSetConsoleColor(e_ConsoleTextColor textColor = CTC_WHITE, e_ConsoleBackGroundColor backgroundColor = CBC_BLACK);



// Summary:
//   日志配置结构对象
struct LOG_CONFIG
{
    char                    szLogDir[MAX_PATH];              //日志文件的目录
    char                    szFileName[MAX_FILE_NAME];       //日志文件名
    char                    szModulePrefix[COMMON_NAME_LEN]; //日志信息模块前缀
    BOOL                    bWrithLock;                      //日志是否写加锁
    LPUINT32                dwLogMode;                       //日志模式
    LPUINT32                dwLogLevelConfig;                //日志等级配置
    LPUINT32                dwOutputMask;                    //日志输出掩码

    LPUINT32                dwMaxLogLoopBufSize;             //文件日志缓冲区大小，默认=10*1024*1024
    LPUINT32                dwMaxLogSingleFileSize;          //文件日志单个文件最大容量，默认=10*1024*1024
    LPUINT32                dwMaxLogOneMsgSize;              //日志消息最大字节数，默认=4096
    LPUINT32                dwLogFlushInterval;              //日志flush间隔（单位/毫秒），默认=3000
    LPINT32                 nTimezone;                       //时区
};



// Summary:
//      日志接口类
class DECLARE ILPLogger
{
public:

    // Summary:
    //      无
    virtual ~ILPLogger() {}

    // Summary:
    //      记录日志
    // Input:
    //      pszLog：日志内容
    virtual void LPAPI LogText(const char *pszLog, LPUINT32 dwLen) = 0;

    // Summary:
    //      创建日志消息id
    // Input:
    //      pszLog：日志内容
    virtual LPUINT32 LPAPI CreateMsgId() = 0;

};



// Summary:
//      日志控制接口类
class DECLARE ILPLoggerCtrl
{
public:

    // Summary:
    //      无
    virtual ~ILPLoggerCtrl() {}

    // Summary:
    //   初始化
    // Input:
    //   pLogger：底层使用的日志对象
    //   stLogConfig：日志配置
    virtual BOOL LPAPI Init(lp_shared_ptr<ILPLogger>, LOG_CONFIG& stLogConfig) = 0;

    // Summary:
    //      lua信息
    virtual void LPAPI Lua(const char* format, ...) = 0;

    // Summary:
    //      致命错误信息
    virtual void LPAPI Fatal(const char* format, ...) = 0;

    // Summary:
    //      一般性错信息
    virtual void LPAPI Error(const char* format, ...) = 0;

    // Summary:
    //      警告
    virtual void LPAPI Warn(const char* format, ...) = 0;

    // Summary:
    //      重要信息
    virtual void LPAPI Important(const char* format, ...) = 0;

    // Summary:
    //      普通信息
    virtual void LPAPI Info(const char* format, ...) = 0;

    // Summary:
    //      调试信息
    virtual void LPAPI Debug(const char* format, ...) = 0;

public:

    // Summary:
    //   初始化LZPL默认日志控制对象
    //   注：使用层应当在使用LZPL组件之前调用此日志模块初始化函数
    // Input:
    //   stLogConfig: 日志配置
    // Return:
    //   TRUE-成功，FALSE-失败
    static BOOL LPAPI InitLzplLoggerCtrl(LOG_CONFIG& stLogConfig);

    // Summary:
    //      设置组件内部使用的ILPLoggerCtrl（不使用默认的）
    // Input:
    //      poLzplLoggerCtrl：日志控制器对象
    // Return:
    //      TRUE-成功，FALSE-失败
    static BOOL LPAPI SetLzplLoggerCtrl(lp_shared_ptr<ILPLoggerCtrl> pLzplLoggerCtrl);

    // Summary:
    //      获取LZPL内部日志控制对象
    // Return:
    //      网络组建日志控制对象
    static lp_shared_ptr<ILPLoggerCtrl> LPAPI GetLzplLoggerCtrl();

public:

    static lp_shared_ptr<ILPLoggerCtrl> m_pLzplLoggerCtrl;

public:

    static lp_shared_ptr<ILPLoggerCtrl> LPAPI CreateLoggerCtrl();
    static void LPAPI DeleteLoggerCtrl(lp_shared_ptr<ILPLoggerCtrl>& pLoggerCtrl);
};



// Summary:
//      文件日志接口类
class DECLARE ILPFileLogger : public ILPLogger
{
public:

    // Summary:
    //      无
    virtual ~ILPFileLogger() {}

    // Summary:
    //   初始化
    // Input:
    //   stLogConfig：日志配置
    virtual BOOL LPAPI Init(LOG_CONFIG& stLogConfig) = 0;

public:

    // Summary:
    //      创建文件日志对象
    // Return:
    //      文件日志对象
    static lp_shared_ptr<ILPFileLogger> LPAPI CreateFileLogger();
    static void LPAPI DeleteFileLogger(lp_shared_ptr<ILPFileLogger>& pLogger);
};



//lua相关消息（不屏蔽）
#define LUA        if(nullptr != ILPLoggerCtrl::GetLzplLoggerCtrl()) ILPLoggerCtrl::GetLzplLoggerCtrl()->Lua
//致命消息（不屏蔽）
#define FTL        if(nullptr != ILPLoggerCtrl::GetLzplLoggerCtrl()) ILPLoggerCtrl::GetLzplLoggerCtrl()->Fatal
//错误消息（不屏蔽）
#define ERR        if(nullptr != ILPLoggerCtrl::GetLzplLoggerCtrl()) ILPLoggerCtrl::GetLzplLoggerCtrl()->Error
//警告消息（不屏蔽）
#define WRN        if(nullptr != ILPLoggerCtrl::GetLzplLoggerCtrl()) ILPLoggerCtrl::GetLzplLoggerCtrl()->Warn
//重要消息（不屏蔽）
#define IMP        if(nullptr != ILPLoggerCtrl::GetLzplLoggerCtrl()) ILPLoggerCtrl::GetLzplLoggerCtrl()->Important
//一般消息（release可屏蔽）
#define INF        if(nullptr != ILPLoggerCtrl::GetLzplLoggerCtrl()) ILPLoggerCtrl::GetLzplLoggerCtrl()->Info
//调试提示（release屏蔽）
#define DBG        if(nullptr != ILPLoggerCtrl::GetLzplLoggerCtrl()) ILPLoggerCtrl::GetLzplLoggerCtrl()->Debug



//end声明所处的名字空间
NS_LZPL_END

#endif