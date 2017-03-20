#ifndef SOURCETEXTUREMIX_H
#define SOURCETEXTUREMIX_H


#include "Source.h"
#include "SourceTexture.h"

namespace MatPart {

	/**
	 * combine several textures via blending.
	 * e.g. for terrain: RGB texture that contains the mixing + 3 to-be-mixed textures [1 = red, 2 = green, 3 = blue]
	 */
	class SourceTextureMix : public Source {

	private:

		std::string texID;
		std::string mode;

		std::vector<SourceTexture*> textures;

	public:

		SourceTextureMix() {
			;
		}

		void build(Material2* material, XMLElem* node) override {

			// sanity check
			XML_ASSERT_NODE_NAME("textureMix", node);

			// mode
			mode = (node->Attribute("mode")) ? (node->Attribute("mode")) : ("rgb");

			// load contained textures
			XML_FOREACH_ELEM_NAMED("texture", sub, node) {
				SourceTexture* tex = new SourceTexture();
				tex->build(material, sub);
				textures.push_back(tex);
			}


		}

		std::string get() override {

			// start with normal texture coordinate stored within the global variable "uv"
			return get("uv");

		}

		std::string get(std::string uv) {

			if ("rgb" == mode) {

				return std::string() +
				"vec4(" +
				    "(" +
				        textures[0]->get(uv) + ".r * " + textures[1]->get(uv) + ".rgb + " +
				        textures[0]->get(uv) + ".g * " + textures[2]->get(uv) + ".rgb + " +
				        textures[0]->get(uv) + ".b * " + textures[3]->get(uv) + ".rgb " +
				    "), " +
				    "1.0" +
				")";

			} else {
				throw Exception("unsupported mode");
			}

		}

	};

}


#endif // SOURCETEXTUREMIX_H
