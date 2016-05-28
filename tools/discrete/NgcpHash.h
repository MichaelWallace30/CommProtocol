/*
  This file contains the NGCP researched hash implementation, so as to create
  discrete numbers from data, which will then be used to store and separate 
  this data from others. It attempts to avoid the most collisions possible,
  so as to prevent any conflicts with data handling and storage. This is still
  an ongoing research, and we will hope to continue working on improving this 
  hash for the long run.

  Copyright (C) 2016  Mario Garcia.

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
#ifndef __NGCPHASH_H
#define __NGCPHASH_H

#include <architecture/os/include-defines.h>
#include <architecture/macros.h>

#define HASH_BIT                 32
#define HASH_SEED                 1
#define HASH_VERSION              0



#endif // __NGCPHASH_H
