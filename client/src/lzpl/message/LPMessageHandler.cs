using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace lzpl
{
    public class LPMessageHandler : LPError, ILPNetMessageHandler
    {
        private ILPMessageSerializer m_RecvMessageSerializer = null;
        private ILPMessageSerializer m_SendMessageSerializer = null;

        private delegate void pfunMessageCallback(LPSocker socker, byte[] buf, int size);
        private pfunMessageCallback[] m_MessageCallBackList = null;

        public LPMessageHandler()
        {
        }

        public bool Init()
        {
            m_RecvMessageSerializer = new LPMessageSerializer();
            if (LOG_ERROR(m_RecvMessageSerializer != null)) { goto Exit0; }

            m_SendMessageSerializer = new LPMessageSerializer();
            if (LOG_ERROR(m_SendMessageSerializer != null)) { goto Exit0; }

            m_MessageCallBackList = new pfunMessageCallback[LPProtocol.max_message_count];
            if (LOG_ERROR(m_MessageCallBackList != null)) { goto Exit0; }

            // 消息回调函数注册
            m_MessageCallBackList[(int)T2C_MESSAGE_ID.t2c_login_ack] = OnGTLoginAck;

            return true;
        Exit0:
            return false;
        }

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
	    bool DoGTLoginReq(LPSocker socker)
        {
            bool result = false;
            C2T_LOGIN_REQ msg = null;

            msg = new C2T_LOGIN_REQ();
            if (LOG_ERROR(msg != null)) { goto Exit0; }
            msg.MsgId = (UInt16)C2T_MESSAGE_ID.c2t_login_req;
            msg.byValue = 1;
            msg.wValue = 2;
            msg.dwValue = 3;
            msg.qwValue = 4;
            msg.sValue = "I'm client, i want to login !";

            result = m_SendMessageSerializer.Init(null, 0, null, 0);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = msg.Serialize(m_SendMessageSerializer);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = socker.Send(m_SendMessageSerializer.GetSerializeBuf(), 0, m_SendMessageSerializer.GetSerializeSize());
            if (LOG_ERROR(result)) { goto Exit0; }

            result = m_SendMessageSerializer.UnInit();
            if (LOG_ERROR(result)) { goto Exit0; }

            return true;

        Exit0:

            result = m_SendMessageSerializer.UnInit();
            if (LOG_ERROR(result)) { goto Exit0; }

            return false;
        }

        void OnGTLoginAck(LPSocker socker, byte[] buf, int length)
        {
            bool result = false;
            T2C_LOGIN_ACK msg = null;

            msg = new T2C_LOGIN_ACK();
            if (LOG_ERROR(msg != null)) { goto Exit0; }

            result = m_RecvMessageSerializer.Init(null, 0, buf, length);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = msg.UnSerialize(m_RecvMessageSerializer);
            if (LOG_ERROR(result)) { goto Exit0; }

            LP.Logger.IMP("recv gateserver login ack, login successfully !");
            LP.Logger.IMP("T2C_LOGIN_ACK(byValue={0},wValue={1},dwValue={2},qwValue={3},szValue={4})",
                msg.byValue, msg.wValue, msg.dwValue, msg.qwValue, msg.sValue);

        Exit0:

            result = m_RecvMessageSerializer.UnInit();
            LOG_ERROR(result);

            return;
        }

        public void OnAccepted(LPSocker socker)
        {
            LP.Logger.IMP("OnAccepted output : socker id is {0}", socker.ID);
        }

        public void OnConnected(LPSocker socker)
        {
            bool result = false;
            
            LP.Logger.IMP("OnConnected output : socker id is {0}", socker.ID);

            result = DoGTLoginReq(socker);
            if (LOG_ERROR(result)) { goto Exit0; }

        Exit0:
            return;
        }

        public void OnConnectError(LPConnector connector, int errorNo)
        {
            LP.Logger.IMP("OnConnectError output : connector id is {0}, error no is {1}", connector.ID, errorNo);
        }

        public void OnMessage(LPSocker socker, byte[] buf, int size)
        {
            int Index = 0;
            UInt16 MsgId = 0;
            UInt16 MsgSize = 0;
            int MsgHeadSize = 4;

            LP.Logger.IMP("OnMessage output : socker id is {0}", socker.ID);

            if (LOG_ERROR(socker != null)) { goto Exit0; }
            if (LOG_ERROR(buf != null)) { goto Exit0; }
            if (LOG_ERROR(size >= MsgHeadSize)) { goto Exit0; }

            MsgId = (UInt16)(buf[Index++] | buf[Index++]);
            if (LOG_ERROR(MsgId > LPProtocol.min_message_id && MsgId < LPProtocol.max_message_id)) { goto Exit0; }

            MsgSize = (UInt16)(buf[Index++] | buf[Index++]);
            if (LOG_ERROR(MsgSize == size)) { goto Exit0; }

            if (LOG_ERROR(m_MessageCallBackList[MsgId] != null)) { goto Exit0; }

            try
            {
                m_MessageCallBackList[MsgId](socker, buf, size);
            }
            catch(Exception e)
            {
                LPMiniDump.GenerateNormalDump(e);
            }

        Exit0:
            return;
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
