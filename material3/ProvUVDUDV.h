#ifndef PROVUVDUDV_H
#define PROVUVDUDV_H

#include "ProvUV.h"
#include "ProvColor.h"
#include "../Assert.h"

/** modify uv coordinates based on a UV-texture */
class ProvUVModUVDisplace : public ProvUVMod {

private:

	ProvColor* color = nullptr;
	float sx = 1.0;
	float sy = 1.0;

	Var var = Var::getRandom(VarType::VEC2, "uvModded_dudv", VarScope::LOCAL, VarPrecission::MEDIUM);
	Var vUVadd = Var::getRandom(VarType::VEC2, "uvAdd_dudv");

public:

	/** ctor */
	ProvUVModUVDisplace(ProvUV* parent, ProvColor* color, const float sx = 1, const float sy = 1) : ProvUVMod(parent), color(color), sx(sx), sy(sy) {
		Assert::isNotNull(color, "color-provider must not be null");
	}

	CodePart _getOneTimePrecalculations() const override {
		CodePart code;
		code += parent->getOneTimePrecalculations();
		code += color->getFragmentOnce();
		return code;
	}

	CodePart _getCalculation() const override {

		CodePart code;
		code += parent->getCalculationOnce();
		//code += tex->getCode();

		// my calculations
		//code.main += CodeLine(vUVadd, tex->getUV()).terminate();
		code.main += CodeLine(vUVadd, color->getVar().name + ".rg * 2.0 - vec2(1.0, 1.0)").terminate();

		// my result
		code.main += CodeLine(var, modifyInplace(parent->getVar())).terminate();

		// usage
		//code.addUsage(tex->getName());

		return code;

	}

	Var getVar() const override {
		return var;
	}

	std::string modifyInplace(const Var uvIn) const override {
		std::string line = uvIn.name + " + " + vUVadd.name;
		if (sx != 1 || sy != 1) {line += " * vec2(" + std::to_string(sx) + "," + std::to_string(sy) + ")";}
		return line;
	}

};

#endif // PROVUVDUDV_H
