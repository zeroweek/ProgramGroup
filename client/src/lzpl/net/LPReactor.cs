using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;

namespace lzpl
{
    public enum EHandlerType
    {
        Invalid = 0,
        Socker,
        Listener,
        Connector,
        Total
    }

    public class HandlerParam
    {
        public object Handler { get; set; }
        public EHandlerType HandlerType { get; set; }

        public HandlerParam(EHandlerType handlerType, object handler)
        {
            Handler = handler;
            HandlerType = handlerType;
        }
    }

    public class LPReactor : LPError
    {
        public bool Init()
        {
            return true;
        }

        public bool UnInit()
        {
            return true;
        }

        public  void OnAccept(IAsyncResult ar)
        {
            bool result = false;
            LPSocker socker = null;
            Socket socket = null;
            LPListener listener = null;
            HandlerParam handlerParam = null;
            IAsyncResult retAr = null;

            try
            {
                if (LOG_ERROR(ar != null)) { goto Exit0; }

                handlerParam = (HandlerParam)ar.AsyncState;
                if (LOG_ERROR(handlerParam != null)) { goto Exit0; }

                if (LOG_ERROR(handlerParam.HandlerType == EHandlerType.Listener)) { goto Exit0; }
                listener = (LPListener)handlerParam.Handler;

                LP.Logger.IMP("accept success.");

                socket = listener.Listener.EndAcceptSocket(ar);
                if (LOG_ERROR(socket != null)) { goto Exit0; }

                retAr = listener.Listener.BeginAcceptSocket(LP.NetModule.Reactor.OnAccept, handlerParam);
                if (LOG_ERROR(retAr != null)) { goto Exit0; }

                socker = LP.NetModule.CreateSocker(socket, listener.PacketParser, listener, true);
                if (LOG_ERROR(socker != null)) { goto Exit0; }

                result = LP.NetModule.EventMgr.PushEstablishEvent(socker);
                if (LOG_ERROR(result)) { goto Exit0; }
            }
            catch(SocketException e)
            {
                //if(!listener.IsStoped)
                {
                    LP.Logger.P_ERR("ErrorCode={0},SocketErrorCode={1}, Message:{2}, StackTrace:{3}",
                        e.ErrorCode, e.SocketErrorCode, e.Message, e.StackTrace);
                }
            }
            catch(Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }

        Exit0:
            return;
        }

        public void OnConnect(IAsyncResult ar)
        {
            bool result = false;
            LPSocker socker = null;
            LPConnector connector = null;
            HandlerParam handlerParam = null;

            try
            {
                if (LOG_ERROR(ar != null)) { goto Exit0; }

                handlerParam = (HandlerParam)ar.AsyncState;
                if (LOG_ERROR(handlerParam != null)) { goto Exit0; }

                if (LOG_ERROR(handlerParam.HandlerType == EHandlerType.Connector)) { goto Exit0; }
                connector = (LPConnector)handlerParam.Handler;

                LP.Logger.IMP("connect success.");

                connector.Client.EndConnect(ar);

                socker = LP.NetModule.CreateSocker(connector.Client.Client, connector.PacketParser, connector, false);
                if (LOG_ERROR(socker != null)) { goto Exit0; }

                result = LP.NetModule.EventMgr.PushEstablishEvent(socker);
                if (LOG_ERROR(result)) { goto Exit0; }

            }
            catch (SocketException e)
            {
                //if(!connector.IsStoped)
                {
                    LP.Logger.P_ERR("ErrorCode={0},SocketErrorCode={1}, Message:{2}, StackTrace:{3}",
                        e.ErrorCode, e.SocketErrorCode, e.Message, e.StackTrace);

                    LP.NetModule.EventMgr.PushConnectorErrorEvent(connector, e.ErrorCode);
                }
            }
            catch (Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }

        Exit0:
            return;
        }

        public void OnReceive(IAsyncResult ar)
        {
            bool result = false;
            int recvLen = 0;
            int useLen = 0;
            LPSocker socker = null;
            HandlerParam handlerParam = null;
            SocketError socketError = SocketError.SocketError;

            try
            {
                if (LOG_ERROR(ar != null)) { goto Exit0; }

                handlerParam = (HandlerParam)ar.AsyncState;
                if (LOG_ERROR(handlerParam != null)) { goto Exit0; }

                if (LOG_ERROR(handlerParam.HandlerType == EHandlerType.Socker)) { goto Exit0; }
                socker = (LPSocker)handlerParam.Handler;

                recvLen = socker.Sock.EndReceive(ar, out socketError);
                if(recvLen <= 0 || socketError != SocketError.Success)
                {
                    result = LP.NetModule.EventMgr.PushTerminateEvent(socker);
                    if (LOG_ERROR(result)) { goto Exit0; }
                }
                else
                {
                    socker.RecvBuf.FinishWrite(recvLen);

                    while(socker.RecvBuf.GetTotalReadableLen() > 0)
                    {
                        useLen = socker.PacketParser.Parse(socker.RecvBuf);
                        if (0 == useLen)
                        {
                            //接收区已满，但还没有装下一个包，只好断开连接
                            if (socker.RecvBuf.GetTotalWritableLen() <= 0)
                            {
                                socker.Close(ESockerCloseType.RecvError, 1, false);
                                if (LOG_ERROR(false)) { goto Exit0; }
                            }

                            break;
                        }
                        else if (useLen > 0)
                        {
                            if (useLen > socker.RecvBuf.GetTotalReadableLen())
                            {
                                socker.Close(ESockerCloseType.RecvError, 2, false);
                                if (LOG_ERROR(false)) { goto Exit0; }
                            }

                            result = LP.NetModule.EventMgr.PushRecvEvent(socker, socker.RecvBuf, useLen);
                            if (!result)
                            {
                                socker.Close(ESockerCloseType.RecvError, 3, false);
                                if (LOG_ERROR(false)) { goto Exit0; }
                            }
                        }
                        else
                        {
                            socker.Close(ESockerCloseType.RecvError, 4, false);
                            if (LOG_ERROR(false)) { goto Exit0; }
                        }
                    }

                    socker.PostRecv();
                }
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

        Exit0:
            return;
        }

        public void OnSend(IAsyncResult ar)
        {
            bool result = false;
            int sendLen = 0;
            LPSocker socker = null;
            SocketError socketError = SocketError.SocketError;
            HandlerParam handlerParam = null;

            try
            {
                if (LOG_ERROR(ar != null)) { goto Exit0; }

                handlerParam = (HandlerParam)ar.AsyncState;
                if (LOG_ERROR(handlerParam != null)) { goto Exit0; }

                if (LOG_ERROR(handlerParam.HandlerType == EHandlerType.Socker)) { goto Exit0; }
                socker = (LPSocker)handlerParam.Handler;

                sendLen = socker.Sock.EndSend(ar, out socketError);
                if(sendLen <= 0 || socketError != SocketError.Success)
                {
                    result = LP.NetModule.EventMgr.PushTerminateEvent(socker);
                    if (LOG_ERROR(result)) { goto Exit0; }
                }
                else
                {
                    socker.SendBuf.FinishRead(sendLen);
                    socker.IsSending = false;
                }
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

        Exit0:
            return;
        }
    }
}
