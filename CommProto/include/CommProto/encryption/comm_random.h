

#ifndef COMM_RANDOM_H
#define COMM_RANDOM_H

#include <CommProto/architecture/api.h>
#include <time.h>
#include <random>

namespace comnet {

	class COMM_EXPORT CommRandom
	{
	private:
		std::mt19937 randomGenerator;
		std::uniform_int_distribution<int> intDistribution;

	public:
		CommRandom(int min, int max)
		{
			randomGenerator = std::mt19937(time(0));
			intDistribution = std::uniform_int_distribution<int>(min, max);
		}

		~CommRandom(){};

		int randomUint8()
		{
			return intDistribution(randomGenerator);
		}

	};

}//end namespace coment

#endif//end COMM_RANDOM_H