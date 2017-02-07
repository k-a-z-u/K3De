#ifndef INDEXEDMESH_H
#define INDEXEDMESH_H

#include <KLib/Assertions.h>

#include "MeshVertex.h"
#include "../3D/AABB.h"

#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../gl/VAO.h"
#include "../misc/Error.h"

#include "../math/AffineTransform.h"

#include "IMesh.h"
#include "Transformable.h"

/**
 * an indexed mesh is a mesh that contains each vertex only once
 * and the vertices are not necessarily stored in triangle order.
 * the triangles are created using an index were consecutive entries
 * form a triangle by referencing the corresponding vertices using
 * their indicies.
 * this should speed things up for meshes with many polygons and
 * will save some GPU-RAM.
 */
class IndexedMesh : public IMesh, public Transformable {

private:

	friend class MeshFactory;

	VBOArrayStatic<AttrVertexNormalTangentTexture> vertices;
	VBOArrayIndexStatic indices;
	VAO vao;

	AABB bbox;

private:

	/** hidden CTOR */
	IndexedMesh() {;}


	void configure() {

		vao.bind();

		vertices.bind();

		// VertexNormalTexture
//		vao.setVertices(0, 8*4);
//		vao.setNormals(1, 8*4, 3*4);
//		vao.setTexCoords(2, 8*4, 6*4);

		// VertexNormalTangentTexture
		vao.setVertices(0, 11*4);
		vao.setNormals(1, 11*4, 3*4);
		vao.setTangents(3, 11*4, 6*4);		// todo swap indicies [2,3] here and within shaders?
		vao.setTexCoords(2, 11*4, 9*4);


		indices.bind();

		vao.unbind();

		// update the bbox;
		for (const AttrVertexNormalTangentTexture& vnt : vertices) {
			bbox.add(vnt.getVertex());
		}

	}

public:

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	void render(const SceneState&, const RenderState&) override {

		if (material2) {material2->bind();}

		vao.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		Error::assertOK();
		vao.unbind();

		if (material2) {material2->unbind();}

	}

	AABB getAABBmodel() const override {
		return bbox;
	}

	AABB getAABBworld() const override {
		return bbox.getTransformed(transform.getMatrix());
	}

};

#endif // INDEXEDMESH_H
