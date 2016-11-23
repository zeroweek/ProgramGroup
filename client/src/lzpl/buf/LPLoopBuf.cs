using System;
using System.Threading;

namespace lzpl
{
    public class LPLoopBuf : LPError
    {
	    private byte[] m_BufBytes;    // 缓冲区
        private int m_nReadInx;       // 读索引
        private int m_nWriteInx;      // 写索引
        private long m_lDataLen;      // 有效数据长度
        private int m_nBufSize;       // 缓冲区大小

        public int ReadInx { get { return m_nReadInx; } }
        public int WriteInx { get { return m_nWriteInx; } }
        public byte[] BufBytes { get { return m_BufBytes; } }

        public LPLoopBuf()
        {
            _Reset();
        }

        ~LPLoopBuf()
        {
            
        }

        public bool Init(int nSize)
        {
            if (LOG_ERROR(m_BufBytes == null)) { goto Exit0; }

            m_nBufSize = nSize;
            m_BufBytes = new byte[m_nBufSize];
            if (LOG_ERROR(m_BufBytes != null)) { goto Exit0; }

            return true;
        Exit0:
            return false;
        }
	
        public int GetTotalReadableLen()
        {
            return _GetDataLen();
        }
	
        public int GetOnceReadableLen()
        {
            int nLineReadableLen = m_nBufSize - m_nReadInx;

            if (nLineReadableLen > GetTotalReadableLen())
            {
                nLineReadableLen = GetTotalReadableLen();
            }

            return nLineReadableLen;
        }

        public int GetTotalWritableLen()
        {
            return m_nBufSize - GetTotalReadableLen();
        }

        public int GetOnceWritableLen()
        {
            int nLineWritableLen = m_nBufSize - m_nWriteInx;

            if (nLineWritableLen > GetTotalWritableLen())
            {
                nLineWritableLen = GetTotalWritableLen();
            }

            return nLineWritableLen;
        }

        public bool Read(byte[] dstBytes, int nStartIndex, int nReadLen, bool bDoRead)
        {
            int nLineLen = 0;

            if (LOG_ERROR(dstBytes != null)) { goto Exit0; }
            if (LOG_ERROR(dstBytes.Length >= nStartIndex + nReadLen)) { goto Exit0; }
            if (LOG_ERROR(GetTotalReadableLen() >= nReadLen)) { goto Exit0; }

            nLineLen = m_nBufSize - m_nReadInx;
            if (nLineLen >= nReadLen)
            {
                Array.Copy(m_BufBytes, m_nReadInx, dstBytes, nStartIndex, nReadLen);
                if (bDoRead)
                {
                    m_nReadInx += nReadLen;
                }
            }
            else
            {
                Array.Copy(m_BufBytes, m_nReadInx, dstBytes, nStartIndex, nLineLen);
                Array.Copy(m_BufBytes, 0, dstBytes, nStartIndex + nLineLen, nReadLen - nLineLen);
                if (bDoRead)
                {
                    m_nReadInx = nReadLen - nLineLen;
                }
            }

            if (bDoRead)
            {
                _SubDataLen(nReadLen);
            }

            if (m_nReadInx == m_nBufSize)
            {
                m_nReadInx = 0;
            }

            return true;
        Exit0:
            return false;
        }

        public bool Write(byte[] srcBytes, int nStartIndex, int nWriteLen)
        {
            int nLineLen = 0;

            if (LOG_ERROR(srcBytes != null)) { goto Exit0; }
            if (LOG_ERROR(srcBytes.Length >= nWriteLen + nStartIndex)) { goto Exit0; }
            if (LOG_ERROR(GetTotalWritableLen() >= nWriteLen)) { goto Exit0; }

            nLineLen = m_nBufSize - m_nWriteInx;
            if (nLineLen >= nWriteLen)
            {
                Array.Copy(srcBytes, nStartIndex, m_BufBytes, m_nWriteInx, nWriteLen);
                m_nWriteInx += nWriteLen;
            }
            else
            {
                Array.Copy(srcBytes, nStartIndex, m_BufBytes, m_nWriteInx, nLineLen);
                Array.Copy(srcBytes, nStartIndex + nLineLen, m_BufBytes, 0, nWriteLen - nLineLen);
                m_nWriteInx = nWriteLen - nLineLen;
            }

            if (m_nWriteInx == m_nBufSize)
            {
                m_nWriteInx = 0;
            }

            _AddDataLen(nWriteLen);

            return true;
        Exit0:
            return false;
        }

        public void FinishRead(int nReadLen)
        {
            int nLineLen = 0;

            LOG_ERROR(nReadLen <= GetTotalReadableLen());

            if (nReadLen > GetTotalReadableLen())
            {
                nReadLen = GetTotalReadableLen();
            }

            nLineLen = m_nBufSize - m_nReadInx;
            if (nLineLen >= nReadLen)
            {
                m_nReadInx += nReadLen;
            }
            else
            {
                m_nReadInx = nReadLen - nLineLen;
            }

            if (m_nBufSize == m_nReadInx)
            {
                m_nReadInx = 0;
            }

            _SubDataLen(nReadLen);
        }

        public void FinishWrite(int nWriteLen)
        {
            int nLineLen = 0;

            LOG_ERROR(nWriteLen <= GetTotalWritableLen());

            if (nWriteLen > GetTotalWritableLen())
            {
                nWriteLen = GetTotalWritableLen();
            }

            nLineLen = m_nBufSize - m_nWriteInx;
            if (nLineLen >= nWriteLen)
            {
                m_nWriteInx += nWriteLen;
            }
            else
            {
                m_nWriteInx = nWriteLen - nLineLen;
            }

            if (m_nBufSize == m_nWriteInx)
            {
                m_nWriteInx = 0;
            }

            _AddDataLen(nWriteLen);
        }

        private void _Reset()
        {
            m_nReadInx = 0;
            m_nWriteInx = 0;
            _SetDataLen(0);
        }

        private int _GetDataLen()
        {
            return (int)Interlocked.Read(ref m_lDataLen);
        }

        private void _SetDataLen(int nLen)
        {
            Interlocked.Exchange(ref m_lDataLen, nLen);
        }

        private void _AddDataLen(int nLen)
        {
            Interlocked.Add(ref m_lDataLen, nLen);
        }

        private void _SubDataLen(int nLen)
        {
            Interlocked.Add(ref m_lDataLen, -nLen);
        }
    }
}
