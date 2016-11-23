using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    public interface ILPNetMessageHandler
    {
        void OnAccepted(LPSocker socker);

        void OnConnected(LPSocker socker);

        void OnConnectError(LPConnector connector, int errorNo);

        void OnMessage(LPSocker socker, byte[] buf, int size);

        void OnDisconnected(LPSocker socker);

        void OnConnectDisconnected(LPSocker socker, LPConnector connector);
    }
}
