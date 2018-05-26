#ifndef UNIFORM_H
#define UNIFORM_H

#include <string>
#include "../gl/gl.h"

/** all default uniforms within most shaders */
enum DefaultUniform {

	CLIPPING,

	MODEL_MATRIX,
	VIEW_MATRIX,
	PROJECTION_MATRIX,

	VIEW_PROJECTION_MATRIX,
	MODEL_VIEW_PROJECTION_MATRIX,

	TIME,

	CAMERA_POSITION,

	SCREEN_WIDTH,
	SCREEN_HEIGHT,

	LIGHTS,

};


/** describes one uniform variable within a program */
struct Uniform {

	GLuint programID;
	std::string name;
	GLenum type;
	GLsizei size;		// number of elements
	GLint location;

	/** ctor */
	Uniform(GLint programID, const std::string& name, GLenum type, GLsizei size, GLint location) : programID(programID), name(name), type(type), size(size), location(location) {

	}

	/** ctor */
	Uniform(GLint programID, const std::string& name, GLenum type, GLsizei size) : programID(programID), name(name), type(type), size(size) {
		location = glGetUniformLocation(programID, name.c_str());
	}


	/** convert to string */
	std::string asString() const {
		return name + " " + getUniformTypeAsString(type) + " (" + std::to_string(size) + " elements)";
	}

	/** get the base-name for a compound uniform: Lights.light[5].attenuationLinear -> Lights.light[0] */
	static std::string getBaseName(const std::string& compound) {
		const size_t pos = compound.find_last_of('.');
		if (pos == std::string::npos) {return compound;}
		const std::string base = compound.substr(0, pos);
		return base;
	}


	/** convert uniform type enum to string */
	static std::string getUniformTypeAsString(const GLenum type) {
		switch(type) {
			case GL_FLOAT:			return "GL_FLOAT";
			case GL_FLOAT_VEC2:		return "GL_FLOAT_VEC2";
			case GL_FLOAT_VEC3:		return "GL_FLOAT_VEC3";
			case GL_FLOAT_VEC4:		return "GL_FLOAT_VEC4";
			case GL_INT:			return "GL_INT";
			case GL_INT_VEC2:		return "GL_INT_VEC2";
			case GL_INT_VEC3:		return "GL_INT_VEC3";
			case GL_INT_VEC4:		return "GL_INT_VEC4";
			case GL_BOOL:			return "GL_BOOL";
			case GL_BOOL_VEC2:		return "GL_BOOL_VEC2";
			case GL_BOOL_VEC3:		return "GL_BOOL_VEC3";
			case GL_BOOL_VEC4:		return "GL_BOOL_VEC4";
			case GL_FLOAT_MAT2:		return "GL_FLOAT_MAT2";
			case GL_FLOAT_MAT3:		return "GL_FLOAT_MAT3";
			case GL_FLOAT_MAT4:		return "GL_FLOAT_MAT4";
			case GL_SAMPLER_1D:		return "GL_SAMPLER_1D";
			case GL_SAMPLER_2D:		return "GL_SAMPLER_2D";
			case GL_SAMPLER_CUBE:	return "GL_SAMPLER_CUBE";
			case GL_SAMPLER_3D:		return "GL_SAMPLER_3D";
			case GL_IMAGE_2D:		return "GL_IMAGE_2D";
			case GL_IMAGE_3D:		return "GL_IMAGE_3D";
			default:				return "UNKNOWN";
		}
	}

};


#endif // UNIFORM_H
