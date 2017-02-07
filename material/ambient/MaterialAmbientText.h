#ifndef MATERIALAMBIENTTEXT_H
#define MATERIALAMBIENTTEXT_H

#include "MaterialAmbient.h"
#include "../../textures/ITexture.h"
#include "../../textures/Multitexture.h"

/**
 * used to render (colored) text
 * the complete font [all letters] is stored within one texture file
 * this texture is used as alpha texture
 * the letter's color is given by an additional color attribute for each vertex
 */
class MaterialAmbientText : public MaterialAmbient {

private:

	ITexture* texAmbient0;
	Multitexture textures;

public:

	/** ctor */
	MaterialAmbientText(ITexture* tex) : texAmbient0(tex) {
		;
	}

	std::string getHeaderCode() override {
		std::stringstream ss;
		ss << "uniform sampler2D texAmbient0;\n";
		ss << "in vec4 color_M;\n";
		return ss.str();
	}

	std::string getMainCode() override {

		std::stringstream ss;

		// output variable
		ss << "\tvec4 ambient = texture(texAmbient0, uv).a * color_M;\n";

		// done
		return ss.str();

	}

	void addVariables(ShaderParamsOLD& params) override {

	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {
		shader->setInt("texAmbient0", state.nextFreeTextureSlot);
		textures.set(state.nextFreeTextureSlot, texAmbient0);
		++state.nextFreeTextureSlot;
	}

	virtual void bind() {
		textures.bindAll();
	}

	virtual void unbind() {
		textures.unbindAll();
	}

};


#endif // MATERIALAMBIENTTEXT_H
