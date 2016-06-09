/* 
  Data type configuration for comm library.

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
#ifndef __COMM_TYPES_H
#define __COMM_TYPES_H

/**
   Boolean data type that will be used to control boolean functions. bool doesn't
   allow us to properly check for error.
 */
enum comm_bool {
  _FALSE = 0,
  _TRUE  = 1,
};

//////////////////////////////////////////////////////////////////
//
// Comm data type Declarations
//
//////////////////////////////////////////////////////////////////

typedef unsigned char          comm_byte;
typedef short int              comm_int16;
typedef unsigned short int     comm_uint16;
typedef int                    comm_int32;
typedef unsigned int           comm_uint32;
typedef long long int          comm_int64;
typedef unsigned long long int comm_uint64;

typedef float                  comm_real32;
typedef double                 comm_real64;

#endif // __COMM_TYPES_H
