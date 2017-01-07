#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H

#include "Math.h"

enum class AffineTransformOrder {

	// usual way. loaded object is centered at (0,0,0), scaled, rotated, and translated to its world position
	SCALE_ROTATE_TRANSLATE,

	// for attached meshes (e.g. weapons) that are not correctly centered at (0,0,0) / rotated.
	SCALE_TRANSLATE_ROTATE,

};

class AffineTransform {

private:

	AffineTransformOrder order;

	Vec3 position;
	Vec3 rotate;
	Vec3 scale;

	mutable bool needsUpdate;
	mutable Mat4 local;
	mutable Mat4 global;

	bool overwritten;

	AffineTransform* pre;
	AffineTransform* post;

public:

	/** default CTOR */
	AffineTransform() : order(AffineTransformOrder::SCALE_ROTATE_TRANSLATE), position(0,0,0), rotate(0,0,0), scale(1,1,1), needsUpdate(true), overwritten(false), pre(nullptr), post(nullptr) {;}

	/** manually set the whole matrix and overwrite all other values */
	void setManually(const Mat4& matrix) {
		local = (matrix);
		overwritten = true;
		needsUpdate = false;
	}

	/** set the order of applying translation, rotation and scaling */
	void setOrder(const AffineTransformOrder order) {
		this->order = order;
	}

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

	/** configure the rotation (x,y,z) [in radians] */
	void setRotationRad(const float x_rad, const float y_rad, const float z_rad) {
		rotate.x = x_rad;
		rotate.y = y_rad;
		rotate.z = z_rad;
		needsUpdate = true;
	}

	/** get the current rotation (x,y,z) [in radians] */
	Vec3 getRotationRad() const {
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

		if (pre || post) {
			if (pre && post)	{global = post->getMatrix() *	local *	pre->getMatrix();}
			if (pre && !post)	{								local *	pre->getMatrix();}
			if (!pre && post)	{global = post->getMatrix() *	local;}
			return global;
		} else {
			return local;
		}

	}

	/** set a transform to apply before this transform [e.g. mesh attached to another mesh] */
	void setPre(AffineTransform* pre) {
		this->pre = pre;
	}

	/** set a transform to apply after this transform [e.g. mesh attached to another mesh] */
	void setPost(AffineTransform* post) {
		this->post = post;
	}

private:

	void updateIfNeeded() const {

		if (needsUpdate && !overwritten) {
			needsUpdate = false;

			const Mat4 mT = Math::getTranslation(position);
			const Mat4 mS = Math::getScale(scale);
			const Mat4 mR = Math::getRotation(rotate);

			switch (order) {
				case AffineTransformOrder::SCALE_ROTATE_TRANSLATE:	local = mT*mR*mS; break;
				case AffineTransformOrder::SCALE_TRANSLATE_ROTATE:	local = mR*mT*mS; break;
				default: throw Exception("invalid AffineTransformOrder");
			}

		}

	}

};

#endif // AFFINETRANSFORM_H
