/*
  encryption protocol.

  Copyright (C) 2016  Michael Wallace.

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
#ifndef ENCRYPTION_INTERFACE_H
#define ENCRYPTION_INTERFACE_H

#include <stdint.h>
#include <CommProto/architecture/api.h>
#define BLOCK_SIZE 16

namespace comnet {
namespace encryption {


// abstract class to use for aes encryption
class COMM_EXPORT EncryptionInterface {
public:
  virtual ~EncryptionInterface() { }
  /** 
    input c string as the form of encrytion key
   */
  virtual uint8_t LoadKey(char* key) = 0;
  /** 
    load file which contatins the encryption key by the file name
  */
  virtual uint8_t LoadKeyFromFile(char*keyFileName) = 0;
  /** 
    Encrypt buffer for desired length of data stream and return any agumented legnth by reference
	    A return value of - value is an error 
  */
  virtual int32_t Encrypt(uint8_t* buffer, uint32_t length, uint8_t iv[BLOCK_SIZE]) = 0;
  /** 
    Decrypt buffer for desired length of data stream and return any agumented legnth by reference
	    A return value of - value is an error 
  */
  virtual int32_t Decrypt(uint8_t* buffer, uint32_t length, uint8_t iv[BLOCK_SIZE]) = 0;
  /** 
    Random number generator which fills an array of size length
   */
  virtual uint8_t GenerateRandomIV(uint8_t * buffer, uint32_t length) = 0;

  virtual bool KeyIsLoaded() = 0;
};
} // encryption
} // comnet
#endif // ENCRYPTION_INTERFACE_H