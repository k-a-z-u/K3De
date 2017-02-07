#ifndef BOX_H
#define BOX_H


#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../math/AffineTransform.h"
#include "../scene/Renderable.h"
#include "../mesh/Transformable.h"
#include "Estimator.h"

class Box3 : public Renderable, public Transformable {

	VAO vao;
	VBOArrayStatic<AttrVertexNormalTangentTexture> vertices;

public:

	Box3() {
		;
	}

	void create(const float w, const float h, const float d, const Vec3 offset = Vec3(0,0,0)) {

		const float x1 = -w/2 + offset.x;
		const float x2 = +w/2 + offset.x;

		const float y1 = -h/2 + offset.y;
		const float y2 = +h/2 + offset.y;

		const float z1 = -d/2 + offset.z;
		const float z2 = +d/2 + offset.z;

		const Vec3 nX(1,0,0);
		const Vec3 nY(0,1,0);
		const Vec3 nZ(0,0,1);

		// front
		addQuad(Vec3(x1, y1, z1), Vec3(x2, y1, z1), Vec3(x2, y2, z1), Vec3(x1, y2, z1), -nZ);

		// back
		addQuad(Vec3(x1, y1, z2), Vec3(x1, y2, z2), Vec3(x2, y2, z2), Vec3(x2, y1, z2),  nZ);

		// top
		addQuad(Vec3(x1, y2, z1), Vec3(x2, y2, z1), Vec3(x2, y2, z2), Vec3(x1, y2, z2),  nY);

		// bottom
		addQuad(Vec3(x1, y2, z1), Vec3(x1, y2, z2), Vec3(x2, y2, z2), Vec3(x2, y2, z1), -nY);	// TODO: check

		// right
		addQuad(Vec3(x2, y1, z1), Vec3(x2, y1, z2), Vec3(x2, y2, z2), Vec3(x2, y2, z1), -nX);

		// left
		addQuad(Vec3(x1, y1, z1), Vec3(x1, y2, z1), Vec3(x1, y2, z2), Vec3(x1, y1, z2),  nX);


		vertices.upload();

		vao.bind();

		vertices.bind();

//		//VertexNormalTexture
//		vao.setVertices(0, 8*4, 0);
//		vao.setNormals(1, 8*4, 3*4);
//		vao.setTexCoords(2, 8*4, 6*4);

		// VertexNormalTangentTexture
		vao.setVertices(0, 11*4);
		vao.setNormals(1, 11*4, 3*4);
		vao.setTangents(3, 11*4, 6*4);		// todo swap indicies [2,3] here and within shaders?
		vao.setTexCoords(2, 11*4, 9*4);

		vao.unbind();

	}

	void addQuad(const Vec3 v1, const Vec3 v2, const Vec3 v3, const Vec3 v4, const Vec3 n) {

		// dummy tangent
		const Vec3 ta(0,0,0);

		const Vec2 t1 = getTexCoord(v1, n);
		const Vec2 t2 = getTexCoord(v2, n);
		const Vec2 t3 = getTexCoord(v3, n);
		const Vec2 t4 = getTexCoord(v4, n);

		AttrVertexNormalTangentTexture vnt1(v1, n, ta, t1);
		AttrVertexNormalTangentTexture vnt2(v2, n, ta, t2);
		AttrVertexNormalTangentTexture vnt3(v3, n, ta, t3);
		AttrVertexNormalTangentTexture vnt4(v4, n, ta, t4);

		// calculate real tangents
		Estimator::estimateTangents(vnt1, vnt3, vnt2);
		Estimator::estimateTangents(vnt1, vnt4, vnt3);

		vertices.append(vnt1);
		vertices.append(vnt3);
		vertices.append(vnt2);

		vertices.append(vnt1);
		vertices.append(vnt4);
		vertices.append(vnt3);

	}

	Vec2 getTexCoord(const Vec3 vec, const Vec3 n) const {

		const Vec2 scale(0.33f, 0.33f);

		if (std::abs(n.x) == 1) {
			return Vec2(vec.y, vec.z) * scale;
		} else if (std::abs(n.y) == 1) {
			return Vec2(vec.x, vec.z) * scale;
		} else if (std::abs(n.z) == 1) {
			return Vec2(vec.x, vec.y) * scale;
		} else {
			throw 1;
		}

	}

	void render(const SceneState&, const RenderState&) override {

		material2->bind();
		vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		Error::assertOK();

		vao.unbind();
		material2->unbind();

	}

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}

};

#endif // BOX_H
