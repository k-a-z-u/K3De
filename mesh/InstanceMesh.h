#ifndef INSTANCEMESH_H
#define INSTANCEMESH_H

#include "IMesh.h"
#include "../math/AffineTransform.h"
#include "Transformable.h"
#include "../3D/Triangles3.h"

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
		//TODO("todo. ugly top copy those ones..")
		this->shader = mesh->getShader();
		this->material = mesh->getMaterial();
	}

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	Triangles3 getTriangles() const override {
		return mesh->getTriangles();
	}

	virtual AABB getAABBmodel() const override {
		throw Exception("not yet implemented");		// needs this matrix
		//return mesh->getAABBmodel();
	}

	virtual AABB getAABBworld() const override {
		throw Exception("not yet implemented");		// needs this matrix
		//return mesh->getAABBworld();
	}

//	void setMaterial(Material3* mat) {
//		throw Exception("must not call setMaterial() on an instance mesh. call this on the wrapped mesh instead");
//	}

//	AABB getAABBmodel() const override {
//		return mesh->getAABBmodel();
//	}

//	AABB getAABBworld() const override {
//		return getAABBmodel().getTransformed(transform.getMatrix());
//	}

	virtual void onBeforeRender(const SceneState& ss) override {
		this->mesh->onBeforeRender(ss);
	}

	void render(const SceneState& ss, const RenderState& rs) override {
		if (material) {material->bind();}		// should be null
		mesh->render(ss, rs);
		if (material) {material->unbind();}
	}

	// TODO:
	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}


};

#endif // INSTANCEMESH_H
