#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"
#include "Matrix.h"
#include "../Exception.h"
#include <KLib/Assertions.h>

// http://tfc.duke.free.fr/coding/md5-specs-en.html
// To convert a 3D vector to a quaternion, copy the x, y and z components and set the w component to 0.

// http://www.geeks3d.com/20141201/how-to-rotate-a-vertex-by-a-quaternion-in-glsl/

struct Quaternion {

	union {
		float data[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
	};

	/** empty ctor */
	Quaternion() : x(0), y(0), z(0), w(0) {;}

	/** ctor */
	Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {;}

	/** ctor for unit quaternion missing the w coordinate */
	Quaternion(const float x, const float y, const float z) : x(x), y(y), z(z), w(0) {
		fixW();
	}


	/** create a quaternion from "rotation by X around axis V" */
	static Quaternion fromAxisAngle(const Vec3 axis, const float angle) {

		const float angle2 = angle / 2.0f;
		return Quaternion(
			axis.x *	std::sin(angle2),
			axis.y *	std::sin(angle2),
			axis.z *	std::sin(angle2),
						std::cos(angle2)
		).normalized();

	}

	/** turn a 3D position vector into a quaternion [e.g. to rotate this position] */
	static Quaternion fromPosition(const Vec3 pos) {
		return Quaternion(pos.x, pos.y, pos.z, 0);
	}



	/** if this is a unit quaternion and only (x,y,z) is known, W can be calculated */
	void fixW() {

		const float t = 1.0f - (x * x) - (y * y) - (z * z);

		if (t < 0.0f) {
			w = 0.0f;
		} else {
			w = -std::sqrt(t);
		}

		if (!isUnitQuaternion()) {throw Exception("coding error");}

	}

	/** array access operator */
	float& operator [] (const int idx) {return data[idx];}

	/** const array access operator */
	float operator [] (const int idx) const {return data[idx];}

	Quaternion swappedYZ() const {
		//ew.w = old.w ; new.x = -old.x; new.y=- old.z ; new.z= new.y; ?
		//return Quaternion(-x, -z, y, w);
		//return Quaternion(z,x,y,w);
		return Quaternion(x,z,y, -w);
	}

	/** multiply two quaternions (= concatenate two rotations) */
	Quaternion operator * (const Quaternion& b) const {

//		const Quaternion& a = *this;
//		const Vec3 cross = ::cross(a.val.xyz(), b.val.xyz());
//		const float dot = ::dot(a.val.xyz(), b.val.xyz());
//		const float wa = a.val.w;
//		const float wb = b.val.w;

//		return Quaternion(
//			(a.val[0] * wb) + (b.val[0] * wa) + cross[0],
//			(a.val[1] * wb) + (b.val[1] * wa) + cross[1],
//			(a.val[2] * wb) + (b.val[2] * wa) + cross[2],
//			wa * wb - dot
//		);

		// http://tfc.duke.free.fr/coding/md5-specs-en.html
		const Quaternion& a = *this;
		return Quaternion(
			(a.x * b.w) + (a.w * b.x) + (a.y * b.z) - (a.z * b.y),
			(a.y * b.w) + (a.w * b.y) + (a.z * b.x) - (a.x * b.z),
			(a.z * b.w) + (a.w * b.z) + (a.x * b.y) - (a.y * b.x),
			(a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z)


//					 (a.x * b.w) + (a.w * b.x) + (a.y * b.z) - (a.z * b.y),
//					 (a.y * b.w) + (a.w * b.y) + (a.z * b.x) - (a.x * b.z),
//					(a.z * b.w) + (a.w * b.z) + (a.x * b.y) - (a.y * b.x),
//					(a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z)

		);

	}

	/** get the inverse of this quaternion. */
	Quaternion inverse() const {

		_assertTrue(isUnitQuaternion(), "only supported for unit quaternions");
		return Quaternion(-x, -y, -z, w);

		// more complex:
		//q = [x, y, z, w]
		//norm = |q| = sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w)
		//q-1 = [-x, -y, -z, w] / |q|
		//q-1 = [-x/|q|, -y/|q|, -z/|q|, w/|q|]

	}

