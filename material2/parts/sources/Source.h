#ifndef SOURCE_H
#define SOURCE_H

#include <string>
#include "../../Material2.h"
#include "../../xml.h"
#include "../HasModifiers.h"

namespace MatPart {

	class Source : public HasModifiers {

	public:

		virtual void build(Material2* material, XMLElem* node) = 0;

		virtual std::string get() = 0;

	};

}

#endif // SOURCE_H
