#ifndef PROVUV_H
#define PROVUV_H

#include "CodePart.h"

/** provides UV coordinates */
class ProvUV {
protected:

	bool oneTimePrecalcAdded = false;
	bool calculationAdded = false;

	/** if the UV-provider has one-time calculations, independent of the to-be-modified uv, add them here */
	virtual CodePart _getOneTimePrecalculations() const = 0;

	/** get the code (if any) needed to calculate the UV */
	virtual CodePart _getCalculation() const = 0;

public:

	/** get the variable that contains the calculated UV */
	virtual Var getVar() const = 0;

	/** if the UV-provider has one-time calculations, independent of the to-be-modified uv, add them here */
	CodePart getOneTimePrecalculations() {
		if (oneTimePrecalcAdded) {return CodePart();}
		oneTimePrecalcAdded = true;
		return _getOneTimePrecalculations();
	}

	/** get the code that builds the UV-variable */
	CodePart getCalculationOnce() {
		if (calculationAdded) {return CodePart();}
		calculationAdded = true;
		return _getCalculation();
	}

};

/** provides the default UV coordinates from the model */
class ProvUVDefault : public ProvUV {

public:

	CodePart _getOneTimePrecalculations() const override {
		return CodePart();
	}

	CodePart _getCalculation() const override {
		CodePart code;
		//code.mainPre += CodeLine(FRAG_VERTEX_UV).terminate();
		code.addUsage(FRAG_VERTEX_UV);
		return code;
	}

	Var getVar() const override {
		return VAR_FRAG_IN_VERTEX_UV;
	}

};


/** screen coordinates for water refraction */
class ProvUVRefract : public ProvUV {

	/** output variable */
	Var var = Var::getRandom(VarType::VEC2, "uvRefract", VarScope::LOCAL, VarPrecission::MEDIUM);

public:

	/** get the variable that contains the calculated UV */
	Var getVar() const override {return var;}

	CodePart _getOneTimePrecalculations() const override {
		return CodePart();
	}

	virtual CodePart _getCalculation() const override {
		CodePart code;
		code.main += CodeLine(var, std::string() + "(" + FRAG_VERTEX_SCREEN_POS + ".xy / " + FRAG_VERTEX_SCREEN_POS + ".w) * 0.5 + vec2(0.5, 0.5)").terminate();
		return code;
	}

};

/** screen coordinates for water reflection */
class ProvUVReflect : public ProvUV {

	/** output variable */
	Var var = Var::getRandom(VarType::VEC2, "uvReflect", VarScope::LOCAL, VarPrecission::MEDIUM);

public:

	/** get the variable that contains the calculated UV */
	Var getVar() const override {return var;}

	CodePart _getOneTimePrecalculations() const override {
		return CodePart();
	}

	virtual CodePart _getCalculation() const override {
		CodePart code;
		//code.main += CodeLine(var, "vec2("+refract.name+".x, 1.0-"+refract.name+".y)").terminate();
		code.main += CodeLine(var, std::string() + "(" + FRAG_VERTEX_SCREEN_POS + ".xy / " + FRAG_VERTEX_SCREEN_POS + ".w) * 0.5 + vec2(0.5, 0.5)").terminate();
		code.main += CodeLine(var.name + ".y = 1.0 - " + var.name + ".y").terminate();
		return code;
	}

};






/** modify uv coordinates */
class ProvUVMod : public ProvUV {

protected:

	ProvUV* parent = nullptr;

public:

	/** ctor with the uv-provider to modify */
	ProvUVMod(ProvUV* parent) : parent(parent) {;}

	virtual std::string modifyInplace(const Var uvIn) const = 0;

};


/** modify uv coordinate based on the time */
class ProvUVModTime : public ProvUVMod {

public:

	enum Mode {
		ADD,
	};

protected:

	Mode mode;

	Vec2 scale;

	/** return variable */
	Var var = Var::getRandom(VarType::VEC2, "uvModded_time", VarScope::LOCAL, VarPrecission::MEDIUM);

public:

	/** ctor with the uv-provider to modify */
	ProvUVModTime(ProvUV* parent, const Mode mode, const Vec2 scale) : ProvUVMod(parent), mode(mode), scale(scale) {
		;
	}

	Var getVar() const override {
		return var;
	}

	CodePart _getOneTimePrecalculations() const override {
		return parent->getOneTimePrecalculations();
	}

	CodePart _getCalculation() const override {

		// parent calculations
		Var vUVorig = parent->getVar();
		CodePart code = parent->getCalculationOnce();

		// my result
		code.main += CodeLine(var, modifyInplace(vUVorig)).terminate();

		// usage
		code.addUsage(FRAG_TIME);

		return code;

	}

	std::string modifyInplace(const Var uvIn) const override {
		const std::string vTime = std::string() + "vec2(" + FRAG_TIME + "," + FRAG_TIME + ")";
		const std::string vScale = std::string() + "vec2(" + std::to_string(scale.x) + "," + std::to_string(scale.y) + ")";
		return (uvIn.name + " + " + vTime + " * " + vScale);
	}

};





#endif // PROVUV_H
