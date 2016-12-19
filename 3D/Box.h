#ifndef BOX_H
#define BOX_H


#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../math/AffineTransform.h"
#include "../scene/Renderable.h"

class Box3 : public Renderable {

	VAO vao;
	VBOArray<VertexNormalTexture> vertices;
	AffineTransform transform;


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
		vao.setVertices(0, 8*4, 0);
		vao.setNormals(1, 8*4, 3*4);
		vao.setTexCoords(2, 8*4, 6*4);

		vao.unbind();

	}

	void addQuad(const Vec3 v1, const Vec3 v2, const Vec3 v3, const Vec3 v4, const Vec3 n) {

		const Vec2 t1 = getTexCoord(v1, n);
		const Vec2 t2 = getTexCoord(v2, n);
		const Vec2 t3 = getTexCoord(v3, n);
		const Vec2 t4 = getTexCoord(v4, n);

		const VertexNormalTexture vnt1(v1, n, t1);
		const VertexNormalTexture vnt2(v2, n, t2);
		const VertexNormalTexture vnt3(v3, n, t3);
		const VertexNormalTexture vnt4(v4, n, t4);

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

	void setPosition(const float x, const float y, const float z) {
		transform.setPosition(x,y,z);
	}

	void setRotation(const float x, const float y, const float z) {
		transform.setRotation(x/180*3.1415,y/180*3.1415,z/180*3.1415);
	}

	void setScale(const float x, const float y, const float z) {
		transform.setScale(x,y,z);
	}

	void render(const RenderStage& rs) override {

		material->bind();
		vao.bind();

		//glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		Error::assertOK();

		vao.unbind();
		material->unbind();

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
