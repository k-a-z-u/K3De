#ifndef MODIFIER_UV_H
#define MODIFIER_UV_H

#include "Modifier.h"

namespace MatPart {

	class ModifierUV : public Modifier {

	public:

		ModifierUV(const std::string& id) : Modifier(id) {;}

		virtual std::string getOffset(const std::string& uv) = 0;

	};

}

#endif // UV_H
