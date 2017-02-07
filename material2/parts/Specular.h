#ifndef SPECULAR_H
#define SPECULAR_H

#include "Buildable.h"
#include "sources/SourceTexture.h"

namespace MatPart {

	class Specular : public Buildable {

	public:

		void build(Material2* material, XMLElem* node) override {

			// sanity check
			XML_ASSERT_NODE_NAME("specular", node);

			XMLElem* sub = node->FirstChildElement();

			const float shininess = sub->FloatAttribute("shininess");
			const float multiplier = sub->FloatAttribute("multiplier");

			if (shininess == 0) {throw Exception("shininess must not be 0");}
			if (multiplier == 0) {throw Exception("multiplier must not be 0");}


			if (std::string("fixed") == sub->Name()) {

				// specular color is white: TODO!
				material->getFragmentParams().addMainLine("const vec3 specularColor = vec3(1.0) * " + std::to_string(multiplier) + "f;");

				// fixed shininess
				material->getFragmentParams().addMainLine("const float specularShininess = " + std::to_string(shininess) + "f;");

			} else if (std::string("texture") == sub->Name()) {

				// get the needed specular texture
				MatPart::SourceTexture mpTex;
				mpTex.build(material, sub);

				// get its value
				material->getFragmentParams().addMainLine("vec4 specularTexVal = " + mpTex.get() + ";");

				if (mpTex.getTexture()->isAlphaOnly()) {

					// specular color comes from RGB value of the texture
					material->getFragmentParams().addMainLine("const vec3 specularColor = vec3(1.0);");

				} else {

					// specular color comes from RGB value of the texture
					material->getFragmentParams().addMainLine("vec3 specularColor = specularTexVal.rgb * " + std::to_string(multiplier) + "f;");

				}

				// specular shininess comes from the alpha [0:1] value of the texture multiplied by the maximum shininess [stretching]
				material->getFragmentParams().addMainLine("float specularShininess = specularTexVal.a * " + std::to_string(shininess) + "f;");

			}

		}




//		ss << "\tvec4 texSpecularMap0Color = texture(texSpecularMap0, uv);\n";

//		if (texSpecularMap0->isAlphaOnly()) {
//			ss << "\tvec3 specularColor = vec3(texSpecularMap0Color.a, texSpecularMap0Color.a, texSpecularMap0Color.a) * " << std::to_string(mul) << "f;\n";
//		} else {
//			ss << "\tvec3 specularColor = texSpecularMap0Color.rgb * " << std::to_string(mul) << "f;\n";
//		}

//		ss << "\tfloat specularShininess = (texSpecularMap0Color.a * " + std::to_string(maxShininess) + "f);\n";


	};


}

#endif // SPECULAR_H
