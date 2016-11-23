using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using lzpl;

namespace TestClient
{
    class Client : LPError
    {
        public static bool IsClosed = false;

        public static bool Test_ClientMessageHandler()
        {
            bool result = false;
            LPConnector connector = null;
            ClientMessageHandler clientMessageHandler = null;

            clientMessageHandler = new ClientMessageHandler();
            if (LOG_ERROR(clientMessageHandler != null)) { goto Exit0; }

            result = LP.Init(EPlatform.Pc, true, clientMessageHandler);
            if (ERROR(result)) { goto Exit0; }

            connector = LP.NetModule.CreateConnector(clientMessageHandler);
            if (LOG_ERROR(connector != null)) { goto Exit0; }

            result = connector.Start("127.0.0.1", 7777);
            if (LOG_ERROR(result)) { goto Exit0; }

        Exit0:
            return true;
        }

        public static bool Test_GTMessageHandler()
        {
            bool result = false;
            LPConnector connector = null;
            LPMessageHandler gtMessageHandler = null;
            LPPacketParser gtPacketParser = null;

            gtMessageHandler = new LPMessageHandler();
            if (LOG_ERROR(gtMessageHandler != null)) { goto Exit0; }

            gtPacketParser = new LPPacketParser();
            if (LOG_ERROR(gtPacketParser != null)) { goto Exit0; }

            result = gtMessageHandler.Init();
            if (ERROR(result)) { goto Exit0; }

            result = LP.Init(EPlatform.Pc, true, gtMessageHandler);
            if (ERROR(result)) { goto Exit0; }

            connector = LP.NetModule.CreateConnector(gtPacketParser);
            if (LOG_ERROR(connector != null)) { goto Exit0; }

            result = connector.Start("127.0.0.1", 6002);
            if (LOG_ERROR(result)) { goto Exit0; }

        Exit0:
            return true;
        }

        static bool Init()
        {
            bool result = false;

            result = Test_GTMessageHandler();
            if (ERROR(result)) { goto Exit0; }

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

        static void Main(string[] args)
        {
            bool result = false;
            bool isSpare = false;

            try
            {
                LPG.RedefinedConsole();

                result = Client.Init();
                if (ERROR(result)) { goto Exit0; }

                LP.Logger.IMP("program start...");

                //test case
                //result = Test_Tcp(clientMessageHandler);
                //if (ERROR(result)) { goto Exit0; }

                while (!Client.IsClosed)
                {
                    isSpare = Client.MainLoop();
                    if (isSpare)
                    {
                        Thread.Sleep(1);
                    }
                }

                //Client.Close();
            }
            catch (Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }

            LP.Logger.IMP("program exit...");

        Exit0:

            result = Client.UnInit();
            PTF_ERROR(result);

            Console.WriteLine("\nenter any key to quit");

            return;
        }
    }
}
