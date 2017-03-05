#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include "gl.h"

#include "../misc/Error.h"

class Renderbuffer {

private:

	GLuint rbID;

public:

	/** ctor */
	Renderbuffer() {
		create();
	}

	/** dtor */
	~Renderbuffer() {
		remove();
	}

	void bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, rbID);
	}

	/** use the render-buffer as depth-buffer with the given size */
	void setToDepthMode(const int w, const int h) {
		bind();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	}

	/** get the Renderbuffer's ID */
	GLuint getID() const {
		return rbID;
	}

private:

	void create() {
		glGenRenderbuffers(1, &rbID);
		Error::assertOK();
	}

	void remove() {
		glDeleteRenderbuffers(1, &rbID);
		Error::assertOK();
	}

};

#endif // RENDERBUFFER_H
