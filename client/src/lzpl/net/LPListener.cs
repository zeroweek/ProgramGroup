using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace lzpl
{
    public class LPListener : LPError
    {
        public int ID { get; set; }
        public bool IsStoped { get; set; }
        public TcpListener Listener { get; set; }
        public ILPPacketParser PacketParser { get; set; }


        internal LPListener()
        {
            IsStoped = false;
        }

        public bool Start()
        {
            HandlerParam handlerParam = null;
            IAsyncResult ar = null;

            try
            {
                Listener.Start();

                handlerParam = new HandlerParam(EHandlerType.Listener, this);
                if (LOG_ERROR(handlerParam != null)) { goto Exit0; }
                ar = Listener.BeginAcceptSocket(LP.NetModule.Reactor.OnAccept, handlerParam);
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
                Listener.Stop();
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
        }
    }
}
