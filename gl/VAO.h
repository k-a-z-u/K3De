#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>
#include "../misc/Error.h"

class VAO {

private:

	GLuint id;

public:

	/** ctor */
	VAO() {
		glGenVertexArrays(1, &id);
		Error::assertOK();
	}

	~VAO() {
		if (id) {glDeleteVertexArrays(1, &id); id = 0;}
	}

	void bind() {
		glBindVertexArray(id);
		Error::assertOK();
	}

	void unbind() {
		glBindVertexArray(0);
		Error::assertOK();
	}

	void setFloats(const int idx, const int num, const int stride = 0, const size_t offset = 0) {
		glVertexAttribPointer (idx, num, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		Error::assertOK();
		glEnableVertexAttribArray (idx);
		Error::assertOK();
	}

	void setVertices(const int idx, const int stride = 0, const size_t offset = 0) {
		glVertexAttribPointer (idx, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		Error::assertOK();
		glEnableVertexAttribArray (idx);
		Error::assertOK();
	}

	void setColors(const int idx, const int stride = 0, const size_t offset = 0) {
		glVertexAttribPointer (idx, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		Error::assertOK();
		glEnableVertexAttribArray (idx);
		Error::assertOK();
	}

	void setNormals(const int idx, const int stride = 0, const size_t offset = 0) {
		glVertexAttribPointer (idx, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		Error::assertOK();
		glEnableVertexAttribArray (idx);
		Error::assertOK();
	}

	void setTexCoords(const int idx, const int stride = 0, const size_t offset = 0) {
		glVertexAttribPointer (idx, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		Error::assertOK();
		glEnableVertexAttribArray (idx);
		Error::assertOK();
	}

	void setTangents(const int idx, const int stride = 0, const size_t offset = 0) {
		glVertexAttribPointer (idx, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		Error::assertOK();
		glEnableVertexAttribArray (idx);
		Error::assertOK();
	}

	void setColor(const int idx, const int stride = 0, const size_t offset = 0) {
		glVertexAttribPointer (idx, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		Error::assertOK();
		glEnableVertexAttribArray (idx);
		Error::assertOK();
	}



};

#endif // VAO_H
