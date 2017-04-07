/*
Copyright (C) 2016  Michael Wallace, Mario Garcia, Alex Craig.

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

#pragma once
#include <CommProto/serialization/objectstream.h>
#include <CommProto/tools/allocator/commpointer.h>
#include "ObjectStream.h"

using namespace System;
using namespace System::Collections;
using namespace comnet::tools::allocator;

namespace Comnet {
	namespace Serialization {
		/**
		Class that can be serialized by ObjectStream
		*/
		public ref class ObjSerializable : public IComparable<ObjSerializable^>
		{
		public:
			static UInt64 ObjIDCounter = 0;
			//Sets the id of this
			ObjSerializable() {
				id = ObjIDCounter++;
			}

			//Used to serialize objects to stream (equivalent to ABSPacket::Pack)
			virtual void Input(ObjectStream^ obj) = 0;
			
			//Used to parse objects from stream (equivalent to ABSPacket::Unpack)
			virtual void Output(ObjectStream^ obj) = 0;

			//Return a new instance of a derived class
			virtual ObjSerializable^ Create() = 0;

			virtual int CompareTo(ObjSerializable^ obj) {
				return id - obj->id;
			}

			//Uniquely identifies each object since pointer values cannot be used
			UInt64 id;
		};

		/*
		Below are:
			Int16Ser
			Int32Ser
			Int64Ser

			ByteSer
			SByteSer

			StringSer

			UInt16Ser
			UInt32Ser
			UInt64Ser

			They are used when storing primitives inside of containers due to 
			C++/CLI not supporting templates. Each can be implicitly casted from
			the data type preceding "Ser"
		*/
		public ref class Int16Ser : public ObjSerializable
		{
		public:
			Int16Ser()
				:ObjSerializable()
			{
				this->val = val;
			}
			Int16Ser(Int16 val) 
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator Int16(Int16Ser^ obj) {
				return obj->val;
			}

			static operator Int16Ser ^ (Int16 obj) {
				return gcnew Int16Ser(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew Int16Ser();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((Int16Ser^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			Int16 val;
		};

		public ref class Int32Ser : public ObjSerializable
		{
		public:
			Int32Ser()
				:ObjSerializable()
			{
				this->val = val;
			}
			Int32Ser(Int32 val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator Int32(Int32Ser^ obj) {
				return obj->val;
			}

			static operator Int32Ser ^ (Int32 obj) {
				return gcnew Int32Ser(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew Int32Ser();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((Int32Ser^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			Int32 val;
		};

		public ref class Int64Ser : public ObjSerializable
		{
		public:
			Int64Ser()
				:ObjSerializable()
			{
				this->val = val;
			}
			Int64Ser(Int64 val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator Int64(Int64Ser^ obj) {
				return obj->val;
			}

			static operator Int64Ser ^ (Int64 obj) {
				return gcnew Int64Ser(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew Int64Ser();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((Int64Ser^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			Int64 val;
		};

		public ref class SingleSer : public ObjSerializable
		{
		public:
			SingleSer()
				:ObjSerializable()
			{
				this->val = val;
			}
			SingleSer(Single val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator Single(SingleSer^ obj) {
				return obj->val;
			}

			static operator SingleSer ^ (Single obj) {
				return gcnew SingleSer(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew SingleSer();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((SingleSer^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			Single val;
		};

		public ref class DoubleSer : public ObjSerializable
		{
		public:
			DoubleSer()
				:ObjSerializable()
			{
				this->val = val;
			}
			DoubleSer(Double val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator Double(DoubleSer^ obj) {
				return obj->val;
			}

			static operator DoubleSer ^ (Double obj) {
				return gcnew DoubleSer(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew DoubleSer();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((DoubleSer^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			Double val;
		};

		public ref class ByteSer : public ObjSerializable
		{
		public:
			ByteSer()
				:ObjSerializable()
			{
				this->val = val;
			}
			ByteSer(Byte val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator Byte(ByteSer^ obj) {
				return obj->val;
			}

			static operator ByteSer ^ (Byte obj) {
				return gcnew ByteSer(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew ByteSer();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((ByteSer^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			Byte val;
		};

		public ref class SByteSer : public ObjSerializable
		{
		public:
			SByteSer()
				:ObjSerializable()
			{
				this->val = val;
			}
			SByteSer(SByte val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator SByte(SByteSer^ obj) {
				return obj->val;
			}

			static operator SByteSer ^ (SByte obj) {
				return gcnew SByteSer(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew SByteSer();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((SByteSer^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			SByte val;
		};

		public ref class UInt16Ser : public ObjSerializable
		{
		public:
			UInt16Ser()
				:ObjSerializable()
			{
				this->val = val;
			}
			UInt16Ser(UInt16 val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator UInt16(UInt16Ser^ obj) {
				return obj->val;
			}

			static operator UInt16Ser ^ (UInt16 obj) {
				return gcnew UInt16Ser(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew UInt16Ser();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((UInt16Ser^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			UInt16 val;
		};

		public ref class UInt32Ser : public ObjSerializable
		{
		public:
			UInt32Ser()
				:ObjSerializable()
			{
				this->val = val;
			}
			UInt32Ser(UInt32 val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator UInt32(UInt32Ser^ obj) {
				return obj->val;
			}

			static operator UInt32Ser ^ (UInt32 obj) {
				return gcnew UInt32Ser(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew UInt32Ser();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((UInt32Ser^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			UInt32 val;
		};

		public ref class UInt64Ser : public ObjSerializable
		{
		public:
			UInt64Ser()
				:ObjSerializable()
			{
				this->val = val;
			}
			UInt64Ser(UInt64 val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator UInt64(UInt64Ser^ obj) {
				return obj->val;
			}

			static operator UInt64Ser ^ (UInt64 obj) {
				return gcnew UInt64Ser(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew UInt64Ser();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val.CompareTo(((UInt64Ser^)other)->val);
			}

			String^ ToString() override {
				return val.ToString();
			}

			UInt64 val;
		};

		public ref class StringSer : public ObjSerializable
		{
		public:
			StringSer()
				:ObjSerializable()
			{
				this->val = val;
			}
			StringSer(String^ val)
				:ObjSerializable()
			{
				this->val = val;
			}

			static operator String^(StringSer^ obj) {
				return obj->val;
			}

			static operator StringSer ^ (String^ obj) {
				return gcnew StringSer(obj);
			}

			virtual void Input(ObjectStream^ obj) override {
				obj->Input(val);
			}

			virtual void Output(ObjectStream^ obj) override {
				obj->Output(val);
			}

			virtual ObjSerializable^ Create() override {
				return gcnew StringSer();
			}

			int CompareTo(ObjSerializable^ other) override {
				return val->CompareTo(((StringSer^)other)->val);
			}

			String^ ToString() override {
				return val->ToString();
			}

			String^ val;
		};
	}
}