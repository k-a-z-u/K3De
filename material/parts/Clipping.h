#ifndef CLIPPING_H
#define CLIPPING_H

#include <string>
#include "../Material2.h"
#include "../xml.h"
#include "Buildable.h"
#include "../ProgramVariables.h"

namespace MatPart {

	class Clipping : public Buildable {

	public:

		void build(Material2* mat, XMLElem* node) {

			XML_ASSERT_NODE_NAME("usesClipping", node);

			// input variable. currently only fixed y-clipping is supported
			//mat->getFragmentParams().addVariable("clipY", "uniform vec2 clipY;", 1);
			mat->getFragmentParams().addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::POS2, "clipY"));

		}

		std::string doClip(Material2* mat) const {

			mat->getFragmentParams().usedVariable("vertex_M");
			mat->getFragmentParams().usedVariable("clipY");

			return
			    "if ((vertex_M.y < clipY[0]) || (vertex_M.y > clipY[1])) {discard;}\n";

		}

	};

}

#endif // CLIPPING_H
