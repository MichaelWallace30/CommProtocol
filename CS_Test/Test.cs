using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CS_Test
{
  class Test
  {
    public static void Main()
    {
      Console.WriteLine("hi");
      Comnet.Ping.Pinger pinger = new Comnet.Ping.Pinger(1);
      Console.ReadKey();
    }
  }
}
