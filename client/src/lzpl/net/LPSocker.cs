using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace lzpl
{
    public enum ESockerCloseType
    {
        Invalid = -1,
        AcceptFail,            // 接受失败
        ConnectFail,           // 连接失败
        SendBufOverflow,       // 发送缓冲区溢出
        PostRecvFail,          // post异步接收操作失败
        PostSendFail,          // post异步发送失败
        ReactorErrorEvent,     // 反应器错误事件
        RecvError,             // 接收数据错误
        Total
    }

    public class LPSocker : LPError
    {
        public int ID { get; set; }
        public object Parent { get; set; }
        public bool IsAcceptCreate { get; set; }
        public bool IsClosed { get; set; }
        public bool IsPassiveClosed { get; set; }
        public bool IsSending { get; set; }
        public Socket Sock { get; set; }
        internal LPLoopBuf RecvBuf { get; set; }
        internal LPLoopBuf SendBuf { get; set; }
        internal ILPPacketParser PacketParser { get; set; }
        internal long DelayCloseBeginTick { get; set; }
        internal long DelayCloseDuration { get; set; }
        internal long DelayReleaseBeginTick { get; set; }
        internal long DelayReleaseDuration { get; set; }

        public void Close(ESockerCloseType eType, int param, bool isPassiveClose)
        {
            bool result = false;

            if (SUCCESS(IsClosed)) { goto Exit1; }
            IsClosed = true;
            IsPassiveClosed = isPassiveClose;

            Sock.Shutdown(SocketShutdown.Send);

            result = LP.NetModule.EventMgr.PushTerminateEvent(this);
            if (LOG_ERROR(result)) { goto Exit0; }

        Exit1:
        Exit0:
            return;
        }

        public bool Send(string msg)
        {
            bool result = false;
            bool overflow = false;
            byte[] msgBytes = null;

            //检测是否sock是否连接中
            if (SUCCESS(IsClosed)) { goto Exit1; }

            msgBytes = Encoding.UTF8.GetBytes(msg.ToCharArray());
            if (LOG_ERROR(msgBytes != null)) { goto Exit0; }

            if (SendBuf.GetTotalWritableLen() < msgBytes.Length)
            {
                overflow = true;
            }
            else
            {
                result = SendBuf.Write(msgBytes, 0, msgBytes.Length);
                if (LOG_ERROR(result)) { goto Exit0; }
            }

            if (overflow)
            {
                Close(ESockerCloseType.SendBufOverflow, 1, false);
                if (LOG_ERROR(false)) { goto Exit0; }
            }

        Exit1:
            return true;
        Exit0:
            return false;
        }

        public bool Send(byte[] data, int startIndex, int length)
        {
            bool result = false;
            bool overflow = false;

            //检测是否sock是否连接中
            if (SUCCESS(IsClosed)) { goto Exit1; }

            if (SendBuf.GetTotalWritableLen() < length)
            {
                overflow = true;
            }
            else
            {
                result = SendBuf.Write(data, startIndex, length);
                if (LOG_ERROR(result)) { goto Exit0; }
            }

            if (overflow)
            {
                Close(ESockerCloseType.SendBufOverflow, 1, false);
                if (LOG_ERROR(false)) { goto Exit0; }
            }

        Exit1:
            return true;
        Exit0:
            return false;
        }

        internal void OnClose()
        {
            LP.NetModule.DelayClose(this);
        }

        internal LPSocker()
        {

        }

        internal void PostRecv()
        {
            IAsyncResult ar = null;
            SocketError socketError = SocketError.SocketError;
            HandlerParam handlerParam = null;

            try
            {
                handlerParam = new HandlerParam(EHandlerType.Socker, this);
                if (LOG_ERROR(handlerParam != null)) { goto Exit0; }

                ar = Sock.BeginReceive(RecvBuf.BufBytes, RecvBuf.WriteInx, RecvBuf.GetOnceWritableLen(), SocketFlags.None, out socketError,
                    LP.NetModule.Reactor.OnReceive, handlerParam);
                if(socketError != SocketError.Success)
                {
                    Close(ESockerCloseType.PostRecvFail, 1, true);
                    goto Exit0;
                }
                if (LOG_ERROR(ar != null)) { goto Exit0; }
            }
            catch(Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }

        Exit0:
            return;
        }

        internal bool PostSend()
        {
            bool haveDataSend = false;
            IAsyncResult ar = null;
            SocketError socketError = SocketError.SocketError;
            HandlerParam handlerParam = null;

            try
            {
                if (SUCCESS(IsClosed)) { goto Exit1; }
                if (SUCCESS(IsSending)) { goto Exit1; }

                if (SendBuf.GetOnceReadableLen() > 0)
                {
                    IsSending = true;
                    haveDataSend = true;

                    handlerParam = new HandlerParam(EHandlerType.Socker, this);
                    if (LOG_ERROR(handlerParam != null)) { goto Exit0; }

                    ar = Sock.BeginSend(SendBuf.BufBytes, SendBuf.ReadInx, SendBuf.GetOnceReadableLen(), SocketFlags.None, out socketError,
                        LP.NetModule.Reactor.OnSend, handlerParam);
                    if (socketError != SocketError.Success)
                    {
                        Close(ESockerCloseType.PostSendFail, 1, true);
                        goto Exit0;
                    }
                    if (LOG_ERROR(ar != null)) { goto Exit0; }
                }
            }
            catch (Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }
            
        Exit1:
        Exit0:
            return haveDataSend;
        }
    }
}
