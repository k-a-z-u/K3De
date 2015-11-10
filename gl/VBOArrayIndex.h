#ifndef VBOARRAYINDEX_H
#define VBOARRAYINDEX_H

#include "VBO.h"

class VBOArrayIndex : public VBO<GLuint> {

public:

	/** bind this VBO as index-to-vertex-buffer */
	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		Error::assertOK();
	}

	/** unbind the buffer */
	void unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		Error::assertOK();
	}

	/** send the data to the GPU */
	void upload() {
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(GLuint), data.data(), GL_STATIC_DRAW);
		Error::assertOK();
		unbind();
	}

};

#endif // VBOARRAYINDEX_H
