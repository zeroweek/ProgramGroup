using System;

namespace lzpl
{
    public class BASE_LIST_NODE : LPError
    {
        public static void Clear(BASE_LIST_NODE node)
        {
            if(node != null)
            {
                node.Data = null;
                node.Prev = null;
                node.Next = null;
            }
        }

        public object Data { get; set; }
        public BASE_LIST_NODE Prev { get; set; }
        public BASE_LIST_NODE Next { get; set; }

        public BASE_LIST_NODE()
        {
            
        }
    }

    public class LPBaseList : LPError
    {
        private int m_Size;
        private BASE_LIST_NODE m_Head = new BASE_LIST_NODE();
        private BASE_LIST_NODE m_Rear = new BASE_LIST_NODE();

        public int Size { get { return m_Size; } }
        public BASE_LIST_NODE Head { get { return m_Head; } }
        public BASE_LIST_NODE Rear { get { return m_Rear; } }

        public LPBaseList()
        {
            Clear();
        }

        public void Clear()
        {
            m_Size = 0;
            m_Head.Data = null;
            m_Head.Prev = null;
            m_Head.Next = m_Rear;
            m_Rear.Data = null;
            m_Rear.Prev = m_Head;
            m_Rear.Next = null;
        }

        public bool IsEmpty()
        {
            if (m_Head.Next == m_Rear)
            {
                LOG_ERROR(m_Rear.Prev == m_Head);
            }
            if (m_Rear.Prev == m_Head)
            {
                LOG_ERROR(m_Head.Next == m_Rear);
            }

            return (m_Head.Next == m_Rear);
        }

        public bool PushHead(object data)
        {
            BASE_LIST_NODE node = null;

            node = new BASE_LIST_NODE();
            if (LOG_ERROR(node != null)) { goto Exit0; }
            if (LOG_ERROR(node.Prev == null && node.Next == null)) { goto Exit0; }

            node.Data = data;

            node.Prev = m_Head;
            node.Next = m_Head.Next;

            node.Next.Prev = node;
            m_Head.Next = node;
            ++m_Size;

            return true;
        Exit0:
            return false;
        }

        public BASE_LIST_NODE PopHead()
        {
            BASE_LIST_NODE result = null;

            if (LOG_ERROR(m_Head.Next != m_Rear)) { goto Exit0; }

            result = m_Head.Next;
            m_Head.Next = result.Next;
            result.Next.Prev = m_Head;

            result.Prev = null;
            result.Next = null;
            --m_Size;

            return result;
        Exit0:
            return null;
        }

        public bool PushRear(object data)
        {
            BASE_LIST_NODE node = null;

            node = new BASE_LIST_NODE();
            if (LOG_ERROR(node != null)) { goto Exit0; }
            if (LOG_ERROR(node.Prev == null && node.Next == null)) { goto Exit0; }

            node.Data = data;

            node.Prev = m_Rear.Prev;
            node.Next = m_Rear;

            m_Rear.Prev = node;
            node.Prev.Next = node;
            ++m_Size;

            return true;
        Exit0:
            return false;
        }

        public BASE_LIST_NODE PopRear()
        {
            BASE_LIST_NODE result = null;

            if (LOG_ERROR(m_Rear.Prev != m_Head)) { goto Exit0; }

            result = m_Rear.Prev;
            m_Rear.Prev = result.Prev;
            result.Prev.Next = m_Rear;

            result.Prev = null;
            result.Next = null;
            --m_Size;

            return result;
        Exit0:
            return null;
        }

        public bool InsertAfter(BASE_LIST_NODE target, BASE_LIST_NODE node)
        {
            if (LOG_ERROR(target != null)) { goto Exit0; }
            if (LOG_ERROR(target.Next != null)) { goto Exit0; }
            if (LOG_ERROR(node != null)) { goto Exit0; }
            if (LOG_ERROR(node.Prev == null && node.Next == null)) { goto Exit0; }

            node.Prev = target;
            node.Next = target.Next;
            target.Next.Prev = node;
            target.Next = node;
            ++m_Size;

            return true;
        Exit0:
            return false;
        }

        public bool InsertBefore(BASE_LIST_NODE target, BASE_LIST_NODE node)
        {
            if (LOG_ERROR(target != null)) { goto Exit0; }
            if (LOG_ERROR(target.Next != null)) { goto Exit0; }
            if (LOG_ERROR(node != null)) { goto Exit0; }
            if (LOG_ERROR(node.Prev == null && node.Next == null)) { goto Exit0; }

            node.Next = target;
            node.Prev = target.Prev;
            target.Prev.Next = node;
            target.Prev = node;
            ++m_Size;

            return true;
        Exit0:
            return false;
        }

        public bool Remove(BASE_LIST_NODE node)
        {
            if (LOG_ERROR(node != null)) { goto Exit0; }
            if (LOG_ERROR(node.Prev != null && node.Next != null)) { goto Exit0; }

            node.Prev.Next = node.Next;
            node.Next.Prev = node.Prev;

            node.Prev = null;
            node.Next = null;
            --m_Size;

            return true;
        Exit0:
            return false;
        }
    }
}
