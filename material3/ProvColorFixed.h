#ifndef PROVCOLORFIXED_H
#define PROVCOLORFIXED_H

/** provider a fixed color within the shader */
class ProvColorFixed : public ProvColor {

private:

	Vec4 color;

	Var var = Var::getRandom(VarType::VEC4, "color", VarScope::LOCAL, VarPrecission::LOW);

public:

	/** ctor */
	ProvColorFixed(Vec4 color) : color(color) {;}

	Var getVar() const override {
		return var;
	}

	CodePart getVertex() const override {
		return CodePart();
	}

	CodePart getFragment() const override {

		CodePart code;

		// get the color
		code.main += CodeLine(var,
			"vec4(" + std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," + std::to_string(color.a) + ")"
		).terminate();

		return code;

	}

};


#endif // PROVCOLORFIXED_H
