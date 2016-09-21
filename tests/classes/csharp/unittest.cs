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

namespace CommProtoCSharpText {
	/// <summary>
	/// Testing ABSPacket inheritence.
	/// </summary>
	class Bing : ABSPacket 
	{
		public Bing() : base("Bing") {
		}

		public override ABSPacket Create() {
			throw new NotImplementedException();
		}

		public override void Pack(ObjectStreamWrapper obj) {
		}

		public override void Unpack(ObjectStreamWrapper obj) {
			throw new NotImplementedException();
		}
	}

	/// <summary>
	/// Testing the ABSPacket inheritance.
	/// </summary>
	class Bong : ABSPacket {
		private int cat;


		public Bong(int cc) : base("Bong") {
			cat = cc;
		}

		public Bong() : this(0) {
		}

		public override ABSPacket Create() {
			throw new NotImplementedException();
		}

		public override void Pack(ObjectStreamWrapper obj) {
			obj.input(cat);
		}

		public override void Unpack(ObjectStreamWrapper obj) {
			cat = obj.outputInt32();
		}

		public int GetCat() {
			return cat;
		}
	}

	// Delegate function pointer testing.
	class Calls {
		public Int32 EatShit(HeaderWrapper header, ABSPacket packet) {
			Bong temp = ABSPacket.GetValue<Bong>(packet);
			Console.WriteLine("Hello Woild!");
			Console.WriteLine(temp.GetCat());
			return (Int32)CallBackCodes.CALLBACK_SUCCESS;
		}
	}

	class UnitTest {

		static void Main(string[] args) {
			Calls caa = new Calls();
			CallBack call = new CallBack(new Bing(), caa.EatShit);

			ABSPacket packet = new Bong(5);
            CommsWrapper comms1 = new CommsWrapper(1);
            CommsWrapper comms2 = new CommsWrapper(2);
			ObjectStreamWrapper obj = new ObjectStreamWrapper();

			Bong g = new Bong(11);
			ABSPacket ab = g;
			Console.WriteLine(ABSPacket.GetValue<Bong>(ab).GetCat());
			ab.Pack(obj);

			Bong newBong = new Bong();
			newBong.Unpack(obj);

			Console.WriteLine(newBong.GetCat());
			// Testing the call function.
			call.CallFunction(new HeaderWrapper(), new Bong(111));
			comms1.initConnection(TransportProtocol.UDP_LINK, "1337", "127.0.0.1", 0);
            comms2.initConnection(TransportProtocol.UDP_LINK, "1338", "127.0.0.1", 0);

            comms1.addAddress(2,"127.0.0.1", 1338);
            comms1.addAddress(1, "127.0.0.1", 1337);

			comms1.run();
			comms2.run();

            while(true)
            {
				// TODO(Garcia):
				// This needs to be taken care of soon as well.
                comms1.send(2);
                System.Threading.Thread.Sleep(500);
            }
		}
	}
}
