#ifndef VBOARRAY_H
#define VBOARRAY_H

#include "VBO.h"
#include "../mesh/MeshVertex.h"

/** templating this one did not work out?! */
template <typename T> class VBOArray : public VBO<T> {

public:

	/** bind this VBO as vertex-buffer */
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		Error::assertOK();
	}

	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Error::assertOK();
	}

	/** send the data to the GPU */
	void upload() {
		bind();
		glBufferData(GL_ARRAY_BUFFER, this->data.size()*sizeof(T), this->data.data(), GL_STATIC_DRAW);
		Error::assertOK();
		unbind();
	}

	uint8_t* allocateDynamic(const int size) {
		bind();
		const int flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ARRAY_BUFFER, size, nullptr, flags);
		Error::assertOK();
		uint8_t* res = (uint8_t*) glMapBufferRange(GL_ARRAY_BUFFER, 0, size, flags);
		Error::assertOK();
		return res;
	}

	/** begin working on a buffer fetched with allocateDynamic() */
	void beginSync() {
		syncObj = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);		Error::assertOK();
		glClientWaitSync(syncObj, GL_SYNC_FLUSH_COMMANDS_BIT, 1);		Error::assertOK();
	}

	/** stop working on a buffer fetched with allocateDynamic() */
	void endSync() {
		glDeleteSync(syncObj);											Error::assertOK();
	}

private:

	GLsync syncObj = 0;


};

#endif // VBOARRAY_H
