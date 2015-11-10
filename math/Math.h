#ifndef MMATH_H
#define MMATH_H

//#include <eigen3/Eigen/Dense>
//#include <eigen3/Eigen/Eigenvalues>

#include <cmath>

#ifdef WITH_TESTS
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

//typedef Eigen::Vector4f EVec4;
//typedef Eigen::Vector3f EVec3;
//typedef Eigen::Vector2f EVec2;
//typedef Eigen::Matrix3f EMat3;
//typedef Eigen::Matrix4f EMat4;

//#include "Vector.h"
#include "Matrix.h"
//typedef glm::vec2 Vec2;
//typedef glm::vec3 Vec3;
//typedef glm::vec4 Vec4;
//typedef glm::mat3 Mat3;
//typedef glm::mat4 Mat4;

//class MISC {

//public:


//	static EVec4 toEigen (const Vec4& in) {
//		return EVec4(in.x, in.y, in.z, in.w);
//	}

//	static EVec3 toEigen (const Vec3& in) {
//		return EVec3(in.x, in.y, in.z);
//	}

//	static EVec2 toEigen (const Vec2& in) {
//		return EVec2(in.x, in.y);
//	}

//	static Vec3 toGLM(const EVec3& in) {
//		return Vec3(in(0), in(1), in(2));
//	}

//	/** get the eigenvector beloging to the smallest eigenvalue */
//	static EVec3 getEigenvectorForSmallestEigenvalue(const EMat3 mat) {

//		Eigen::SelfAdjointEigenSolver<EMat3> solver(mat);

//		EVec3 eVecSmall;
//		float eValSmall = 99999999;
//		for (int i = 0; i < 3; ++i) {
//			float eVal = solver.eigenvalues()[i];
//			if (eVal < eValSmall) {
//				eValSmall = eVal;
//				eVecSmall = solver.eigenvectors().col(i);
//			}
//		}

//		return eVecSmall;

//	}

//};


class Math {

	/**
	 * notes:
	 * https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/
	 * http://www.songho.ca/opengl/gl_projectionmatrix.html
	 * http://stackoverflow.com/questions/21057640/gluperspective-code-messy/21064935#21064935
	 * original from GLU
	 *	https://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
	 *	https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
	 *  https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
	 */

public:

	//static constexpr float NINF = std::numeric_limits<float>::infinity();
	static constexpr float INF = std::numeric_limits<float>::infinity();

	//static Mat4 identity4() {return glm::mat4(1.0f);}
	static Mat4 identity4() {return Mat4::identity();}

	/** get the look-at matrix V for the given parameters */
	static Mat4 camLookAt(const Vec3& pos, const Vec3& lookAt, const Vec3& up) {

		const Vec3 f = normalize(lookAt - pos);
		const Vec3 s = normalize(cross(f, normalize(up)));
		const Vec3 u = cross(s, f);
		const float data[16] = {
			s.x,	s.y,	s.z,	0,
			u.x,	u.y,	u.z,	0,
			-f.x,	-f.y,	-f.z,	0,
			0,		0,		0,		1
		};
		//return translate(glm::transpose( glm::make_mat4(mat) ), -pos);
		//Mat4 mPos(1.0f); mPos = translate(mPos, -pos);
		Mat4 trans = getTranslation(-pos);
		Mat4 m = Mat4::fromRawRowMajor(data);
		return m*trans;
//		Mat4 mat(data);
//		mat = translate(mat, -pos);
//		mat.transpose();
//		return mat;
		//return mat * mPos;
		//return glm::lookAt(pos, lookAt, up);

	}



	/** get the perspective matrix P for the given parameters */
	static Mat4 camPerspective(const float fov, const float aspect, const float near, const float far) {
		const float f = cotan(fov/2.0f);
		const float data[16] = {
			f/aspect,	0,			0,			0,
			0,			f,			0,			0,
			0,			0,			(far+near)/(near-far),	(2*far*near)/(near-far),
			0,			0,			-1,			0
		};
		return Mat4::fromRawRowMajor(data);
	}

	static Mat4 camOrthographic(const float left, const float right, const float top, const float bottom, const float near, const float far) {
		const float tx = -(right+left)/(right-left);
		const float ty = -(top+bottom)/(top-bottom);
		const float tz = -(far+near)/(far-near);
		const float data[16] = {
			2/(right-left),	0,				0,				tx,
			0,				2/(top-bottom),	0,				ty,
			0,				0,				-2/(far-near),	tz,
			0,				0,				0,				1
		};
		return Mat4::fromRawRowMajor(data);
	}

	static double cotan(const double i) { return(1.0 / std::tan(i)); }

	/** get a translation matrix */
	static Mat4 getTranslation(const Vec3& position) {
		Mat4 m(1.0f);
		m.set(3, 0, position.x);
		m.set(3, 1, position.y);
		m.set(3, 2, position.z);
		return m;
	}

	/** get a scalin matrix */
	static Mat4 getScale(const Vec3& scale) {
		Mat4 m(1.0f);
		m.set(0, 0, scale.x);
		m.set(1, 1, scale.y);
		m.set(2, 2, scale.z);
		return m;
	}

	/** get a rotation matrix for the given x,y,z rotation (in radians) */
	static Mat4 getRotation(const Vec3& rotation) {
		const float g = rotation.x; const float b = rotation.y; const float a = rotation.z;
		const float a11 = cos(a)*cos(b);
		const float a12 = cos(a)*sin(b)*sin(g)-sin(a)*cos(g);
		const float a13 = cos(a)*sin(b)*cos(g)+sin(a)*sin(g);
		const float a21 = sin(a)*cos(b);
		const float a22 = sin(a)*sin(b)*sin(g)+cos(a)*cos(g);
		const float a23 = sin(a)*sin(b)*cos(g)-cos(a)*sin(g);
		const float a31 = -sin(b);
		const float a32 = cos(b)*sin(g);
		const float a33 = cos(b)*cos(g);
		const float data[16] = {
			a11,	a12,	a13,	0,
			a21,	a22,	a23,	0,
			a31,	a32,	a33,	0,
			0,		0,		0,		1
		};
		return Mat4::fromRawRowMajor(data);
	}

	static Mat4 translate(const Mat4& src, const Vec3& position) {
		const Mat4 m = getTranslation(position);
		return m*src;						// column-major version
		//return glm::translate(src, position);
	}

	static Mat4 scale(const Mat4& src, const Vec3& scale) {
		const Mat4 m = getScale(scale);
		return m*src;						// column-major version
		//return glm::scale(src, scale);
	}

	/** create a rotation matrix from 3 angles (in radians) */
	static Mat4 rotate(const Mat4& src, const Vec3& angles) {
		const Mat4 m = getRotation(angles);
		return m*src;						// column-major version
		//return glm::eulerAngleYXZ(rotate.y, rotate.x, rotate.z);
	}

	/** convert from degree to radians */
	static inline float toRadians(const float angle) {
		return angle * M_PI / 180.0f;
	}

	static inline Vec3 cross(const Vec3& a, const Vec3& b) {
		const float x = a.y*b.z - a.z*b.y;
		const float y = a.z*b.x - a.x*b.z;
		const float z = a.x*b.y - a.y*b.x;
		return Vec3(x,y,z);
		//return glm::cross(a, b);
	}

	static inline float dot(const Vec3& a, const Vec3& b) {
		return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
	}

	/** get the length of the given vector */
	static inline float length(const Vec3& vec) {
		return std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
		//return glm::length(vec);
	}

	static inline Vec3 normalize(const Vec3& vec) {
		return vec / length(vec);
	}


};

#endif // MMATH_H
