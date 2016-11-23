using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace lzpl
{
    public class LPConnector : LPError
    {
        public int ID { get; set; }
        public bool IsStoped { get; set; }
        public TcpClient Client { get; set; }
        public ILPPacketParser PacketParser { get; set; }

        internal LPConnector()
        {
            IsStoped = false;
        }

        public bool Start(string ip, int port)
        {
            HandlerParam handlerParam = null;
            IAsyncResult ar = null;

            try
            {
                handlerParam = new HandlerParam(EHandlerType.Connector, this);
                if (LOG_ERROR(handlerParam != null)) { goto Exit0; }
                ar = Client.BeginConnect(ip, port, LP.NetModule.Reactor.OnConnect, handlerParam);
                if (LOG_ERROR(ar != null)) { goto Exit0; }
            }
            catch (SocketException e)
            {
                LP.Logger.P_ERR("ErrorCode={0},SocketErrorCode={1}, Message:{2}, StackTrace:{3}",
                    e.ErrorCode, e.SocketErrorCode, e.Message, e.StackTrace);

                if (LOG_ERROR(false)) { goto Exit0; }
            }
            catch (Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }

            return true;
        Exit0:
            return false;
        }

        public void Stop()
        {
            try
            {
                IsStoped = true;
                Client.Close();
            }
            catch (SocketException e)
            {
                LP.Logger.P_ERR("ErrorCode={0},SocketErrorCode={1}, Message:{2}, StackTrace:{3}",
                    e.ErrorCode, e.SocketErrorCode, e.Message, e.StackTrace);
            }
            catch (Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }
            IsStoped = true;
        }
    }
}
