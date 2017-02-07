#ifndef MODIFIERFACTORY_H
#define MODIFIERFACTORY_H

#include "../../Material2.h"
#include "../../xml.h"

#include "Modifier.h"

#include "UVTexture.h"
#include "UVTime.h"
#include "ModifierCompound.h"

namespace MatPart {

	class ModifierFactory {

	public:

		static Modifier* get(Material2* material, XMLElem* node) {

			const std::string nodeName = node->Name();

			if ("uv" == nodeName) {
				return getUV(material, node);
			} else if ("color" == nodeName) {
				return getColor(material, node);
//			} else if ("compound" == nodeName) {
//				return getCompound(material, node);
			} else {
				throw Exception("unsupported modifier type: " + nodeName);
			}

		}

	private:


		/** get a UV-coordinate modifier */
		static Modifier* getUV(Material2* material, XMLElem* node) {
			ModifierUV* mod;
			const std::string type = node->Attribute("type");
			const std::string id = XML_ID(node);
			if ("time" == type) {
				mod = new ModifierUVTime(id);
			} else if ("uvMap" == type) {
				mod = new ModifierUVTexture(id);
			} else {
				throw Exception("unsupported uv-modifier: " + type);
			}
			mod->build(material, node);
			return mod;
		}

		/** get a vec4 color modifier */
		static Modifier* getColor(Material2* material, XMLElem* node) {
			ModifierColor* mod = new ModifierColor(XML_ID(node));
			mod->build(material, node);
			return mod;
		}

		/** get a compoundr modifier */
		static Modifier* getCompound(Material2* material, XMLElem* node) {
			ModifierCompound* mod = new ModifierCompound(XML_ID(node));
			mod->build(material, node);
			return mod;
		}

	};

}

#endif // MODIFIERFACTORY_H
