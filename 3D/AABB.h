#ifndef AABB_H
#define AABB_H

#include "../math/Math.h"
#include <array>

/**
 * represents an axis-aligned bounding-box
 */
class AABB {

private:

	/** the minimum position */
	Vec3 _min;

	/** the maximum position */
	Vec3 _max;

//	mutable std::array<Vec3, 8> edges;
//	mutable bool edgesDirty = true;


public:

	/** empty ctor */
	AABB() : _min(+Math::INF, +Math::INF, +Math::INF), _max(-Math::INF, -Math::INF, -Math::INF) {
		;
	}

	/** ctor */
	AABB(const Vec3& min, const Vec3& max) : _min(min),_max(max) {
		;
	}

	/** get the minimum corner */
	const Vec3& min() const {return _min;}

	/** get the maximum corner */
	const Vec3& max() const {return _max;}


	/** check whether the bounding-box is valid */
	bool isValid() const {
		return (_min.x != Math::INF);		// todo: check size?
	}

	/** add a new point to the bounding-box */
	void add(const Vec3& p) {
		if (p.x < _min.x) {_min.x = p.x;}
		if (p.y < _min.y) {_min.y = p.y;}
		if (p.z < _min.z) {_min.z = p.z;}
		if (p.x > _max.x) {_max.x = p.x;}
		if (p.y > _max.y) {_max.y = p.y;}
		if (p.z > _max.z) {_max.z = p.z;}
	}



	std::array<Vec3, 8> getEdges() const {

		std::array<Vec3, 8> edges;

		edges[0].set(_min.x, _min.y, _min.z);
		edges[1].set(_max.x, _min.y, _min.z);
		edges[2].set(_max.x, _max.y, _min.z);
		edges[3].set(_min.x, _max.y, _min.z);

		edges[4].set(_min.x, _min.y, _max.z);
		edges[5].set(_max.x, _min.y, _max.z);
		edges[6].set(_max.x, _max.y, _max.z);
		edges[7].set(_min.x, _max.y, _max.z);

		return edges;

	}

	/** does this bounding-box intersect with the unit cube? [-1,-1,-1] [+1,+1,+1] */
	bool intersectsUnitCube() const {
		static AABB unitAABB( Vec3(-1,-1,-1), Vec3(+1,+1,+1) );
		return this->intersects(unitAABB);
	}

	/** does this bounding-box intersect with the given bounding-box? */
	bool intersects(const AABB& o) const {
		if (o._min.x > _max.x) {return false;}
		if (o._min.y > _max.y) {return false;}
		if (o._min.z > _max.z) {return false;}
		if (o._max.x < _min.x) {return false;}
		if (o._max.y < _min.y) {return false;}
		if (o._max.z < _min.z) {return false;}
		return true;
	}

	/** get a new, transformed version of this BBox using the provided matrix */
	AABB getTransformed(const Mat4& mat) const {
		AABB res;
		for(const Vec3& e : getEdges()) {
			const Vec4 v = (mat*e.xyz1());
			if (v.w < 0) {continue;}			// TODO: check. ignore behind-the-camera stuff?
			res.add( v.divW().xyz() );
		}
		return res;
	}

};

#endif // AABB_H
