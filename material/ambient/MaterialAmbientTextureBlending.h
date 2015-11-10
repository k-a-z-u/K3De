#ifndef MATERIALAMBIENTTEXTUREBLENDING_H
#define MATERIALAMBIENTTEXTUREBLENDING_H

#include "MaterialAmbient.h"
#include "../../textures/Texture2D.h"
#include "../../textures/Texture2DArray.h"
#include "../../textures/Multitexture.h"

#include <sstream>

/**
 * ambient depends on a blending between several
 * textures using a blend-map
 */
class MaterialAmbientTextureBlending : public MaterialAmbient {

private:

	Texture2DArray* texAmbient;
	std::vector<Vec2> tiles;
	Texture2D* texAmbientBlend;

	Multitexture textures;

public:

	/** ctor */
	MaterialAmbientTextureBlending(Texture2DArray* textures, const std::vector<Vec2>& tiles, Texture2D* blendMap)
		: texAmbient(textures), tiles(tiles), texAmbientBlend(blendMap) {
		;
	}

	std::string getHeaderCode() override {

		const int num = texAmbient->getNumLayers();

		std::stringstream ss;
		ss << "uniform sampler2DArray texAmbient;\n";
		ss << "uniform sampler2D texAmbientBlend;\n\n";

		ss << "vec4 getAmbient() {\n";
		ss << "\tfloat blend = texture(texAmbientBlend, uv).a * " << (num-1) << ";\n";
		ss << "\tvec3 res;\n";
		for (int i = 0; i < num; ++i) {

			ss << "\tres += texture(texAmbient, ";
			ss << "vec3(uv * vec2(" << tiles[i].x << "," << tiles[i].y << "), " << i << ")).rgb ";
			ss << "* max(1.0-abs(blend-"<< i <<"), 0.0);\n";
		}
		ss << "\treturn vec4(res,1);\n";
		ss << "}\n";

		return ss.str();

	}

	std::string getMainCode() override {
		return "getAmbient();\n";
	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {

		shader->setInt("texAmbient", state.nextFreeTextureSlot);
		textures.set(state.nextFreeTextureSlot, texAmbient);
		++state.nextFreeTextureSlot;

		shader->setInt("texAmbientBlend", state.nextFreeTextureSlot);
		textures.set(state.nextFreeTextureSlot, texAmbientBlend);
		++state.nextFreeTextureSlot;

	}

	virtual void bind() {
		textures.bindAll();
	}

	virtual void unbind() {
		textures.unbindAll();
	}

};

#endif // MATERIALAMBIENTTEXTUREBLENDING_H
