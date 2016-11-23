using System;
using System.Text;
using System.Diagnostics;

namespace lzpl
{
    // Summary:
    //		日志类型
    public enum ELogType
    {
        Invalid = 0,              // invalid
        File,                     // 文件日志，日志记录在文件
        Net,                      // 网络日志，日志发送到网络
        Total                     // total
    };

    // Summary:
    //		日志级别定义
    public enum ELogLevel
    {
        Off = 0x00000000,         // 关闭所有
        Debug = 0x00000001,       // 调试日志
        Warn = 0x00000002,        // 警告
        Info = 0x00000004,        // 普通信息
        Important = 0x00000008,   // 警告
        Error = 0x00000010,       // 一般性错误
        Fatal = 0x00000020,       // 致命错误
        All = 0x0fffffff          // 打开所有
    };

    // Summary:
    //		日记记录模式：针对文件类型日志
    public enum ELogMode
    {
        Invalid = 0,              // invalid
        SingleFile,               // 记录在单个文件
        DayDivide,                // 按天分开记在不同文件
        MonthDivide,              // 按月分开记在不同文件
        Total
    };

    // Summary:
    //		日记输出级别定义
    public enum EOutputType
    {
        Invalid = -1,             // invalid
        Off = 0,                  // all off
        File = 0x00000001,        // 输出到文件
        Console = 0x00000002,     // 输出到控制台
        Debug = 0x00000004,       // 调试输出
        All = 0x0fffffff          // all
    };

    public abstract class LPLogger : LPError
    {
        private int m_nLogLevel = (int)ELogLevel.Off;
        protected int m_nOutputMask = (int)EOutputType.Off;
        private bool m_bUseLock = false;
        private object m_oLocker = new object();
        private string m_sPrefix = null;

        private const string LOG_LEVEL_PROMPT_DBG = "[DBG] ";
        private const string LOG_LEVEL_PROMPT_WRN = "[WRN] ";
        private const string LOG_LEVEL_PROMPT_INF = "[INF] ";
        private const string LOG_LEVEL_PROMPT_IMP = "[IMP] ";
        private const string LOG_LEVEL_PROMPT_ERR = "[ERR] ";
        private const string LOG_LEVEL_PROMPT_FTL = "[FTL] ";

        public LPLogger()
        {

        }

        ~LPLogger()
        {
            
        }

        public static LPLogger CreateLogger(ELogType eLogType, string sDir, string sFileName,
            ELogMode eLogMode, int nOutputMask, string sPrefix, int nLogLevel, bool bUseLock)
        {
            bool bResult = false;
            LPLogger lg = null;

            switch (eLogType)
            {
                case ELogType.File:
                    {
                        lg = new LPFileLogger();
                        if (PTF_ERROR(lg != null)) { goto Exit0; }

                        bResult = lg.Init(sDir, sFileName, eLogMode, nOutputMask, sPrefix, nLogLevel, bUseLock);
                        if (PTF_ERROR(bResult)) { goto Exit0; }
                    }
                    break;
                case ELogType.Invalid:
                case ELogType.Net:
                case ELogType.Total:
                default:
                    if (PTF_ERROR(false)) { goto Exit0; }
                    break;
            }

            return lg;
        Exit0:

            if(lg != null)
            {
                PTF_ERROR(lg.UnInit());
                lg = null;
            }
            return null;
        }

        public virtual bool Init(string sDir, string sFileName, 
            ELogMode eLogMode, int nOutputMask, string sPrefix, int nLogLevel, bool bUseLock)
        {
            m_bUseLock = bUseLock;
            m_sPrefix = sPrefix;
            m_nLogLevel = nLogLevel;
            m_nOutputMask = nOutputMask;

            return true;
        }
        public virtual bool UnInit()
        {
            return true;
        }

        public virtual void FTL(String format, params object[] args)
        {
            _Log(ELogLevel.Fatal, format, args);
        }

        public virtual void ERR(String format, params object[] args)
        {
            _Log(ELogLevel.Error, format, args);
        }

