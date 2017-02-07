#ifndef MODIFIER_UVTIME_H
#define MODIFIER_UVTIME_H

#include "UV.h"

namespace MatPart {

	class ModifierUVTime : public ModifierUV {

	private:

		Vec2 scale;

	public:

		ModifierUVTime(const std::string& id) : ModifierUV(id) {;}


		void build(Material2* material, XMLElem* node) override {
			scale.x = node->FloatAttribute("scaleX");
			scale.y = node->FloatAttribute("scaleY");
			material->getFragmentParams().usedVariable("time");		// i need the time variable
		}

		std::string getOffset(const std::string& uv) override {

			(void) uv;

			// use the original input coordinate [uv] and shift it over time
			return "vec2(" + std::to_string(scale.x) + "f * time, " + std::to_string(scale.y) + "f * time) ";

		}

	};

}

#endif // UVTIME_H
