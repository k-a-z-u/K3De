#ifndef PROVLIGHTED_H
#define PROVLIGHTED_H

#include "CodePart.h"
#include "NamedTexture.h"
#include "ProvUV.h"
#include "../math/Vector.h"

#include "ProvLightShadow.h"
#include "ProvLightSpecular.h"
#include "ProvLightHelper.h"

class ProvLighted {

public:

	/** the variable that will contain the lighted result */
	virtual Var getVar() const = 0;

	virtual CodePart getFragment(Var color, Var normal) const = 0;

	virtual CodePart getVertex() const = 0;

	virtual void setShadowing(ProvShadow* prov) = 0;

};









/** default lighting model */
class ProvLightedPhong : public ProvLighted {

private:

	float ambient = 0.2;

	ProvSpecular* specular = nullptr;

	ProvShadow* shadow = nullptr;

	Var vRes = Var::getRandom(VarType::VEC4, "lighted_phong", VarScope::LOCAL, VarPrecission::LOW);

public:

	/** set the amount of ambient lighting */
	void setAmbient(const float amount) {
		this->ambient = amount;
	}

	/** set the specular provider (if any) */
	void setSpecular(ProvSpecular* prov) {
		this->specular = prov;
	}

	/** set the shadow provider (if any) */
	void setShadowing(ProvShadow* prov) override {
		this->shadow = prov;
	}

	Var getVar() const override {
		return vRes;
	}

	CodePart getVertex() const override {

		CodePart code;

		if (shadow) {
			code += shadow->getVertex();
		}

		return code;

	}

	CodePart getFragment(Var color, Var normal) const override {

		CodePart code;

//		if (specular) {

//			Var vSpecCol(VarScope::LOCAL, VarType::VEC3, VarPrecission::LOW, "specularColor");
//			code.main += CodeLine(vSpecCol, "vec3(1.0) * 0.200000f").terminate();	// TODO

//			Var vSpecShine(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "specularShininess");
//			code.main += CodeLine(vSpecShine, "15.000000f").terminate();		// TODO

//		}

		Var vEye(VarScope::LOCAL, VarType::VEC3, VarPrecission::MEDIUM, "E");
		code.main += CodeLine(vEye, "normalize(surfaceToEye_M)").terminate();
		code.addUsage("surfaceToEye_M");

		// output color. start with alpha only
		//Var vRes = Var::getRandom(VarType::VEC4, "colorRes", VarScope::LOCAL, VarPrecission::LOW);
		//code.main += CodeLine(vRes, "vec4(0.0, 0.0, 0.0, "+color.name+".a)").terminate();

		// ambient color
		//code.main += vRes.name + ".rgb += " + getAmbientColor() + " * " + color.name + ".rgb;";	// TODO ambient coefficient?

		// output color. start with orig alpha and ambient color
		code.main += CodeLine(vRes, "vec4(" + ProvLightHelper::getAmbientColor() + " * " + color.name + ".rgb, "+color.name+".a)").terminate();


		// specular lighting?
		if (specular) {
			// calculate material's specular-color/shininess once.
			// each light will refer to both values
			code += specular->getCode();
		}

		// process each light
		// UNROLLED
		for (int i = 0; i < MAX_LIGHTS; ++i) {
			code += get(std::to_string(i), color, normal, vRes);
		}

		// AS LOOP
//		code.main += "for (int i = 0; i < " + std::to_string(MAX_LIGHTS) + "; ++i) {";
//		code += get("i", color, normal, vRes);
//		code.main += "}";

		return code;

	}

