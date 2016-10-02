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

	class Calls {
		public int PingCallback(Header header, ABSPacket packet) {
			Ping ping = ABSPacket.GetValue<Ping>(packet);
			Console.WriteLine("Ping message recieved: " + ping.GetMessage());
			return (Int32)CallBackCodes.CALLBACK_SUCCESS;
		}
	}

	class UnitTest {
		static void Main(string[] args) {
			Calls call = new Calls();
			CommNode comm1 = new Comms(1);

			comm1.LinkCallback(new Ping(), new CallBack(new Ping(), call.PingCallback));
			comm1.InitConnection(TransportProtocol.UDP_LINK, "1337", "192.168.1.106", 0);
			comm1.AddAddress(2, "10.0.2.15", 1338);
			
			comm1.Run();

			while (true) {
				System.Threading.Thread.Sleep(1000);
			}
		}
	}
}
