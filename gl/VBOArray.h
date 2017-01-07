#ifndef VBOARRAY_H
#define VBOARRAY_H

#include "VBO.h"
#include "../mesh/MeshVertex.h"

template <typename T> class VBOArrayStatic : public VBOStatic<T, GL_ARRAY_BUFFER> {};
template <typename T> class VBOArrayDynamic : public VBODynamic<T, GL_ARRAY_BUFFER> {};


///** templating this one did not work out?! */
//template <typename T> class VBOArray : public VBOStatic<T, GL_ARRAY_BUFFER> {

//public:

////	/** bind this VBO as vertex-buffer */
////	void bind() {
////		glBindBuffer(, this->id);
////		Error::assertOK();
////	}

////	/** unbind the vertex-buffer */
////	void unbind() {
////		glBindBuffer(GL_ARRAY_BUFFER, 0);
////		Error::assertOK();
////	}

////	/** send the STATIC data to the GPU (once) */
////	void upload() {
////		bind();
////		glBufferData(GL_ARRAY_BUFFER, this->data.size()*sizeof(T), this->data.data(), GL_STATIC_DRAW);
////		Error::assertOK();
////		unbind();
////	}

//};

#endif // VBOARRAY_H
