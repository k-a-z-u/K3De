#ifndef MATERIALSPECULAR_H
#define MATERIALSPECULAR_H

/**
 * base-class for all specular materials
 */
class MaterialSpecular {

public:

	virtual ~MaterialSpecular() {;}

	/** bind the specular-material. before rendering */
	virtual void bind() = 0;

	/** unbind the specular-material. after rendering */
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

#endif // MATERIALSPECULAR_H