        public virtual void P_ERR(String format, params object[] args)
        {
            StackTrace st = new StackTrace(1, true);
            string msg = string.Format(format, args);
            _Log(ELogLevel.Error, "{0} function:{1} at line:{2} of file:{3}",
                msg, st.GetFrame(0).GetMethod(), st.GetFrame(0).GetFileLineNumber(), st.GetFrame(0).GetFileName());
        }

        public virtual void IMP(String format, params object[] args)
        {
            _Log(ELogLevel.Important, format, args);
        }

        public virtual void INF(String format, params object[] args)
        {
            _Log(ELogLevel.Info, format, args);
        }

        public virtual void WRN(String format, params object[] args)
        {
            _Log(ELogLevel.Warn, format, args);
        }

        public virtual void P_WRN(String format, params object[] args)
        {
            StackTrace st = new StackTrace(1, true);
            string msg = string.Format(format, args);
            _Log(ELogLevel.Warn, "{0} function:{1} at line:{2} of file:{3}",
                msg, st.GetFrame(0).GetMethod(), st.GetFrame(0).GetFileLineNumber(), st.GetFrame(0).GetFileName());
        }

        public virtual void DBG(String format, params object[] args)
        {
            _Log(ELogLevel.Debug, format, args);
        }

        protected virtual void LogText(byte[] logBytes, int nStartIndex, int nLen)
        {
            PTF_ERROR(false);
        }

        private string _GetLogLevelPrompt(ELogLevel eLogLevel)
        {
            string sResult = string.Empty;

            switch (eLogLevel)
            {
                case ELogLevel.Debug:
                    sResult = LOG_LEVEL_PROMPT_DBG;
                    break;
                case ELogLevel.Warn:
                    sResult = LOG_LEVEL_PROMPT_WRN;
                    break;
                case ELogLevel.Info:
                    sResult = LOG_LEVEL_PROMPT_INF;
                    break;
                case ELogLevel.Important:
                    sResult = LOG_LEVEL_PROMPT_IMP;
                    break;
                case ELogLevel.Error:
                    sResult = LOG_LEVEL_PROMPT_ERR;
                    break;
                case ELogLevel.Fatal:
                    sResult = LOG_LEVEL_PROMPT_FTL;
                    break;
                default:
                    PTF_ERROR(false);
                    break;
            }

            return sResult;
        }

        private void _Log(ELogLevel eLogLevel, String format, params object[] args)
        {
            string sErrFlag = string.Empty;
            string msg = string.Empty;
            byte[] msgBytes = null;

            if (LOG_ERROR(eLogLevel >= ELogLevel.Off)) { goto Exit0; }
            if (LOG_ERROR(eLogLevel <= ELogLevel.All)) { goto Exit0; }

            if(eLogLevel == ELogLevel.Error || eLogLevel == ELogLevel.Fatal)
            {
                sErrFlag = "*** ";
            }

            // 添加log level提示符前缀字符串还有回车换行
            if(m_sPrefix != null)
            {
                msg = string.Format("{0}{1}{2}", m_sPrefix, _GetLogLevelPrompt(eLogLevel), sErrFlag);
            }
            else
            {
                msg = string.Format("{0}{1}", _GetLogLevelPrompt(eLogLevel), sErrFlag);
            }
            if (LOG_ERROR(msg != null)) { goto Exit0; }

            msg = msg + string.Format(format, args) + "\r\n";
            if (LOG_ERROR(msg != null)) { goto Exit0; }

            if ((m_nOutputMask & (int)EOutputType.Console) > 0)
            {
                Console.Write(msg);
            }

            if ((m_nOutputMask & (int)EOutputType.Debug) > 0)
            {
                if (Debugger.IsAttached)
                {
                    Trace.Write(msg);
                }
            }

            msgBytes = Encoding.UTF8.GetBytes(msg.ToCharArray());
            if (LOG_ERROR(msgBytes != null)) { goto Exit0; }

            if (m_bUseLock)
            {
                lock (m_oLocker)
                {
                    LogText(msgBytes, 0, msgBytes.Length);
                }
            }
            else
            {
                LogText(msgBytes, 0, msgBytes.Length);
            }

        Exit0:
            return;
        }
    }
}
