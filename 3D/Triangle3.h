#ifndef TRIANGLE3_H
#define TRIANGLE3_H

#include "../math/Math.h"

#include "Ray3.h"

struct Triangle3 {

	Vec3 p1;
	Vec3 p2;
	Vec3 p3;

public:

	/** empty ctor */
	Triangle3() : p1(), p2(), p3() {;}

	/** ctor */
	Triangle3(Vec3 p1, Vec3 p2, Vec3 p3) : p1(p1), p2(p2), p3(p3) {;}


	Triangle3 operator - (const Vec3 o) const {
		return Triangle3(p1-o, p2-o, p3-o);
	}

	Triangle3& operator += (const Vec3 o) {
		p1 += o;
		p2 += o;
		p3 += o;
		return *this;
	}

	Triangle3& operator -= (const Vec3 o) {
		p1 -= o;
		p2 -= o;
		p3 -= o;
		return *this;
	}

	Triangle3 transform(const Mat4& mat) const {
		return Triangle3( (mat*p1.xyz1()).xyz(), (mat*p2.xyz1()).xyz(), (mat*p3.xyz1()).xyz() );
	}

	Vec3 getNormal() const {
		return cross( p2-p1, p3-p1 ).normalized();
	}


	// http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
	bool intersects(const Ray3& ray, Vec3& pos) const {

		const Vec3 e1 = p2-p1;
		const Vec3 e2 = p3-p1;

		const Vec3 h = cross(ray.dir, e2);
		const double a = dot(e1, h);

		if (a > -0.00001 && a < 0.00001) {return false;}

		const double f = 1/a;

		const Vec3 s = ray.start - p1;
		const double u = f * dot(s,h);

		if (u < 0.0 || u > 1.0) {return false;}

		const Vec3 q = cross(s, e1);
		const double v = f * dot(ray.dir, q);

		if (v < 0.0 || u + v > 1.0) {return false;}
		const double t = f * dot(e2,q);


		if (t > 0.00001) {
			pos = ray.start + (ray.dir * t);
			return true;
		}

		return false;

	}

};

#endif // TRIANGLE3_H
