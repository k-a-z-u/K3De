#ifndef MATERIALBUMPMAP_H
#define MATERIALBUMPMAP_H

#include "../ShaderState.h"
#include "../../shader/Shader.h"

/**
 * base-class for all bump-map materials
 */
class MaterialBumpMap {

public:

	virtual ~MaterialBumpMap() {;}

	/** bind the bumpmap-material. before rendering */
	virtual void bind() = 0;

	/** unbind the bumpmap-material. after rendering */
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

#endif // MATERIALBUMPMAP_H
