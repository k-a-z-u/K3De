#ifndef TRIANGLES3_H
#define TRIANGLES3_H

#include "../math/Math.h"
#include "Triangle3.h"
#include <vector>

class Triangles3 : public std::vector<Triangle3> {

public:

	Triangles3 transform(const Mat4& mat) const {
		Triangles3 res;
		for (const Triangle3& tria : *this) {
			res.push_back(tria.transform(mat));
		}
		return res;
	}

	bool nearestIntersection(const Ray3& ray, Vec3& bestPos) const {
		bool found = false;
		for (const Triangle3& tria : *this) {
			Vec3 pos;
			if (tria.intersects(ray, pos)) {
				if ((ray.start-pos).length() < (ray.start-bestPos).length()) {
					bestPos = pos;
					found = true;
				}
			}
		}
		return found;
	}

};

#endif // TRIANGLES3_H
