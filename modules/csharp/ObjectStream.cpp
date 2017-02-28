/*
Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#include <ObjectStream.h>
#include <msclr/marshal_cppstd.h>
#include <string>

using namespace System;
using namespace Comnet;
using namespace Comnet::Serialization;

ObjectStream::ObjectStream()
{
  try {
   unmangedObjectStream = new CommPointer<comnet::serialization::ObjectStream>(new comnet::serialization::ObjectStream());
  } catch (System::Runtime::InteropServices::SEHException()) {
    Console::WriteLine("SEH ERROR");
  }
}

ObjectStream::ObjectStream(CommPointer<comnet::serialization::ObjectStream>* pointer) 
: unmangedObjectStream(new CommPointer<comnet::serialization::ObjectStream>(*pointer))
{
}

ObjectStream::~ObjectStream()
{
 delete unmangedObjectStream;
 unmangedObjectStream = NULL;
}

Int32 ObjectStream::GetPosition()
{
 return unmangedObjectStream->Get().GetPosition();
}

Int32 ObjectStream::GetSize()
{
 return unmangedObjectStream->Get().GetSize();
}

void ObjectStream::SetHeader(Header^ header)
{
  unmangedObjectStream->Get().header_packet.dest_id = header->GetDestID();
  unmangedObjectStream->Get().header_packet.source_id = header->GetSourceID();
  unmangedObjectStream->Get().header_packet.msg_len = header->GetMessageLength();
  unmangedObjectStream->Get().header_packet.msg_id = header->GetMessageID();
  for (int x = 0; x < KEY_LENGTH; x++)
  {
    unmangedObjectStream->Get().header_packet.iv[x] = header->GetIV()[x];
  }
}

void ObjectStream::SerializeHeader()
{
 unmangedObjectStream->Get().SerializeHeader();
}

Header^ ObjectStream::DeserializeHeader()
{
 unmangedObjectStream->Get().header_packet = unmangedObjectStream->Get().DeserializeHeader();
 Header^ header = gcnew Header();
 header->SetDestID(unmangedObjectStream->Get().header_packet.dest_id);
 header->SetSourceID(unmangedObjectStream->Get().header_packet.source_id);
 header->SetMessageLength(unmangedObjectStream->Get().header_packet.msg_len);
 header->SetMessageID(unmangedObjectStream->Get().header_packet.msg_id);
 array<Byte>^ IV = gcnew array<Byte>(KEY_LENGTH);
 for(int x = 0; x < KEY_LENGTH; x++)
 {
  IV[x] = unmangedObjectStream->Get().header_packet.iv[x];
 }
 header->SetIV(IV);
 return header;
}

//intput
void ObjectStream::Input(String^ data)
 {
 std::string value = msclr::interop::marshal_as<std::string>(data);
 unmangedObjectStream->Get() << value;
}
void ObjectStream::Input(Byte data)  { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(SByte data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(UInt16 data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Int16 data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Int32 data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(UInt32 data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Int64 data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(UInt64 data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Single data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Double data){ unmangedObjectStream->Get() << data; }

//Ooutput
void ObjectStream::Output(String^% data)
{
 std::string value;
  comnet::serialization::ObjectStream& obj = unmangedObjectStream->Get();
 obj >> value;
 data = gcnew String(value.c_str());	
}
void ObjectStream::Output(Byte% data) { unmangedObjectStream->Get() >> (uint8_t)data; }
void ObjectStream::Output(SByte% data) { unmangedObjectStream->Get() >> (int8_t)data; }
void ObjectStream::Output(UInt16% data){ unmangedObjectStream->Get() >> (uint16_t)data; }
void ObjectStream::Output(Int16% data)  { Int16 data;  unmangedObjectStream->Get() >> data; return data; }
void ObjectStream::Output(){ UInt32 data; unmangedObjectStream->Get() >> data; return data; }
void ObjectStream::Output()  { 
    Int32 data;  
    unmangedObjectStream->Get() >> data;
    return data; 
}
UInt64 ObjectStream::OutputUInt64(){ UInt64 data; unmangedObjectStream->Get() >> data; return data; }
Int64 ObjectStream::OutputInt64()  { Int64 data;  unmangedObjectStream->Get() >> data; return data; }
Single ObjectStream::OutputSingle(){ Single data; unmangedObjectStream->Get() >> data; return data; }
Double ObjectStream::OutputDouble(){ Double data; unmangedObjectStream->Get() >> data; return data; }


