using System;
using System.Text;
using System.Threading;
using System.IO;


namespace lzpl
{
    internal class LPFileLogger : LPLogger
    {
        private static int CNT_LOG_BUF_SIZE = 10 * 1024 * 1024;
        private static int CNT_LOG_SINGLE_FILE_SIZE = 10 * 1024 * 1024;


        private bool m_bRun = false;                       // 线程运行标记
        private bool m_bInit = false;                      // 是否初始化标记
        private bool m_bErrorLog = false;                  // 日志错误
        private Thread m_oThread = null;
        private ELogMode m_eLogMode = ELogMode.Invalid;      // 日志记录模式
        private string m_sDir = string.Empty;              // 目录路径
        private string m_sFileName = string.Empty;         // 日志文件名
        private string m_sFilePath = string.Empty;         // 文件路径
        private FileStream m_oFile = null;                 // 打开的文件句柄
        private bool m_bDirty = false;                     // 是否有数据还没fflush到文件
        private LPLoopBuf m_oLoopBuf = null;               // 日志缓冲区
        private int m_nSubFile = 0;                        // 子文件编号（超过单个文件大小则需要分子文件记录）
        private DateTime m_tLastTime;                      // 上次时间
        private DateTime m_tCurTime;                       // 当前时间
        private int m_nMsgId = 0;                          // 消息id编号


        internal LPFileLogger()
        {

        }

        ~LPFileLogger()
        {
            UnInit();
        }

        public override bool Init(string sDir, string sFileName,
            ELogMode eLogMode, int nOutputMask, string sPrefix, int nLogLevel, bool bUseLock)
        {
            bool bResult = false;
            DirectoryInfo dirInfo = null;

            bResult = base.Init(sDir, sFileName, eLogMode, nOutputMask, sPrefix, nLogLevel, bUseLock);
            if (PTF_ERROR(bResult)) { goto Exit0; }

            if (PTF_ERROR(m_bInit == false)) { goto Exit0; }

            if (PTF_ERROR(m_oLoopBuf == null)) { goto Exit0; }
            m_oLoopBuf = new LPLoopBuf();
            if (PTF_ERROR(m_oLoopBuf != null)) { goto Exit0; }

            bResult = m_oLoopBuf.Init(CNT_LOG_BUF_SIZE);
            if (PTF_ERROR(bResult)) { goto Exit0; }

            m_eLogMode = eLogMode;
            if (PTF_ERROR(m_eLogMode > ELogMode.Invalid)) { goto Exit0; }
            if (PTF_ERROR(m_eLogMode < ELogMode.Total)) { goto Exit0; }

            m_sDir = sDir;
            m_sFileName = sFileName;
            if (PTF_ERROR(m_eLogMode > ELogMode.Invalid)) { goto Exit0; }
            dirInfo = Directory.CreateDirectory(m_sDir);
            if (PTF_ERROR(dirInfo != null)) { goto Exit0; }

            _UpdateFilePointer(); 
            if (PTF_ERROR(m_oFile != null)) { goto Exit0; }

            m_bRun = true;
            m_oThread = new Thread(_ThreadProc);
            m_oThread.Start(this);

            m_bInit = true;

            return true;
        Exit0:
            return false;
        }

        public override bool UnInit()
        {
            PTF_ERROR(m_bInit);

            if(m_bRun)
            {
                m_bRun = false;
                m_oThread.Join();
            }

            m_oThread = null;
            _CloseFile();
            m_oLoopBuf = null;

            return true;
        }

