#ifndef MATERIALAMBIENTTEXTUREBLENDRGB_H
#define MATERIALAMBIENTTEXTUREBLENDRGB_H


#include "MaterialAmbient.h"
#include "../../textures/Texture2D.h"
#include "../../textures/Texture2DArray.h"
#include "../../textures/Multitexture.h"

#include <sstream>

/**
 * ambient depends on a blending between several
 * textures using a blend-map. this blend map uses the RGB format:
 * black = texture 0 (background)
 * red = texture 1
 * green = texture 2
 * blue = texture 3
 *
 * so we support up to 4 different textures and need at least 2.
 */
class MaterialAmbientTextureBlendRGB : public MaterialAmbient {

private:

	Texture2DArray* texAmbient;
	std::vector<Vec2> tiles;
	Texture2D* texAmbientBlend;

	Multitexture textures;

public:

	/** ctor */
	MaterialAmbientTextureBlendRGB(Texture2DArray* textures, const std::vector<Vec2>& tiles, Texture2D* blendMap)
		: texAmbient(textures), tiles(tiles), texAmbientBlend(blendMap) {

		// sanity checks
		_assertBetween(textures->getNumLayers(), 2, 4, "number of textures to blend must be between 2 and 4");
		_assertEqual((int)tiles.size(), textures->getNumLayers(), "number of textures must match the number of tiling-factors");

	}

	std::string getHeaderCode() override {

		const int num = texAmbient->getNumLayers();

		std::stringstream ss;
		ss << "uniform sampler2DArray texAmbient;\n";			// the ambient texture array (2-4 layers)
		ss << "uniform sampler2D texAmbientBlend;\n\n";			// the RGB blend-map

		// ambient color calculation
		ss << "vec4 getAmbient() {\n";
		ss << "\tvec3 blend = texture(texAmbientBlend, uv).rgb;\n";
		ss << "\tfloat len = length(blend);\n";
		ss << "\tfloat bg = max(0.0, 1.0-len);\n";
		ss << "\tif (len > 1) {blend /= len;}\n";

		ss << "\tvec4 res = vec4(0,0,0,1);\n";
		if (num >= 1) {ss << "\tif (bg != 0)		{res.rgb +=      bg * texture(texAmbient, vec3(uv * vec2(" << tiles[0].x << "," << tiles[0].y << "), 0)).rgb;}\n";}
		if (num >= 2) {ss << "\tif (blend.r != 0)	{res.rgb += blend.r * texture(texAmbient, vec3(uv * vec2(" << tiles[1].x << "," << tiles[1].y << "), 1)).rgb;}\n";}
		if (num >= 3) {ss << "\tif (blend.g != 0)	{res.rgb += blend.g * texture(texAmbient, vec3(uv * vec2(" << tiles[2].x << "," << tiles[2].y << "), 2)).rgb;}\n";}
		if (num >= 4) {ss << "\tif (blend.b != 0)	{res.rgb += blend.b * texture(texAmbient, vec3(uv * vec2(" << tiles[3].x << "," << tiles[3].y << "), 3)).rgb;}\n";}

		// done
		ss << "\treturn res;\n";
		ss << "}\n";

		return ss.str();

	}

	std::string getMainCode() override {
		return "\tvec4 ambient = getAmbient();\n";
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


#endif // MATERIALAMBIENTTEXTUREBLENDRGB_H
