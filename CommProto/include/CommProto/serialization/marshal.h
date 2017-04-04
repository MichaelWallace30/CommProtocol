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

#ifndef __MARSHAL_H
#define __MARSHAL_H

#define LITTLE_ENDIAN_COMNET

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>//str_length()
#include <string>//wstring
#include <algorithm>//reverse
#include <cstdlib>//malloc, free, mbtowcs, wcstombs

namespace comnet {
namespace serialization {

	typedef uint8_t* marshal_t;
	typedef char* string_t;

	uint32_t PackString(string_t data, uint8_t len, marshal_t input);
	uint32_t UnpackString(string_t data, uint8_t len, marshal_t input);

	uint32_t PackWideString(const std::wstring  &data, uint8_t len, marshal_t input);
	uint32_t UnpackWideString(std::wstring &data, uint8_t len, marshal_t input);

	uint32_t PackByte(uint8_t data, marshal_t input);
	uint8_t UnpackByte(marshal_t input);

	uint32_t PackUint16(uint16_t data, marshal_t input);
	uint16_t UnpackUint16(marshal_t input);

	uint32_t PackInt16(int16_t data, marshal_t input);
	int16_t UnpackInt16(marshal_t input);

	uint32_t PackUint32(uint32_t data, marshal_t input);
	uint32_t UnpackUint32(marshal_t input);

	uint32_t PackInt32(int32_t data, marshal_t input);
	int32_t UnpackInt32(marshal_t input);

	uint32_t PackUint64(uint64_t data, marshal_t input);
	uint64_t UnpackUint64(marshal_t input);

	uint32_t PackInt64(int64_t data, marshal_t input);
	int64_t UnpackInt64(marshal_t input);

	uint32_t PackReal32(real32_t data, marshal_t input);
	real32_t UnpackReal32(marshal_t input);

	uint32_t PackReal64(real64_t data, marshal_t input);
	real64_t UnpackReal64(marshal_t input);

	template <typename T>
	void SwapEndian(T& pX)
	{
	#ifdef LITTLE_ENDIAN_COMNET
		// should static assert that T is a POD
		char& raw = reinterpret_cast<char&>(pX);
		std::reverse(&raw, &raw + sizeof(T));
	#endif
	}

	template <typename T>
	T SwapEndianCopy(T pX)
	{
		SwapEndian(pX);
		return pX;
	}

	template <typename T>
	int16_t PackGeneric(T data, marshal_t input)
	{
		SwapEndian(data);
		memcpy(input, &data, sizeof(T));
		return sizeof(T);	
	}

	template <typename T>
	T UnpackGeneric(marshal_t input)
	{
		T value;
		memcpy(&value, input, sizeof(T));
		SwapEndian(value);
		return value;
	}

} // Serialization namespace
} // Comnet namespace
#endif // __MARSHAL_H
