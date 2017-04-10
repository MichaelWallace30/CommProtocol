/*
Abstract class that can be serialized by an ObjectStream

Copyright (C) 2016  Michael Wallace, Mario Garcia, Alex Craig.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(At your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __OBJ_SERIALIZABLE_H
#define __OBJ_SERIALIZABLE_H

#define INHERITS_OBJSERIALIZABLE public comnet::serialization::ObjSerializable
#define CHAIN_OBJSERIALIZABLE comnet::serialization::ObjSerializable()

#include <CommProto/architecture/api.h>

namespace comnet {
	namespace serialization {

		class ObjectStream;

		/**
		Class that can be serialized by ObjectStream
		*/
		class COMM_EXPORT ObjSerializable {
		public:
			//Used to serialize objects to stream (equivalent to ABSPacket::Pack)
			virtual void Input(ObjectStream& obj) const = 0;

			//Used to parse objects from stream (equivalent to ABSPacket::Unpack)
			virtual void Output(ObjectStream& obj) = 0;
		};
	}
}
#endif
