#ifndef MATERIALBUMPMAPTEXTURE_H
#define MATERIALBUMPMAPTEXTURE_H

#include "MaterialBumpMap.h"
#include "../MaterialTexture.h"

class MaterialBumpMapTexture : public MaterialBumpMap {

private:

	MaterialTexture texBumpMap;

public:

	/** ctor */
	MaterialBumpMapTexture(ITexture* tex, Vec2 tiling = Vec2(1,1)) : texBumpMap("texBumpMap0", tex, tiling) {
		;
	}

	std::string getHeaderCode() override {
		//return "uniform sampler2D texBumpMap0;\n";
		return texBumpMap.getHeaderCode();
	}

	std::string getMainCode() override {
		//return "normalize(texture(texBumpMap0, uv).rgb - 0.5);\n";
		return "normalize(" + texBumpMap.getValue("uv") + ".rgb - 0.5);\n";
	}

	void addVariables(ShaderParamsOLD& params) override {
		texBumpMap.addVariables(params);
	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {
		texBumpMap.configureShader(shader, state);
	}

	virtual void bind() override {
		texBumpMap.bind();
	}

	virtual void unbind() override {
		texBumpMap.unbind();
	}

//	virtual void configureShader(Shader* shader, ShaderState& state) override {
//		shader->setInt("texBumpMap0", state.nextFreeTextureSlot);
//		textures.set(state.nextFreeTextureSlot, texBumpMap0);
//		++state.nextFreeTextureSlot;
//	}

//	virtual void bind() override {
//		textures.bindAll();
//	}

//	virtual void unbind() override {
//		textures.unbindAll();
//	}

};


#endif // MATERIALBUMPMAPTEXTURE_H
