#ifndef HASMODIFIERS_H
#define HASMODIFIERS_H

#include "modifiers/Modifier.h"
#include "modifiers/UV.h"
#include "modifiers/ModifierColor.h"
#include "../xml.h"

class MaterialFactory;

namespace MatPart {

	/**
	 * helper-base-class for everything that has attachable modifiers
	 */
	class HasModifiers {

		friend class ::MaterialFactory;

	protected:

		ModifierUV* modUV = nullptr;
		ModifierColor* modColor = nullptr;

	protected:

		/** attach all modifiers, defined within the XML structure */
		void buildModifiers(Material2* material, XMLElem* node) {

			const XMLElem* eUV = node->FirstChildElement("uv");
			const XMLElem* eColor = node->FirstChildElement("color");

			// UV modifiers
			if (eUV) {
				const XMLElem* mod = eUV->FirstChildElement("modifier");
				if (mod) {
					const std::string id = XML_WITH_ID(mod);
					Modifier* mod = material->getModifiers().byID(id);
					if (!mod) {throw Exception("no modifier with id: " + id);}
					modUV = dynamic_cast<ModifierUV*>(mod);
					if (!modUV) {throw Exception("not a UV-modifier: " + id);}
				}
			}

			// color modifiers
			if (eColor) {
				const XMLElem* mod = eColor->FirstChildElement("modifier");
				if (mod) {
					const std::string id = XML_WITH_ID(mod);
					Modifier* mod = material->getModifiers().byID(id);
					if (!mod) {throw Exception("no modifier with id: " + id);}
					modColor = dynamic_cast<ModifierColor*>(mod);
					if (!modColor) {throw Exception("not a Color-modifier: " + id);}
				}
			}

		}

	};

}

#endif // HASMODIFIERS_H
