#ifndef LIGHTING_H
#define LIGHTING_H


#include "Buildable.h"
#include "sources/SourceTexture.h"

namespace MatPart {

	class Lighting : public Buildable {

	public:

		void build(Material2* mat, XMLElem* root) {

			(void) root;

			mat->getFragmentParams().addVariable("lights",

				"struct Light {\n"

				"	float x,y,z;\n"
				"	float r,g,b;\n"

				"	float impact;\n"
				"	float attenuationLinear;\n"
				"	float attenuationQuadratic;\n"

				"	int enabled;\n"
				"	int castsShadows;\n"

				"	int pad1;\n"

				"	mat4 pvShadow;\n"


				"};\n"

				"layout (std140) uniform Lights {\n"
					"Light light[8];\n"
				"} lights;\n"

			);

			mat->getFragmentParams().usedVariable("lights");

			/** get the number of to-be-used lights */
			//mat->getFragmentParams().addFunction("getNumLights",		"int getNumLights() {return lights.cnt;}", {"lights"}, {});
			//mat->paramsFragment.usedVariable("lights");

			/** get the position of the idx-th light */
			//mat->getFragmentParams().addFunction("getLightPos",			"vec3 getLightPos(const int idx) {return vec3(lights.light[idx].x, lights.light[idx].y, lights.light[idx].z);}\n", {"lights"}, {});
			//mat->paramsFragment.usedVariable("lights");

			/** get the color of the idx-th ligth */
			//mat->getFragmentParams().addFunction("getLightColor",		"vec3 getLightColor(const int idx) {return vec3(lights.light[idx].r, lights.light[idx].g, lights.light[idx].b);}\n", {"lights"}, {});
			//mat->paramsFragment.usedVariable("lights");

			/** get the impact of the idx-th ligth */
			//mat->getFragmentParams().addFunction("getLightImpact",		"float getLightImpact(const int idx) {return lights.light[idx].impact;}\n", {"lights"}, {});

			/** get the attenuation of the idx-th ligth */
			//mat->getFragmentParams().addFunction("getLightAttenuation",	"float getLightAttenuation(const int idx) {return lights.light[idx].attenuation;}\n", {"lights"}, {});

		}

		std::string isEnabled(const std::string idx) const {
			return "lights.light[" + idx + "].enabled == 1";
		}

		std::string castsShadows(const std::string idx) const {
			return "lights.light[" + idx + "].castsShadows == 1";
		}

		std::string getPos(const std::string idx) const {
			return "vec3(lights.light["+idx+"].x, lights.light["+idx+"].y, lights.light["+idx+"].z)";
		}

		std::string getColor(const std::string idx) const {
			return "vec3(lights.light["+idx+"].r, lights.light["+idx+"].g, lights.light["+idx+"].b)";
		}

		// att = 1.0 / (1.0 + 0.1*dist + 0.01*dist*dist)

		std::string getAttenuationLinear(const std::string idx) const {
			return "lights.light["+idx+"].attenuationLinear";
		}

		std::string getAttenuationQuadratic(const std::string idx) const {
			return "lights.light["+idx+"].attenuationQuadratic";
		}

		std::string getImpact(const std::string idx) const {
			return "lights.light["+idx+"].impact";
		}

	};


}


#endif // LIGHTING_H
