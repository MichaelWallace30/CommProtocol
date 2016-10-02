/*
  This file Contains the NGCP researched hash implementation, so as to Create
  discrete numbers from data, which will then be used to store and separate 
  this data from others. It attempts to avoid the most collisions possible,
  so as to prevent any conflicts with data handling and storage. This is still
  an ongoing research, and we will hope to continue working on improving this 
  hash for the long Run.

  Non-cryptographic hash algorithm.

  Copyright (C) 2016  Mario Garcia.

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
#ifndef __NGCPHASH_H
#define __NGCPHASH_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>


#define HASH_BIT16                0X10
#define HASH_BIT32                0x20
#define HASH_BIT64                0x40
#define HASH_BIT128               0x80

#define HASH_SEED                 1
#define HASH_VERSION              1

namespace comnet {
namespace hash {

#if HASH_VERSION > 0

#define NGCP_PUBLIC_API

typedef uint32_t ngcp_hash32_t;
typedef uint64_t ngcp_hash64_t;
typedef unsigned char byte;


/**************************************************************************************
 Public hash APIs used for hashing.
***************************************************************************************/
_COMNET_PUBLIC_API_ ngcp_hash32_t ngcp_hash32(void* input, uint32_t length, unsigned seed = 0);
_COMNET_PUBLIC_API_ ngcp_hash64_t ngcp_hash64(void* input, uint32_t length, unsigned seed = 0);


/*************************************************************************************
 Overloaded APIs used for specialization.
**************************************************************************************/
_COMNET_PUBLIC_API_ ngcp_hash32_t ngcp_hash32(const char* input, unsigned seed = 0);
_COMNET_PUBLIC_API_ ngcp_hash64_t ngcp_hash64(const char* input, unsigned seed = 0);

_COMNET_PUBLIC_API_ ngcp_hash32_t ngcp_hash32(unsigned int input, unsigned seed = 0);
_COMNET_PUBLIC_API_ ngcp_hash64_t ngcp_hash64(unsigned int input, unsigned seed = 0);


_COMNET_PUBLIC_API_ ngcp_hash32_t ngcp_hash32(int input, unsigned seed = 0);
_COMNET_PUBLIC_API_ ngcp_hash64_t ngcp_hash64(int input, unsigned seed = 0);


_COMNET_PUBLIC_API_ ngcp_hash32_t ngcp_hash32(unsigned long long input, unsigned seed = 0);
_COMNET_PUBLIC_API_ ngcp_hash64_t ngcp_hash64(unsigned long long input, unsigned seed = 0);


_COMNET_PUBLIC_API_ ngcp_hash32_t ngcp_hash32(float input, unsigned seed = 0);
_COMNET_PUBLIC_API_ ngcp_hash64_t ngcp_hash64(float input, unsigned seed = 0);


_COMNET_PUBLIC_API_ ngcp_hash32_t ngcp_hash32(double input, unsigned seed = 0);
_COMNET_PUBLIC_API_ ngcp_hash64_t ngcp_hash64(double input, unsigned seed = 0);




#undef NGCP_PUBLIC_API

#else
 #error "This hash version is no longer supported. Consider upgrading to prevent possible, incorrect hashing."

#endif // HASH_VERSION > 0
} // Hash namespace 
} // Comnet namespace 
#endif // __NGCPHASH_H
