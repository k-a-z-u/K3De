#ifndef MATERIALAMBIENTTEXTURE_H
#define MATERIALAMBIENTTEXTURE_H

#include "MaterialAmbient.h"
#include "../MaterialTexture.h"
#include "../mod/ModUVOffset.h"

class MaterialAmbientTexture : public MaterialAmbient {

private:

//	ITexture* texAmbient0;
//	Vec2 tiling0;
//	Multitexture textures;

	MaterialTexture texAmbient;

public:

//	/** ctor */
//	MaterialAmbientTexture(ITexture* tex, const Vec2 tiling = Vec2(1,1)) : texAmbient0(tex), tiling0(tiling) {
//		;
//	}

	/** ctor */
	MaterialAmbientTexture(ITexture* tex, const Vec2 tiling = Vec2(1,1)) : texAmbient("texAmbient0", tex, tiling) {
		;
	}

	std::string getHeaderCode() override {
		return texAmbient.getHeaderCode();
	}

	std::string getMainCode() override {

//		// use UV as is or multiplied by a tiling factor?
//		std::string uv;
//		if (tiling0 != Vec2(1,1)) {
//			uv = "uv * vec2(" + std::to_string(tiling0.x) + "," + std::to_string(tiling0.y) + ")";
//		} else {
//			uv = "uv";
//		}

//		std::stringstream ss;

//		// output variable
//		ss << "\tvec4 ambient = ";

//		// alpha-only textures are handled differently
//		if (texAmbient0->isAlphaOnly()) {
//			ss << "vec4( 1, 1, 1, texture(texAmbient0, " + uv + ").a );\n";
//		} else {
//			ss << "texture(texAmbient0, uv);\n";
//		}

//		// done
//		return ss.str();

		return "\tvec4 ambient = " + texAmbient.getValue("uv") + ";\n";

	}

	void addVariables(ShaderParamsOLD& params) override {
		texAmbient.addVariables(params);
	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {
		texAmbient.configureShader(shader, state);
	}

	virtual void bind() override {
		texAmbient.bind();
	}

	virtual void unbind() override {
		texAmbient.unbind();
	}

//	virtual void configureShader(Shader* shader, ShaderState& state) override {
//		shader->setInt("texAmbient0", state.nextFreeTextureSlot);
//		textures.set(state.nextFreeTextureSlot, texAmbient0);
//		++state.nextFreeTextureSlot;
//	}

//	virtual void bind() {
//		textures.bindAll();
//	}

//	virtual void unbind() {
//		textures.unbindAll();
//	}

};

#endif // MATERIALAMBIENTTEXTURE_H
