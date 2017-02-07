#ifndef MATERIALELEMENT_H
#define MATERIALELEMENT_H

#include "ShaderState.h"
#include "ShaderParams.h"
#include "../shader/Shader.h"

/**
 * base class for all material elements
 */
class MaterialElement {

private:

	friend class Material;
	friend class ShaderCompiler;

	/** bind the bumpmap-material. before rendering */
	virtual void bind() = 0;

	/** unbind the bumpmap-material. after rendering */
	virtual void unbind() = 0;

	/** get code parts needed for the header (before main()) */
	virtual std::string getHeaderCode() = 0;

	/** get code paarts needed within main() */
	virtual std::string getMainCode() = 0;

	/** attach needed variables */
	virtual void addVariables(ShaderParamsOLD& params) = 0;

	/** configure shader uniforms etc. to match this material */
	virtual void configureShader(Shader* shader, ShaderState& state) = 0;

};

#endif // MATERIALELEMENT_H
