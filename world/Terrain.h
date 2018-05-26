#ifndef TERRAIN_H
#define TERRAIN_H

#include "../math/Math.h"

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../mesh/MeshVertex.h"
#include "../3D/AABBRenderable.h"

#include "../scene/Renderable.h"
#include "../textures/Multitexture.h"
#include "TerrainPart.h"

class Terrain : public Renderable {

public:

	int lastVisible;

private:

	friend class TerrainFactory;

	/** the terrain is divided into several parts to render only the visible ones */
	std::vector<TerrainPart> parts;

	/** terrain's model matrix */
	Mat4 identity;

	/** all applied textures */
	//Multitexture textures;

	/** the terrains details */
	struct {

		/** redundant. just used for a faster getHeight() */
		std::vector<Vertex> vertices;
		Vec3 center;
		Vec3 size;			// terrain's size in world-coordinates
		Vec3 scale;			// translate between world-size and number of vertices
		int numXVertices;	// number of vertices in x direction
		int numYVertices;	// number of vertices in y direction
		int numXTiles;
		int numYTiles;
	} params;


public:

	/** ctor */
	Terrain() : identity(1.0f) {
		;
	}

//	/** set the mesh's texture */
//	void setTexture(const size_t idx, Texture* texture) {
//		textures.set(idx, texture);
//	}

	/** get the terrain's size */
	const Vec3& getSizeOld() const {
		return params.size;
	}

	/** get the terrain's size */
	AABB getSize() const {
		Vec3 p1 = -(params.size * params.scale * 0.5);
		Vec3 p2 =  (params.size * params.scale * 0.5);
		return AABB(p1, p2);
	}

	/** get the terrain's height at the given (x,z) position */
	float getHeightAt(const Vec2& pos) {

		// convert everything relative to actual center [usually (0,0)]
		const Vec2 p = pos - params.center.xy();// + (params.size.xz() / 2);

		// get the nearest vertex (no interpolation!)
		const float x01 = p.x / params.scale.x + 0.5f;
		const float y01 = p.y / params.scale.y + 0.5f;
		const int vx = std::round(x01 * params.numXVertices);
		const int vy = std::round(y01 * params.numYVertices);
		const int vIdx = vx + vy * params.numXVertices;

		// sanity check
		_assertTrue(vx >= 0 && vx < params.numXVertices, "out of bounds");
		_assertTrue(vy >= 0 && vy < params.numYVertices, "out of bounds");

		// the the nearest vertex's height
		const float height = params.vertices.at(vIdx).z;
		return height;

	}

	virtual void render(const SceneState& ss, const RenderState& rs) override {

		material->bind();

		lastVisible = 0;
		for (TerrainPart& part : parts) {
			if (part.isVisible(rs.matrices.PVM)) {

				part.render(ss, rs);
				++lastVisible;

			}
//			{
//				AABBRenderable rr;
//				HasAABB* m = dynamic_cast<HasAABB*>(&part);
//				if (m) {
//					rr.set(m);
//					rr.render(rs);
//				}
//			}

		}

		material->unbind();

	}

	virtual const Mat4& getMatrix() const override {
		return identity;
	}


	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}

};

#endif // TERRAIN_H
