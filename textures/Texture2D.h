#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <GL/glew.h>
#include "Texture.h"

class Texture2D : public Texture {

public:

	/** ctor */
	Texture2D(const int width, const int height) : Texture(GL_TEXTURE_2D, width, height) {
		;
	}

};

#endif // TEXTURE2D_H
