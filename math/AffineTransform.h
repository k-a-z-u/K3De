#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H

#include "Math.h"

struct AffineTransform {

	Vec3 position;
	Vec3 rotate;
	Vec3 scale;

	mutable bool needsUpdate;
	mutable Mat4 toWorld;

	/** default CTOR */
	AffineTransform() : position(0,0,0), rotate(0,0,0), scale(1,1,1), needsUpdate(true) {;}

	/** configure the current position */
	void setPosition(const float x, const float y, const float z) {
		position.x = x;
		position.y = y;
		position.z = z;
		needsUpdate = true;
	}

	/** get the current position */
	Vec3 getPosition() const {
		return position;
	}

	/** configure the rotation (x,y,z) in degree */
	void setRotation(const float x, const float y, const float z) {
		rotate.x = x;
		rotate.y = y;
		rotate.z = z;
		needsUpdate = true;
	}

	/** get the current rotation in degree */
	Vec3 getRotation() const {
		return rotate;
	}

	/** configure the scaling */
	void setScale(const float x, const float y, const float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
		needsUpdate = true;
	}

	/** get the current scale */
	Vec3 getScale() const {
		return scale;
	}

	/** get the resulting transformation matrix */
	const Mat4& getMatrix() const {
		updateIfNeeded();
		return toWorld;
	}

private:

	void updateIfNeeded() const {
		if (!needsUpdate) {return;}
		needsUpdate = false;
		Mat4 mT = Math::getTranslation(position);//Math::translate(Math::identity4(), position);
		Mat4 mS = Math::getScale(scale);//Math::scale(Math::identity4(), scale);
		Mat4 mR = Math::getRotation(rotate);//Math::eulerAngleXYZ(rotate);
		toWorld = mT*mR*mS;
	}

} pos;

#endif // AFFINETRANSFORM_H
