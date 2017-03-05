#ifndef MODIFIER_UVTEXTURE_H
#define MODIFIER_UVTEXTURE_H

#include "UV.h"
#include "../sources/SourceTexture.h"

namespace MatPart {

	class ModifierUVTexture : public ModifierUV {

	private:

		Vec2 scale;
		SourceTexture tex;
		ModifierUV* preAdjust = nullptr;

	public:

		ModifierUVTexture(const std::string& id) : ModifierUV(id) {;}


		virtual void build(Material2* material, XMLElem* node) override {

			// uv-map scaling
			scale.x = node->FloatAttribute("scaleX");
			scale.y = node->FloatAttribute("scaleY");

			// to-be-used UV-map texture
			XMLElem* eTex = node->FirstChildElement("texture");
			tex.build(material, eTex);
//			texID = XML_WITH_ID(eTex);
//			const MatPart::Texture& tex = material->getTextures().byID(texID);
//			material->getFragmentParams().usedVariable(texID);

			// additional input modifier?
			XMLElem* eMod = node->FirstChildElement("modifier");
			if (eMod) {
				preAdjust = dynamic_cast<ModifierUV*>(material->getModifiers().byID(XML_WITH_ID(eMod)));
			}

		}

		std::string getOffset(const std::string& uv) override {
			const std::string modIn = (preAdjust) ? ("(" + uv + " + " + preAdjust->getOffset(uv) + ")") : (uv);
			return "((" + tex.get(modIn) + ".rg - vec2(0.5, 0.5)) * vec2(" + std::to_string(scale.x) + "f, " + std::to_string(scale.y) + "f))";
		}

	};

}

#endif // UVTEXTURE_H
