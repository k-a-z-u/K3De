#ifndef PROVLIGHTSHADOW_H
#define PROVLIGHTSHADOW_H

#include "CodePart.h"
#include "ProvLightHelper.h"
#include <random>

class ProvShadow {

private:

	float quality;
	int mode = 1;

	std::minstd_rand gen;

public:

	ProvShadow(float quality) : quality(quality) {
		;
	}

	bool testAdded = false;

	CodePart getVertex() {

		CodePart code;

		// matrix to convert model-coordinates to shadow-map-texture coordinates
		code.addGlobalVariable(VAR_VERT_OUT_SHADOW_MAP_COORD);
		code.addUsage(VAR_VERT_OUT_SHADOW_MAP_COORD.name);

		if (!testAdded) {
			testAdded = true;
			// testing
			code.functions += R"(
			vec4 worldToTexture(vec4 src) {
			  vec4 tmp = normalize(src/src.w);
			  float u = (atan(tmp.z, tmp.x) / (3.1415926535));
			  float v = asin(tmp.y) / (3.1415926535 / 2.0);
			  //u *= 0.99;
			  //v *= 0.99;
			  return vec4(u,v,src.z/src.w,1.0);
			}
			)";
		}

		// calculate shadow coordinate for each light
		for (int i = 0; i < MAX_LIGHTS; ++i) {

			code.main +=	std::string()+
							"if (" + ProvLightHelper::lightCastsShadows(std::to_string(i)) + ") {"+
								"shadowMapCoord[" + std::to_string(i) + "] = " +
								"lights.light[" + std::to_string(i) + "].pvShadow * vec4(vertex_M, 1.0);"
								//"worldToTexture(  lights.light[" + std::to_string(i) + "].pvShadow * vec4(vertex_M, 1.0)  );" +	// TESTING
							"}";

			code.addUsage("vertex_M");
			//vert.setUsed("lights");
			code.addUsage("shadowMapCoord["+std::to_string(MAX_LIGHTS)+"]");	// UGLY

		}

