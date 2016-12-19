#ifndef PLANE3_H
#define PLANE3_H

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../math/AffineTransform.h"
#include "../scene/Renderable.h"

class Plane3 : public Renderable {

	VAO vao;
	VBOArray<VertexNormalTexture> vertices;
	//VBOArrayIndex indices;
	AffineTransform transform;

public:

	Plane3() {


	}

	void addPlaneY(const float y, const Vec2 x1z1, const Vec2 x2z2) {

		const Vec2 t1(0,0);
		const Vec2 t2(5,5);

		const float x1 = x1z1.x;
		const float x2 = x2z2.x;

		const float z1 = x1z1.y;
		const float z2 = x2z2.y;

		const Vec3 n(0,1,0);

		const VertexNormalTexture vt1(x1, y, z1,	n.x,n.y,n.z,	t1.x, t2.y);
		const VertexNormalTexture vt2(x2, y, z1,	n.x,n.y,n.z,	t2.x, t2.y);
		const VertexNormalTexture vt3(x2, y, z2,	n.x,n.y,n.z,	t2.x, t1.y);
		const VertexNormalTexture vt4(x1, y, z2,	n.x,n.y,n.z,	t1.x, t1.y);

//		indices.append(0);
//		indices.append(1);
//		indices.append(2);

//		indices.append(0);
//		indices.append(2);
//		indices.append(3);


		vertices.append(vt1);
		vertices.append(vt3);
		vertices.append(vt2);

		vertices.append(vt1);
		vertices.append(vt4);
		vertices.append(vt3);

		vertices.upload();
		//indices.upload();


		vao.bind();

		vertices.bind();
		vao.setVertices(0, 8*4, 0);
		vao.setNormals(1, 8*4, 3*4);
		vao.setTexCoords(2, 8*4, 6*4);

		//indices.bind();

		vao.unbind();

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

#endif
