using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace lzpl
{
    internal enum ENetEventType
    {
        Invalid = 0,
        Recv,
        Terminate,
        Establish,
        ConnectError,
        Total
    }

    internal class LPNetEvent : BASE_LIST_NODE
    {
        public ENetEventType eType { get; set; }
        public int Flag { get; set; }
    }

    internal class LPRecvNetEvent : LPNetEvent
    {
        public int RecvLength { get; set; }
        public LPSocker Socker { get; set; }
    }

    internal class LPTerminateNetEvent : LPNetEvent
    {
        public LPSocker Socker { get; set; }
    }

    internal class LPEstablishNetEvent : LPNetEvent
    {
        public LPSocker Socker { get; set; }
    }

    internal class LPConnectErrorNetEvent : LPNetEvent
    {
        public LPConnector Connector { get; set; }
        public int ErrorNo { get; set; }
    }

    internal class LPNetEventMgr : LPError
    {
        private LPLoopBuf m_EventBuf = null;
        private byte[] m_TempPacketBuf = null;
        private ILPNetMessageHandler m_NetMessageHandler = null;
        private int m_EventListCount = 0;
        private LPBaseList[] m_EventLists = null;
        private object[] m_EventListLocker = null;

        internal LPNetEventMgr()
        {

        }

        internal bool Init(ILPNetMessageHandler netMessageHandler, int eventListCount)
        {
            bool result = false;

            if (LOG_ERROR(eventListCount > 0)) { goto Exit0; }
            if (LOG_ERROR(netMessageHandler != null)) { goto Exit0; }

            m_NetMessageHandler = netMessageHandler;

            m_EventBuf = new LPLoopBuf();
            if (LOG_ERROR(m_EventBuf != null)) { goto Exit0; }

            result = m_EventBuf.Init(LP.NetEventBufSize);
            if (LOG_ERROR(result)) { goto Exit0; }

            m_TempPacketBuf = new byte[LP.MaxPacketLen];
            if (LOG_ERROR(m_TempPacketBuf != null)) { goto Exit0; }

            m_EventListCount = eventListCount;

            m_EventLists = new LPBaseList[m_EventListCount];
            if (LOG_ERROR(m_EventLists != null)) { goto Exit0; }
            for(int i = 0; i < m_EventListCount; ++i)
            {
                m_EventLists[i] = new LPBaseList();
                if (LOG_ERROR(m_EventLists[i] != null)) { goto Exit0; }
            }

            m_EventListLocker = new object[m_EventListCount];
            if (LOG_ERROR(m_EventListLocker != null)) { goto Exit0; }
            for (int i = 0; i < m_EventListCount; ++i)
            {
                m_EventListLocker[i] = new object();
                if (LOG_ERROR(m_EventListLocker[i] != null)) { goto Exit0; }
            }

            return true;
        Exit0:
            return false;
        }

        internal bool UnInit()
        {
            return true;
        }

        internal bool PushRecvEvent(LPSocker socker, LPLoopBuf recvLoopBuf, int len)
        {
            bool result = false;
            int retryCount = 0;
            int lineSize = 0;
            LPRecvNetEvent recvEvent = null;

            if (LOG_ERROR(socker != null)) { goto Exit0; }
            if (LOG_ERROR(recvLoopBuf != null)) { goto Exit0; }

            recvEvent = (LPRecvNetEvent)_CreateEvent(ENetEventType.Recv);
            if (LOG_ERROR(recvEvent != null)) { goto Exit0; }

            while (m_EventBuf.GetTotalWritableLen() < len)
            {
                LP.Logger.P_WRN("event buf not enough, sleep and try again !");
                Thread.Sleep(1);
                ++retryCount;

                // 超过10秒，丢弃
                if (retryCount > 10000)
                {
                    recvLoopBuf.FinishRead(len);
                    if (LOG_ERROR(false)) { goto Exit0; }
                }
            }

            lineSize = recvLoopBuf.GetOnceReadableLen();
            if (lineSize > len)
            {
                lineSize = len;
            }

            result = m_EventBuf.Write(recvLoopBuf.BufBytes, recvLoopBuf.ReadInx, lineSize);
            LOG_ERROR(result);

            recvLoopBuf.FinishRead(lineSize);

            if (lineSize < len)
            {
                result = m_EventBuf.Write(recvLoopBuf.BufBytes, recvLoopBuf.ReadInx, len - lineSize);
                LOG_ERROR(result);

                recvLoopBuf.FinishRead(len - lineSize);
            }

            recvEvent.Flag = socker.ID;
            recvEvent.RecvLength = len;
            recvEvent.Socker = socker;

            lock (m_EventListLocker[recvEvent.Flag % m_EventListCount])
            {
                result = m_EventLists[recvEvent.Flag % m_EventListCount].PushRear(recvEvent);
                if (LOG_ERROR(result)) { goto Exit0; }
            }

            return true;
        Exit0:
            return false;
        }

        internal bool PushTerminateEvent(LPSocker socker)
        {
            bool result = false;
            LPTerminateNetEvent terminateEvent = null;

            if (LOG_ERROR(socker != null)) { goto Exit0; }


            terminateEvent = (LPTerminateNetEvent)_CreateEvent(ENetEventType.Terminate);
            if (LOG_ERROR(terminateEvent != null)) { goto Exit0; }

            
            terminateEvent.Flag = socker.ID;
            terminateEvent.Socker = socker;

            lock (m_EventListLocker[terminateEvent.Flag % m_EventListCount])
            {
                result = m_EventLists[terminateEvent.Flag % m_EventListCount].PushRear(terminateEvent);
                if (LOG_ERROR(result)) { goto Exit0; }
            }

            return true;
        Exit0:
            return false;
        }

        internal bool PushEstablishEvent(LPSocker socker)
        {
            bool result = false;
            LPEstablishNetEvent establishEvent = null;

            if (LOG_ERROR(socker != null)) { goto Exit0; }


            establishEvent = (LPEstablishNetEvent)_CreateEvent(ENetEventType.Establish);
            if (LOG_ERROR(establishEvent != null)) { goto Exit0; }


            establishEvent.Flag = socker.ID;
            establishEvent.Socker = socker;

            lock (m_EventListLocker[establishEvent.Flag % m_EventListCount])
            {
                result = m_EventLists[establishEvent.Flag % m_EventListCount].PushRear(establishEvent);
                if (LOG_ERROR(result)) { goto Exit0; }
            }

            return true;
        Exit0:
            return false;
        }

        internal bool PushConnectorErrorEvent(LPConnector connector, int errorNo)
        {
            bool result = false;
            LPConnectErrorNetEvent connectErrorEvent = null;

            if (LOG_ERROR(connector != null)) { goto Exit0; }


            connectErrorEvent = (LPConnectErrorNetEvent)_CreateEvent(ENetEventType.ConnectError);
            if (LOG_ERROR(connectErrorEvent != null)) { goto Exit0; }


            connectErrorEvent.Flag = connector.ID;
            connectErrorEvent.Connector = connector;
            connectErrorEvent.ErrorNo = errorNo;

            lock (m_EventListLocker[connectErrorEvent.Flag % m_EventListCount])
            {
                result = m_EventLists[connectErrorEvent.Flag % m_EventListCount].PushRear(connectErrorEvent);
                if (LOG_ERROR(result)) { goto Exit0; }
            }

            return true;
        Exit0:
            return false;
        }

        internal bool HaveEventForHandle()
        {
            for (int i = 0; i < m_EventListCount; ++i)
            {
                if (!m_EventLists[i].IsEmpty())
                {
                    return true;
                }
            }

            return false;
        }

        internal void HandleOneEvent()
        {
            LPNetEvent netEvent = null;
            BASE_LIST_NODE node = null;

            for (int i = 0; i < m_EventListCount; ++i)
            {
                if (m_EventLists[i].IsEmpty())
                {
                    continue;
                }

                lock(m_EventListLocker[i])
                {
                    node = m_EventLists[i].PopHead();
                    if (LOG_ERROR(node != null)) { goto Exit0; }

                    netEvent = (LPNetEvent)node.Data;
                    if (LOG_ERROR(netEvent != null)) { goto Exit0; }
                }

                switch (netEvent.eType)
                {
                    case ENetEventType.Recv:
                        {
                            _ProcRecvEvent(netEvent);
                        }
                        break;
                    case ENetEventType.Terminate:
                        {
                            _ProcTerminateEvent(netEvent);
                        }
                        break;
                    case ENetEventType.Establish:
                        {
                            _ProcEstablishEvent(netEvent);
                        }
                        break;
                    case ENetEventType.ConnectError:
                        {
                            _ProcConnectErrorEvent(netEvent);
                        }
                        break;
                    default:
                        if (LOG_ERROR(false)) { goto Exit0; }
                        break;
                }
            }

        Exit0:

            if (netEvent != null) _FreeEvent(netEvent);

            return;
        }

        private LPNetEvent _CreateEvent(ENetEventType eType)
        {
            LPNetEvent netEvent = null;

            switch (eType)
            {
                case ENetEventType.Recv:
                    {
                        netEvent = new LPRecvNetEvent();
                        if (LOG_ERROR(netEvent != null)) { goto Exit0; }
                        netEvent.eType = ENetEventType.Recv;
                    }
                    break;
                case ENetEventType.Terminate:
                    {
                        netEvent = new LPTerminateNetEvent();
                        if (LOG_ERROR(netEvent != null)) { goto Exit0; }
                        netEvent.eType = ENetEventType.Terminate;
                    }
                    break;
                case ENetEventType.Establish:
                    {
                        netEvent = new LPEstablishNetEvent();
                        if (LOG_ERROR(netEvent != null)) { goto Exit0; }
                        netEvent.eType = ENetEventType.Establish;
                    }
                    break;
                case ENetEventType.ConnectError:
                    {
                        netEvent = new LPConnectErrorNetEvent();
                        if (LOG_ERROR(netEvent != null)) { goto Exit0; }
                        netEvent.eType = ENetEventType.ConnectError;
                    }
                    break;
                default:
                    if (LOG_ERROR(false)) { goto Exit0; }
                    break;
            }

            return netEvent;
        Exit0:
            return null;
        }

        private void _FreeEvent(LPNetEvent netEvent)
        {

        }

        private void _ProcRecvEvent(LPNetEvent netEvent)
        {
            bool result = false;
            LPRecvNetEvent recvEvent = null;

            if (LOG_ERROR(netEvent != null)) { goto Exit0; }

            recvEvent = (LPRecvNetEvent)netEvent;
            if (LOG_ERROR(recvEvent.RecvLength < LP.MaxPacketLen)) { goto Exit0; }

            result = m_EventBuf.Read(m_TempPacketBuf, 0, recvEvent.RecvLength, true);
            if (LOG_ERROR(result)) { goto Exit0; }

            m_NetMessageHandler.OnMessage(recvEvent.Socker, m_TempPacketBuf, recvEvent.RecvLength);

        Exit0:
            return;
        }

        private void _ProcTerminateEvent(LPNetEvent netEvent)
        {
            LPSocker socker = null;
            LPConnector connector = null;
            LPTerminateNetEvent terminateEvent = null;

            if (LOG_ERROR(netEvent != null)) { goto Exit0; }

            terminateEvent = (LPTerminateNetEvent)netEvent;
            socker = terminateEvent.Socker;
            if (!socker.IsAcceptCreate)
            {
                connector = (LPConnector)socker.Parent;
                m_NetMessageHandler.OnConnectDisconnected(socker, connector);
            }
            else
            {
                m_NetMessageHandler.OnDisconnected(socker);
            }

            socker.OnClose();

        Exit0:
            return;
        }

        private void _ProcEstablishEvent(LPNetEvent netEvent)
        {
            LPSocker socker = null;
            LPEstablishNetEvent establishEvent = null;

            if (LOG_ERROR(netEvent != null)) { goto Exit0; }

            establishEvent = (LPEstablishNetEvent)netEvent;
            socker = establishEvent.Socker;

            if (socker.IsAcceptCreate)
            {
                m_NetMessageHandler.OnAccepted(socker);
            }
            else
            {
                m_NetMessageHandler.OnConnected(socker);
            }

            socker.PostRecv();

        Exit0:
            return;
        }

        private void _ProcConnectErrorEvent(LPNetEvent netEvent)
        {
            LPConnectErrorNetEvent connectErrorEvent = null;

            if (LOG_ERROR(netEvent != null)) { goto Exit0; }
            connectErrorEvent = (LPConnectErrorNetEvent)netEvent;

            m_NetMessageHandler.OnConnectError(connectErrorEvent.Connector, connectErrorEvent.ErrorNo);
        Exit0:
            return;
        }
    }
}
