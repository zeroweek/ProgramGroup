using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    // 编码规范：
    //   1. 变量前缀：
    //     * 函数参数不加前缀，第一个单词小写（驼峰命名法）
    //     * 类私有成员变量加前缀"m_"，且第一个字母大写
    //     * 类共有成员变量第一个字母大写
    //   1. 变量后缀：
    //     * 数组参数名后面加后缀"s"，除非参数名自带数组含义（如：array）
    //   2. 函数命名
    //     * 类私有成员函数加前缀"_"，且第一个字母大写
    //     * 类共有成员函数第一个字母大写

    public enum EPlatform
    {
        Pc = 0,
        Ios,
        Android,
    }

    public static class LP
    {
        public static string LogDir { get { return m_sLogDir; } }
        public static string DumpDir { get { return m_sDumpDir; } }
        public static string ConfigDir { get { return m_sConfigDir; } }
        public static string ProgramName { get { return m_sProgramName; } }
        public static LPLogger Logger { get { return m_Logger; } }
        public static LPNet NetModule { get { return m_NetModule; } }

        public static int RecvBufSize { get { return m_RecvBufSize; } }
        public static int SendBufSize { get { return m_SendBufSize; } }
        public static int NetEventBufSize { get { return m_NetEventBufSize; } }
        public static int NetEventListCount { get { return m_NetEventListCount; } }
        public static int MaxPacketLen { get { return m_MaxPacketLen; } }

        private static string m_sLogDir = string.Empty;
        private static string m_sDumpDir = string.Empty;
        private static string m_sConfigDir = string.Empty;
        private static string m_sProgramName = string.Empty;
        private static LPLogger m_Logger = null;
        private static LPNet m_NetModule = null;

        private static int m_RecvBufSize = 128 * 1024;
        private static int m_SendBufSize = 128 * 1024;
        private static int m_NetEventBufSize = 10 * 1024 * 1024;
        private static int m_NetEventListCount = 3;
        private static int m_MaxPacketLen = 65535;


        public static bool Init(EPlatform ePlatform, bool isUseNetModule, ILPNetMessageHandler netMessageHandler)
        {
            bool bResult = false;

            bResult = _InitConfig(ePlatform);
            if (LPError.PTF_ERROR(bResult)) { goto Exit0; }

            bResult = _InitLogger();
            if (LPError.PTF_ERROR(bResult)) { goto Exit0; }

            if(isUseNetModule)
            {
                bResult = _InitNet(netMessageHandler);
                if (LPError.LOG_ERROR(bResult)) { goto Exit0; }
            }

            return true;
        Exit0:
            return false;
        }

        public static bool UnInit()
        {
            if(m_Logger != null)
            {
                LPError.PTF_ERROR(m_Logger.UnInit());
                m_Logger = null;
            }

            if(m_NetModule != null)
            {
                LPError.PTF_ERROR(m_NetModule.UnInit());
                m_NetModule = null;
            }

            return true;
        }

        private static bool _InitConfig(EPlatform ePlatform)
        {
            switch(ePlatform)
            {
                case EPlatform.Pc:
                    {
                        m_sLogDir = "log_pc";
                        m_sDumpDir = m_sLogDir + "/dump_pc";
                        m_sConfigDir = "config_pc";
                        m_sProgramName = LPG.GetExeProcessName() + "_pc";
                    }
                    break;
                case EPlatform.Ios:
                    {
                        m_sLogDir = "log_ios";
                        m_sDumpDir = m_sLogDir + "/dump_ios";
                        m_sConfigDir = "config_ios";
                        m_sProgramName = LPG.GetExeProcessName() + "_ios";
                    }
                    break;
                case EPlatform.Android:
                    {
                        m_sLogDir = "log_android";
                        m_sDumpDir = m_sLogDir + "/dump_android";
                        m_sConfigDir = "config_android";
                        m_sProgramName = LPG.GetExeProcessName() + "_android";
                    }
                    break;
                default:
                    if (LPError.PTF_ERROR(false)) { goto Exit0; }
                    break;
            }

            return true;
        Exit0:
            return false;
        }

        private static bool _InitLogger()
        {
            #if DEBUG
                m_Logger = LPLogger.CreateLogger(ELogType.File, m_sLogDir, m_sProgramName, ELogMode.DayDivide,
                    (int)(EOutputType.File | EOutputType.Debug | EOutputType.Console), null, (int)(ELogLevel.All), false);
            #else
                m_Logger = LPLogger.CreateLogger(ELogType.File, m_sLogDir, m_sProgramName, ELogMode.DayDivide,
                    (int)(EOutputType.File | EOutputType.Console), null, (int)(ELogLevel.All), false);
            #endif

            if (LPError.PTF_ERROR(m_Logger != null)) { goto Exit0; }

            #if DEBUG
                LP.Logger.IMP("{0}", "debug mode...");
            #else
                LP.Logger.IMP("{0}", "release mode...");
            #endif

            return true;
        Exit0:
            return false;
        }

        private static bool _InitNet(ILPNetMessageHandler netMessageHandler)
        {
            bool bResult = false;

            m_NetModule = new LPNet();
            if (LPError.LOG_ERROR(m_NetModule != null)) { goto Exit0; }

            bResult = m_NetModule.Init(netMessageHandler);
            if (LPError.LOG_ERROR(bResult)) { goto Exit0; }

            return true;
        Exit0:
            return false;
        }
    }
}
