using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using lzpl;

namespace TestClient
{
    class ClientMessageHandler : LPError, ILPNetMessageHandler, ILPPacketParser
    {
        public int Parse(LPLoopBuf loopBuf)
        {
            int ret = -1;

            if (LOG_ERROR(loopBuf != null)) { goto Exit0; }

            if (loopBuf.GetTotalReadableLen() >= 10)
            {
                ret = 10;
            }
            else
            {
                ret = 0;
            }

            Exit0:
            return ret;
        }

        public void OnAccepted(LPSocker socker)
        {
            LP.Logger.IMP("OnAccepted output : socker id is {0}", socker.ID);

            socker.Send("0123456789012345678901234");
            Thread.Sleep(500);
            socker.Send("56789");
        }

        public void OnConnected(LPSocker socker)
        {
            LP.Logger.IMP("OnConnected output : socker id is {0}", socker.ID);

            socker.Send("0123456789012345678901234");
            Thread.Sleep(500);
            socker.Send("56789");
        }

        public void OnConnectError(LPConnector connector, int errorNo)
        {
            LP.Logger.IMP("OnConnectError output : connector id is {0}, error no is {1}", connector.ID, errorNo);
        }

        public void OnMessage(LPSocker socker, byte[] buf, int size)
        {
            string msg = Encoding.UTF8.GetChars(buf, 0, size).ToString();
            LP.Logger.IMP("OnMessage output : socker id is {0}, message({1})", socker.ID, msg);
        }

        public void OnDisconnected(LPSocker socker)
        {
            LP.Logger.IMP("OnDisconnected output : socker id is {0}", socker.ID);
        }

        public void OnConnectDisconnected(LPSocker socker, LPConnector connector)
        {
            LP.Logger.IMP("OnConnectDisconnected output : socker id is {0}, connector id is {1}", socker.ID, connector.ID);
        }
    }
}
