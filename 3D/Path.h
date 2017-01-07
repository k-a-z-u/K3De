#ifndef PATH_H
#define PATH_H

#include "../math/Math.h"
#include <vector>

class Path {

private:

	struct PathEntry {
		float percent;
		Vec3 vec;
		explicit PathEntry(const Vec3& vec) : percent(-1), vec(vec) {;}
	};

	/** all of the path's points */
	std::vector<PathEntry> points;

public:

	Path& operator *= (const float val) {
		for (PathEntry& pe : points) {pe.vec *= val;}
		update();
		return *this;
	}

	/** add a new point to the path */
	void add(const Vec3& pos) {
		points.push_back(PathEntry(pos));
		update();
	}

	/** get all points within the path */
	const std::vector<PathEntry>& getPoints() const {return points;}

	/** get the path's position at the given percentage */
	Vec3 getPosAt(float percent) const {
		percent = fmodf(percent, 1);
		size_t i;
		for (i = 0; i < points.size(); ++i) {
			if (points[i].percent > percent) {break;}
		}

		const float p1 = points[i-1].percent;
		const Vec3& v1 = points[i-1].vec;
		const float p2 = points[i  ].percent;
		const Vec3& v2 = points[i  ].vec;

		const float p = (percent - p1) / (p2-p1);
		return (v2 * p) + (v1 * (1-p));

	}

	/** get the path's angle at the given percentage */
	Vec3 getAnglesAt(const float percent, const float delta = 0.05) const {
		const Vec3 v1 = getPosAt(percent - delta);
		const Vec3 v2 = getPosAt(percent + delta);
		const float y = -atan2(v2.z-v1.z, v2.x-v1.x);
		return Vec3(0,y,0);
	}

	/** round this path */
	void round(const float f = 0.5) {

		std::vector<PathEntry> nPoints;

		nPoints.push_back(points.front());
		for (int i = 1; i < (int) points.size() - 1; ++i) {

			const Vec3 vl = points[i-1].vec;
			const Vec3 vc = points[i].vec;
			const Vec3 vr = points[i+1].vec;

			const Vec3 v1 = vc - (vc - vl) * f;//(vl * f) + (vc * (1-f));
			const Vec3 v2 = vc + (vr - vc) * f;//(vr * f) + (vc * (1-f));
			nPoints.push_back(PathEntry(v1));
			nPoints.push_back(PathEntry(v2));

		}
		nPoints.push_back(points.back());

		points = nPoints;
		update();

	}

private:

	/**
	 * 1) calculate the path's total length
	 * 2) calculate the percentage (within the path) for each of its vertices
	 */
	void update() {

		// calculate the path's total length
		float length = 0;
		for (int i = 0; i < (int) points.size()-1; ++i) {
			const Vec3& v1 = points[i].vec;
			const Vec3& v2 = points[i+1].vec;
			length += Math::length(v2-v1);
		}

		// calculate the "percent-of-total-length-reached" for each vertex
		points[0].percent = 0;
		float curLength = 0;
		for (int i = 1; i < (int) points.size(); ++i) {
			const Vec3& v1 = points[i-1].vec;
			const Vec3& v2 = points[i].vec;
			curLength += Math::length(v2-v1);
			points[i].percent = curLength/length;
		}

	}

};

#endif // PATH_H
