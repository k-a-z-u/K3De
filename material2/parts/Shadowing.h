#ifndef SHADOWING_H
#define SHADOWING_H

#include <string>
#include "../Material2.h"
#include "../xml.h"
#include "HasModifiers.h"
#include "Buildable.h"

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
			mat->getFragmentParams().addVariable("texShadowMap", "uniform sampler2D texShadowMap[" + std::to_string(MAX_LIGHTS) + "];", 1);
			mat->getFragmentParams().usedVariable("vertex_M");



			// is the given fragment shadowed?
			mat->getFragmentParams().addFunction("isShadowed",
				"float isShadowed(sampler2D shadowMap, const vec3 sc) {"
				"	float nearest = texture(shadowMap, sc.st).r;"		// smallest distance from this given (x,y) to the light
				"	float curDist = sc.z;"											// current distance to the light
				"	return (nearest <= curDist) ? (0.10f) : (1.0f);"				// shadowed?
				"}"
			,  {"texShadowMap"}, {});


			std::string modFunc;
			modFunc += "vec3 modShadowCoord(vec3 inp) {\n";
			if (modUV) {
				modFunc += "vec2 diff = " + modUV->getOffset("inp.xy") + ";\n";		// difference between orig and distorted UV coordinates
				modFunc += "inp.xy += diff ;\n";									// TODO: apply correct scaling depending on light distance. seems hard.. tiling, light-pos, mesh, ..
				modFunc += "return inp;\n";
			} else {
				modFunc += "return inp;\n";
			}
			modFunc += "}\n";

			mat->getFragmentParams().addFunction("modShadowCoord", modFunc, {}, {});


			// get the shadowing for the idx-th light
			mat->getFragmentParams().addFunction("getShadowAmount",

				"float getShadowAmount(sampler2D shadowMap, const int lightIdx) {\n"

				// get texture coordinate within the depth-texture, rendered form the idx-th light's perspective
				"	vec4 shadowCoord = lights.light[lightIdx].pvShadow * vec4(vertex_M, 1);\n"

				// if we are behind the light, no shadowing is possible (prevents artifacts)
				"	if (shadowCoord.w <= 0.0) {return 1.0;}\n"

				"	vec3 sc = shadowCoord.xyz / shadowCoord.w;\n"

				// add some clearance to prevent almost-equal-z artifcats
				"	sc.z -= 0.00025f;\n"

				// apply UV distortion [if any]
				"	sc = modShadowCoord(sc);\n"

				// "smooth" the shadow TODO: size 0.2 variable
				"	float s = 0.2 / shadowCoord.w;\n"

				// fixed scatter pattern looks better than a random pattern
				"	return (\n"

				"		isShadowed(shadowMap, sc + vec3(-0.1f,  0.0f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3(+0.1f,  0.0f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3( 0.0f, +0.1f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3( 0.0f, -0.1f, 0.0f) * s) + \n"

				"		isShadowed(shadowMap, sc + vec3(+0.1f, +0.1f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3(-0.1f, +0.1f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3(+0.1f, -0.1f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3(-0.1f, -0.1f, 0.0f) * s) + \n"

				"		isShadowed(shadowMap, sc + vec3(-0.2f,  0.0f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3(+0.2f,  0.0f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3( 0.0f, +0.2f, 0.0f) * s) + \n"
				"		isShadowed(shadowMap, sc + vec3( 0.0f, -0.2f, 0.0f) * s) + \n"

				"		isShadowed(shadowMap, sc)\n"

				"	) / 13.0f;\n"

				"}"

			, {"shadowCoord"}, {"isShadowed"});

		}


	};

}

#endif // SHADOWING_H
