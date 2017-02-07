#ifndef MATERIALAMBIENTFIXEDCOLOR_H
#define MATERIALAMBIENTFIXEDCOLOR_H

#include "MaterialAmbient.h"
#include "../../textures/ITexture.h"
#include "../../textures/Multitexture.h"

class MaterialAmbientFixedColor : public MaterialAmbient {

private:

	Vec4 color;

public:

	/** ctor */
	MaterialAmbientFixedColor(Vec4 color) : color(color) {
		;
	}

	std::string getHeaderCode() override {
		return "";
	}

	std::string getMainCode() override {
		return "\tvec4 ambient = vec4(" + std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," + std::to_string(color.a) + ");\n";
	}

	void addVariables(ShaderParamsOLD& params) override {

	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {
		(void) shader;
		(void) state;
	}

	virtual void bind() {
		;
	}

	virtual void unbind() {
		;
	}

};


#endif // MATERIALAMBIENTFIXEDCOLOR_H
