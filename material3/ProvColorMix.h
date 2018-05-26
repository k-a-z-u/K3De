#ifndef PROVCOLORMIX_H
#define PROVCOLORMIX_H


#include "ProvColor.h"
#include "ProvTexture.h"
#include "HasOnce.h"
#include "HasTexture.h"

#include "ProvUV.h"
#include "CodePart.h"
#include "../math/Vector.h"

/** mix two colors into using a fixed amount */
class ProvColorMix : public ProvColor {

private:

	ProvColor* col1 = nullptr;
	ProvColor* col2 = nullptr;
	float amount;

	Var var = Var::getRandom(VarType::VEC4, "colormix", VarScope::LOCAL, VarPrecission::LOW);

public:

	/** ctor with the texture to use */
	ProvColorMix(ProvColor* col1, ProvColor* col2, float amount) : col1(col1), col2(col2), amount(amount) {

	}

	Var getVar() const override {
		return var;
	}

	CodePart getVertex() const override {
		return CodePart();
	}

	CodePart getFragment() const override {

		CodePart code;
		code += col1->getFragmentOnce();
		code += col2->getFragmentOnce();

		// get the color using mix(a,b,amount);
		code.main += var.getAssigned("mix(" + col1->getVar().name + ", " + col2->getVar().name + ", " + std::to_string(amount) + ");");
		//code.main += var.getAssigned(col1->getVar().name + " * " + col2->getVar().name + ";");

		return code;

	}

};


/** mix two colors using a mix-mask (red-channel) */
class ProvColorMixMask : public ProvColor {

private:

	ProvColor* col1 = nullptr;
	ProvColor* col2 = nullptr;
	ProvColor* colM = nullptr;
	float amount;

	Var var = Var::getRandom(VarType::VEC4, "colormix", VarScope::LOCAL, VarPrecission::LOW);

public:

	/** ctor with the texture to use */
	ProvColorMixMask(ProvColor* col1, ProvColor* col2, ProvColor* colM) : col1(col1), col2(col2), colM(colM) {

	}

	Var getVar() const override {
		return var;
	}

	CodePart getVertex() const override {
		return CodePart();
	}

	CodePart getFragment() const override {

		CodePart code;
		code += col1->getFragmentOnce();
		code += col2->getFragmentOnce();
		code += colM->getFragmentOnce();

		// get the color using mix(a,b,amount);
		code.main += var.getAssigned("mix(" + col1->getVar().name + ", " + col2->getVar().name + ", " + colM->getVar().name + ".r);");

		return code;

	}

};


#endif // PROVCOLORMIX_H
