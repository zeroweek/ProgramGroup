using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    class LPMessageSerializer : ILPMessageSerializer
    {
        private const UInt16 MAX_MESSAGE_LEN = LPMessageHeader.MAX_PACKET_LEN;

        private byte[] m_SerializeBufDefault = new byte[MAX_MESSAGE_LEN];
        private byte[] m_UnSerializeBufDefault = new byte[MAX_MESSAGE_LEN];

        private int m_SerializeSize = 0;
        private int m_SerializeBufSize = 0;
        private byte[] m_SerializeBufExt = null;

        private int m_UnSerializeSize = 0;
        private int m_UnSerializeBufSize = 0;
        private byte[] m_UnSerializeBufExt = null;

        public override bool Init(byte[] serializeBuf, int serializeBufSize, byte[] unSerializeBuf, int unSerializeBufSize)
        {
            if(LOG_ERROR(serializeBuf == null && serializeBufSize == 0
                    || serializeBuf != null  && serializeBufSize != 0)) { goto Exit0; };

            if(LOG_ERROR(unSerializeBuf == null && unSerializeBufSize == 0
                    || unSerializeBuf != null  && unSerializeBufSize != 0)) { goto Exit0; };

            m_SerializeSize = 0;
            m_SerializeBufExt = serializeBuf;
            m_SerializeBufSize = m_SerializeBufExt != null ? serializeBufSize : MAX_MESSAGE_LEN;

            m_UnSerializeSize = 0;
            m_UnSerializeBufExt = unSerializeBuf;
            m_UnSerializeBufSize = m_UnSerializeBufExt != null ? unSerializeBufSize : MAX_MESSAGE_LEN;

            return true;
        Exit0:
	        return false;
        }

        public override bool UnInit()
        {
            m_SerializeSize = 0;
            m_SerializeBufSize = 0;
            m_SerializeBufExt = null;
            m_SerializeBufDefault = new byte[MAX_MESSAGE_LEN];

            m_UnSerializeSize = 0;
            m_UnSerializeBufSize = 0;
            m_UnSerializeBufExt = null;
            m_UnSerializeBufDefault = new byte[MAX_MESSAGE_LEN];

            return true;
        }

        public override bool ReadUInt8(out Byte value)
        {
            byte[] buf = null;

            value = 0;
            if (LOG_ERROR(m_UnSerializeSize + 1 <= m_UnSerializeBufSize)) { goto Exit0; }

            buf = m_UnSerializeBufExt != null ? m_UnSerializeBufExt : m_UnSerializeBufDefault;
            value = buf[m_UnSerializeSize];

            m_UnSerializeSize += 1;
            
            return true;
        Exit0:
            return false;
        }

        public override bool WriteUInt8(Byte value)
        {
            byte[] buf = null;

            if (LOG_ERROR(m_SerializeSize + 1 <= m_SerializeBufSize)) { goto Exit0; }

            buf = m_SerializeBufExt != null ? m_SerializeBufExt : m_SerializeBufDefault;
            buf[m_SerializeSize] = value;

            m_SerializeSize += 1;
            
            return true;
        Exit0:
            return false;
        }

        public override bool ReadUInt16(out UInt16 value)
        {
            bool result = false;
            byte data1 = 0;
            byte data2 = 0;

            value = 0;

            result = ReadUInt8(out data1);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = ReadUInt8(out data2);
            if (LOG_ERROR(result)) { goto Exit0; }

            value = (UInt16)(((UInt16)data1 << 8) | (UInt16)data2);
            
            return true;
        Exit0:
            return false;
        }

        public override bool WriteUInt16(UInt16 value)
        {
            bool result = false;
            byte data1 = 0;
            byte data2 = 0;

            data1 = (byte)(value >> 8);
            result = WriteUInt8(data1);
            if (LOG_ERROR(result)) { goto Exit0; }

            data2 = (byte)((value << 8) >> 8);
            result = WriteUInt8(data2);
            if (LOG_ERROR(result)) { goto Exit0; }
            
            return true;
        Exit0:
            return false;
        }

        public override bool ReadUInt32(out UInt32 value)
        {
            bool result = false;
            UInt16 data1 = 0;
            UInt16 data2 = 0;

            value = 0;

            result = ReadUInt16(out data1);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = ReadUInt16(out data2);
            if (LOG_ERROR(result)) { goto Exit0; }

            value = (UInt32)(((UInt32)data1 << 16) | (UInt32)data2);
            
            return true;
        Exit0:
            return false;
        }

        public override bool WriteUInt32(UInt32 value)
        {
            bool result = false;
            UInt16 data1 = 0;
            UInt16 data2 = 0;

            data1 = (UInt16)(value >> 16);
            result = WriteUInt16(data1);
            if (LOG_ERROR(result)) { goto Exit0; }

            data2 = (UInt16)((value << 16) >> 16);
            result = WriteUInt16(data2);
            if (LOG_ERROR(result)) { goto Exit0; }
            
            return true;
        Exit0:
            return false;
        }

        public override bool ReadUInt64(out UInt64 value)
        {
            bool result = false;
            UInt32 data1 = 0;
            UInt32 data2 = 0;

            value = 0;

            result = ReadUInt32(out data1);
            if (LOG_ERROR(result)) { goto Exit0; }

            result = ReadUInt32(out data2);
            if (LOG_ERROR(result)) { goto Exit0; }

            value = (UInt64)(((UInt64)data1 << 32) | (UInt64)data2);
            
            return true;
        Exit0:
            return false;
        }

        public override bool WriteUInt64(UInt64 value)
        {
            bool result = false;
            UInt32 data1 = 0;
            UInt32 data2 = 0;

            data1 = (UInt32)(value >> 32);
            result = WriteUInt32(data1);
            if (LOG_ERROR(result)) { goto Exit0; }

            data2 = (UInt32)((value << 32) >> 32);
            result = WriteUInt32(data2);
            if (LOG_ERROR(result)) { goto Exit0; }
            
            return true;
        Exit0:
            return false;
        }

        public override bool ReadString(out string s)
        {
            bool result = false;
            UInt32 len = 0;
            byte[] buf = null;
            byte[] outBuf = null;

            s = string.Empty;

            result = ReadUInt32(out len);
            if (LOG_ERROR(result)) { goto Exit0; }

            outBuf = new byte[len];
            if (LOG_ERROR(outBuf != null)) { goto Exit0; }

            if (LOG_ERROR(m_UnSerializeSize + len <= m_UnSerializeBufSize)) { goto Exit0; }

            buf = m_UnSerializeBufExt != null ? m_UnSerializeBufExt : m_UnSerializeBufDefault;
            Array.Copy(buf, m_UnSerializeSize, outBuf, 0, len);

            m_UnSerializeSize += (int)len;

            s = Encoding.UTF8.GetString(outBuf);

            return true;
        Exit0:
            return false;
        }

        public override bool ReadString(Byte[] buf, int startIndex, out int length)
        {
            bool result = false;
            UInt32 len = 0;
            byte[] serializeBuf = null;

            length = 0; ;

            if (LOG_ERROR(buf != null)) { goto Exit0; }

            result = ReadUInt32(out len);
            if (LOG_ERROR(result)) { goto Exit0; }

            if (LOG_ERROR(m_UnSerializeSize + len <= m_UnSerializeBufSize)) { goto Exit0; }

            serializeBuf = m_UnSerializeBufExt != null ? m_UnSerializeBufExt : m_UnSerializeBufDefault;
            Array.Copy(serializeBuf, m_UnSerializeSize, buf, startIndex, len);

            m_UnSerializeSize += (int)len;

            length = (int)len;

            return true;
        Exit0:
            return false;
        }

        public override bool WriteString(string s)
        {
            bool result = false;
            byte[] serializeBuf = null;
            byte[] writeData = null;

            if (LOG_ERROR(s != null)) { goto Exit0; }

            writeData = Encoding.UTF8.GetBytes(s.ToCharArray());
            if (LOG_ERROR(writeData != null)) { goto Exit0; }

            result = WriteUInt32((UInt32)writeData.Length);
            if (LOG_ERROR(result)) { goto Exit0; }

            if (LOG_ERROR(m_SerializeSize + writeData.Length <= m_SerializeBufSize)) { goto Exit0; }
            serializeBuf = m_SerializeBufExt != null ? m_SerializeBufExt : m_SerializeBufDefault;
            Array.Copy(writeData, 0, serializeBuf, m_SerializeSize, writeData.Length);

            m_SerializeSize += writeData.Length;

            return true;
        Exit0:
            return false;
        }

        public override bool WriteString(Byte[] buf, int startIndex, int length, int maxLength)
        {
            bool result = false;
            byte[] serializeBuf = null;

            if (LOG_ERROR(buf != null)) { goto Exit0; }

            if (length > maxLength)
            {
                length = maxLength;
            }

            result = WriteUInt32((UInt32)length);
            if (LOG_ERROR(result)) { goto Exit0; }

            if (LOG_ERROR(m_SerializeSize + length <= m_SerializeBufSize)) { goto Exit0; }
            serializeBuf = m_SerializeBufExt != null ? m_SerializeBufExt : m_SerializeBufDefault;
            Array.Copy(buf, startIndex, serializeBuf, m_SerializeSize, length);

            m_SerializeSize += length;

            return true;
        Exit0:
            return false;
        }

        public override Byte[] GetSerializeBuf()
        {
            return m_SerializeBufExt != null ? m_SerializeBufExt : m_SerializeBufDefault;
        }

        public override int GetSerializeSize()
        {
            return m_SerializeSize;
        }
    }
}
