#ifndef RANDOM_H
#define RANDOM_H


#include <cmath>

class Random {

public:

	/** get a random number between min and max */
	static float get(const float min, const float max) {
		const float rnd = (float) rand() / (float) RAND_MAX;
		return min + rnd * (max-min);
	}

};

#endif // RANDOM_H
