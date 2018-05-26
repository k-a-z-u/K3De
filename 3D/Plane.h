#ifndef PLANE3_H
#define PLANE3_H

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../math/AffineTransform.h"
#include "../scene/Renderable.h"
#include "../3D/Estimator.h"
#include "../mesh/Transformable.h"

class Plane3 : public Renderable, public Transformable {

	VAO vao;
	VBOArrayStatic<AttrVertexNormalTangentTexture> vertices;
	//VBOArrayIndex indices;

public:

	Plane3() {


	}

	void addPlaneY(const float y, const Vec2 x1z1, const Vec2 x2z2, const Vec2 tile = Vec2(1,1)) {

		const Vec2 t1(0,0);
		const Vec2 t2 = tile;

		const float x1 = x1z1.x;
		const float x2 = x2z2.x;

		const float z1 = x1z1.y;
		const float z2 = x2z2.y;

		const Vec3 ta(0,0,0);		// dummy data
		const Vec3 n(0,1,0);

		AttrVertexNormalTangentTexture vt1(x1, y, z1,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t1.x, t2.y);
		AttrVertexNormalTangentTexture vt2(x2, y, z1,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t2.x, t2.y);
		AttrVertexNormalTangentTexture vt3(x2, y, z2,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t2.x, t1.y);
		AttrVertexNormalTangentTexture vt4(x1, y, z2,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t1.x, t1.y);

		Estimator::estimateTangents(vt1, vt3, vt2);
		Estimator::estimateTangents(vt1, vt4, vt3);

		vertices.append(vt1);
		vertices.append(vt3);
		vertices.append(vt2);

		vertices.append(vt1);
		vertices.append(vt4);
		vertices.append(vt3);

		vertices.upload();

		vao.bind();

		vertices.bind();

			// VertexNormalTangentTexture
			vao.setVertices(0, 11*4);
			vao.setNormals(1, 11*4, 3*4);
			vao.setTangents(3, 11*4, 6*4);		// todo swap indicies [2,3] here and within shaders?
			vao.setTexCoords(2, 11*4, 9*4);

		vao.unbind();

	}

	void addPlaneZ(const float z, const Vec2 x1y1, const Vec2 x2y2, const Vec2 tile = Vec2(1,1)) {

		const Vec2 t1(0,0);
		const Vec2 t2 = tile;

		const float x1 = x1y1.x;
		const float x2 = x2y2.x;

		const float y1 = x1y1.y;
		const float y2 = x2y2.y;

		const Vec3 ta(0,0,0);		// dummy data
		const Vec3 n(0,0,1);

		AttrVertexNormalTangentTexture vt1(x1, y1, z,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t1.x, t2.y);
		AttrVertexNormalTangentTexture vt2(x2, y1, z,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t2.x, t2.y);
		AttrVertexNormalTangentTexture vt3(x2, y2, z,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t2.x, t1.y);
		AttrVertexNormalTangentTexture vt4(x1, y2, z,		n.x,n.y,n.z,	ta.x, ta.y, ta.z,	t1.x, t1.y);

		Estimator::estimateTangents(vt1, vt3, vt2);
		Estimator::estimateTangents(vt1, vt4, vt3);

		// those seem/feel correct.. those for addPlaneY do not
		vertices.append(vt1);
		vertices.append(vt2);
		vertices.append(vt3);

		vertices.append(vt3);
		vertices.append(vt4);
		vertices.append(vt1);

		vertices.upload();

		vao.bind();

			vertices.bind();

			// VertexNormalTangentTexture
			vao.setVertices(0, 11*4);
			vao.setNormals(1, 11*4, 3*4);
			vao.setTangents(3, 11*4, 6*4);		// todo swap indicies [2,3] here and within shaders?
			vao.setTexCoords(2, 11*4, 9*4);

		vao.unbind();

	}

	void render(const SceneState&, const RenderState&) override {

		if (material)	{material->bind();}
		vao.bind();

		//glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		Error::assertOK();

		vao.unbind();
		if (material)	{material->unbind();}

	}

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}

};

#endif
