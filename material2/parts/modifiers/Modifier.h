#ifndef MODIFIER_H
#define MODIFIER_H

#include <string>
#include "../../Material2.h"
#include "../../xml.h"

namespace MatPart {

	class Modifier {

	private:

		std::string id;

	public:

		Modifier(const std::string& id) : id(id) {;}

		const std::string& getID() const {return id;}

		virtual void build(Material2* material, XMLElem* node) = 0;

		//virtual std::string get(const std::string& inp) = 0;

	};

}

#endif // MODIFIER_H
