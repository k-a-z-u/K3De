#ifndef INC_GLEW_H
#define INC_GLEW_H

/**
 * linux and windows needs glew as extension wrangler
 * MacOS has this one included within its OpenGL impl.
 */
#if defined (__APPLE__)
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

enum OpenGLVersion {
	OPENGL_ES_3,
	OPENGL_3_3,
};

/** get the openGL version to use */
static inline OpenGLVersion getOpenGLVersion() {
	#if defined (__APPLE__)
		return OPENGL_3_3;
	#else
		return OPENGL_ES_3;
	#endif
}

#endif // INC_GLEW_H
