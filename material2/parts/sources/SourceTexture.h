#ifndef SOURCETEXTURE_H
#define SOURCETEXTURE_H

#include "Source.h"

namespace MatPart {

	class SourceTexture : public Source {

	private:

		std::string texID;
		Vec2 tile = Vec2(1,1);

		ITexture* tex;

	public:

		void build(Material2* material, XMLElem* node) override {

			// sanity check
			XML_ASSERT_NODE_NAME("texture", node);

			// fetch the (previously loaded) texture
			texID = node->Attribute("with_id");
			const MatPart::LoadedTexture& tex = material->getTextures().byID(texID);
			this->tex = tex.tex;

			material->getVertexParams().usedVariable(FRAG_VERTEX_UV);	// I need UV coordinates
			material->getFragmentParams().usedVariable(FRAG_VERTEX_UV);	// I need UV coordinates
			material->getFragmentParams().usedVariable(tex.id);	// I need the sampler variable

			// load additional modifiers [if any]
			buildModifiers(material, node);

			// parameters
			tile.x = node->FloatAttribute("tileX"); if (tile.x == 0) {tile.x = 1;}
			tile.y = node->FloatAttribute("tileY"); if (tile.y == 0) {tile.y = 1;}

		}

		/** get the underlying texture */
		ITexture* getTexture() const {
			return tex;
		}

		std::string get() override {

			// start with normal texture coordinate stored within the global variable "uv"
			return get("uv");

		}


		std::string get(std::string uv) {

			// TODO: order? tiling->mod or mod->tiling?

			// apply the attached UV modifier [if any] to the original [untiled] uv-coordinate
			if (modUV) {uv = "(" + uv + " + " + modUV->getOffset(FRAG_VERTEX_UV) + ")";}

			// apply texture tiling [if any]
			if (tile != Vec2(1,1)) {uv = "(" + uv + " * vec2(" + std::to_string(tile.x) + "," + std::to_string(tile.y) + "))";}

			// build output
			std::string res = "texture(" + texID + ", " + uv + ")";

			// color modifier?
			if (modColor) {
				res = modColor->getModified(res);
			}

			// done
			return res;

		}

	};

}

#endif // SOURCETEXTURE_H
