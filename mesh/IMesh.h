#ifndef IMESH_H
#define IMESH_H

#include "../scene/Renderable.h"
#include "../3D/HasAABB.h"
#include "../3D/HasTriangles.h"

/**
 * interface for all meshes
 */
class IMesh : public Renderable, public HasAABB, public HasTriangles {

public:

	virtual bool isVisible(const Mat4& MVP) const override {
		// TODO: seems broken;
		(void) MVP;
		return true;//getAABBmodel().getTransformed(MVP).intersectsUnitCube();
	}

	virtual ~IMesh() {;}

};

#endif // IMESH_H
