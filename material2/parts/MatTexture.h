#ifndef MATTEXTURE_H
#define MATTEXTURE_H

#include <string>
#include "../../textures/ITexture.h"

namespace MatPart {

	struct LoadedTexture {

		std::string id;
		int idx;
		ITexture* tex;

		/** ctor */
		LoadedTexture(const std::string& id, const int idx, ITexture* tex) : id(id), idx(idx), tex(tex) {;}

		/** ID-method */
		const std::string getID() const {return id;}

	};

}

#endif // MATTEXTURE_H
