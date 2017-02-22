#ifndef ITRANSFORMABLE_H
#define ITRANSFORMABLE_H

#include "../math/Math.h"
#include "../math/AffineTransform.h"

class ITransformable {

public:

	virtual void setTransformOrder(const AffineTransformOrder order) = 0;


	virtual Vec3 getPosition() const = 0;

	virtual void setPosition(const Vec3 pos) = 0;

	virtual void setPosition(const float x, const float y, const float z) = 0;


	virtual Vec3 getScale() const = 0;

	virtual void setScale(const Vec3 scale) = 0;

	virtual void setScale(const float x, const float y, const float z) = 0;


	virtual Vec3 getRotationRad() const = 0;

	virtual void setRotationRad(const Vec3 rad) = 0;

	virtual void setRotationRad(const float x_rad, const float y_rad ,const float z_rad) = 0;

	virtual Vec3 getRotationDeg() const = 0;

	virtual void setRotationDeg(const Vec3 deg) = 0;

	virtual void setRotationDeg(const float x_deg, const float y_deg, const float z_deg) = 0;

};

#endif // ITRANSFORMABLE_H
