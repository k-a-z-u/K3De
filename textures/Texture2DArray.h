#ifndef TEXTURE2DARRAY_H
#define TEXTURE2DARRAY_H

#include "Texture.h"

/**
 * represents an array of several 2D textures.
 * all those texture have the same width, height and image format!
 */
class Texture2DArray : public Texture {

protected:

	int numLayers;

public:

	/** ctor */
	Texture2DArray(const int widht, const int height, const int numLayers) : Texture(GL_TEXTURE_2D_ARRAY, widht, height), numLayers(numLayers) {
		;
	}

	int getNumLayers() const {return numLayers;}

};

#endif // TEXTURE2DARRAY_H
