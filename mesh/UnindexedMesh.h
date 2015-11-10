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
//#include "../textures/Multitexture.h"

#include "IMesh.h"

/**
 * an unindexed mesh is a mesh that might contain vertices
 * several times, as the vertices are stored in triangle order.
 * if one vertex belongs to several triangles, the vertex is
 * present once per triangle.
 * this should be faster for small meshes with low polygon counts
 * and will definitely hurt for meshes with many polygons ;)
 */
class UnindexedMesh : public IMesh {

protected:

	friend class MeshFactory;

	AffineTransform transform;
	VBOArray<VertexNormalTexture> vertices;
	VAO vao;

	//Multitexture textures;

	AABB bbox;

protected:

	/** hidden CTOR */
	UnindexedMesh() {;}


	void configure() {

		vao.bind();

		vertices.bind();
		vao.setVertices(0, sizeof(VertexNormalTexture));
		vao.setNormals(1, sizeof(VertexNormalTexture), 3*4);
		vao.setTexCoords(2, sizeof(VertexNormalTexture), 6*4);

		vao.unbind();

	}

public:

//	/** set the mesh's texture */
//	void setTexture(const size_t idx, Texture* texture) {
//		textures.set(idx, texture);
//	}

	Material* getMaterial() {return material;}
	void setMaterial(Material* mat) {this->material = mat;}

	void setPosition(const float x, const float y, const float z) {
		transform.setPosition(x,y,z);
	}

	void setRotation(const float x, const float y, const float z) {
		transform.setRotation(x/180*3.1415,y/180*3.1415,z/180*3.1415);
	}

	void setScale(const float x, const float y, const float z) {
		transform.setScale(x,y,z);
	}

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	void render(const RenderStage& rs) override {

		material->bind();
		//textures.bindAll();
		//shader->bind();

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();

		//shader->unbind();
		//textures.unbindAll();
		material->unbind();

	}

	AABB getAABBmodel() const override {
		return bbox;
	}

	AABB getAABBworld() const override {
		return bbox.getTransformed(transform.getMatrix());
	}

};

#endif // UNINDEXEDMESH_H
