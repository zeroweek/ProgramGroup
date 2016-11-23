using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    public class LPPacketParser : LPError, ILPPacketParser
    {
        public int Parse(LPLoopBuf loopBuf)
        {
            bool result = false;
            int parseCount = -1;
            UInt16 MsgId = 0;
            UInt16 MsgSize = 0;
            int MsgHeadSize = 4;
            byte[] MsgIdBytes = new byte[MsgHeadSize];

            if (LOG_ERROR(loopBuf != null)) { goto Exit0; }

            if (loopBuf.GetTotalReadableLen() >= MsgHeadSize)
            {
                if (LOG_ERROR(MsgIdBytes != null)) { goto Exit0; }
                result = loopBuf.Read(MsgIdBytes, 0, MsgHeadSize, false);
                if (LOG_ERROR(result)) { goto Exit0; }

                MsgId = (UInt16)(MsgIdBytes[0] | MsgIdBytes[1]);
                if (LOG_ERROR(MsgId > LPProtocol.min_message_id && MsgId < LPProtocol.max_message_id)) { goto Exit0; }

                MsgSize = (UInt16)(MsgIdBytes[2] | MsgIdBytes[3]);
                if (loopBuf.GetTotalReadableLen() >= MsgSize)
                {
                    parseCount = MsgSize;
                    if (SUCCESS(true)) { goto Exit1; }
                }
            }

            parseCount = 0;
            if (SUCCESS(true)) { goto Exit1; }

        Exit0:
            parseCount = -1;

        Exit1:
            return parseCount;
        }
    }
}
