#ifndef SUBMESH_H
#define SUBMESH_H

#include "IMesh.h"
#include "../math/AffineTransform.h"
#include "Transformable.h"
#include "../3D/Triangles3.h"

/**
 * this class allows placing one mesh
 * within another mesh. that is:
 * it inherits its transformation
 */
class SubMesh : public IMesh {

protected:

	/** the mesh to render */
	IMesh* mesh;

	/** the parent we are attached to */
	IMesh* parent;

	mutable Mat4 _mat;

public:

	/** ctor */
	SubMesh(IMesh* mesh, IMesh* parent) : mesh(mesh), parent(parent) {
		//TODO("todo. ugly top copy those ones..")
		this->shader = mesh->getShader();
		this->material = mesh->getMaterial();
	}

	const Mat4& getMatrix() const override {
		_mat = parent->getMatrix() * mesh->getMatrix();	// chained
		return _mat; // TODO: better solution? we must not return a temporary object here..
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

#endif // SUBMESH_H
