#ifndef MODIFIERCOMPOUND_H
#define MODIFIERCOMPOUND_H

#include "Modifier.h"

namespace MatPart {

	/** combine several modifiers into one [chain] */
	class ModifierCompound : public Modifier {

	private:

		std::vector<Modifier*> chain;

	public:

		ModifierCompound(const std::string& id) : Modifier(id) {;}

		virtual void build(Material2* material, XMLElem* node) override {
			XML_FOREACH_ELEM_NAMED("id", idNode, node) {
				const std::string id = idNode->FirstChild()->Value();
				Modifier* mod = material->getModifiers().byID(id);
				if (!mod) {throw Exception("there is no modifier with id: " + id);}
				chain.push_back(mod);
				int i = 0; (void) i;
			}
		}

//		virtual std::string get(const std::string& inp) override {
//			std::string cur = inp;
//			for (Modifier* m : chain) {
//				cur = m->get(cur);
//			}
//			return cur;
//		}

	};

}

#endif // MODIFIERCOMPOUND_H
