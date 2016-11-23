using System;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace lzpl
{
    // Taken almost verbatim from http://blog.kalmbach-software.de/2008/12/13/writing-minidumps-in-c/

    public class LPMiniDump : LPError
    {
        private static int m_nDumpIndex = 0;
        private static string m_sLastGenDumpTime = string.Empty;

        private static int m_nDumpLogIndex = 0;
        private static string m_sLastGenDumpLogTime = string.Empty;
        private static byte[] m_CRLFBytes = Encoding.UTF8.GetBytes("\r\n".ToCharArray());

        public static void GenerateNormalDump(Exception e)
        {
            bool result = false;

            result = GenerateDumpFile(true);
            LOG_ERROR(result);

            result = GenerateDumpLogFile(e.Message, e.StackTrace);
            LOG_ERROR(result);
        }

        public static bool GenerateDumpLogFile(string sMessage, string sStackTrace)
        {
            bool bSuffix = false;
            string sCurTime = string.Empty;
            string sDir = string.Empty;
            string sFileName = string.Empty;
            DirectoryInfo dirInfo = null;
            IntPtr ExceptionPointers = IntPtr.Zero;

            byte[] messageBytes = null;
            byte[] stackTraceBytes = null;

            messageBytes = Encoding.UTF8.GetBytes(sMessage.ToCharArray());
            if (LOG_ERROR(messageBytes != null)) { goto Exit0; }
            stackTraceBytes = Encoding.UTF8.GetBytes(sStackTrace.ToCharArray());
            if (LOG_ERROR(stackTraceBytes != null)) { goto Exit0; }

            dirInfo = Directory.CreateDirectory(LP.DumpDir);
            if (LOG_ERROR(dirInfo != null)) { goto Exit0; }

            sCurTime = DateTime.Now.ToString("yyyy_MM_dd_HH_mm_ss");
            if (sCurTime.Equals(m_sLastGenDumpLogTime))
            {
                bSuffix = true;
            }
            else
            {
                bSuffix = false;
                m_sLastGenDumpLogTime = sCurTime;
                m_nDumpLogIndex = 0;
            }

            if (bSuffix)
            {
                sFileName = string.Format("{0}/{1}_{2}_{3}.dumplog", LP.DumpDir, LP.ProgramName, sCurTime, ++m_nDumpLogIndex);
            }
            else
            {
                sFileName = string.Format("{0}/{1}_{2}.dumplog", LP.DumpDir, LP.ProgramName, sCurTime);
            }
            if (LOG_ERROR(sFileName != null)) { goto Exit0; }

            using (FileStream fs = new FileStream(sFileName, FileMode.Create, FileAccess.ReadWrite, FileShare.Write))
            {
                if (LOG_ERROR(fs != null)) { goto Exit0; }

                fs.Write(messageBytes, 0, messageBytes.Length);
                fs.Write(m_CRLFBytes, 0, m_CRLFBytes.Length);
                fs.Write(stackTraceBytes, 0, stackTraceBytes.Length);

                LP.Logger.ERR("generate dump file: {0}", sFileName);
            }

            return true;
        Exit0:
            return false;
        }

        public static bool GenerateDumpFile(bool bExceptionPointers)
        {
            bool bResult = false;
            bool bSuffix = false;
            string sCurTime = string.Empty;
            string sDir = string.Empty;
            string sFileName = string.Empty;
            DirectoryInfo dirInfo = null;
            IntPtr ExceptionPointers = IntPtr.Zero;

            if(bExceptionPointers)
            {
                ExceptionPointers = System.Runtime.InteropServices.Marshal.GetExceptionPointers();
            }

            dirInfo = Directory.CreateDirectory(LP.DumpDir);
            if (LOG_ERROR(dirInfo != null)) { goto Exit0; }

            sCurTime = DateTime.Now.ToString("yyyy_MM_dd_HH_mm_ss");
            if (sCurTime.Equals(m_sLastGenDumpTime))
            {
                bSuffix = true;
            }
            else
            {
                bSuffix = false;
                m_sLastGenDumpTime = sCurTime;
                m_nDumpIndex = 0;
            }

            if (bSuffix)
            {
                sFileName = string.Format("{0}/{1}_{2}_{3}.dmp", LP.DumpDir, LP.ProgramName, sCurTime, ++m_nDumpIndex);
            }
            else
            {
                sFileName = string.Format("{0}/{1}_{2}.dmp", LP.DumpDir, LP.ProgramName, sCurTime);
            }
            if (LOG_ERROR(sFileName != null)) { goto Exit0; }

            using (FileStream fs = new FileStream(sFileName, FileMode.Create, FileAccess.ReadWrite, FileShare.Write))
            {
                if (LOG_ERROR(fs != null)) { goto Exit0; }

                bResult = _WriteDump(fs.SafeFileHandle, ExceptionPointers);
                LOG_ERROR(bResult);

                if (bResult)
                {
                    LP.Logger.ERR("generate dump file: {0}", sFileName);
                }
            }

            return true;
        Exit0:
            return false;
        }

        [Flags]
        public enum EDumpOption : uint
        {
            // From dbghelp.h:
            Normal = 0x00000000,
            WithDataSegs = 0x00000001,
            WithFullMemory = 0x00000002,
            WithHandleData = 0x00000004,
            FilterMemory = 0x00000008,
            ScanMemory = 0x00000010,
            WithUnloadedModules = 0x00000020,
            WithIndirectlyReferencedMemory = 0x00000040,
            FilterModulePaths = 0x00000080,
            WithProcessThreadData = 0x00000100,
            WithPrivateReadWriteMemory = 0x00000200,
            WithoutOptionalData = 0x00000400,
            WithFullMemoryInfo = 0x00000800,
            WithThreadInfo = 0x00001000,
            WithCodeSegs = 0x00002000,
            WithoutAuxiliaryState = 0x00004000,
            WithFullAuxiliaryState = 0x00008000,
            WithPrivateWriteCopyMemory = 0x00010000,
            IgnoreInaccessibleMemory = 0x00020000,
            ValidTypeFlags = 0x0003ffff,
        };

        public enum EExceptionInfo
        {
            None,
            Present
        }

        //typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
        //    DWORD ThreadId;
        //    PEXCEPTION_POINTERS ExceptionPointers;
        //    BOOL ClientPointers;
        //} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;

        [StructLayout(LayoutKind.Sequential, Pack = 4)]  // Pack=4 is important! So it works also for x64!
        public struct MINI_DUMP_EXCEPTION_INFORMATION
        {
            public uint ThreadId;
            public IntPtr ExceptionPointers;

            [MarshalAs(UnmanagedType.Bool)]
            public bool ClientPointers;
        }

        //BOOL
        //WINAPI
        //MiniDumpWriteDump(
        //    __in HANDLE hProcess,
        //    __in DWORD ProcessId,
        //    __in HANDLE hFile,
        //    __in MINIDUMP_TYPE DumpType,
        //    __in_opt PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
        //    __in_opt PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
        //    __in_opt PMINIDUMP_CALLBACK_INFORMATION CallbackParam
        //    );

        // Overload requiring MiniDumpExceptionInformation
        [DllImport("dbghelp.dll", EntryPoint = "MiniDumpWriteDump", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, ExactSpelling = true, SetLastError = true)]
        private static extern bool _MiniDumpWriteDump(IntPtr hProcess, uint processId, SafeHandle hFile, uint dumpType, ref MINI_DUMP_EXCEPTION_INFORMATION expParam, IntPtr userStreamParam, IntPtr callbackParam);

        // Overload supporting MiniDumpExceptionInformation == NULL
        [DllImport("dbghelp.dll", EntryPoint = "MiniDumpWriteDump", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode, ExactSpelling = true, SetLastError = true)]
        private static extern bool _MiniDumpWriteDump(IntPtr hProcess, uint processId, SafeHandle hFile, uint dumpType, IntPtr expParam, IntPtr userStreamParam, IntPtr callbackParam);

        [DllImport("kernel32.dll", EntryPoint = "GetCurrentThreadId", ExactSpelling = true)]
        private static extern uint _GetCurrentThreadId();

        [DllImport("kernel32.dll", EntryPoint = "GetCurrentProcess", ExactSpelling = true)]
        private static extern IntPtr _GetCurrentProcess();

        [DllImport("kernel32.dll", EntryPoint = "GetCurrentProcessId", ExactSpelling = true)]
        private static extern uint _GetCurrentProcessId();

        private static bool _WriteDump(SafeHandle fileHandle, IntPtr ExceptionPointers)
        {
            bool bResult = false;
            MINI_DUMP_EXCEPTION_INFORMATION exp;

            exp.ThreadId = _GetCurrentThreadId();
            exp.ClientPointers = false;
            exp.ExceptionPointers = ExceptionPointers;

            if (exp.ExceptionPointers == IntPtr.Zero)
            {
                bResult = _MiniDumpWriteDump(_GetCurrentProcess(), _GetCurrentProcessId(), fileHandle, (uint)(EDumpOption.Normal), IntPtr.Zero, IntPtr.Zero, IntPtr.Zero);
                if (LOG_ERROR(bResult)) { goto Exit0; }
            }
            else
            {
                bResult = _MiniDumpWriteDump(_GetCurrentProcess(), _GetCurrentProcessId(), fileHandle, (uint)(EDumpOption.Normal), ref exp, IntPtr.Zero, IntPtr.Zero);
                if (LOG_ERROR(bResult)) { goto Exit0; }
            }

            return true;
        Exit0:
            return false;
        }
    }
}