        protected override void LogText(byte[] logBytes, int nStartIndex, int nLen)
        {
            bool bResult = false;
            int nSleepCount = 0;

            int nTotalLen = 0;
            byte[] totalLenBytes = null;

            string sMsgPrefix = null;
            byte[] msgPrefixBytes = null;

            if (PTF_ERROR(logBytes != null)) { goto Exit0; }
            if (PTF_ERROR(logBytes.Length >= nLen + nStartIndex)) { goto Exit0; }
            if (PTF_ERROR(m_oLoopBuf != null)) { goto Exit0; }

            //添加时间和id编号前缀
            sMsgPrefix = string.Format("{0}[{1:00000000}]", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"), ++m_nMsgId);
            if (PTF_ERROR(sMsgPrefix != null)) { goto Exit0; }
            msgPrefixBytes = Encoding.UTF8.GetBytes(sMsgPrefix.ToCharArray());
            if (PTF_ERROR(msgPrefixBytes != null)) { goto Exit0; }

            nTotalLen = msgPrefixBytes.Length + nLen;

            if (PTF_ERROR(m_bErrorLog == false)) { goto Exit0; }

            if (m_oLoopBuf.GetTotalWritableLen() < nTotalLen + sizeof(int))
            {
                PTF_ERROR(false);
                Thread.Sleep(1);
                while (m_oLoopBuf.GetTotalWritableLen() < nTotalLen + sizeof(int))
                {
                    Thread.Sleep(1);
                    nSleepCount++;
                    if (nSleepCount > 3000)
                    {
                        if (PTF_ERROR(false)) { goto Exit0; }
                    }
                }
            }

            totalLenBytes = BitConverter.GetBytes(nTotalLen);
            if (PTF_ERROR(totalLenBytes != null)) { goto Exit0; }
            if (PTF_ERROR(totalLenBytes.Length == sizeof(int))) { goto Exit0; }

            bResult = m_oLoopBuf.Write(totalLenBytes, 0, totalLenBytes.Length);
            if (PTF_ERROR(bResult)) { goto Exit0; }

            bResult = m_oLoopBuf.Write(msgPrefixBytes, 0, msgPrefixBytes.Length);
            if (PTF_ERROR(bResult)) { goto Exit0; }

            bResult = m_oLoopBuf.Write(logBytes, 0, nLen);
            if (PTF_ERROR(bResult)) { goto Exit0; }

            return;

        Exit0:

            m_bErrorLog = true;
            Console.WriteLine("Log fail: {0}", logBytes);

            return;
        }

        private void _Flush()
        {
            if (m_oFile != null && m_bDirty)
            {
                m_oFile.Flush();
                m_bDirty = false;
            }
        }

        private void _CloseFile()
        {
            _Flush();
            m_oFile = null;
        }

        private void _UpdateFilePointer()
        {
            //第一次创建日志文件
            if (m_oFile == null)
            {
                m_tLastTime = DateTime.Now;
                m_oFile = new FileStream(_GetFilePath(m_tLastTime), FileMode.Append);
                if (PTF_ERROR(m_oFile != null)) { goto Exit0; }
                if (SUCCESS(true)) { goto Exit1; }

            }

            m_tCurTime = DateTime.Now;

            //检查是否需要创建新文件
            switch (m_eLogMode)
            {
                case ELogMode.SingleFile:
                    break;
                case ELogMode.DayDivide:
                    {
                        if (m_tCurTime.Year != m_tLastTime.Year
                            || m_tCurTime.Month != m_tLastTime.Month
                            || m_tCurTime.Day != m_tLastTime.Day)
                        {
                            m_nSubFile = 0;

                            _CloseFile();
                            m_oFile = new FileStream(_GetFilePath(m_tCurTime), FileMode.Append);
                            if (PTF_ERROR(m_oFile != null)) { goto Exit0; }
                        }
                    }
                    break;
                case ELogMode.MonthDivide:
                    {
                        if (m_tCurTime.Year != m_tLastTime.Year
                            || m_tCurTime.Month != m_tLastTime.Month)
                        {
                            m_nSubFile = 0;

                            _CloseFile();
                            m_oFile = new FileStream(_GetFilePath(m_tCurTime), FileMode.Append);
                            if (PTF_ERROR(m_oFile != null)) { goto Exit0; }
                        }
                    }
                    break;
                default:
                    if (PTF_ERROR(false)) { goto Exit0; }
                    break;
            }

            //如果日志文件超过单个文件大小这创建子文件
            if (PTF_ERROR(m_oFile != null)) { goto Exit0; }
            if(m_oFile.Length > CNT_LOG_SINGLE_FILE_SIZE)
            {
                ++m_nSubFile;
                _CloseFile();
                m_oFile = new FileStream(_GetFilePath(m_tCurTime), FileMode.Append);
                if (PTF_ERROR(m_oFile != null)) { goto Exit0; }
            }

            m_tLastTime = m_tCurTime;

        Exit1:
        Exit0:
            return;
        }

        // Summary:
        //		获取文件路径
        private string _GetFilePath(DateTime t)
        {
            m_sFilePath = string.Empty;

            switch (m_eLogMode)
            {
                case ELogMode.SingleFile:
                    {
                        if (m_nSubFile == 0)
                        {
                            m_sFilePath = string.Format("{0}/{1}_{2:00}_{3}.log", m_sDir, m_sFileName, m_nSubFile, DateTime.Now.ToString("yyyy_MM_dd_HH_mm_ss"));
                        }
                        else
                        {
                            m_sFilePath = string.Format("{0}/{1}_{2}.log", m_sDir, m_sFileName, DateTime.Now.ToString("yyyy_MM_dd_HH_mm_ss"));
                        }
                    }
                    break;
                case ELogMode.DayDivide:
                    {
                        if (m_nSubFile == 0)
                        {
                            m_sFilePath = string.Format("{0}/{1}_{2}_{3:00}.log", 
                                m_sDir, m_sFileName, DateTime.Now.ToString("yyyy_MM_dd_HH_mm_ss"), m_nSubFile);
                        }
                        else
                        {
                            m_sFilePath = string.Format("{0}/{1}_{2}.log",
                                m_sDir, m_sFileName, DateTime.Now.ToString("yyyy_MM_dd_HH_mm_ss"));
                        }
                    }
                    break;
                case ELogMode.MonthDivide:
                    {
                        if (m_nSubFile == 0)
                        {
                            m_sFilePath = string.Format("{0}/{1}_{2:0000}_{3:00}_{4:00}.log",
                                m_sDir, m_sFileName, DateTime.Now.Year, DateTime.Now.Month, m_nSubFile);
                        }
                        else
                        {
                            m_sFilePath = string.Format("{0}/{1}_{2:0000}_{3:00}.log",
                                m_sDir, m_sFileName, DateTime.Now.Year, DateTime.Now.Month);
                        }
                    }
                    break;
                default:
                    if (PTF_ERROR(false)) { goto Exit0; }
                    break;
            }

        Exit0:
            return m_sFilePath;
        }

        private static void _ThreadProc(Object param)
        {
            bool bResult = false;
            int nSleepMicSec = 10;                   // 每次睡眠毫秒数
            int nFlushIntervalSec = 3000;            // flush间隔（毫秒）
            int nLastFlushTimeMicSecInc = 0;         // 上次flush后的时间增量（毫秒）
            int nStopRunWaitCount = 0;

            int nOnceReadableLen = 0;
            LPFileLogger oFileLogger = null;
            LPLoopBuf oLoopBuf = null;

            int nLen = 0;
            byte[] lenBytes = null;

            oFileLogger = (LPFileLogger)param;
            if (PTF_ERROR(oFileLogger != null)) { goto Exit0; }
            oLoopBuf = oFileLogger.m_oLoopBuf;
            if (PTF_ERROR(oLoopBuf != null)) { goto Exit0; }

            lenBytes = new byte[sizeof(int)];
            if (PTF_ERROR(lenBytes != null)) { goto Exit0; }

            while (oFileLogger.m_bRun || oLoopBuf.GetOnceReadableLen() > 0)
            {
                //防止LPLoopBuf出错造成不退出（等待500 * 10毫米）
                if(!oFileLogger.m_bRun)
                {
                    ++nStopRunWaitCount;
                    if(nStopRunWaitCount > 500)
                    {
                        PTF_ERROR(false);
                        break;
                    }
                }

                while (oLoopBuf.GetTotalReadableLen() > sizeof(int))
                {
                    bResult = oLoopBuf.Read(lenBytes, 0, sizeof(int), false);
                    PTF_ERROR(bResult);

                    nLen = BitConverter.ToInt32(lenBytes, 0);
                    if (oLoopBuf.GetTotalReadableLen() < nLen)
                    {
                        break;
                    }

                    oLoopBuf.FinishRead(sizeof(int));
                    oFileLogger._UpdateFilePointer();

                    nOnceReadableLen = oLoopBuf.GetOnceReadableLen();

                    if ((oFileLogger.m_nOutputMask & (int)EOutputType.File) > 0)
                    {
                        if (nOnceReadableLen < nLen)
                        {
                            oFileLogger.m_oFile.Write(oLoopBuf.BufBytes,
                                oLoopBuf.ReadInx, nOnceReadableLen);
                            oFileLogger.m_oFile.Write(oLoopBuf.BufBytes,
                                0, nLen - nOnceReadableLen);
                        }
                        else
                        {
                            oFileLogger.m_oFile.Write(oLoopBuf.BufBytes,
                                oLoopBuf.ReadInx, nLen);
                        }
                    }

                    oLoopBuf.FinishRead(nLen);

                    oFileLogger.m_bDirty = true;
                }

                if (oFileLogger.m_bErrorLog)
                {
                    oLoopBuf.FinishRead(oLoopBuf.GetTotalReadableLen());
                    oFileLogger.m_bErrorLog = false;
                }

                nLastFlushTimeMicSecInc += nSleepMicSec;
                if (nLastFlushTimeMicSecInc > nFlushIntervalSec)
                {
                    nLastFlushTimeMicSecInc = 0;
                    oFileLogger._Flush();
                }

                if (oLoopBuf.GetTotalReadableLen() <= sizeof(int))
                {
                    Thread.Sleep(nSleepMicSec);
                }
            }

        Exit0:

            oFileLogger._Flush();

            return;
        }
    }
}
