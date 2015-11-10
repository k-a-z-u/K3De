#ifndef UBO_H
#define UBO_H


#include <GL/glew.h>

#include "../misc/Error.h"

class UBO {

private:

	GLuint id;

public:

	UBO() : id(0) {
		create();
	}

	~UBO() {
		remove();
	}

	void create() {
		glGenBuffers(1, &id);					Error::assertOK();
	}

	void remove() {
		glDeleteBuffers(1, &id);				Error::assertOK();
	}

	void bind() const {
		 glBindBuffer(GL_UNIFORM_BUFFER, id);	Error::assertOK();
	}

	void upload(const uint8_t* data, const uint32_t size) const {
		bind();
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
		Error::assertOK();
	}

	GLuint getID() const {
		return id;
	}



};

#endif // UBO_H
