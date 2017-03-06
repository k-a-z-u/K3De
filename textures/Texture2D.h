#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "../gl/gl.h"

#include "Texture.h"

class Texture2D : public Texture {

public:

	/** ctor */
	Texture2D(const GLuint format, const int width, const int height) : Texture(GL_TEXTURE_2D, format, width, height) {
		;
	}

};

#endif // TEXTURE2D_H
