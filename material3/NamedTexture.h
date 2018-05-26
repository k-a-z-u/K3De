#ifndef NAMEDTEXTURE_H
#define NAMEDTEXTURE_H

#include <string>
#include "../textures/ITexture.h"
#include "VarType.h"

struct NamedTexture {

	std::string name;

	ITexture* tex = nullptr;

	NamedTexture() : tex(nullptr) {
		;
	}

	NamedTexture(const std::string& name, ITexture* tex) : name(name), tex(tex) {
		;
	}

	std::string getR(const Var uv) const {
		return "texture2D(" + name + ", " + uv.name + ").r";
	}

	std::string getRGB(const Var uv) const {
		return "texture2D(" + name + ", " + uv.name + ").rgb";
	}

	std::string getRGBA(const Var uv) const {
		return "texture2D(" + name + ", " + uv.name + ").rgba";
	}

	std::string getUV(const Var uv) const {
		return "(texture2D(" + name + ", " + uv.name + ").rg * 2.0 - vec2(1.0, 1.0))";
	}

	std::string getBumpMapNormal(const Var uv) const {
		return "(texture2D(" + name + ", " + uv.name + ").rgb * 2.0 - vec3(1.0, 1.0, 1.0))";
	}

};

#endif // NAMEDTEXTURE_H
