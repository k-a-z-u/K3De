#ifndef MATERIALSPECULARTEXTURE_H
#define MATERIALSPECULARTEXTURE_H


#include "MaterialSpecular.h"
#include "../../textures/ITexture.h"
#include "../../textures/Multitexture.h"
#include <sstream>

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
	const float mul;

	ITexture* texSpecularMap0;
	Multitexture textures;


public:

	/** ctor */
	MaterialSpecularTexture(ITexture* tex, const float maxShininess, const float mul = 1) : maxShininess(maxShininess), mul(mul) {

		// sanity check
		//if (!tex->isAlphaOnly()) {throw Exception("specular-texture must be an alpha-only texture e.g. by loading only a grayscale image");}

		texSpecularMap0 = tex;

	}

	std::string getHeaderCode() override {
		return "uniform sampler2D texSpecularMap0;\n";
	}

	std::string getMainCode() override {

		std::stringstream ss;

		ss << "\n";
		ss << "\t// specular texture\n";

		ss << "\tvec4 texSpecularMap0Color = texture(texSpecularMap0, uv);\n";

		if (texSpecularMap0->isAlphaOnly()) {
			ss << "\tvec3 specularColor = vec3(texSpecularMap0Color.a, texSpecularMap0Color.a, texSpecularMap0Color.a) * " << std::to_string(mul) << "f;\n";
		} else {
			ss << "\tvec3 specularColor = texSpecularMap0Color.rgb * " << std::to_string(mul) << "f;\n";
		}

		ss << "\tfloat specularShininess = (texSpecularMap0Color.a * " + std::to_string(maxShininess) + "f);\n";
		ss << "\n";

		return ss.str();

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
