#ifndef PROVCOLORTEXTURE_H
#define PROVCOLORTEXTURE_H

#include "ProvColor.h"
#include "ProvTexture.h"
#include "HasOnce.h"
#include "HasTexture.h"

#include "ProvUV.h"
#include "CodePart.h"
#include "../math/Vector.h"

/** sample a color from a texture */
class ProvColorTexture : public ProvColor {

private:

	ProvTexture* tex = nullptr;

	Var var = Var::getRandom(VarType::VEC4, "color", VarScope::LOCAL, VarPrecission::LOW);

public:

	/** ctor with the texture to use */
	ProvColorTexture(ProvTexture* tex) : tex(tex) {

	}

	Var getVar() const override {
		return var;
	}

	CodePart getVertex() const override {
		return CodePart();
	}

	CodePart getFragment() const override {

		CodePart code = tex->getCode();

		// get the color
		code.main += CodeLine(var, tex->getRGBA()).terminate();


		return code;

	}

};

#endif // PROVCOLORTEXTURE_H
