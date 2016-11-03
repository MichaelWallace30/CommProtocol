/*
  Encryptor implementations.

  Copyright (C) 2016  Mario Garcia, Michael Wallace.

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
#ifndef __COMM_ENCRYPTOR_H
#define __COMM_ENCRYPTOR_H

#include <CommProto/encryption/encryption_interface.h>
#include <memory>
#include <cstdlib>

namespace comnet {
namespace encryption {


class CommDecryptor;
// Enables possible multiple encryptions?
enum CryptProtocol {
  NONE,
  AES,
};


/**
  Encryptor class intended for encrypting buffers for communications.
  Observer to the decryptor.
*/
class COMM_EXPORT CommEncryptor {
  COMM_DISALLOW_COPYING(CommEncryptor);
public:
  CommEncryptor(CryptProtocol proto);
  CommEncryptor(CryptProtocol proto, CommDecryptor* decryptor);
  ~CommEncryptor();

  uint8_t LoadKey(char* key);
  uint8_t LoadKeyFromFile(char* filename_key);
  int32_t Encrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]);

  CommDecryptor* GetDecryptor() { return decryptor; }
  void LinkDecryptor(CommDecryptor* decrypt) { decryptor = decrypt; }
  CryptProtocol GetEncryptionType() { return protocol; }
private:
  void Setup();

  std::shared_ptr<EncryptionInterface> encryption;
  CommDecryptor* decryptor;
  CryptProtocol protocol;
  friend class CommDecryptor;
};
} // encryption
} // comnet
#endif // __COMM_ENCRYPTOR_H
