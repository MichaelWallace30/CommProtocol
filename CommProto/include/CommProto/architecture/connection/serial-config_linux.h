/*
  Linux serial configuration.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia, Alex Craig.

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
#ifndef __SERIAL_CONFIG_LINUX_H
#define __SERIAL_CONFIG_LINUX_H

#include <CommProto/architecture/connection/serial_status.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

struct serial_info {
  
  speed_t baudrate;
  serial_status serial_s;
  port_id fd;

  char device[50];
};

typedef struct serial_info serial_t;
#endif // __SERIAL_CONFIG_LINUX_H
