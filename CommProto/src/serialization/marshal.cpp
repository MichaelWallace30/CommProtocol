/*
Serial configurations.

Copyright (C) 2016  Michael Wallace, Mario Garcia.

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

#include <CommProto/serialization/marshal.h>
#include <CommProto/debug/comms_debug.h>

#include <string.h>
#include <string>
#include <typeinfo>

#include <iostream>

namespace comnet {
namespace serialization {

	uint32_t PackString(string_t data, uint8_t len, marshal_t input)
	{
		//+1 for null termination
		memcpy(input, data, len + 1);
		//Pack length At the End use length out side of this function
		memcpy(input + len + 1, &len, sizeof(uint8_t));
		return len + 1 + sizeof(uint8_t);
	}


	uint32_t UnpackString(string_t data, uint8_t len, marshal_t input)
	{
		memcpy(data, input, len +1);
		return len;
	}


	uint32_t PackWideString(const std::wstring &data, uint8_t len, marshal_t input)
	{
		string_t temp = (string_t)malloc(len);
		const wchar_t* wtemp = data.c_str();
		wcstombs(temp, wtemp, len);
		int newLen = PackString(temp, len, input);
		free(temp);
		
		return newLen;
	}


	uint32_t UnpackWideString(std::wstring &data, uint8_t len, marshal_t input)
	{
		string_t tempMB = (string_t)malloc(len);
		int newLen = UnpackString(tempMB, len, input);
		wchar_t tempW[512];
		mbstowcs(tempW, tempMB, len);
		free(tempMB);

		data = tempW;
		
		return newLen;
	}

	uint32_t PackByte(uint8_t data, marshal_t input)
	{
		memcpy(input, &data, 1);
		return sizeof(uint8_t);
	}

	uint8_t UnpackByte(marshal_t input)
	{
		uint8_t value;
		memcpy(&value, input, 1);
		return value;
	}

	uint32_t PackUint16(uint16_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(uint16_t));
		return sizeof(uint16_t);
	}

	uint16_t UnpackUint16(marshal_t input)
	{
		uint16_t value;
		memcpy(&value, input, sizeof(uint16_t));
		SwapEndian(value);
		return value;
	}

	uint32_t PackInt16(int16_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(int16_t));
		return sizeof(int16_t);
	}

	int16_t UnpackInt16(marshal_t input)
	{
		int16_t value;
		memcpy(&value, input, sizeof(int16_t));
		SwapEndian(value);
		return value;
	}

	uint32_t PackUint32(uint32_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(uint32_t));
		return sizeof(uint32_t);
	}

	uint32_t UnpackUint32(marshal_t input)
	{
		uint32_t value;
		memcpy(&value, input, sizeof(uint32_t));
		SwapEndian(value);
		return value;
	}

	uint32_t PackInt32(int32_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(int32_t));
		return sizeof(int32_t);
	}

	int32_t UnpackInt32(marshal_t input)
	{
		int32_t value;
		memcpy(&value, input, sizeof(int32_t));
		SwapEndian(value);
		return value;
	}

	uint32_t PackUint64(uint64_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(uint64_t));
		return sizeof(uint64_t);
	}

	uint64_t UnpackUint64(marshal_t input)
	{
		uint64_t value;
		memcpy(&value, input, sizeof(uint64_t));
		SwapEndian(value);
		return value;
	}

	uint32_t PackInt64(int64_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(int64_t));
		return sizeof(int64_t);
	}

	int64_t UnpackInt64(marshal_t input)
	{
		int64_t value;
		memcpy(&value, input, sizeof(int64_t));
		SwapEndian(value);
		return value;
	}

	uint32_t PackReal32(real32_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(real32_t));
		return sizeof(real32_t);
	}

	real32_t UnpackReal32(marshal_t input)
	{
		real32_t value;
		memcpy(&value, input, sizeof(real32_t));
		SwapEndian(value);
		return value;
	}

	uint32_t PackReal64(real64_t data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(real64_t));
		return sizeof(real64_t);
	}
	real64_t UnpackReal64(marshal_t input)
	{
		real64_t value;
		memcpy(&value, input, sizeof(real64_t));
		SwapEndian(value);
		return value;
	}

} // Serialization namespace
} // Comnet namespace
