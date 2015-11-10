#ifndef SKYBOX_H
#define SKYBOX_H

#include "../3D/AABB.h"

#include "../gl/VBOArray.h"
#include "../gl/VAO.h"
#include "../mesh/UnindexedMesh.h"

class SkyBox : public UnindexedMesh {

public:

	/** ctor */
	SkyBox() {
		create();
	}

	/** scale the skybox. same value for all directions */
	void setSize(const float s) {
		setScale(s,s,s);
	}

	void create() {

		const float M10 = 1.0;
		const float M66 = 2.0 / 3.0;
		const float M33 = 1.0 / 3.0;
		const float M00 = 0.0;
		const float d = 0.001;	// delta to prevent black edges due to rounding

		// the 8 edges
		const Vec3 v1(-1,-1,-1);
		const Vec3 v2(+1,-1,-1);
		const Vec3 v3(+1,+1,-1);
		const Vec3 v4(-1,+1,-1);
		const Vec3 v5(-1,-1,+1);
		const Vec3 v6(+1,-1,+1);
		const Vec3 v7(+1,+1,+1);
		const Vec3 v8(-1,+1,+1);

		// the texture coordinates
		const Vec2 m1(0.25+d, M66-d);	// lower-left
		const Vec2 m2(0.50-d, M66-d);	// lower-right
		const Vec2 m3(0.50-d, M33+d);	// upper-right
		const Vec2 m4(0.25+d, M33+d);	// upper-left;

		const Vec2 r1(0.50+d, M66-d);	// lower-left
		const Vec2 r2(0.75-d, M66-d);	// lower-right
		const Vec2 r3(0.75-d, M33+d);	// upper-right
		const Vec2 r4(0.50+d, M33+d);	// upper-left;

		const Vec2 l1(0.00+d, M66-d);	// lower-left
		const Vec2 l2(0.25-d, M66-d);	// lower-right
		const Vec2 l3(0.25-d, M33+d);	// upper-right
		const Vec2 l4(0.00+d, M33+d);	// upper-left;

		const Vec2 b1(0.75+d, M66-d);	// lower-left
		const Vec2 b2(1.00-d, M66-d);	// lower-right
		const Vec2 b3(1.00-d, M33+d);	// upper-right
		const Vec2 b4(0.75+d, M33+d);	// upper-left;

		const Vec2 t1(0.25+d, M33-d);	// lower-left
		const Vec2 t2(0.50-d, M33-d);	// lower-right
		const Vec2 t3(0.50-d, M00+d);	// upper-right
		const Vec2 t4(0.25+d, M00+d);	// upper-left;

		const Vec2 bo1(0.25+d, M10-d);	// lower-left
		const Vec2 bo2(0.50-d, M10-d);	// lower-right
		const Vec2 bo3(0.50-d, M66+d);	// upper-right
		const Vec2 bo4(0.25+d, M66+d);	// upper-left;

		addQuad(v1, v2, v3, v4, m1, m2, m3, m4);	// front
		addQuad(v5, v1, v4, v8, l1, l2, l3, l4);	// left
		addQuad(v2, v6, v7, v3, r1, r2, r3, r4);	// right
		addQuad(v6, v5, v8, v7, b1, b2, b3, b4);	// back

		addQuad(v4, v3, v7, v8, t1, t2, t3, t4);		// top
		addQuad(v5, v6, v2, v1, bo1, bo2, bo3, bo4);	// bottom

//		vertices.append(VertexNormalTexture(v5, n, m3));
//		vertices.append(VertexNormalTexture(v7, n, m1));
//		vertices.append(VertexNormalTexture(v6, n, m4));

//		vertices.append(VertexNormalTexture(v5, n, m3));
//		vertices.append(VertexNormalTexture(v8, n, m2));
//		vertices.append(VertexNormalTexture(v7, n, m1));


//		vertices.append(VertexNormalTexture(v1, n, r3));
//		vertices.append(VertexNormalTexture(v8, n, r1));
//		vertices.append(VertexNormalTexture(v5, n, r4));

//		vertices.append(VertexNormalTexture(v1, n, r3));
//		vertices.append(VertexNormalTexture(v3, n, r2));
//		vertices.append(VertexNormalTexture(v8, n, r1));


//		vertices.append(VertexNormalTexture(v6, n, l3));
//		vertices.append(VertexNormalTexture(v4, n, l1));
//		vertices.append(VertexNormalTexture(v2, n, l4));

//		vertices.append(VertexNormalTexture(v8, n, l3));
//		vertices.append(VertexNormalTexture(v4, n, l2));
//		vertices.append(VertexNormalTexture(v7, n, l1));

		// upload
		vertices.upload();
		configure();

	}

	void addQuad(const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec3& v4, const Vec2& t1, const Vec2& t2, const Vec2& t3, const Vec2& t4) {

		const Vec3 n(0,0,0);

		vertices.append(VertexNormalTexture(v1, n, t1));
		vertices.append(VertexNormalTexture(v2, n, t2));
		vertices.append(VertexNormalTexture(v3, n, t3));

		vertices.append(VertexNormalTexture(v1, n, t1));
		vertices.append(VertexNormalTexture(v3, n, t3));
		vertices.append(VertexNormalTexture(v4, n, t4));

	}

	/** skybox is always visible */
	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}



};

#endif // SKYBOX_H
