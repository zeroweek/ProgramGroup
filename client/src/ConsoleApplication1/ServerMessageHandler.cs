using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using lzpl;

namespace ConsoleApplication1
{
    public class ServerMessageHandler : LPError, ILPNetMessageHandler, ILPPacketParser
    {
        public ServerMessageHandler()
        {

        }
        public int Parse(LPLoopBuf loopBuf)
        {
            int ret = -1;

            if (LOG_ERROR(loopBuf != null)) { goto Exit0; }

            if(loopBuf.GetTotalReadableLen() >= 10)
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
        }

        public void OnConnected(LPSocker socker)
        {
            LP.Logger.IMP("OnConnected output : socker id is {0}", socker.ID);
        }

        public void OnConnectError(LPConnector connector, int errorNo)
        {
            LP.Logger.IMP("OnConnectError output : connector id is {0}, error no is {1}", connector.ID, errorNo);
        }

        public void OnMessage(LPSocker socker, byte[] buf, int size)
        {
            string msg = Encoding.UTF8.GetString(buf, 0, size);
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
