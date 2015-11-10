#ifndef MATERIALSPECULARTEXTURE_H
#define MATERIALSPECULARTEXTURE_H


#include "MaterialSpecular.h"
#include "../../textures/ITexture.h"
#include "../../textures/Multitexture.h"

/**
 * use a texture as specular-map.
 * each texel will have its own specular strength
 *
 * the texture must be in RGBA format.
 * RGB:		determines the specular color
 * Alpha:	determines the specular strength
 *
 */
class MaterialSpecularTexture : public MaterialSpecular {

private:

	const float maxShininess;
	ITexture* texSpecularMap0;
	Multitexture textures;


public:

	/** ctor */
	MaterialSpecularTexture(ITexture* tex, const float maxShininess) : maxShininess(maxShininess) {
		texSpecularMap0 = tex;
	}

	std::string getHeaderCode() override {
		return "uniform sampler2D texSpecularMap0;\n";
	}

	std::string getMainCode() override {
		return	std::string() +
				"vec4 texSpecularMap0Color = texture(texSpecularMap0, uv);\n" +
				"vec3 specularColor = texSpecularMap0Color.rgb;\n" +
				"float specularShininess = texSpecularMap0Color.a * " + std::to_string(maxShininess) + ";\n";
	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {
		shader->setInt("texSpecularMap0", state.nextFreeTextureSlot);
		textures.set(state.nextFreeTextureSlot, texSpecularMap0);
		++state.nextFreeTextureSlot;
	}

	virtual void bind() {
		textures.bindAll();
	}

	virtual void unbind() {
		textures.unbindAll();
	}

};

#endif // MATERIALSPECULARTEXTURE_H
