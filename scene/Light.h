#ifndef LIGHT_H
#define LIGHT_H

#include "../math/Math.h"

struct vec3 {
	float x, y, z;
} __attribute__((packed));//__attribute__ ((aligned(16)));

struct LightAttributes {

	vec3 pos;	// position
	vec3 color;	// color

	void setPosition(const float x, const float y, const float z) {pos.x = x; pos.y = y; pos.z = z;}
	void setColor(const float r, const float g, const float b) {color.x = r; color.y = g; color.z = b;}

} __attribute__((packed));//__attribute__ ((aligned(16)));

class Light {

private:

	LightAttributes* attrs;

public:

	/** ctor */
	Light(LightAttributes* attrs) : attrs(attrs) {;}

	void setPosition(const float x, const float y, const float z) {
		attrs->setPosition(x,y,z);
	}

	void setPosition(const Vec3& pos) {
		attrs->setPosition(pos.x, pos.y, pos.z);
	}

	Vec3 getPosition() const {
		return Vec3(attrs->pos.x, attrs->pos.y, attrs->pos.z);
	}

	void setColor(const float r, const float g, const float b) {
		attrs->setColor(r,g,b);
	}

	void mirrorY(const float y) {
		attrs->pos.y = -attrs->pos.y;
	}

};

#endif // LIGHT_H
