#ifndef UNINDEXEDMESH_H
#define UNINDEXEDMESH_H

#include <KLib/Assertions.h>

#include "MeshVertex.h"
#include "../3D/AABB.h"


#include "../gl/VBOArray.h"
#include "../gl/VAO.h"
#include "../misc/Error.h"

#include "../math/AffineTransform.h"

#include "../textures/TextureFactory.h"

#include "../shader/Shader.h"

#include "IMesh.h"
#include "Transformable.h"

/**
 * an unindexed mesh is a mesh that might contain vertices
 * several times, as the vertices are stored in triangle order.
 * if one vertex belongs to several triangles, the vertex is
 * present once per triangle.
 * this should be faster for small meshes with low polygon counts
 * and will definitely hurt for meshes with many polygons ;)
 */
class UnindexedMesh : public IMesh, public Transformable {

protected:

	friend class MeshFactory;

	VBOArrayStatic<AttrVertexNormalTexture> vertices;
	VAO vao;

	AABB bbox;

protected:

	/** hidden CTOR */
	UnindexedMesh() {;}


	void configure() {

		vao.bind();

		vertices.bind();
		vao.setVertices(0, sizeof(AttrVertexNormalTexture));
		vao.setNormals(1, sizeof(AttrVertexNormalTexture), 3*4);
		vao.setTexCoords(2, sizeof(AttrVertexNormalTexture), 6*4);

		vao.unbind();

	}

public:


	Material2* getMaterial() {return material2;}

	//void setMaterial(Material2* mat) {this->material2 = mat;}

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	void render(const SceneState&, const RenderState&) override {

		if (material2) {material2->bind();}

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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

#endif // UNINDEXEDMESH_H
