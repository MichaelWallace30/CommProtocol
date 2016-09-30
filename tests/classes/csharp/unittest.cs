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
	/// <summary>
	/// Testing ABSPacket inheritence.
	/// </summary>
	public class Bing : ABSPacket 
	{
		public Bing() : base("Bing") {
		}

		public override ABSPacket Create() {
			throw new NotImplementedException();
		}

		public override void Pack(CObjectStream obj) {
		}

		public override void Unpack(CObjectStream obj) {
			throw new NotImplementedException();
		}
	}

	/// <summary>
	/// Testing the ABSPacket inheritance.
	/// </summary>
	public class Bong : ABSPacket {
		private int cat;


		public Bong(int cc) : base("Bong") {
			cat = cc;
		}

		public Bong() : this(0) {
		}

		public override ABSPacket Create() {
			return new Bong();
		}

		public override void Pack(CObjectStream obj) {
			obj.input(cat);
		}

		public override void Unpack(CObjectStream obj) {
			cat = obj.outputInt32();
		}

		public int GetCat() {
			return cat;
		}
	}


	class Comms : CCommNode {

		public Comms(uint dd) : base(dd) {

		}

		public override Boolean AddAddress(UInt16 destID, String address, UInt16 port) {
			throw new NotImplementedException();
		}

		public override Boolean InitConnection(TransportProtocol connType, String port, String address, UInt32 baudRate) {
			throw new NotImplementedException();
		}

		public override ABSPacket Receive(ref Byte sourceId) {
			throw new NotImplementedException();
		}

		public override Boolean RemoveAddress(UInt16 destID) {
			throw new NotImplementedException();
		}

		public override Boolean Send(ABSPacket packet, Byte destId) {
			throw new NotImplementedException();
		}
	}


	// Delegate function pointer testing.
	class Calls {
		public Int32 EatShit(CHeader header, ABSPacket packet) {
			Bong temp = ABSPacket.GetValue<Bong>(packet);
			Console.WriteLine("Hello Woild!");
			Console.WriteLine(temp.GetCat());
			return (Int32)CallBackCodes.CALLBACK_SUCCESS;
		}
	}

	public delegate Int32 cool(CHeader head, Bong a);

	class UnitTest {

		static void Main(string[] args) {
			CComms comm1 = new CComms(1);
			CComms comm2 = new CComms(2);
			bool success = false;
			Calls call = new Calls();
			CommFunct fun = new CommFunct(call.EatShit);
			comm1.InitConnection(TransportProtocol.UDP_LINK, "1337", "127.0.0.1", 0);
			comm2.InitConnection(TransportProtocol.UDP_LINK, "1338", "127.0.0.1", 0);

			success = comm1.AddAddress(2, "127.0.0.1", 1338);
			Console.WriteLine("comm1 connected: " + success);
			success = comm2.AddAddress(1, "127.0.0.1", 1337);
			Console.WriteLine("comm2 connected: " + success);

			success = comm2.LinkCallback(new Bong(), new CallBack(new Bong(), fun));
			Console.WriteLine("Did comm2 get linked? " + success);

			comm1.Run();
			comm2.Run();
            while (true)
            {
				// TODO(Garcia):
				// This needs to be taken care of soon as well.
				comm1.Send(new Bong(11), 2);
                System.Threading.Thread.Sleep(500);
            }
		}
	}
}
