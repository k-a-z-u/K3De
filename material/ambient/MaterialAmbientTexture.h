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
		if (tiling0 != Vec2(1,1)) {
			std::string v = "vec2(" + std::to_string(tiling0.x) + "," + std::to_string(tiling0.y) + ")";
			return "texture(texAmbient0, uv * "+v+");\n";
		} else {
			return "texture(texAmbient0, uv);\n";
		}
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
