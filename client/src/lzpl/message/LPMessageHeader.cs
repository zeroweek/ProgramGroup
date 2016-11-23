using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace lzpl
{
    public class LPMessageHeader
    {
        public const int MAX_PACKET_LEN = 65535;
    }

    public class MESSAGE_HEADER : LPError
    {
        public UInt16 MsgId;
        public UInt16 MsgSize;

        public virtual bool Serialize(ILPMessageSerializer messageSerializer)
        {
            bool result = false;
            UInt16 size = 0;
            byte[] buf = null;

            if (LOG_ERROR(messageSerializer != null)) { goto Exit0; }

            // # begin serialize field #

            result = messageSerializer.WriteUInt16(MsgId);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.WriteUInt16(4);
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

        public virtual bool UnSerialize(ILPMessageSerializer messageSerializer)
        {
            bool result = false;

            if (LOG_ERROR(messageSerializer != null)) { goto Exit0; }

            // # begin unserialize field #

            result = messageSerializer.ReadUInt16(out MsgId);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = messageSerializer.ReadUInt16(out MsgSize);
            if (LOG_ERROR(result)) { goto Exit0; }

            // # end #

            return true;
        Exit0:
            return false;
        }
    }
}
