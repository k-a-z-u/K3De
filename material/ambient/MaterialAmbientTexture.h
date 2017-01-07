#ifndef MATERIALAMBIENTTEXTURE_H
#define MATERIALAMBIENTTEXTURE_H

#include "MaterialAmbient.h"
#include "../../textures/ITexture.h"
#include "../../textures/Multitexture.h"

class MaterialAmbientTexture : public MaterialAmbient {

private:

	ITexture* texAmbient0;
	Vec2 tiling0;

	Multitexture textures;

public:

	/** ctor */
	MaterialAmbientTexture(ITexture* tex, const Vec2 tiling = Vec2(1,1)) : texAmbient0(tex), tiling0(tiling) {
		;
	}

	std::string getHeaderCode() override {
		return "uniform sampler2D texAmbient0;\n";
	}

	std::string getMainCode() override {

		// use UV as is or multiplied by a tiling factor?
		std::string uv;
		if (tiling0 != Vec2(1,1)) {
			uv = "uv * vec2(" + std::to_string(tiling0.x) + "," + std::to_string(tiling0.y) + ")";
		} else {
			uv = "uv";
		}

		std::stringstream ss;

		// output variable
		ss << "\tvec4 ambient = ";

		// alpha-only textures are handled differently
		if (texAmbient0->isAlphaOnly()) {
			ss << "vec4( 1, 1, 1, texture(texAmbient0, " + uv + ").a );\n";
		} else {
			ss << "texture(texAmbient0, uv);\n";
		}

		// done
		return ss.str();

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

#endif // MATERIALAMBIENTTEXTURE_H
