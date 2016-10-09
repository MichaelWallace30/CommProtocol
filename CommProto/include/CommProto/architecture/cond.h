/*
  Condition Checking for threads and mutexes.

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
#ifndef __CONDITION_H
#define __CONDITION_H


#define LIB_CPLUSPLUS11



#if defined (LIB_CPLUSPLUS11)


namespace comnet {


//
// Comm Implmentation of Condition Status for std::cv_status
//
enum ConditionStatus { 
  TIMEOUT,      // Condition timed out.
  NO_TIMEOUT,   // Condition was awakened by notify_one or notify_all.
};

} // comnet
#endif 
#endif // __CONDITION_H