	CodePart get(const std::string iStr, Var color, Var normal, Var vRes) const {

		CodePart code;

		// ignore?
		code.main += "if (" + ProvLightHelper::isLightEnabled(iStr) + ") {";

			Var vLightStrength = Var::getRandom(VarType::FLOAT, "lightStr", VarScope::LOCAL, VarPrecission::LOW);
			code.main += CodeLine(vLightStrength, "0.0").terminate();

			Var vL(VarScope::LOCAL, VarType::VEC3, VarPrecission::MEDIUM, "L");
			code.main += CodeLine(vL, "vec3(0.0)").terminate();

			Var vTheta(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "theta");

			// point light?
			code.main += "if (" + ProvLightHelper::isPointLight(iStr) + ") {";

				// distance towards the light
				Var vDistToLight = Var::getRandom(VarType::FLOAT, "dToLight", VarScope::LOCAL, VarPrecission::LOW);
				code.main += CodeLine(vDistToLight, "length("+ProvLightHelper::getLightPos(iStr)+" - vertex_M)").terminate();
				code.addUsage("vertex_M");
				code.addUsage("lights");

				// light's strength
				std::string dist = std::string() +
						//"lights.light["+std::to_string(light)+"].impact / "+
						"1.0f / " +
						"(1.0f + " +
							"(lights.light["+iStr+"].attenLin * "+vDistToLight.name+") + "+
							"(lights.light["+iStr+"].attenQuad * "+vDistToLight.name+" * "+vDistToLight.name+")"+
						")";

				code.main += CodeLine(vLightStrength.name + " = " + dist).terminate();
				code.main += CodeLine(vL.name + " = normalize("+ProvLightHelper::getLightPos(iStr)+" - vertex_M)").terminate();


			code.main += "}";

			// directional light?
			code.main += "if (" + ProvLightHelper::isDirectionalLight(iStr) + ") {";

				// fixed light strength
				code.main += CodeLine(vLightStrength.name + " = 1.0").terminate();
				code.main += CodeLine(vL.name + " = -normalize("+ProvLightHelper::getLightPos(iStr)+")").terminate();

			code.main += "}";

			// shadowing?
			if (shadow) {
				code += shadow->getFragment(vLightStrength, iStr);
			}

			// still visible?
			code.main += "if ("+vLightStrength.name+" > 0.002) {";

				// diffuse color
				code.main += CodeLine(vTheta, "dot("+normal.name+", L)").terminate();
				code.main += vRes.name + ".rgb += "+color.name+".rgb * "+ProvLightHelper::getLightColor(iStr)+" * "+vLightStrength.name+" * max(0.0f, theta);";
				code.addUsage("vertex_M");
				code.addUsage("lights");

				// specular?
				if (specular) {

					code.main += "if (theta > 0.0) {";

						Var vR(VarScope::LOCAL, VarType::VEC3, VarPrecission::MEDIUM, "R");
						Var vCosAlpha(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "cosAlpha");

						code.main += CodeLine(vR, "reflect(-L, "+normal.name+")").terminate();
						code.main += CodeLine(vCosAlpha, "max( 0.0f, dot(E, R) )").terminate();
						code.main += vRes.name + ".rgb += "+specular->getSpecColor().name+" * "+ProvLightHelper::getLightColor(iStr)+" * pow(cosAlpha, "+specular->getSpecShininess().name+") * "+vLightStrength.name+";";

					code.main += "}";

				}

			code.main += "}";

		//code.main += "} else {break;}";
		code.main += "}";	// faster?!

		return code;

	}

};


/** default lighting model */
class ProvLightedNone : public ProvLighted {

private:

	Var vRes = Var::getRandom(VarType::VEC4, "lighted_none", VarScope::LOCAL, VarPrecission::LOW);

public:

	Var getVar() const override {
		return vRes;
	}

	CodePart getVertex() const override {
		CodePart code;
		return code;
	}

	CodePart getFragment(Var color, Var normal) const override {

		CodePart code;

		// output color. start with orig alpha and ambient color
		code.main += CodeLine(vRes, color.name).terminate();

		return code;

	}

	void setShadowing(ProvShadow* shadow) override {

	}

};

#endif // PROVLIGHTED_H
