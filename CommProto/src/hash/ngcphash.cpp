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
#include <CommProto/hash/ngcphash.h>

#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/os/arch.h>

#define NGCP_PRIVATE_API


namespace comnet {
namespace hash {

/**
   Reverse the byte binary values.
*/
FORCE_INLINE
ngcp_hash32_t reverse_byte(byte* _byte) {
  ngcp_hash32_t hash = 0;
  hash |= (*_byte & 0x80) >> 7;
  hash |= (*_byte & 0x40) >> 5;
  hash |= (*_byte & 0x20) >> 3;
  hash |= (*_byte & 0x10) >> 1;
  hash |= (*_byte & 0x8) << 1;
  hash |= (*_byte & 0x4) << 3;
  hash |= (*_byte & 0x2) << 5;
  hash |= (*_byte & 0x1) << 7;
  return hash;
}

/**
   Check if the pointer is null.
 */
FORCE_INLINE
bool ValueIsNull(void* input) {
  if (input == NULL) {
    return true;
  } else {
    return false;
  }
}

/**
   Hash experimental. Uses researched, custom, non-cryptographic hashing, with a bit of 
   Jenkins avalanche technique. Credit attributed to Jenkins hash. 
   NOTE(): Seed still needs to be implemented...
*/
ngcp_hash32_t ngcp_hash32(void* input, uint32_t length, unsigned seed) {
  ngcp_hash32_t hash = 0;
  if (ValueIsNull(input)) {
    return hash;
  }
  
 unsigned int i = 0;
 byte* p = (byte*)input;
 hash = reverse_byte((p+0));

 while (i < length) {
   hash += *(p+i) << 7 | (hash >> 15);
   hash ^= (hash >> 6);

   hash ^= 0xff;
   i++;
 }
 hash += (hash << 3);
 hash ^= (hash >> 11);
 hash += (hash << 15);
 hash += reverse_byte((p+0));
 return hash;
}

/**
   64-bit hash value, used if you do not need to include wider use of hashing.
 */
ngcp_hash64_t ngcp_hash64(void* input, uint32_t length, unsigned seed) {
  ngcp_hash64_t hash = 0;
  if (ValueIsNull(input)) {
    return hash;
  }
  
  unsigned int i = 0;
  byte* p = (byte*)input;
  hash = reverse_byte((p+0));
  
  while (i < length) {
    hash += (*(p+i) << 7) | (hash >> 31);
    hash ^= (hash >> 6);
    
    hash ^= 0xffff;
    if (hash <= 0xff000000) {
      hash += 0x00000000000000ff;
      hash += 0x000000000000ff00;
      hash += 0x0000000000ff0000;
      hash += 0x00000000ff000000;
      hash += 0x000000ff00000000;
      hash += 0x0000ff0000000000;
      hash += 0x00ff000000000000;
      hash ^= 0xff00000000000000;
    }
    i++;
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  hash += reverse_byte((p+0));
  return hash;
}

/**
   String hash used if string literal was declared. This does not gaurantee that the hash
   algorithm will be able to determine exact size of the given string literal. It will, however
   be able to accurately determine the string size if null terminator was specified At the End of the 
   string.
 */
ngcp_hash32_t ngcp_hash32(const char* input, unsigned seed) {
  unsigned int length = 0;
  if (ValueIsNull((void*)input)) {
    return length;
  }
  
  str_length(input, length);
  
  return (ngcp_hash32((void*)input, length, seed));
}

/**
   64-bit version of string hash.
 */
ngcp_hash64_t ngcp_hash64(const char* input, unsigned seed) { 
  unsigned int length = 0;
  if (ValueIsNull((void*)input)) {
    return length;
  }
  str_length(input, length);
  return (ngcp_hash64((void*)input, length, seed));
}


ngcp_hash32_t ngcp_hash32(unsigned int input, unsigned seed) {
  return (ngcp_hash32((void *)&input, 4, seed));
}


/**
   Specialization for ngcp hash function, specifically for unsigned int.
 */
ngcp_hash64_t ngcp_hash64(unsigned int input, unsigned seed) {
  return (ngcp_hash64((void*)&input, 4, seed));
}


/**
   Specialization for ngcp hash function, specifically for int.
 */
ngcp_hash32_t ngcp_hash32(int input, unsigned seed) {
  return (ngcp_hash32((void*)&input, 4, seed));
}


/**
   Specialization for ngcp hash function, specifically for int.
 */
ngcp_hash64_t ngcp_hash64(int input, unsigned seed) {
  return (ngcp_hash64((void*)&input, 4, seed));
}

/**
   Specialization for ngcp hash function, specifically for unsigned long long int.
 */
ngcp_hash32_t ngcp_hash32(unsigned long long input, unsigned seed) {
  return (ngcp_hash32((void*)&input, 8, seed));
}


/**
   Specialization for ngcp hash function, specifically for unsigned long long int.
 */
ngcp_hash64_t ngcp_hash64(unsigned long long input, unsigned seed) {
  return (ngcp_hash64((void*)&input, 8, seed));
}


/**
   Specialization for ngcp hash function, specifically for float.
 */
ngcp_hash32_t ngcp_hash32(float input, unsigned seed) {
  return (ngcp_hash32((void*)&input, 4, seed));
}

/**
   Specialization for ngcp hash function, specifically for float.
 */
ngcp_hash64_t ngcp_hash64(float input, unsigned seed) {
  return (ngcp_hash64((void*)&input, 4, seed));
}


/**
   Specialization for ngcp hash function, specifically for double.
 */
ngcp_hash32_t ngcp_hash32(double input, unsigned seed) {
  return (ngcp_hash32((void*)&input, 8, seed));
}


/**
   Specialization for ngcp hash function, specifically for double.
 */
ngcp_hash64_t ngcp_hash64(double input, unsigned seed) {
  return (ngcp_hash64((void*)&input, 8, seed));
}
} // Hash namespace
} // Comnet namespace

#undef NGCP_PRIVATE_API
#undef FORCE_INLINE
