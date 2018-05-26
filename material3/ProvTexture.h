#ifndef PROVTEXTURE_H
#define PROVTEXTURE_H

#include "NamedTexture.h"
#include "ProvUV.h"

class ProvTexture {

private:

	ProvUV* uv;

	NamedTexture nt;

	Vec2 tiling;

public:

	ProvTexture(ProvUV* uv, const NamedTexture& nt) : uv(uv), nt(nt), tiling(1,1) {
		;
	}

	ProvTexture(ProvUV* uv, const NamedTexture& nt, Vec2 tiling) : uv(uv), nt(nt), tiling(tiling) {
		;
	}

	const std::string& getName() const {
		return nt.name;
	}

	CodePart getCode() const {

		// ensure the UV coordinate is available
		//Var vUV = uv->getVar();
		CodePart code;

		code += uv->getOneTimePrecalculations();
		code += uv->getCalculationOnce();

		// track the usage
		code.addUsage(nt.name);

		return code;

	}

	std::string get() const {
		const Var vUV = uv->getVar();
		std::string sTile = (tiling.x != 1 || tiling.y != 1) ? (" * vec2(" + std::to_string(tiling.x) + "," + std::to_string(tiling.y) + ")") : "";
		return "texture2D(" + nt.name + ", " + vUV.name + sTile + ")";
	}

	std::string getRGBA() const {
		return get() + ".rgba";
	}

	std::string getUV() const {
		return "(" + get() + ".rg * 2.0 - vec2(1.0, 1.0) )";
	}

	std::string getBumpMapNormal() const {
		return "(" + get() + ".rgb * 2.0 - vec3(1.0, 1.0, 1.0) )";
	}

};

#endif // PROVTEXTURE_H
