#ifndef GPUPARAMS_H
#define GPUPARAMS_H

#include <GL/glew.h>
#include "misc/Error.h"

class GPUParams {

public:

	static void show() {
		std::cout << "max anisotropy:\t" << getMaxAnisotropy() << "\n";
		std::cout << "max clipping planes:\t" << getMaxNumClippingPlanes() << "\n";
		std::cout << "max combined textures:\t" << getMaxCombinedTextures() << "\n";
		std::cout << "max uniform locations:\t" << getMaxUniformLocations() << "\n";
		std::cout << "max vertex uniforms:\t" << getMaxVertexUniformComp() << "\n";
		std::cout << "max geometry uniforms:\t" << getMaxGeometryUniformComp() << "\n";
		std::cout << "max fragment uniforms:\t" << getMaxFragmentUniformComp() << "\n";
		std::cout << "shader version:\t" << getShaderVersion() << "\n";
	}

	static int getInt(GLenum val) {
		GLint i;
		glGetIntegerv(val, &i);
		return (Error::isOK()) ? (i) : (-1);
	}

	static std::string getString(GLenum val) {
		std::string res = std::string((char*)glGetString(val));
		return (Error::isOK()) ? (res) : ("?");
	}

	static float getMaxAnisotropy() {
		GLfloat max;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
		return (Error::isOK()) ? (max) : (-1);
	}

	static int getMaxNumClippingPlanes() {return getInt(GL_MAX_CLIP_PLANES);}

	static int getMaxCombinedTextures() {return getInt(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);}

	static int getMaxUniformLocations() {return getInt(GL_MAX_UNIFORM_LOCATIONS);}
	static int getMaxVertexUniformComp() {return getInt(GL_MAX_VERTEX_UNIFORM_COMPONENTS);}
	static int getMaxGeometryUniformComp() {return getInt(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS);}
	static int getMaxFragmentUniformComp() {return getInt(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);}

	static std::string getShaderVersion() {return getString ( GL_SHADING_LANGUAGE_VERSION );}



};

#endif // GPUPARAMS_H
