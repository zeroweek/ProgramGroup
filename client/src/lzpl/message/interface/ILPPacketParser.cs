using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    public interface ILPPacketParser
    {
        //
        // Summary:
        //     解析网络数据包。
        //
        // Parameters:
        //   loopBuf:
        //     待解析数据缓冲区。
        //
        // Returns:
        //     "<0" -> 解析错误，"=0" -> 不完整包，">0" -> 完整包长度。
        int Parse(LPLoopBuf loopBuf);
    }
}
