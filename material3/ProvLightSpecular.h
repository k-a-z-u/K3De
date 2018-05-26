#ifndef PROVLIGHTSPECULAR_H
#define PROVLIGHTSPECULAR_H

#include "ProvUV.h"

class ProvSpecular {

private:

	const Var varSpecColor = Var::getRandom(VarType::VEC3, "specColor", VarScope::LOCAL, VarPrecission::LOW);
	const Var varSpecShininess = Var::getRandom(VarType::FLOAT, "specShine", VarScope::LOCAL, VarPrecission::LOW);

public:

	Var getSpecColor() const {return varSpecColor;}
	Var getSpecShininess() const {return varSpecShininess;}

	virtual CodePart getCode() const = 0;

};

class ProvSpecularFixed : public ProvSpecular {

private:

	Vec3 color;
	float shininess;


public:

	ProvSpecularFixed(Vec3 color, float shininess) : color(color), shininess(shininess) {
		;
	}

	virtual CodePart getCode() const {
		CodePart code;
		code.main += CodeLine(getSpecColor(), "vec3(" + std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + ")").terminate();
		code.main += CodeLine(getSpecShininess(), std::to_string(shininess)).terminate();
		return code;
	}

};

class ProvSpecularTexture : public ProvSpecular {

private:

	NamedTexture tex;
	ProvUV* uv;
	float shininess;


public:

	ProvSpecularTexture(NamedTexture tex, ProvUV* uv, float shininess) : tex(tex), uv(uv), shininess(shininess) {
		;
	}

	virtual CodePart getCode() const {

		Var vUV = uv->getVar();

		CodePart code;

		code += uv->getOneTimePrecalculations();
		code += uv->getCalculationOnce();

		code.preMain += CodeLine(getSpecColor(), "texture2D(" + tex.name + "," + vUV.name + ").rrr").terminate();
		code.preMain += CodeLine(getSpecShininess(), std::to_string(shininess)).terminate();
		code.addUsage(tex.name);
		return code;

	}

};



#endif // PROVLIGHTSPECULAR_H