	/** is this quaternion a unit-quaternion? */
	bool isUnitQuaternion() const {
		const float err = std::abs(1.0f - length());
		return err < 0.0001f;
	}

	/** get this quaternion's length */
	float length() const {
		return std::sqrt( x*x + y*y + z*z + w*w );
	}

	/** normalize this quaternion (length 1.0) */
	Quaternion normalized() const {
		const float l = length();
		const Quaternion q(x/l, y/l, z/l, w/l);
		_assertTrue(q.isUnitQuaternion(), "unit quaternion failed");
		return q;
	}

	/** rotate the given 3D vector using this quaternion */
	Vec3 rotate(const Vec3 vec) const {

		const Quaternion p = Quaternion::fromPosition(vec);

		// https://youtu.be/9APeaQlK4hQ?t=1265
		const Quaternion res = *this * p * this->inverse();
		return Vec3(res.x, res.y, res.z);

	}

	/** convert to pitch/roll/yaw angles in radians */
	Vec3 toEulerAngles() const {

		// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

		const float ysqr = y * y;

		// roll (x-axis rotation)
		const float t0 = +2.0f * (w * x + y * z);
		const float t1 = +1.0f - 2.0f * (x * x + ysqr);
		const float roll = std::atan2(t0, t1);

		// pitch (y-axis rotation)
		float t2 = +2.0f * (w * y - z * x);
		t2 = t2 > 1.0f ? 1.0f : t2;
		t2 = t2 < -1.0f ? -1.0f : t2;
		const float pitch = std::asin(t2);

		// yaw (z-axis rotation)
		const float t3 = +2.0f * (w * z + x * y);
		const float t4 = +1.0f - 2.0f * (ysqr + z * z);
		const float yaw = std::atan2(t3, t4);

		return Vec3(roll, pitch, yaw);

	}

	/** convert unit quaternion to vec3 for storing */
	Vec3 toVec3() const {
		_assertTrue(isUnitQuaternion(), "only supported for unit quaternions");
		return Vec3(x,y,z);
	}

	Mat4 toMat4() const {

//		// https://youtu.be/9APeaQlK4hQ?t=2344
//		float data[16] = {

//			1.0f - 2.0f * (y*y + z*z),
//			2.0f * (x*y + w*z),
//			2.0f * (x*z - w*y),
//			0.0f,

//			2.0f * (x*y - w*z),
//			1.0f - 2.0f * (x*x + z*z),
//			2.0f * (y*z + w*x),
//			0.0f,

//			2.0f * (x*z + w*y),
//			2.0f * (y*z - w*x),
//			1.0f - 2.0f * (x*x + y*y),
//			0.0f,

//			0.0f,
//			0.0f,
//			0.0f,
//			1.0f,

//		};

//		return Mat4::fromRawColumnMajor(data);

//		float data[16] = {
//			1.0f - 2.0f * (y*y + z*z),	2.0f * (x*y + w*z),			2.0f * (x*z - w*y),			0.0f,
//			2.0f * (x*y - w*z),			1.0f - 2.0f * (x*x + z*z),	2.0f * (y*z + w*x),			0.0f,
//			2.0f * (x*z + w*y),			2.0f * (y*z - w*x),			1.0f - 2.0f * (x*x + y*y),	0.0f,
//			0.0f,						0.0f,						0.0f,						1.0f,
//		};

		float data[16] = {
			1.0f - 2.0f * (y*y + z*z),	2.0f * (x*y - w*z),			2.0f * (x*z + w*y),			0.0f,
			2.0f * (x*y + w*z),			1.0f - 2.0f * (x*x + z*z),	2.0f * (y*z - w*x),			0.0f,
			2.0f * (x*z - w*y),			2.0f * (y*z + w*x),			1.0f - 2.0f * (x*x + y*y),	0.0f,
			0.0f,						0.0f,						0.0f,						1.0f
		};

		// faster. does not need transpose
		return Mat4::fromRawRowMajor(data);

	}

};

///** interpolate between two quaternions (animation) */
//inline Quaternion slerp(const Quaternion& a, const Quaternion& b, const float w) {
//	// TODO
//	return Quaternion();
//}

#endif // QUATERNION_H
