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

#include "../textures/TextureFactory.h"
#include "IMesh.h"
#include "../shader/Shader.h"
//#include "../textures/Multitexture.h"



/**
 * an indexed mesh is a mesh that contains each vertex only once
 * and the vertices are not necessarily stored in triangle order.
 * the triangles are created using an index were consecutive entries
 * form a triangle by referencing the corresponding vertices using
 * their indicies.
 * this should speed things up for meshes with many polygons and
 * will save some GPU-RAM.
 */
class IndexedMesh : public IMesh {

private:

	friend class MeshFactory;

	AffineTransform transform;
	VBOArray<VertexNormalTexture> vertices;
	VBOArrayIndex indices;
	VAO vao;

	//Multitexture textures;
	//Material* material;

	AABB bbox;

private:

	/** hidden CTOR */
	IndexedMesh() {;}


	void configure() {

		vao.bind();

		vertices.bind();
		vao.setVertices(0, 8*4);
		vao.setNormals(1, 8*4, 3*4);
		vao.setTexCoords(2, 8*4, 6*4);

		indices.bind();

		vao.unbind();

		// update the bbox;
		for (const VertexNormalTexture& vnt : vertices.getData()) {
			bbox.add(vnt.getVertex());
		}

	}

public:

//	/** set the mesh's texture */
//	void setTexture(const size_t idx, Texture* texture) {
//		textures.set(idx, texture);
//	}



	void setPosition(const float x, const float y, const float z) {
		transform.setPosition(x,y,z);
	}

	Vec3 getPosition() const {
		return transform.getPosition();
	}

	void setRotation(const float x, const float y, const float z) {
		transform.setRotation(x/180.0f*M_PI, y/180.0f*M_PI, z/180.0f*M_PI);
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
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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

#endif // INDEXEDMESH_H
