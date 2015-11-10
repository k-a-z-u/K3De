#ifndef INSTANCEMESH_H
#define INSTANCEMESH_H

#include "IMesh.h"
#include "../math/AffineTransform.h"

/**
 * this flyweight wrapper class can be used to render one (static) mesh
 * at several positions (position, rotation, scale).
 *
 */
class InstanceMesh : public IMesh {

protected:

	/** the mesh to render */
	IMesh* mesh;

	/** the transform to apply to the mesh */
	AffineTransform iTransform;

public:

	/** ctor */
	InstanceMesh(IMesh* mesh) : mesh(mesh) {
		this->shader = mesh->getShader();
	}

	void setPosition(const float x, const float y, const float z) {
		iTransform.setPosition(x,y,z);
	}

	void setPosition(const Vec3& vec) {
		setPosition(vec.x, vec.y, vec.z);
	}

	void setRotation(const float x, const float y, const float z) {
		iTransform.setRotation(x/180*3.1415,y/180*3.1415,z/180*3.1415);
	}
	void setRotationRad(const Vec3& vec) {
		iTransform.setRotation(vec.x, vec.y, vec.z);
	}


	void setScale(const float x, const float y, const float z) {
		iTransform.setScale(x,y,z);
	}

	const Mat4& getMatrix() const override {
		return iTransform.getMatrix();
	}

	AABB getAABBmodel() const override {
		return mesh->getAABBmodel();
	}

	AABB getAABBworld() const override {
		return getAABBmodel().getTransformed(iTransform.getMatrix());
	}

	void render(const RenderStage& rs) override {
		glDisable(GL_CULL_FACE);
		mesh->render(rs);
		glEnable(GL_CULL_FACE);
	}

};

#endif // INSTANCEMESH_H
