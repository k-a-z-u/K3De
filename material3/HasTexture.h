#ifndef HASTEXTURE_H
#define HASTEXTURE_H

#include "NamedTexture.h"

class HasTexture {

protected:

	NamedTexture tex;

public:

	/** ctor */
	HasTexture(NamedTexture tex) : tex(tex) {;}

	/** get the texture */
	NamedTexture getTexture() const {return tex;}

};

#endif // HASTEXTURE_H
