#ifndef MATERIALAMBIENT_H
#define MATERIALAMBIENT_H

#include "../ShaderState.h"
#include "../../shader/Shader.h"

/**
 * base-class for all ambient materials
 */
class MaterialAmbient {

public:

	virtual ~MaterialAmbient() {;}

	/** bind the ambient-material. before rendering */
	virtual void bind() = 0;

	/** unbind the ambient-material. after rendering */
	virtual void unbind() = 0;

private:

	friend class Material;
	friend class ShaderCompiler;

	/** get code parts needed for the header (before main()) */
	virtual std::string getHeaderCode() = 0;

	/** get the GLSL code calculating the ambient color */
	virtual std::string getMainCode() = 0;

	/** configure shader uniforms to match this material */
	virtual void configureShader(Shader* shader, ShaderState& state) = 0;

};

#endif // MATERIALAMBIENT_H
