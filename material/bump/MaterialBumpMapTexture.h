#ifndef MATERIALBUMPMAPTEXTURE_H
#define MATERIALBUMPMAPTEXTURE_H

#include "MaterialBumpMap.h"
#include "../../textures/ITexture.h"
#include "../../textures/Multitexture.h"

class MaterialBumpMapTexture : public MaterialBumpMap {

private:

	ITexture* texBumpMap0;
	Multitexture textures;

public:

	/** ctor */
	MaterialBumpMapTexture(ITexture* tex) {
		texBumpMap0 = tex;
	}

	std::string getHeaderCode() override {
		return "uniform sampler2D texBumpMap0;\n";
	}

	std::string getMainCode() override {
		return "normalize(texture(texBumpMap0, uv).rgb - 0.5);\n";
	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {
		shader->setInt("texBumpMap0", state.nextFreeTextureSlot);
		textures.set(state.nextFreeTextureSlot, texBumpMap0);
		++state.nextFreeTextureSlot;
	}

	virtual void bind() override {
		textures.bindAll();
	}

	virtual void unbind() override {
		textures.unbindAll();
	}

};


#endif // MATERIALBUMPMAPTEXTURE_H
