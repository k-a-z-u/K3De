#ifndef LIGHT_H
#define LIGHT_H

#include "../math/Math.h"

#define setBit(val, bit)		(val |  (1 << bit))
#define clearBit(val, bit)		(val & ~(1 << bit))

#pragma pack(push,1)
struct LightAttributes {

	// 4x4 bytes
	float x, y, z, w;			// position

	// 4x4 bytes
	float r, g, b, a;			// color

	// 4x4 bytes
	float impact;				// "brightness"
	float attenuationLinear;	// attenuation over linear distance
	float attenuationQuadratic;	// attenuation over quadratic distance
	uint32_t flags;				// active? casts shadows?

	// 4x4 bytes
	Mat4 shadowPV;


	LightAttributes() :
		x(0), y(0), z(0), r(0), g(0), b(0), impact(1.0), attenuationLinear(0), attenuationQuadratic(0), flags(0) {;}

	void setPosition(const float x, const float y, const float z) {this->x = x; this->y = y; this->z = z;}
	void setColor(const float r, const float g, const float b) {this->r = r; this->g = g; this->b = b;}
	void setImpact(const float i) {this->impact = i;}
	void setAttenuationLinear(const float a) {this->attenuationLinear = a;}
	void setAttenuationQuadratic(const float a) {this->attenuationQuadratic = a;}

	void setShadowPV(const Mat4& mat) {shadowPV = mat.transposed();}

	void setActive(const bool enabled) {
		if (enabled) {flags = setBit(flags, 0);} else {flags = clearBit(flags, 0);}
	}
	bool isActive() const {
		return flags & (1 << 0);
	}

	void setCastsShadows(const bool casts) {
		if (casts) {flags = setBit(flags, 1);} else {flags = clearBit(flags, 1);}
	}
	bool getCastsShadows() const {
		return flags & (1 << 1);
	}

};// __attribute__((packed));
#pragma pack(pop)


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
		attrs->setCastsShadows(cast);
	}

	/** does this light cast shadows? */
	bool getCastsShadows() const {
		return attrs->getCastsShadows();
	}


	void setActive(const bool active) {
		attrs->setActive(active);
	}

	bool isActive() const {
		return attrs->isActive();
	}


};

#endif // LIGHT_H
