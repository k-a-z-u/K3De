#ifndef RAY3_H
#define RAY3_H

#include "../math/Math.h"

struct Ray3 {

	/** starting point */
	Vec3 start;

	/** ray's direction */
	Vec3 dir;

public:

	/** empty */
	Ray3() : start(), dir() {
		;
	}

	/** ctor */
	Ray3(Vec3 start, Vec3 dir) : start(start), dir(dir) {
		;
	}

};

#endif // RAY3_H
