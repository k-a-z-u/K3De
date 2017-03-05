#ifndef SHADOWING_H
#define SHADOWING_H

#include <string>
#include "../Material2.h"
#include "../xml.h"
#include "HasModifiers.h"
#include "Buildable.h"
#include "../ProgramVariables.h"

namespace MatPart {

	class Shadowing : public HasModifiers, public Buildable {


	public:

//		std::string getShadowCoord(const std::string& lightIdx) const {
//			return "lights.light[" + lightIndex + "] * vec4(vertex_M, 1)";

//		}

		void build(Material2* mat, XMLElem* node) {

			buildModifiers(mat, node);

			// vertex shader is not needed

			// fragment shader
			mat->getFragmentParams().addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TEXTURE_2D, "texShadowMap", "[" + std::to_string(MAX_LIGHTS) + "]"));
			//mat->getFragmentParams().addVariable("texShadowMap", "uniform sampler2D texShadowMap[" + std::to_string(MAX_LIGHTS) + "];", 1);
			mat->getFragmentParams().usedVariable("vertex_M");
			mat->getFragmentParams().usedVariable("texShadowMap");
			mat->getFragmentParams().usedVariable(FRAG_SHADOW_MAP_COORD);

//			// is the given fragment shadowed?
//			mat->getFragmentParams().addFunction("isShadowed",
//				"float isShadowed(sampler2D shadowMap, const vec3 sc) {"
//				"	lowp float nearest = texture(shadowMap, sc.st).r;"				// smallest distance from this given (x,y) to the light
//				"	lowp float curDist = sc.z;"										// current distance to the light
//				"	return (nearest <= curDist) ? (0.10f) : (1.0f);"				// shadowed?
//				"}"
//			,  {"texShadowMap"}, {});


			// this one will not work as expected.. its impossible to just mess with the shadow coordinates using UV maps
//			std::string modFunc;
//			modFunc += "vec3 modShadowCoord(vec3 inp) {\n";
//			if (modUV) {
//				modFunc += "vec2 diff = " + modUV->getOffset("inp.xy") + ";\n";		// difference between orig and distorted UV coordinates
//				modFunc += "inp.xy += diff ;\n";									// TODO: apply correct scaling depending on light distance. seems hard.. tiling, light-pos, mesh, ..
//				modFunc += "return inp;\n";
//			} else {
//				modFunc += "return inp;\n";
//			}
//			modFunc += "}\n";

//			mat->getFragmentParams().addFunction("modShadowCoord", modFunc, {}, {});


			// get the shadowing for the idx-th light
			mat->getFragmentParams().addFunction("getShadowAmount",

				//"float getShadowAmount(sampler2D shadowMap, vec4 shadowCoord) {\n"
				"float getShadowAmount(sampler2D shadowMap, int lightIdx) {\n"

				// get texture coordinate within the depth-texture, rendered form the idx-th light's perspective
			    "	highp vec4 shadowCoord = shadowMapCoord[lightIdx];\n"

				// if we are behind the light, no shadowing is possible (prevents artifacts)
				"	if (shadowCoord.w <= 0.0) {return 1.0;}\n"

			    "	highp vec3 sc = shadowCoord.xyz / shadowCoord.w;\n"

				// add some clearance to prevent almost-equal-z artifcats
			    "	sc.z -= 0.0001f;\n"

//				// apply UV distortion [if any]
//				"	sc = modShadowCoord(sc);\n"

//				// "smooth" the shadow TODO: size 0.2 variable
//				"	lowp float s = 0.2 / shadowCoord.w;\n"

				// fixed scatter pattern looks better than a random pattern
				"	return (\n"

					+ isShadowed( 0.0f,  0.0f) + "+\n"

					+ isShadowed(-1.0f,  0.0f) + "+\n"
					+ isShadowed(+1.0f,  0.0f) + "+\n"
					+ isShadowed( 0.0f, +1.0f) + "+\n"
					+ isShadowed( 0.0f, -1.0f) + "+\n"

					+ isShadowed(+1.0f, +1.0f) + "+\n"
					+ isShadowed(-1.0f, +1.0f) + "+\n"
					+ isShadowed(+1.0f, -1.0f) + "+\n"
					+ isShadowed(-1.0f, -1.0f) + "+\n"

					+ isShadowed(-2.0f,  0.0f) + "+\n"
					+ isShadowed(+2.0f,  0.0f) + "+\n"
					+ isShadowed( 0.0f, +2.0f) + "+\n"
					+ isShadowed( 0.0f, -2.0f) + "\n"

				"	) / 13.0f;\n"

				"}"

			, {"shadowCoord", FRAG_SHADOW_MAP_COORD}, {"isShadowed"});

		}

	private:

		std::string isShadowed(const float ox, const float oy) {

			const float scale = 0.005f;
//TODO("without dividing by shadow coord? difference?")
			std::string o = "(vec2(" + std::to_string(ox*scale) + "," + std::to_string(oy*scale) + "))";// / shadowCoord.w)";
			//std::string o = "(vec2(" + std::to_string(ox*scale*2) + "," + std::to_string(oy*scale*2) + ") / shadowCoord.w)";

			return	"\t\t"
					"( texture(shadowMap, sc.st + " + o + ").r "	// smallest distance from this given (x,y) to the light
					" < "
					" sc.z "										// current distance to the light
					" ? (0.10f) : (1.0f))"							// output
			;
		}


	};

}

#endif // SHADOWING_H
