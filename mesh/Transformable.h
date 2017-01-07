#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "../math/AffineTransform.h"
#include "ITransformable.h"

class Transformable : public ITransformable {

protected:

	AffineTransform transform;

public:

	/** get the transformation applied to this object */
	AffineTransform& getTransform() {return transform;}


	void setTransformOrder(const AffineTransformOrder order) override {transform.setOrder(order);}


	Vec3 getPosition() const override {return transform.getPosition();}

	void setPosition(const Vec3 pos) override {transform.setPosition(pos.x, pos.y, pos.z);}

	void setPosition(const float x, const float y, const float z) override {transform.setPosition(x, y, z);}


	Vec3 getScale() const override {return transform.getScale();}

	void setScale(const Vec3 scale) override {transform.setScale(scale.x, scale.y, scale.z);}

	void setScale(const float x, const float y, const float z) override {transform.setScale(x, y, z);}


	Vec3 getRotationRad() const override {return transform.getRotationRad();}

	void setRotationRad(const Vec3 rad) override {transform.setRotationRad(rad.x, rad.y, rad.z);}

	void setRotationRad(const float x_rad, const float y_rad ,const float z_rad) override {transform.setRotationRad(x_rad, y_rad, z_rad);}

	void setRotationDeg(const Vec3 deg) override {transform.setRotationRad(Math::degToRad(deg.x), Math::degToRad(deg.y), Math::degToRad(deg.z));}

	void setRotationDeg(const float x_deg, const float y_deg, const float z_deg) override {transform.setRotationRad(Math::degToRad(x_deg), Math::degToRad(y_deg), Math::degToRad(z_deg));}


};

#endif // TRANSFORMABLE_H
