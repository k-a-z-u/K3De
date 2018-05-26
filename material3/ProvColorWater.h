#ifndef PROVCOLORWATER_H
#define PROVCOLORWATER_H


#include "ProvColor.h"
#include "HasOnce.h"
#include "HasTexture.h"


#include "ProvUV.h"
#include "CodePart.h"
#include "../math/Vector.h"




/**
 * provides a combination of reflection and refraction
 */
class ProvColorWater : public ProvColor {

private:

	/** output variable */
	Var var = Var::getRandom(VarType::VEC4, "colorWater", VarScope::LOCAL, VarPrecission::LOW);

	ProvUV* uvReflect;
	ProvUV* uvRefract;
	ProvUV* uvDepth;

	NamedTexture texReflect;
	NamedTexture texRefract;
	NamedTexture texDepth;

public:

	/** ctor */
	ProvColorWater(ProvUV* uvReflect, ProvUV* uvRefract, ProvUV* uvDepth) :
		uvReflect(uvReflect), uvRefract(uvRefract), uvDepth(uvDepth),
		texReflect("texReflect", nullptr), texRefract("texRefract", nullptr), texDepth("texWaterDepth", nullptr) {
		;
	}

	Var getVar() const override {
		return var;
	}

	CodePart getVertex() const override {
		CodePart code;
		code.addUsage(FRAG_VERTEX_SCREEN_POS);
		code.main += CodeLine("vertex_S = gl_Position").terminate();
		return code;
	}

	CodePart getFragment() const override {

		CodePart code;

		// get UV coordinates for reflection, refraction and water-depth texture

		//code += uvMod->getCodeOnce();
		code += uvReflect->getOneTimePrecalculations();
		code += uvRefract->getOneTimePrecalculations();
		code += uvDepth->getOneTimePrecalculations();

		code += uvReflect->getCalculationOnce();
		code += uvRefract->getCalculationOnce();
		code += uvDepth->getCalculationOnce();

		// texture coordinates
		//Var vUVRefract = Var::getRandom(VarType::VEC2, "uvRefract", VarScope::LOCAL, VarPrecission::MEDIUM);
		//Var vUVReflect = Var::getRandom(VarType::VEC2, "uvReflect", VarScope::LOCAL, VarPrecission::MEDIUM);
		Var vUVReflect = uvReflect->getVar();
		Var vUVRefract = uvRefract->getVar();
		Var vUVDepth = uvDepth->getVar();
		//code.main += CodeLine(vUVRefract, std::string() + "(" + FRAG_VERTEX_SCREEN_POS + ".xy / " + FRAG_VERTEX_SCREEN_POS + ".w) * 0.5 + vec2(0.5, 0.5)").terminate();
		//code.main += CodeLine(vUVReflect, "vec2("+vUVRefract.name+".x, 1.0-"+vUVRefract.name+".y)").terminate();

		// modify
		//code.main += CodeLine(vUVRefract.name + " = " + uvMod->modifyInplace(vUVRefract)).terminate();
		//code.main += CodeLine(vUVReflect.name + " = " + uvMod->modifyInplace(vUVReflect)).terminate();

		// TODO: vec3??
		// texture values
		Var vColRefract = Var::getRandom(VarType::VEC4, "cRefract", VarScope::LOCAL, VarPrecission::LOW);
		Var vColReflect = Var::getRandom(VarType::VEC4, "cReflect", VarScope::LOCAL, VarPrecission::LOW);
		Var vColDepth =   Var::getRandom(VarType::VEC4, "cDepth", VarScope::LOCAL, VarPrecission::LOW);
		code.main += CodeLine(vColRefract, texRefract.getRGBA(vUVRefract)).terminate();
		code.main += CodeLine(vColReflect, texReflect.getRGBA(vUVReflect)).terminate();
		code.main += CodeLine(vColDepth,   texDepth.getRGBA(vUVDepth)).terminate();

		if (1 == 1) {
		// mix using water depth texture
			code.main += "float depth = clamp(" + vColDepth.name + ".r * 10.0, 0.0, 1.0);";
			code.main += "float darken = 1.0;";//1.0 - depth;";
			code.main += "float mixi = clamp(depth, 0.0, 1.0);";
			code.main += CodeLine(var, std::string() + "mix(" + vColRefract.name + " * darken," + vColReflect.name + ", mixi)").terminate();
		} else if (1 == 0) {
			// 50:50 mix
			//code.main += CodeLine(var, std::string() + "mix(" + vColRefract.name + "," + vColReflect.name + ", 0.5)").terminate();
		}

		//code.main += CodeLine(var, "vec4(depth, depth, depth, 1)").terminate();


		//code.main += CodeLine(var, "vec4(depth, depth, depth, 1.0)" ).terminate();

//					// texture mix
//					"	vec3 N = vec3(0,1,0);\n" +
//					"	vec3 E = normalize(surfaceToEye_M);\n" +
//					"	float waterTexMix = clamp(pow(dot(N,E), 2.0), 0.1, 0.9);\n" +
//					"	return mix(cReflect, cRefract, waterTexMix);\n" +
//					"}", {FRAG_VERTEX_SCREEN_POS, "surfaceToEye_M", "texReflect", "texRefract"}, {}
//				);

		code.addGlobalVariable(Var(VarScope::UNIFORM, VarType::TEXTURE_2D, VarPrecission::MEDIUM, texReflect.name));
		code.addGlobalVariable(Var(VarScope::UNIFORM, VarType::TEXTURE_2D, VarPrecission::MEDIUM, texRefract.name));
		code.addGlobalVariable(Var(VarScope::UNIFORM, VarType::TEXTURE_2D, VarPrecission::MEDIUM, texDepth.name));

		code.addUsage(FRAG_VERTEX_SCREEN_POS);
		code.addUsage(texReflect.name);
		code.addUsage(texRefract.name);
		code.addUsage(texDepth.name);

		return code;

	}

};

#endif // PROVCOLORWATER_H
