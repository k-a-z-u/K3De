#ifndef MODIFIERCOLOR_H
#define MODIFIERCOLOR_H

#include "Modifier.h"

namespace MatPart {

	class ModifierColor : public Modifier {

	private:

		std::string type;
		Vec4 constColor;

	public:

		ModifierColor(const std::string& id) : Modifier(id) {;}

		virtual void build(Material2* material, XMLElem* node) override {

			// sanity check
			XML_ASSERT_NODE_NAME("color", node);

			// type?
			type = node->Attribute("type");

			if ("colorizeByVertexColor" == type) {
				material->getFragmentParams().usedVariable(FRAG_VERTEX_COLOR);
				material->getVertexParams().usedVariable(FRAG_VERTEX_COLOR);
			}

			// parameters [not all might be available]
			constColor.r = node->FloatAttribute("r");
			constColor.g = node->FloatAttribute("g");
			constColor.b = node->FloatAttribute("b");
			constColor.a = node->FloatAttribute("a");

		}

		std::string getModified(const std::string& inp) {

			std::string color = "vec4("
				+ std::to_string(constColor.r) + ","
				+ std::to_string(constColor.g) + ","
				+ std::to_string(constColor.b) + ","
				+ std::to_string(constColor.a) + ")";

			if ("colorizeByConstant" == type) {
				//return "(" + inp + ".a * " + color + ")";
				return "vec4(" + std::to_string(constColor.r) + "," + std::to_string(constColor.g) + "," + std::to_string(constColor.b) + "," + inp + ".a*0.5)";
			} else if ("colorizeByVertexColor" == type) {
				return "(" + inp + ".a * " + FRAG_VERTEX_COLOR + ")";
			} else {
				throw Exception("unsupported color-modifier type: " + type);
			}

		}


	};

}

#endif // MODIFIERCOLOR_H
