using System;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Sockets;
using lzpl;
//using LuaInterface;
using System.Collections.Generic;


namespace ConsoleApplication1
{
    class TestNode : BASE_LIST_NODE
    {
        public int Num { get; set; }
    }

    class LuaTestClass
    {
        public string Output(string s)
        {
            return s + " World !";
        }
    }

    class Program : LPError
    {
        public static bool IsClosed = false;

        static bool Init()
        {
            bool result = false;
            LPListener listener = null;
            ServerMessageHandler serverMessageHandler = null;

            serverMessageHandler = new ServerMessageHandler();
            if (LOG_ERROR(serverMessageHandler != null)) { goto Exit0; }

            result = LP.Init(EPlatform.Pc, true, serverMessageHandler);
            if (ERROR(result)) { goto Exit0; }

            listener = LP.NetModule.CreateListener(IPAddress.Parse("127.0.0.1"), 7777, serverMessageHandler);
            if (LOG_ERROR(listener != null)) { goto Exit0; }

            result = listener.Start();
            if (LOG_ERROR(result)) { goto Exit0; }
            
            return true;
        Exit0:
            return false;
        }

        static bool UnInit()
        {
            bool result = false;

            result = LP.UnInit();
            PTF_ERROR(result);

            return true;
        }

        static bool MainLoop()
        {
            bool isRunOut = false;

            isRunOut = LP.NetModule.Run();

            return isRunOut;
        }

        static void Close()
        {

        }


        //private static Lua m_Lua = null;

        static bool Test_LuaInterface()
        {
            //m_Lua = new Lua();
            //LuaTestClass luaTestClass = new LuaTestClass();
            //m_Lua.RegisterFunction("LuaTestClass", luaTestClass, luaTestClass.GetType().GetMethod("Output"));

            //m_Lua.DoFile("test.lua");

            ////加载乱文件后，使用GetFunction获取函数，再调用Call执行（传参数）  
            //Object[] objs = m_Lua.GetFunction("MyNum").Call(100);
            ////Call函数的返回值为一个Object数组  
            //foreach (Object obj in objs)
            //{
            //    Console.WriteLine(obj);
            //}

            return true;
        }

        static void Main(string[] args)
        {
            bool result = false;
            bool isSpare = false;

            try
            {
                LPG.RedefinedConsole();

                result = Program.Init();
                if (ERROR(result)) { goto Exit0; }

                LP.Logger.IMP("program start...");

                //test case
                result = Test_LuaInterface();
                if (ERROR(result)) { goto Exit0; }

                while (!Program.IsClosed)
                {
                    isSpare = Program.MainLoop();
                    if(isSpare)
                    {
                        Thread.Sleep(1);
                    }
                }

                //Program.Close();
            }
            catch(Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }

            LP.Logger.IMP("program exit...");

        Exit0:

            result = Program.UnInit();
            PTF_ERROR(result);

            Console.WriteLine("\nenter any key to quit");

            return;
        }
    }
}
