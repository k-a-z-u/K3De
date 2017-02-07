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
 *
 * uses 2D texture arrays, as the are a little faster than several single 2D textures
 * note: all input textures thus need to have the same resolution!
 *
 */
class MaterialAmbientTextureBlending : public MaterialAmbient {

private:

	Texture2DArray* texAmbient;
	std::vector<Vec2> tiles;

	Texture2D* texAmbientBlend;
	Vec2 blendTile;

	Multitexture textures;

public:

	/** ctor */
	MaterialAmbientTextureBlending(Texture2DArray* textures, const std::vector<Vec2>& tiles, Texture2D* blendMap, const Vec2 blendTile = Vec2(1,1))
	    : texAmbient(textures), tiles(tiles), texAmbientBlend(blendMap), blendTile(blendTile) {

		// sanity check
		if (!blendMap->isAlphaOnly()) {throw Exception("blend map must be an alpha-only texture");}

	}

	std::string getHeaderCode() override {

		const int num = texAmbient->getNumLayers();
		const float d = 0.00001f;

		std::stringstream ss;
		ss << "uniform sampler2DArray texAmbient;\n";
		ss << "uniform sampler2D texAmbientBlend;\n\n";

		ss << "vec4 getAmbient() {\n"; {

			ss << "\tfloat blend = texture(texAmbientBlend, uv * vec2(" << blendTile.x << ", " << blendTile.y << ")).a * " << (num-1) << ".0f;\n";
			ss << "\tvec3 res = \n";

			for (int i = 0; i < num; ++i) {

				// "i" selects the texture within the 2D texture array
				ss << "\t\ttexture(texAmbient, ";
				ss << "vec3(uv * vec2(" << tiles[i].x+d << "f, " << tiles[i].y+d << "f), " << i << ")).rgb ";
				ss << "* max(1.0f - abs(blend-"<< i <<".0f), 0.0f)";

				// additional values following?
				if (i < num-1) {ss << " + \n";} else {ss << ";\n";}

			}

			ss << "\treturn vec4(res,1);\n";

		} ss << "}\n";

		return ss.str();

	}

	std::string getMainCode() override {
		return "\tvec4 ambient = getAmbient();\n";
	}

	void addVariables(ShaderParamsOLD& params) override {

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
