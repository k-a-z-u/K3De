#ifndef SHADERVERSION_H
#define SHADERVERSION_H

#include "../gl/gl.h"
#include <string>
#include "../Exception.h"

class ShaderVersion {

public:

	/** get the shader version to include within the GLSL header */
	static inline std::string getGLSL() {

		switch(getOpenGLVersion()) {
			case OPENGL_3_3:		return "#version 330\n";	// 3.30
			case OPENGL_ES_3:		return "#version 300 es\n";	// 3.00 ES
		    default:				throw Exception("TODO: getVersionString()");
		}

	}

};

#endif // SHADERVERSION_H
