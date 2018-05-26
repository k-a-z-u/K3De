#ifndef LIGHT_H
#define LIGHT_H

#include "../math/Math.h"
#include "../Assert.h"

#define setBit(val, bit)		(val |  (1 << bit))
#define clearBit(val, bit)		(val & ~(1 << bit))

#pragma pack(push,1)


enum class LightType {
	POINT,
	DIRECTIONAL,
};


struct AmbientAttributes {

	float r, g, b;
	float pad;

	AmbientAttributes():
		r(0), g(0), b(0) {;}

	void setColor(const float r, const float g, const float b) {this->r = r; this->g = g; this->b = b;}

};

struct LightAttributes {

	// 4x4 bytes
	float x, y, z, p1;				// position

	// 4x4 bytes
	float r, g, b, p2;				// color

	// 4x4 bytes
	uint32_t flags;				// active? casts shadows?
	uint32_t type;
	float attenuationLinear;	// attenuation over linear distance
	float attenuationQuadratic;	// attenuation over quadratic distance

	// 4x4 bytes
	Mat4 shadowPV;


	LightAttributes() :
		x(0), y(0), z(0), r(0), g(0), b(0),  flags(0), type(0), attenuationLinear(0), attenuationQuadratic(0) {;}

	void setType(const LightType t) {this->type = (int) t;}
	LightType getType() const {return (LightType) this->type;}

	void setDirection(const float x, const float y, const float z) {this->x = x; this->y = y; this->z = z; this->setType(LightType::DIRECTIONAL);}

	void setPosition(const float x, const float y, const float z) {this->x = x; this->y = y; this->z = z; this->setType(LightType::POINT);}

	void setColor(const float r, const float g, const float b) {this->r = r; this->g = g; this->b = b;}
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


class Ambient {

private:

	/** temporals for pre-calculations */
	float _impact = 0.2f;
	Vec3 _color = Vec3(1,1,1);

	AmbientAttributes* attrs;

public:


	/** empty ctor */
	Ambient() : attrs(nullptr) {
		;
	}

	/** ctor */
	Ambient(AmbientAttributes* attrs) : attrs(attrs) {
		const size_t s = sizeof(AmbientAttributes);
		const size_t d = s % (4*4);
		if (d != 0) {throw Exception("invalid byte alignment. must be multiple of GPU's vec4");}
	}

	/** set the light's color */
	void setColor(const float r, const float g, const float b) {
		_color = Vec3(r,g,b);
		updateColor();
	}

	/** set the light's brightness */
	void setImpact(const float i) {
		_impact = i;
		updateColor();
	}

private:

	/** calculate color*impact */
	void updateColor() {
		attrs->setColor(_color.r*_impact, _color.g*_impact, _color.b*_impact);
	}

};

/** wrapper around LightAttributes */
class Light {

private:

	/** temporals for pre-calculations */
	float _impact = 1.0f;
	Vec3 _color = Vec3(1,1,1);

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

	/** set the light's type */
	void setType(const LightType t) {
		attrs->setType(t);
	}

	/** get the light's type */
	LightType getType() const {
		return attrs->getType();
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


	void setDirection(const float x, const float y, const float z) {
		setDirection(Vec3(x,y,z));
	}
	void setDirection(Vec3 dir) {
		dir.normalize();
		attrs->setDirection(dir.x, dir.y, dir.z);
	}
	Vec3 getDirection() const {
		return Vec3(attrs->x, attrs->y, attrs->z);
	}

	void mirrorZ(const float z) {
		(void) z;	// TODO
		attrs->z = -attrs->z;
	}


	/** set the light's color */
	void setColor(const float r, const float g, const float b) {
		_color = Vec3(r,g,b);
		updateColor();
	}

	Vec3 getColor() const {
		return Vec3(attrs->r, attrs->g, attrs->b);
	}

	/** set the light's brightness */
	void setImpact(const float i) {
		_impact = i;
		updateColor();
	}

	float getImpact() const {
		return _impact;
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
		Assert::isTrue(mat.isSane(), "matrix seems invalid");
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


private:

	/** calculate color*impact */
	void updateColor() {
		attrs->setColor(_color.r*_impact, _color.g*_impact, _color.b*_impact);
	}


};

#endif // LIGHT_H
