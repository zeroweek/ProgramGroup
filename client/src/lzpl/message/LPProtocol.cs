using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    public class LPProtocol
    {
        public const UInt16 max_message_count = 1024;

        public const UInt16 min_message_id = 0;
        public const UInt16 c2t_message_begin = 1;
        public const UInt16 c2t_message_end = 100;
        public const UInt16 t2c_message_begin = 101;
        public const UInt16 t2c_message_end = 200;
        public const UInt16 c2g_message_begin = 201;
        public const UInt16 c2g_message_end = 400;
        public const UInt16 g2c_message_begin = 401;
        public const UInt16 g2c_message_end = 600;
        public const UInt16 c2m_message_begin = 601;
        public const UInt16 c2m_message_end = 800;
        public const UInt16 m2c_message_begin = 801;
        public const UInt16 m2c_message_end = 1000;
        public const UInt16 max_message_id = m2c_message_end + 1;
    }

    public enum C2T_MESSAGE_ID
    {
        c2t_begin = LPProtocol.c2t_message_begin,

        c2t_login_req = c2t_begin,


        c2t_end = LPProtocol.c2t_message_end
    }

    public enum T2C_MESSAGE_ID
    {
        t2c_begin = LPProtocol.t2c_message_begin,

        t2c_login_ack = t2c_begin,


        t2c_end = LPProtocol.t2c_message_end
    }

    public class C2T_LOGIN_REQ : MESSAGE_HEADER
    {
        public Byte byValue;
        public UInt16 wValue;
        public UInt32 dwValue;
        public UInt64 qwValue;
        public string sValue;

        public override bool Serialize(ILPMessageSerializer messageSerializer)
        {
            bool result = false;
            UInt16 size = 0;
            byte[] buf = null;

            if (LOG_ERROR(messageSerializer != null)) { goto Exit0; }

            // # begin serialize field #

            result = base.Serialize(messageSerializer);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt8(byValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt16(wValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt32(dwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt64(qwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteString(sValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            // # end #

            buf = messageSerializer.GetSerializeBuf();
            if (LOG_ERROR(buf != null)) { goto Exit0; }

            if (LOG_ERROR(messageSerializer.GetSerializeSize() <= LPMessageHeader.MAX_PACKET_LEN)) { goto Exit0; }

            size = (UInt16)messageSerializer.GetSerializeSize();
            if (LOG_ERROR(size >= 4)) { goto Exit0; }

            buf[2] = (Byte)(size >> 8);
            buf[3] = (Byte)((size << 8) >> 8);

            return true;
        Exit0:
            return false;
        }

        public override bool UnSerialize(ILPMessageSerializer messageSerializer)
        {
            bool result = false;

            if (LOG_ERROR(messageSerializer != null)) { goto Exit0; }

            // # begin unserialize field #

            result = base.UnSerialize(messageSerializer);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt8(out byValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt16(out wValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt32(out dwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt64(out qwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadString(out sValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            // # end #

            return true;
        Exit0:
            return false;
        }
    }


    public class T2C_LOGIN_ACK : MESSAGE_HEADER
    {
        public Byte byValue;
        public UInt16 wValue;
        public UInt32 dwValue;
        public UInt64 qwValue;
        public string sValue;

        public override bool Serialize(ILPMessageSerializer messageSerializer)
        {
            bool result = false;
            UInt16 size = 0;
            byte[] buf = null;

            if (LOG_ERROR(messageSerializer != null)) { goto Exit0; }

            // # begin serialize field #

            result = base.Serialize(messageSerializer);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt8(byValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt16(wValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt32(dwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt64(qwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteString(sValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            // # end #

            buf = messageSerializer.GetSerializeBuf();
            if (LOG_ERROR(buf != null)) { goto Exit0; }

            if (LOG_ERROR(messageSerializer.GetSerializeSize() <= LPMessageHeader.MAX_PACKET_LEN)) { goto Exit0; }

            size = (UInt16)messageSerializer.GetSerializeSize();
            if (LOG_ERROR(size >= 4)) { goto Exit0; }

            buf[2] = (Byte)(size >> 8);
            buf[3] = (Byte)((size << 8) >> 8);

            return true;
        Exit0:
            return false;
        }

        public override bool UnSerialize(ILPMessageSerializer messageSerializer)
        {
            bool result = false;

            if (LOG_ERROR(messageSerializer != null)) { goto Exit0; }

            // # begin unserialize field #

            result = base.UnSerialize(messageSerializer);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt8(out byValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt16(out wValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt32(out dwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt64(out qwValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadString(out sValue);
            if (LOG_ERROR(result)) { goto Exit0; }

            // # end #

            return true;
        Exit0:
            return false;
        }
    }
}
