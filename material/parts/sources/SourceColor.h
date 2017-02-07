#ifndef SOURCECOLOR_H
#define SOURCECOLOR_H


#include "Source.h"

namespace MatPart {

	class SourceColor : public Source {

	private:

		Vec4 color;

	public:

		void build(Material2* material, XMLElem* node) override {

			// sanity check
			XML_ASSERT_NODE_NAME("color", node);

			// load additional modifiers [if any]
			buildModifiers(material, node);

			// parameters
			color.r = node->FloatAttribute("r");
			color.g = node->FloatAttribute("g");
			color.b = node->FloatAttribute("b");
			color.a = node->FloatAttribute("a");

			// zero alpha does not make sens
			if (color.a == 0) {color.a = 1.0f;}

		}

		std::string get() override {
			return "vec4("	+ std::to_string(color.r) + ","
							+ std::to_string(color.g) + ","
							+ std::to_string(color.b) + ","
							+ std::to_string(color.a) + ")";
		}

	};

}

#endif // SOURCECOLOR_H
