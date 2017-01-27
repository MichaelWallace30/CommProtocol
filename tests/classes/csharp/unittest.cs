/*
 C# Wrapper testing sample. Be sure to create your own Project sln for visual studio.
 Don't forget to add the reference of CPCSharpWrapper.dll library when running this.
 

 Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
  
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
  
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Comnet;
using Comnet.Serialization;
using Comnet.Network;
using Comnet.Tools.DataStructures.Interface;
using Comnet.Tools.DataStructures;

namespace CommProtoCSharpText {
 public class Ping : ABSPacket {
  private string message;

  public Ping() : base("Ping") {
  }

  public Ping(string mess) : base("Ping") {
   message = mess;
  }

  public override ABSPacket Create() {
   return new Ping();
  }

  public override void Pack(ObjectStream obj) {
   obj.Input(message);
  }

  public override void Unpack(ObjectStream obj) {
   message = obj.OutputString();
  }

  public string GetMessage() {
   return message;
  }
 }

 class TargetDesignationCommand : ABSPacket {
  public TargetDesignationCommand() : base("TargetDesignationCommand") {
  }

  public TargetDesignationCommand(float x, float y, float z) : base("TargetDesignationCommand") {
   this.x = x; this.y = y; this.z = z;
  }

  public override ABSPacket Create() {
   return new TargetDesignationCommand();
  }

  public override void Pack(ObjectStream obj) {
   obj.Input(x);
   obj.Input(y);
   obj.Input(z);
  }

  public override void Unpack(ObjectStream obj) {
   z = obj.OutputSingle();
   y = obj.OutputSingle();
   x = obj.OutputSingle();
  }

  public float x;
  public float y;
  public float z;
 }

 class Calls {
  public int PingCallback(Header header, ABSPacket packet, CommNode node) {
   Ping ping = ABSPacket.GetValue<Ping>(packet);
   Console.WriteLine("Ping message recieved: " + ping.GetMessage());
   Ping pins = new Ping("C# got dat message mate...");
   return (Int32)(CallBackCodes.CALLBACK_SUCCESS | CallBackCodes.CALLBACK_DESTROY_PACKET);
  }

  public int TargetCallback(Header header, ABSPacket packet, CommNode node) {
   TargetDesignationCommand comm = ABSPacket.GetValue<TargetDesignationCommand>(packet);
   Console.WriteLine("I got.  X: {0}\tY: {1}\tZ: {2}", comm.x, comm.y, comm.z);
   return (Int32)CallBackCodes.CALLBACK_SUCCESS;
  }
 }

 class UnitTest {
  static void Main(string[] args) {
   Calls call = new Calls();
   CommNode comm1 = new Comms(1);
   CommNode comm2 = new Comms(2);

   comm1.LinkCallback(new Ping(), new CallBack(call.PingCallback));
   comm2.LinkCallback(new Ping(), new CallBack(call.PingCallback));
   
   comm1.InitConnection(TransportProtocol.ZIGBEE_LINK, "COM5", "", 57600);
   comm2.InitConnection(TransportProtocol.ZIGBEE_LINK, "COM6", "", 57600);
   Console.WriteLine(comm1.AddAddress(2, "0013A2004067E4AE", 0));
   Console.WriteLine(comm2.AddAddress(1, "0013A20040917A31", 0));
   //comm1.InitConnection(TransportProtocol.UDP_LINK, "1337", "127.0.0.1", 57600);
   //comm2.InitConnection(TransportProtocol.UDP_LINK, "1338", "127.0.0.1", 57600);
   //Console.WriteLine(comm1.AddAddress(2, "127.0.0.1", 1338));
   //Console.WriteLine(comm2.AddAddress(1, "127.0.0.1", 1337));
   //Ping ping = new Ping("I am from C#!");
   comm1.Run();
   comm2.Run();
   Ping ping = new Ping("From C#, From the xbee!!");
   //TargetDesignationCommand command = new TargetDesignationCommand(45.34f, 23.22f, 234.44f);
   while (true) {
    comm1.Send(ping, 2);
    //comm1.Send(command, 1);
    System.Threading.Thread.Sleep(1000);
   }
  comm1.Stop(); //This call is essential to stop threads because finalizer may not be called.
  comm2.Stop();
  }
 }
}
