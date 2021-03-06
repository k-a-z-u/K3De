#ifndef LIGHTING_H
#define LIGHTING_H


#include "Buildable.h"
#include "Enableable.h"

#include "sources/SourceTexture.h"

namespace MatPart {

	class Lighting : public Buildable, public Enableable {

	private:

		/** reduce the impact of the diffuse angle -> more ambient */
		float diffuseImpact = 1.0;

	public:

		float getDiffuseImpact() {return diffuseImpact;}
		void setDiffuseImpact(const float imp) {this->diffuseImpact = imp;}

		void build(Material2* mat, XMLElem* root) {

			(void) root;

			// limit diffuse impact?
			diffuseImpact = root->Attribute("diffuseImpact") ? root->FloatAttribute("diffuseImpact") : 1.0;

			std::string lights =

				"\n"
				"struct Light {\n"

				"	highp vec3 pos;\n"			// 4 floats for position
				"	float padPos;\n"

				"	lowp vec3 color;\n"			// 4 floats for color
				"	float padColor;\n"

				"	lowp float impact;\n"		// 4x4 bytes for other options
				"	lowp float attenuationLinear;\n"
				"	lowp float attenuationQuadratic;\n"
				"	int flags;\n"

				"	mediump mat4 pvShadow;\n"	// matrix

				"};\n\n"

				"layout (std140) uniform Lights {\n"
				"	Light light[8];\n"
				"} lights;\n"

			;

			mat->getFragmentParams()._addVariable("lights", lights);
			mat->getVertexParams()._addVariable("lights", lights);

			mat->getFragmentParams().usedVariable("lights");
			mat->getVertexParams().usedVariable("lights");

		}

		std::string isLightEnabled(const std::string idx) const {
			return "(lights.light[" + idx + "].flags & 1) != 0";
		}

		std::string lightCastsShadows(const std::string idx) const {
			return "(lights.light[" + idx + "].flags & 2) != 0";
		}

		std::string getLightPos(const std::string idx) const {
			//return "vec3(lights.light["+idx+"].x, lights.light["+idx+"].y, lights.light["+idx+"].z)";
			return "lights.light["+idx+"].pos.xyz";
		}

		std::string getLightColor(const std::string idx) const {
			//return "vec3(lights.light["+idx+"].r, lights.light["+idx+"].g, lights.light["+idx+"].b)";
			return "lights.light["+idx+"].color.rgb";
		}

		// att = 1.0 / (1.0 + 0.1*dist + 0.01*dist*dist)

		std::string getLightAttenuationLinear(const std::string idx) const {
			return "lights.light["+idx+"].attenuationLinear";
		}

		std::string getLightAttenuationQuadratic(const std::string idx) const {
			return "lights.light["+idx+"].attenuationQuadratic";
		}

		std::string getLightImpact(const std::string idx) const {
			return "lights.light["+idx+"].impact";
		}

	};


}


#endif // LIGHTING_H
