using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoCSharp
{
    public class TestScript: MonoScript
    {
        public int numTest;
        public float valueTest;
        public double valueTest2;
        public string strTest;
        public decimal valueTest3;

        public void Update()
        {
            Console.WriteLine("TestScript - Update!");
        }
    }
}
