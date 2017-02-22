#ifndef TRANSFORMABLE2D_H
#define TRANSFORMABLE2D_H

#include "../math/Matrix.h"

#include <KLib/cv/camera/Homography.h>

class Transformable2D {

protected:

	// every element is [by default] starting at (0,0) and has some size (width,height)
	const float x;
	const float y;
	float width_01;
	float height_01;

	Mat4 matrix;

public:

	/** ctor */
	Transformable2D() : x(0), y(0), width_01(0), height_01(0) {
		matrix = Mat4::identity();
	}

	/** get the element's matrix */
	const Mat4& getMatrix() const {
		return matrix;
	}

	/** order: top-left, top-right, bottom-right, bottom-left */
	void setHomography(Vec2 to[4]) {

		K::Homography hom;
		hom.addCorrespondence(to[0].x, to[0].y,		x,			y);
		hom.addCorrespondence(to[1].x, to[1].y,		x+width_01,	y);
		hom.addCorrespondence(to[2].x, to[2].y,		x+width_01, y+height_01);
		hom.addCorrespondence(to[3].x, to[3].y,		x,			y+height_01);
		hom.estimate();
		auto H = hom.getMatrix();

		matrix = {
		    (float)H(0,0),		(float)H(0,1),		(float)0,		(float)H(0,2),
			(float)H(1,0),		(float)H(1,1),		(float)0,		(float)H(1,2),
			(float)0,			(float)0,			(float)1,		(float)0,
			(float)H(2,0),		(float)H(2,1),		(float)0,		(float)H(2,2),
		};
		matrix.transpose();

		int i = 0; (void) i;

	}

	void setPosition(Vec2 pos) {
		matrix = {
			1, 0, 0, pos.x,
			0, 1, 0, pos.y,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		matrix.transpose();
	}

};

#endif // TRANSFORMABLE2D_H
