#ifndef BUILDABLE_H
#define BUILDABLE_H

#include "../Material2.h"
#include "../xml.h"

namespace MatPart {

	class Buildable {

	public:

		virtual void build(Material2* material, XMLElem* node) = 0;

	};

}

#endif // BUILDABLE_H
