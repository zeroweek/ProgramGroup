using System;
using System.Diagnostics;

namespace lzpl
{
    public class LPError
    {
        public static bool LOG_ERROR(bool bCond)
        {
            if (!bCond)
            {
                StackTrace st = new StackTrace(1, true);
                LP.Logger.ERR("LOG_ERROR function:{1} at line:{2} of file:{0}",
                    st.GetFrame(0).GetFileName(), st.GetFrame(0).GetMethod(), st.GetFrame(0).GetFileLineNumber());
                return true;
            }

            return false;
        }

        public static bool PTF_ERROR(bool bCond)
        {
            if (!bCond)
            {
                StackTrace st = new StackTrace(1, true);
                Console.WriteLine("*** PTF_ERROR function:{1} at line:{2} of file:{0}",
                    st.GetFrame(0).GetFileName(), st.GetFrame(0).GetMethod(), st.GetFrame(0).GetFileLineNumber());
                return true;
            }

            return false;
        }

        public static bool ERROR(bool bCond)
        {
            if (!bCond)
            {
                return true;
            }

            return false;
        }

        public static bool SUCCESS(bool bCond)
        {
            if (!bCond)
            {
                return false;
            }
            
            return true;
        }
    }
}
