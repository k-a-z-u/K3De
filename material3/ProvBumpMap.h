#ifndef PROVBUMPMAP_H
#define PROVBUMPMAP_H

#include "CodePart.h"
#include "ProvNormal.h"
#include "ProvColor.h"
#include "ProvUV.h"

class ProvBumpMap : public ProvNormal {
private:
	ProvNormal* parent = nullptr;
public:
	ProvBumpMap(ProvNormal* parent) : parent(parent) {;}
};

/** provide a bump-mapped normal using a bump-map texture */
class ProvBumpMapTexture : public ProvBumpMap {

private:

	ProvColor* color;

	const Var VAR_VERT_OUT_NORMAL_ROT_MAT = Var(VarScope::OUTBOUND, VarType::MAT3, VarPrecission::MEDIUM, "normalRotMat");
	const Var VAR_FRAG_IN_NORMAL_ROT_MAT = Var(VarScope::INBOUND, VarType::MAT3, VarPrecission::MEDIUM, "normalRotMat");

	Var vNormal = Var::getRandom(VarType::VEC3, "normal", VarScope::LOCAL, VarPrecission::MEDIUM);

public:

	/** ctor */
	ProvBumpMapTexture(ProvNormal* parent, ProvColor* color) : ProvBumpMap(parent), color(color) {;}

	Var getVar() const override {
		return vNormal;
	}

	CodePart getFragment() const override {

		CodePart code = color->getFragmentOnce();

		// input
		code.addGlobalVariable(VAR_FRAG_IN_NORMAL_ROT_MAT);
		code.addUsage(VAR_FRAG_IN_NORMAL_ROT_MAT.name);
		//code.addUsage(tex.name);

		// get the original normal
		//Var varNormal = Var::getRandom(VarType::VEC3, "origNormal", VarScope::LOCAL, VarPrecission::MEDIUM);
		//code += parent->getNormalFragment();
		//code.assignedTo(varNormal);

		// get the adjusted normal
		//Var varVec = Var::getRandom(VarType::VEC3, "bumpMapVec", VarScope::LOCAL, VarPrecission::MEDIUM);
		//code.main += CodeLine(vNormal, "normalRotMat * " + tex->getBumpMapNormal()).terminate();
		code.main += CodeLine(vNormal, "normalRotMat * ( " + color->getVar().name + ".rgb * 2.0 - vec3(1.0) )").terminate();

		// done
		return code;

	}

	CodePart getVertex() const override {

		CodePart code;

		// output
		code.addGlobalVariable(VAR_VERT_OUT_NORMAL_ROT_MAT);
		code.addUsage(VAR_VERT_OUT_NORMAL_ROT_MAT.name);

		// main() code
		code.main += "tangent_M = normalize(mat3(M) * tangent);";
		code.main += "normalRotMat = mat3(tangent_M, normalize(cross(normal_M, tangent_M)), normal_M);";

		// used variables
		code.addUsage("normal_M");
		code.addUsage("tangent_M");
		code.addUsage("tangent");

		return code;

	}

};

#endif // PROVBUMPMAP_H
