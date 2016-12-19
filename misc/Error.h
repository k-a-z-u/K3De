#ifndef ERROR_H
#define ERROR_H

#include <GL/glew.h>
#include <KLib/Assertions.h>

#define todo(str)		std::cout << str << std::endl;

class Error {

public:

	/** assert there are no openGL errors */
	static void assertOK() {
		const GLenum err = glGetError();
		_assertEqual(GL_NO_ERROR, err, "an openGL error occurred");
	}

	/** check whether the last openGL call was OK */
	static bool isOK() {
		const GLenum err = glGetError();
		return err == GL_NO_ERROR;
	}

};

#endif // ERROR_H
