using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace lzpl
{
    public class LPNet : LPError
    {
        internal LPReactor Reactor { get; set; }
        internal LPNetEventMgr EventMgr { get; set; }

        private int m_ListenerIDGenerator = 0;
        private int m_ConnectorIDGenerator = 0;
        private int m_SockerIDGenerator = 0;

        private LPBaseList m_ValidSockerList = null;
        private object m_ValidListLocker = new object();
        private Thread m_PostSendThread = null;
        private bool m_PostSendRun = false;

        private LPBaseList m_DelayCloseList = null;
        private LPBaseList m_DelayReleaseList = null;
        private object m_DelayLocker = new object();
        private Thread m_CheckDelayThread = null;
        private bool m_CheckDelayRun = false;

        internal bool Init(ILPNetMessageHandler netMessageHandler)
        {
            bool bResult = false;

            Reactor = new LPReactor();
            if (LOG_ERROR(Reactor != null)) { goto Exit0; }

            bResult = Reactor.Init();
            if (LOG_ERROR(bResult)) { goto Exit0; }

            EventMgr = new LPNetEventMgr();
            if (LOG_ERROR(EventMgr != null)) { goto Exit0; }

            bResult = EventMgr.Init(netMessageHandler, LP.NetEventListCount);
            if (LOG_ERROR(bResult)) { goto Exit0; }

            m_ValidSockerList = new LPBaseList();
            if (LOG_ERROR(m_ValidSockerList != null)) { goto Exit0; }

            m_DelayCloseList = new LPBaseList();
            if (LOG_ERROR(m_DelayCloseList != null)) { goto Exit0; }

            m_DelayReleaseList = new LPBaseList();
            if (LOG_ERROR(m_DelayReleaseList != null)) { goto Exit0; }
            
            m_CheckDelayRun = true;
            m_CheckDelayThread = new Thread(_CheckDelayThreadProc);
            m_CheckDelayThread.Start();

            m_PostSendRun = true;
            m_PostSendThread = new Thread(_PostSendThreadProc);
            m_PostSendThread.Start();

            return true;
        Exit0:
            return false;
        }

        internal bool UnInit()
        {
            if(m_CheckDelayRun)
            {
                m_CheckDelayRun = false;
                m_CheckDelayThread.Join();
            }
            m_CheckDelayThread = null;

            if (m_PostSendRun)
            {
                m_PostSendRun = false;
                m_PostSendThread.Join();
            }
            m_PostSendThread = null;

            if (Reactor != null)
            {
                Reactor.UnInit();
                Reactor = null;
            }

            if (EventMgr != null)
            {
                EventMgr.UnInit();
                EventMgr = null;
            }

            if (EventMgr != null)
            {
                EventMgr.UnInit();
                EventMgr = null;
            }

            return true;
        }


        // Summary:
        //   处理网络包函数
        // Input:
        //   count-每次检测处理网络包的最大数量，-1表示处理所有
        // Return:
        //   true-所有网络包都已被处理，false-有剩余未处理网络包
        public bool Run(int count = -1)
        {
            bool result = false;

            do
            {
                result = EventMgr.HaveEventForHandle();
                if (SUCCESS(!result)) { goto Exit1; }

                EventMgr.HandleOneEvent();

            } while (--count != 0);

            result = EventMgr.HaveEventForHandle();
            if (SUCCESS(!result)) { goto Exit1; }

            return false;
        Exit1:
            return true;
        }

        public LPListener CreateListener(IPAddress addr, int nPort, ILPPacketParser packetParser)
        {
            LPListener listener = null;
            TcpListener tl = null;

            if (LOG_ERROR(packetParser != null)) { goto Exit0; }

            listener = new LPListener();
            if (LOG_ERROR(listener != null)) { goto Exit0; }

            tl = new TcpListener(addr, nPort);
            if (LOG_ERROR(tl != null)) { goto Exit0; }

            listener.Listener = tl;
            listener.ID = _GenerateListenerID();
            listener.PacketParser = packetParser;

            return listener; 
        Exit0:
            return null;
        }

        public LPConnector CreateConnector(ILPPacketParser packetParser)
        {
            TcpClient tc = null;
            LPConnector connector = null;

            if (LOG_ERROR(packetParser != null)) { goto Exit0; }

            connector = new LPConnector();
            if (LOG_ERROR(connector != null)) { goto Exit0; }

            tc = new TcpClient();
            if (LOG_ERROR(tc != null)) { goto Exit0; }

            connector.Client = tc;
            connector.ID = _GenerateConnectorID();
            connector.PacketParser = packetParser;

            return connector; 
        Exit0:
            return null;
        }

        internal LPSocker CreateSocker(Socket socket, ILPPacketParser packetParser, object parent, bool isAcceptCreate)
        {
            bool result = false;
            LPSocker socker = null;

            if (LOG_ERROR(socket != null)) { goto Exit0; }
            if (LOG_ERROR(packetParser != null)) { goto Exit0; }

            socker = new LPSocker();
            if (LOG_ERROR(packetParser != null)) { goto Exit0; }

            socker.ID = _GenerateSockerID();
            socker.Sock = socket;
            socker.Parent = parent;
            socker.IsAcceptCreate = isAcceptCreate;
            socker.IsClosed = false;
            socker.IsSending = false;
            socker.PacketParser = packetParser;

            socker.RecvBuf = new LPLoopBuf();
            if (LOG_ERROR(socker.RecvBuf != null)) { goto Exit0; }

            result = socker.RecvBuf.Init(LP.RecvBufSize);
            if (LOG_ERROR(result)) { goto Exit0; }

            socker.SendBuf = new LPLoopBuf();
            if (LOG_ERROR(socker.SendBuf != null)) { goto Exit0; }

            result = socker.SendBuf.Init(LP.SendBufSize);
            if (LOG_ERROR(result)) { goto Exit0; }

            lock (m_ValidListLocker)
            {
                result = m_ValidSockerList.PushRear(socker);
                if (LOG_ERROR(result)) { goto Exit0; }
            }

            return socker; 
        Exit0:
            return null;
        }

        internal bool FreeSocker(LPSocker socker)
        {
            return true;
        }

        internal void DelayClose(LPSocker socker)
        {
            if (LOG_ERROR(socker != null)) { goto Exit0; }

            lock(m_DelayLocker)
            {
                socker.DelayCloseBeginTick = DateTime.Now.Ticks;
                if (socker.IsPassiveClosed)
                {
                    //被动关闭，无需延迟close socket
                    socker.DelayCloseDuration = 0;
                }
                else
                {
                    //主动关闭，需要等未发送完的数据发送完
                    socker.DelayCloseDuration = 3000;
                }

                m_DelayCloseList.PushRear(socker);
            }

        Exit0:
            return;
        }

        internal void DelayRelease(LPSocker socker)
        {
            if (LOG_ERROR(socker != null)) { goto Exit0; }

            lock (m_DelayLocker)
            {
                socker.DelayReleaseBeginTick = DateTime.Now.Ticks;
                if (socker.IsPassiveClosed)
                {
                    //被动关闭，无需延迟close socket
                    socker.DelayReleaseDuration = 0;
                }
                else
                {
                    //主动关闭，需要延迟释放，因为socker的close操作只关闭了发，
                    //如果不延迟释放，可能在收的过程访问到被释放的socker
                    socker.DelayReleaseDuration = 1500;
                }
                m_DelayReleaseList.PushRear(socker);
            }

        Exit0:
            return;
        }

        private int _GenerateListenerID()
        {
            return Interlocked.Increment(ref m_ListenerIDGenerator);
        }

        private int _GenerateConnectorID()
        {
            return Interlocked.Increment(ref m_ConnectorIDGenerator);
        }

        private int _GenerateSockerID()
        {
            return Interlocked.Increment(ref m_SockerIDGenerator);
        }

        private  void _CheckDelayThreadProc()
        {
            bool result = false;
            long ticks = 0;
            LPSocker socker = null;
            BASE_LIST_NODE node = null;
            BASE_LIST_NODE nextNode = null;

            while(m_CheckDelayRun)
            {
                Thread.Sleep(5);

                ticks = DateTime.Now.Ticks;

                lock (m_DelayLocker)
                {
                    //检查延迟的关闭连接
                    node = m_DelayCloseList.Head.Next;
                    while (node != null && node != m_DelayCloseList.Rear)
                    {
                        nextNode = node.Next;
                        socker = (LPSocker)node.Data;
                        if(socker == null)
                        {
                            LOG_ERROR(false);
                            node = nextNode;
                            continue;
                        }

                        if (ticks >= socker.DelayCloseBeginTick + socker.DelayCloseDuration)
                        {
                            socker.Sock.Close();
                            result = m_DelayCloseList.Remove(node);
                            LOG_ERROR(result);

                            DelayRelease(socker);
                        }

                        node = nextNode;
                    }

                    //检查延迟释放的连接
                    node = m_DelayReleaseList.Head.Next;
                    while (node != null && node != m_DelayReleaseList.Rear)
                    {
                        nextNode = node.Next;
                        socker = (LPSocker)node.Data;
                        if (socker == null)
                        {
                            LOG_ERROR(false);
                            node = nextNode;
                            continue;
                        }

                        if (ticks >= socker.DelayReleaseBeginTick + socker.DelayReleaseDuration)
                        {
                            result = m_DelayReleaseList.Remove(node);
                            LOG_ERROR(result);

                            LP.NetModule.FreeSocker(socker);
                        }

                        node = nextNode;
                    }
                }
            }

            return;
        }

        private void _PostSendThreadProc()
        {
            int sendDataSockerCount = 0;
            LPSocker socker = null;
            BASE_LIST_NODE node = null;
            BASE_LIST_NODE nextNode = null;

            while (m_CheckDelayRun)
            {
                sendDataSockerCount = 0;

                if(m_ValidSockerList.IsEmpty())
                {
                    Thread.Sleep(1);
                    continue;
                }

                lock (m_ValidListLocker)
                {
                    node = m_ValidSockerList.Head.Next;
                    while(node != null && node != m_ValidSockerList.Rear)
                    {
                        nextNode = node.Next;
                        socker = (LPSocker)node.Data;
                        if (socker == null)
                        {
                            LOG_ERROR(false);
                            node = nextNode;
                            continue;
                        }

                        if (socker.PostSend())
                        {
                            ++sendDataSockerCount;
                        }

                        node = nextNode;
                    }
                }
                
                if(sendDataSockerCount > 0)
                {
                    Thread.Sleep(1);
                }
            }

            return;
        }
    }
}
