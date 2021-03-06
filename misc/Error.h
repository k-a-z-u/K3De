#ifndef ERROR_H
#define ERROR_H

#include "../gl/gl.h"

#include <KLib/Assertions.h>

#define todo(str)		std::cout << str << std::endl;

class Error {

public:

#ifdef WITH_ASSERTIONS
	/** assert there are no openGL errors */
	static inline void assertOK() {
		const GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			int i = 0; (void) i;
		}
		_assertEqual(GL_NO_ERROR, err, "an openGL error occurred");
	}
#else
	static inline void assertOK() {;}
#endif

	/** check whether the last openGL call was OK */
	static inline bool isOK() {
		const GLenum err = glGetError();
		return err == GL_NO_ERROR;
	}

};

#endif // ERROR_H
