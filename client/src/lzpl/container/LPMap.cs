using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lzpl
{
    public class LPMapNode : LPError
    {
        public int Index { get; set; }
        public object Data { get; set; }

        public LPMapNode(object data, int index)
        {
            Index = index;
            Data = data;
        }
    }

    public class LPMap : LPError
    {
        public int Capacity { get { return m_Capacity; } }
        public int Count { get { return m_Count; } }

        private int m_Capacity = 0;
        private int m_Count = 0;


        public LPMap(int capacity)
        {

        }

        public LPMapNode Insert(object data)
        {

            return null;
        }

        public object Elem(int index)
        {
            return null;
        }

        public LPMapNode Begin()
        {
            return null;
        }

        public LPMapNode End()
        {
            return null;
        }

        public LPMapNode RBegin()
        {
            return null;
        }

        public LPMapNode REnd()
        {
            return null;
        }
        public LPMapNode Next(LPMapNode node)
        {
            return null;
        }
    }
}
