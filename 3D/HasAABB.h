#ifndef HASAABB_H
#define HASAABB_H

#include "AABB.h"


class HasAABB {

public:

	/** get the model's bounding-box in model coordinates */
	virtual AABB getAABBmodel() const = 0;

	/** get the model's bounding-box in world coordinates */
	virtual AABB getAABBworld() const = 0;

};

#endif // HASAABB_H