		return code;

	}

	bool fragAdded = false;

	std::string shadowMapValDebug(Var sc) const {

		//return sc.name + ".z * 10.0";
		return	"texture(shadowMap, "+sc.name+".st).r ";

	}

	std::string isShadowed(Var sc, const float ox, const float oy) const {

		const float scale = 0.005f;
		std::string o = "(vec2(" + std::to_string(ox*scale) + "," + std::to_string(oy*scale) + "))";

		return	"\t\t"
				"( texture(shadowMap, "+sc.name+".st + " + o + ").r "	// smallest distance from this given (x,y) to the light
				" < " +
				sc.name + ".z "										// current distance to the light
				" ? (0.10f) : (1.0f))"							// output
		;
	}

	std::string isShadowedRand(Var sc, float modx, float mody) const {

		//std::string x = "mod(" + sc.name + ".x + " + std::to_string(modx) + "f, 0.01)-0.005";
		//std::string y = "mod(" + sc.name + ".y + " + std::to_string(mody) + "f, 0.01)-0.005";

		//std::string x = "fract(" + sc.name + ".x * 1337 + " + std::to_string(modx) + "f)-0.5)";
		//std::string y = "fract(" + sc.name + ".y * 1337 + " + std::to_string(mody) + "f)-0.5)";

		const float strength = 0.02f;
		const std::string vecOffsetDyn = "vec2(" + std::to_string(modx) + "f," + std::to_string(mody) + "f)";
		const std::string vecOffsetFixed = "31337.0f";
		//std::string xy = "fract(" + vecOffset + ")";
		const std::string xy = "(fract(" + sc.name + ".st * "+vecOffsetFixed+" + "+vecOffsetDyn+" ) - 0.5f) * " + std::to_string(strength) + "f";
		//std::string xy = "(fract("+vecOffsetFixed+" + "+vecOffsetDyn+" ) - 0.5f) * 0.01f";

		return	"\t\t"
				"( texture( shadowMap, "+sc.name+".st + "+xy+" ).r "	// smallest distance from this given (x,y) to the light
				" < " +
				sc.name + ".z "										// current distance to the light
				" ? (0.10f) : (1.0f))"							// output
		;

	}

	CodePart getFragmentOnce() {

		if (fragAdded) {return CodePart();}
		fragAdded = true;

		CodePart code;

		code.functions += R"(
		vec3 worldToTexture(vec3 src) {
			vec3 tmp = normalize(src.xyz);
			float u = (atan(tmp.z, tmp.x) / (3.1415926535));
			float v = asin(tmp.y) / (3.1415926535 / 2.0);
			u = u * 0.5 + 0.5;
			v = v * 0.5 + 0.5;
			float z = -src.z / 4.5f;
			return vec3(u, v, z);
		}
		)";

		code.functions += "float getShadowAmount(sampler2D shadowMap, int lightIdx) {";

		// get texture coordinate within the depth-texture, rendered form the idx-th light's perspective
		Var vShadowCoord = Var::getRandom(VarType::VEC4, "shadowCoord", VarScope::LOCAL, VarPrecission::MEDIUM);
		code.functions += CodeLine(vShadowCoord, "shadowMapCoord[lightIdx]").terminate();

		// if we are behind the light, no shadowing is possible (prevents artifacts)
		code.functions += "if ("+vShadowCoord.name+".w <= 0.0) {return 1.0;}";

		// adjust
		Var vShadowCoordScaled = Var::getRandom(VarType::VEC3, "sc", VarScope::LOCAL, VarPrecission::MEDIUM);
		code.functions += CodeLine(vShadowCoordScaled, vShadowCoord.name+".xyz / "+vShadowCoord.name+".w").terminate();

		// add some clearance to prevent almost-equal-z artifcats
		code.functions += vShadowCoordScaled.name + ".z -=  0.01f;";

		// lowp float s = 0.2 / shadowCoord.w;


		if (1 == 0) {

			// DEBUG

			Var xxx = Var::getRandom(VarType::VEC3, "sc", VarScope::LOCAL, VarPrecission::MEDIUM);

			code.functions += CodeLine(xxx, "worldToTexture( (lights.light[lightIdx].pvShadow * vec4(vertex_M, 1.0)).xyz )").terminate();
//			code.functions += CodeLine(xxx, vShadowCoord.name).terminate();

			code.functions += "return (";
			//code.functions += shadowMapValDebug(xxx);
			code.functions += isShadowed(xxx, 0, 0);
			code.functions += ");";
			code.functions += "}";

		} else if (mode == 0) {

			int used = 0;
			code.functions += "return (";

				code.functions += isShadowed(vShadowCoordScaled, 0.0f,  0.0f);	++used;

				if (quality >= 0.25) {
					code.functions += "+" + isShadowed(vShadowCoordScaled,-1.0f,  0.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled,+1.0f,  0.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled, 0.0f, +1.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled, 0.0f, -1.0f);	++used;
				}

				if (quality >= 0.50) {
					code.functions += "+" + isShadowed(vShadowCoordScaled,+1.0f, +1.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled,-1.0f, +1.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled,+1.0f, -1.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled,-1.0f, -1.0f);	++used;
				}

				if (quality >= 0.75) {
					code.functions += "+" + isShadowed(vShadowCoordScaled,-2.0f,  0.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled,+2.0f,  0.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled, 0.0f, +2.0f);	++used;
					code.functions += "+" + isShadowed(vShadowCoordScaled, 0.0f, -2.0f);	++used;
				}

			code.functions += ") / " + std::to_string(used) + ".0f;";
			code.functions += "}";

		} else if (mode == 1) {

			// point-light
			//Var xxx = Var::getRandom(VarType::VEC3, "sc", VarScope::LOCAL, VarPrecission::MEDIUM);
			//code.functions += CodeLine(xxx, "worldToTexture( (lights.light[lightIdx].pvShadow * vec4(vertex_M, 1.0)).xyz )").terminate();
			//vShadowCoordScaled = xxx;

			int used = 0;
			code.functions += "return (";

				std::uniform_real_distribution<float> dist(0.0, 1.0);

				code.functions += isShadowedRand(vShadowCoordScaled, dist(gen), dist(gen));	++used;

				for (int i = 1; i < quality * 10; ++i) {
					code.functions += "+" + isShadowedRand(vShadowCoordScaled, dist(gen), dist(gen));	++used;
				}

			code.functions += ") / " + std::to_string(used) + ".0f;";
			code.functions += "}";

		}



		return code;

	}

	CodePart getFragment(Var vLightStrength, std::string light) {

		CodePart code = getFragmentOnce();

		// needed variable
		code.addGlobalVariable(VAR_FRAG_IN_SHADOW_MAP_COORD);
		code.addUsage(VAR_FRAG_IN_SHADOW_MAP_COORD.name);

		// needed texture
		code.addUsage("texShadowMap[" + std::to_string(MAX_LIGHTS) + "]");

		// helper function


		// shadowing for this light?
		code.main +=
				"if (" + ProvLightHelper::lightCastsShadows(light) + ") {"+
					vLightStrength.name + " *= getShadowAmount(texShadowMap["+light+"], "+light+");"+
				"}";

		return code;

	}

};


#endif // PROVLIGHTSHADOW_H
