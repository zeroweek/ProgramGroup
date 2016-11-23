using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    public abstract class ILPMessageSerializer : LPError
    {
        // Summary:
        //		初始化
        // Input:
        //   serializeBuf: 外部传入serialize buf（null则使用内部default serialize buf）
        //   serializeBufSize: 外部传入serialize buf大小（serializeBuf为null时，此参数填0）
        //   unSerializeBuf: 外部传入unserialize buf（null则使用内部default unserialize buf）
        //   unSerializeBufSize: 外部传入unserialize buf大小（unSerializeBuf为null时，此参数填0）
        public abstract bool Init(byte[] serializeBuf, int serializeBufSize, byte[] unSerializeBuf, int unSerializeBufSize);

        public abstract bool UnInit();

        public abstract bool ReadUInt8(out Byte value);

        public abstract bool WriteUInt8(Byte value);

        public abstract bool ReadUInt16(out UInt16 value);

        public abstract bool WriteUInt16(UInt16 value);

        public abstract bool ReadUInt32(out UInt32 value);

        public abstract bool WriteUInt32(UInt32 value);

        public abstract bool ReadUInt64(out UInt64 value);

        public abstract bool WriteUInt64(UInt64 value);

        public abstract bool ReadString(out string s);

        public abstract bool ReadString(Byte[] buf, int startIndex, out int length);

        public abstract bool WriteString(string s);

        public abstract bool WriteString(Byte[] buf, int startIndex, int length, int maxLength);

        public abstract Byte[] GetSerializeBuf();

        public abstract int GetSerializeSize();

    }
}
