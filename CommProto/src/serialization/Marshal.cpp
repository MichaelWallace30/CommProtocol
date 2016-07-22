/*
Serial configurations.

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

#include <CommProto/serialization/Marshal.h>
#include <CommProto/console/CommsDebug.h>

#include <string.h>
#include <string>
#include <typeinfo>

#include <iostream>

namespace Comnet {
namespace Serialization {
	

	uint32_t packString(string_t data, uint8_t len, marshall_t input)
	{			
		//+1 for null termination
		memcpy(input, data, len + 1);
		//pack length at the end use length out side of this function
		memcpy(input + len + 1, &len, sizeof(uint8_t));
		return len + 1 + sizeof(uint8_t);		
	}


	uint32_t unpackString(string_t data, uint8_t len, marshall_t input)
	{			
		memcpy(data, input, len +1);		
		return len;
	}


	uint32_t packWideString(std::wstring &data, uint8_t len, marshall_t input)
	{
		uint32_t temp[512];
		for (int i = 0; i < len; ++i) {
		  temp[i] = (uint32_t)swap_endian_copy<wchar_t>(data[i]);
		  COMMS_DEBUG("Output: %d\n", temp[i]);
		}

		temp[len] = (uint32_t)'\0';

		memcpy(input, temp, (len * sizeof(uint32_t)));
		memcpy(input + (len * sizeof(uint32_t)) + 1, &len, sizeof(uint8_t));

		return (len * sizeof(wchar_t)) + 1 + sizeof(uint8_t);
	}


	uint32_t unpackWideString(std::wstring &data, uint8_t len, marshall_t input)
	{		
		uint32_t temp[512];
		memcpy(temp, input, (len * sizeof(uint32_t)) + sizeof(uint32_t));
		COMMS_DEBUG("NOW UNPACKING, SIZE: %d\n", len);
	       
		for (int x = 0; x < len; ++x)
		{
			data[x] = (wchar_t)swap_endian_copy<wchar_t>((uint32_t)temp[x]);
			std::wcout << (wchar_t)data[x] << std::endl;
		}
		
		COMMS_DEBUG("Fully out!!\n");
		data[len] = '\0';
		
		return len;
	}

	uint32_t packByte(uint8_t data, marshall_t input) 
	{
		memcpy(input, &data, 1);
		return sizeof(uint8_t);
	}

	uint8_t unpackByte(marshall_t input)
	{
		uint8_t value;
		memcpy(&value, input, 1);
		return value;
	}

	uint32_t packUint16(uint16_t data, marshall_t input)
	{
		swap_endian(data);
		memcpy(input, &data, sizeof(uint16_t));
		return sizeof(uint16_t);
	}

	uint16_t unpackUint16(marshall_t input)
	{
		uint16_t value;
		memcpy(&value, input, sizeof(uint16_t));
		swap_endian(value); 
		return value;
	}

	uint32_t packInt16(int16_t data, marshall_t input)
	{		
		swap_endian(data);
		memcpy(input, &data, sizeof(int16_t));
		return sizeof(int16_t);		
	}

	int16_t unpackInt16(marshall_t input)
	{
		int16_t value;
		memcpy(&value, input, sizeof(int16_t));
		swap_endian(value);
		return value;
	}

	uint32_t packUint32(uint32_t data, marshall_t input)
	{
		swap_endian(data);
		memcpy(input, &data, sizeof(uint32_t));
		return sizeof(uint32_t);
	}

	uint32_t unpackUint32(marshall_t input)
	{
		uint32_t value;
		memcpy(&value, input, sizeof(uint32_t));
		swap_endian(value);
		return value;
	}

	uint32_t packInt32(int32_t data, marshall_t input)
	{
		swap_endian(data);
		memcpy(input, &data, sizeof(int32_t));
		return sizeof(int32_t);
	}

	int32_t unpackInt32(marshall_t input)
	{
		int32_t value;
		memcpy(&value, input, sizeof(int32_t));
		swap_endian(value);
		return value;
	}

	uint32_t packUint64(uint64_t data, marshall_t input)
	{
		swap_endian(data);
		memcpy(input, &data, sizeof(uint64_t));
		return sizeof(uint64_t);
	}

	uint64_t unpackUint64(marshall_t input)
	{
		uint64_t value;
		memcpy(&value, input, 2);
		swap_endian(value);
		return value;
	}

	uint32_t packInt64(int64_t data, marshall_t input)
	{
		swap_endian(data);
		memcpy(input, &data, sizeof(int64_t));
		return sizeof(int64_t);
	}

	int64_t unpackInt64(marshall_t input)
	{
		int64_t value;
		memcpy(&value, input, sizeof(int64_t));
		swap_endian(value);
		return value;
	}

	uint32_t packReal32(real32_t data, marshall_t input)
	{
		swap_endian(data);
		memcpy(input, &data, sizeof(real32_t));
		return sizeof(real32_t);
	}

	real32_t unpackReal32(marshall_t input)
	{
		real32_t value;
		memcpy(&value, input, sizeof(real32_t));
		swap_endian(value);
		return value;
	}

	uint32_t packReal64(real64_t data, marshall_t input)
	{
		swap_endian(data);
		memcpy(input, &data, sizeof(real64_t));
		return sizeof(real64_t);
	}
	real64_t unpackReal64(marshall_t input)
	{
		real64_t value;
		memcpy(&value, input, sizeof(real64_t));
		swap_endian(value);
		return value;
	}

} // Serialization namespace
} // Comnet namespace
