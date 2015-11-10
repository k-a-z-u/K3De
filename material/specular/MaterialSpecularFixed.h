#ifndef MATERIALSPECULARFIXED_H
#define MATERIALSPECULARFIXED_H

#include "MaterialSpecular.h"

/**
 * represents a simple, fixed specular value for the whole material
 */
class MaterialSpecularFixed : public MaterialSpecular {

private:

	/** the fixed, specular strength to use */
	const float shininess;

	const float impact;

public:

	/** ctor */
	MaterialSpecularFixed(const float shininess, const float impact) : shininess(shininess), impact(impact) {
		;
	}

	std::string getHeaderCode() override {
		return "";
	}

	std::string getMainCode() override {
		return	std::string() +
				"vec3 specularColor = ambient.rgb * " + std::to_string(impact) + ";\n" +
				"float specularShininess = " + std::to_string(shininess) + ";\n";
	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {
		;
	}

	virtual void bind() {
		;
	}

	virtual void unbind() {
		;
	}

};

#endif // MATERIALSPECULARFIXED_H
