#ifndef TERRAINPART_H
#define TERRAINPART_H

#include "../scene/Renderable.h"
#include "../3D/AABB.h"
#include "../3D/HasAABB.h"
#include "../gl/VAO.h"
#include "../gl/VBOArrayIndex.h"
#include "../gl/VBOArray.h"

/**
 * the terrain is split into parts (tiles).
 * to improve performance, only visible parts are rendered.
 */
class TerrainPart : public HasAABB{

private:

	friend class Terrain;
	friend class TerrainFactory;

	VAO vao;
	VBOArrayStatic<AttrVertexNormalTexture> vertices;
	VBOArrayIndexStatic indices;
	AABB bbox;

	void upload() {

		vertices.upload();
		indices.upload();

		vao.bind();

		vertices.bind();
		vao.setVertices(0, 8*4);
		vao.setNormals(1, 8*4, 3*4);
		vao.setTexCoords(2, 8*4, 6*4);

		indices.bind();

		vao.unbind();

		// update the bounding-box
		for (const AttrVertexNormalTexture vnt : vertices.getData()) {
			bbox.add(vnt.v);
		}

	}

public:

	/** render the terrain part */
	void render(const SceneState&, const RenderState&) {
		vao.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.unbind();
	}

	virtual bool isVisible(const Mat4& MVP) const {
		return bbox.getTransformed(MVP).intersectsUnitCube();
	}

	AABB getAABBmodel() const {
		return bbox;
	}

	AABB getAABBworld() const {
		return bbox;
	}

public:



};

#endif // TERRAINPART_H
