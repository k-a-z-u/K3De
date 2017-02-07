#ifndef INSTANCEMESH_H
#define INSTANCEMESH_H

#include "IMesh.h"
#include "../math/AffineTransform.h"
#include "Transformable.h"

/**
 * this flyweight wrapper class can be used to render one (static) mesh
 * at several positions (position, rotation, scale).
 *
 */
class InstanceMesh : public IMesh, public Transformable {

protected:

	/** the mesh to render */
	IMesh* mesh;

public:

	/** ctor */
	InstanceMesh(IMesh* mesh) : mesh(mesh) {
		this->shader = mesh->getShader();
	}

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	AABB getAABBmodel() const override {
		return mesh->getAABBmodel();
	}

	AABB getAABBworld() const override {
		return getAABBmodel().getTransformed(transform.getMatrix());
	}

	void render(const SceneState& ss, const RenderState& rs) override {
		if (material2) {material2->bind();}
		mesh->render(ss, rs);
		if (material2) {material2->unbind();}
	}

};

#endif // INSTANCEMESH_H
