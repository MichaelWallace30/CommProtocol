/*
  Random number generator using Mersene Twister.

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
#ifndef COMM_RANDOM_H
#define COMM_RANDOM_H

#include <CommProto/architecture/api.h>
#include <time.h>
#include <random>

namespace comnet {


/**
  Lightweight Random number generator.
*/
class COMM_EXPORT CommRandom {
private:
  std::mt19937 randomGenerator;
  std::uniform_int_distribution<int> intDistribution;

public:
  CommRandom(int min, int max)
  : randomGenerator(std::mt19937(static_cast<unsigned int>(time(0)))) 
  , intDistribution(std::uniform_int_distribution<int>(min, max)) { }

  ~CommRandom() { }

  int randomUint8() { return intDistribution(randomGenerator); }
};
} // end namespace coment
#endif//end COMM_RANDOM_H