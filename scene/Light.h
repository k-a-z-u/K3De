#ifndef LIGHT_H
#define LIGHT_H

#include "../math/Math.h"


struct LightAttributes {

	float x, y, z;				// position
	float r, g, b;				// color

	float impact;				// "brightness"
	float attenuationLinear;	// attenuation over linear distance
	float attenuationQuadratic;	// attenuation over quadratic distance

	uint32_t enabled;			// active?
	uint32_t castsShadows;		// casts shadows?

	uint32_t pad1;

	Mat4 shadowPV;


	LightAttributes() :
	    x(0), y(0), z(0), r(0), g(0), b(0), impact(1.0), attenuationLinear(0), attenuationQuadratic(0), enabled(0), castsShadows(0) {;}

	void setPosition(const float x, const float y, const float z) {this->x = x; this->y = y; this->z = z;}
	void setColor(const float r, const float g, const float b) {this->r = r; this->g = g; this->b = b;}
	void setImpact(const float i) {this->impact = i;}
	void setAttenuationLinear(const float a) {this->attenuationLinear = a;}
	void setAttenuationQuadratic(const float a) {this->attenuationQuadratic = a;}

	void setShadowPV(const Mat4& mat) {shadowPV = mat.transposed();}

	void setActive(const bool enabled) {this->enabled = enabled ? 1 : 0;}
	void setCastsShadows(const bool casts) {this->castsShadows = casts ? 1 : 0;}

} __attribute__((packed));



/** wrapper around LightAttributes */
class Light {

private:

	LightAttributes* attrs;

public:

	/** empty ctor */
	Light() : attrs(nullptr) {
		;
	}

	/** ctor */
	Light(LightAttributes* attrs) : attrs(attrs) {
		const size_t s = sizeof(LightAttributes);
		const size_t d = s % (4*4);
		if (d != 0) {throw Exception("invalid byte alignment. must be multiple of GPU's vec4");}
	}

	void setPosition(const float x, const float y, const float z) {
		attrs->setPosition(x,y,z);
	}

	void setPosition(const Vec3& pos) {
		attrs->setPosition(pos.x, pos.y, pos.z);
	}

	Vec3 getPosition() const {
		return Vec3(attrs->x, attrs->y, attrs->z);
	}

	void mirrorY(const float y) {
		attrs->y = -attrs->y;
	}


	void setColor(const float r, const float g, const float b) {
		attrs->setColor(r,g,b);
	}

	Vec3 getColor() const {
		return Vec3(attrs->r, attrs->g, attrs->b);
	}


	void setImpact(const float i) {
		attrs->setImpact(i);
	}

	float getImpact() const {
		return attrs->impact;
	}


	void setAttenuationLinear(const float a) {
		attrs->setAttenuationLinear(a);
	}

	void setAttenuationQuadratic(const float a) {
		attrs->setAttenuationQuadratic(a);
	}

	float getAttenuationLinear() const {
		return attrs->attenuationLinear;
	}

	float getAttenuationQuadratic() const {
		return attrs->attenuationQuadratic;
	}


	/** set the PV matrix for shadow texture projection for this light */
	void setShadowPV(const Mat4& mat) {
		attrs->setShadowPV(mat);
	}


	/** configure whether this light casts shadows */
	void setCastsShadows(const bool cast) {
		attrs->castsShadows = cast;
	}

	/** does this light cast shadows? */
	bool getCastsShadows() const {
		return attrs->castsShadows;
	}


	void setActive(const bool active) {
		attrs->setActive(active);
	}

	bool isActive() const {
		return attrs->enabled;
	}


};

#endif // LIGHT_H
