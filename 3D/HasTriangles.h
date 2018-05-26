#ifndef HASTRIANGLES_H
#define HASTRIANGLES_H

#include "Triangles3.h"

class HasTriangles {

public:

	/** get the model's triangles */
	virtual Triangles3 getTriangles() const = 0;

};


#endif // HASTRIANGLES_H
