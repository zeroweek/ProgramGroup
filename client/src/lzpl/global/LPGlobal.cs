using System;
using System.Diagnostics;
using System.Threading;
using System.Runtime.InteropServices;


namespace lzpl
{
    public class LPG
    {
        public static void GenException(int nType)
        {
            if(nType == 0)
            {
                int i = 0;
                int j = 10 / i;
            }
            else if(nType == 1)
            {
                LPLoopBuf lb = null;
                lb.FinishRead(3);
            }
        }

        public static string GetExeProcessName()
        {
            return System.Diagnostics.Process.GetCurrentProcess().ProcessName;
        }

        public static string GetExeFileName()
        {
            return System.Diagnostics.Process.GetCurrentProcess().MainModule.ModuleName;
        }

        public static string GetExePath()
        {
            return System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
        }

        public static string GetExeDir()
        {
            Process p = Process.GetCurrentProcess();
            return System.AppDomain.CurrentDomain.BaseDirectory;
        }

        public static string GetWorkingDir()
        {
            return System.Environment.CurrentDirectory;
            //return System.IO.Directory.GetCurrentDirectory();
        }

        [DllImport("Kernel32.dll", EntryPoint = "GetConsoleWindow")]
        private static extern IntPtr _GetConsoleWindow();

        [DllImport("User32.dll", EntryPoint = "GetSystemMenu")]
        private static extern IntPtr _GetSystemMenu(IntPtr hWnd, int bRevert);

        [DllImport("User32.dll", EntryPoint = "EnableMenuItem")]
        private static extern int _EnableMenuItem(IntPtr hMenu, uint uIDEnableItem, uint uEnable);

        public static void RedefinedConsole()
        {
            uint SC_CLOSE = 0xF060;
            uint MF_GRAYED = 0x00000001;
            IntPtr hMenu = _GetSystemMenu(_GetConsoleWindow(), 0);
            _EnableMenuItem(hMenu, SC_CLOSE, MF_GRAYED);
        }
    }
}
