#ifndef MATERIALBUMPMAPTEXTUREBLENDRGB_H
#define MATERIALBUMPMAPTEXTUREBLENDRGB_H

#include "MaterialBumpMap.h"
#include "../../textures/Texture2D.h"
#include "../../textures/Texture2DArray.h"
#include "../../textures/Multitexture.h"

#include <sstream>

/**
 * bump-map depends on a blending between several
 * textures using a blend-map. this blend map uses the RGB format:
 * black = texture 0 (background)
 * red = texture 1
 * green = texture 2
 * blue = texture 3
 *
 * so we support up to 4 different textures and need at least 2.
 */
class MaterialBumpMapTextureBlendRGB : public MaterialBumpMap {

private:

	Texture2DArray* texBumpMap;
	std::vector<Vec2> tiles;
	Texture2D* texBumpMapBlend;

	Multitexture textures;

public:

	/** ctor */
	MaterialBumpMapTextureBlendRGB(Texture2DArray* textures, const std::vector<Vec2>& tiles, Texture2D* blendMap)
		: texBumpMap(textures), tiles(tiles), texBumpMapBlend(blendMap) {

		// sanity checks
		_assertBetween(textures->getNumLayers(), 2, 4, "number of textures to blend must be between 2 and 4");
		_assertEqual(tiles.size(), textures->getNumLayers(), "number of textures must match the number of tiling-factors");

	}

	std::string getHeaderCode() override {

		const int num = texBumpMap->getNumLayers();

		std::stringstream ss;
		ss << "uniform sampler2DArray texBumpMap;\n";			// the ambient texture array (2-4 layers)
		ss << "uniform sampler2D texBumpMapBlend;\n\n";			// the RGB blend-map

		// ambient color calculation
		ss << "vec3 getBumpMap() {\n";
		ss << "\tvec3 blend = texture(texBumpMapBlend, uv).rgb;\n";
		ss << "\tfloat len = length(blend);\n";
		ss << "\tfloat bg = max(0.0, 1.0-len);\n";
		ss << "\tif (len > 1) {blend /= len;}\n";

		ss << "\tvec3 res;\n";
		if (num >= 1) {ss << "\tif (bg != 0)		{res.rgb +=      bg * normalize(texture(texBumpMap, vec3(uv * vec2(" << tiles[0].x << "," << tiles[0].y << "), 0)).rgb - 0.5);}\n";}
		if (num >= 2) {ss << "\tif (blend.r != 0)	{res.rgb += blend.r * normalize(texture(texBumpMap, vec3(uv * vec2(" << tiles[1].x << "," << tiles[1].y << "), 1)).rgb - 0.5);}\n";}
		if (num >= 3) {ss << "\tif (blend.g != 0)	{res.rgb += blend.g * normalize(texture(texBumpMap, vec3(uv * vec2(" << tiles[2].x << "," << tiles[2].y << "), 2)).rgb - 0.5);}\n";}
		if (num >= 4) {ss << "\tif (blend.b != 0)	{res.rgb += blend.b * normalize(texture(texBumpMap, vec3(uv * vec2(" << tiles[3].x << "," << tiles[3].y << "), 3)).rgb - 0.5);}\n";}

		// done
		ss << "\treturn res;\n";
		ss << "}\n";

		return ss.str();

	}

	std::string getMainCode() override {
		return "getBumpMap();\n";
	}

	virtual void configureShader(Shader* shader, ShaderState& state) override {

		shader->setInt("texBumpMap", state.nextFreeTextureSlot);
		textures.set(state.nextFreeTextureSlot, texBumpMap);
		++state.nextFreeTextureSlot;

		shader->setInt("texBumpMapBlend", state.nextFreeTextureSlot);
		textures.set(state.nextFreeTextureSlot, texBumpMapBlend);
		++state.nextFreeTextureSlot;

	}

	virtual void bind() {
		textures.bindAll();
	}

	virtual void unbind() {
		textures.unbindAll();
	}


};

#endif // MATERIALBUMPMAPTEXTUREBLENDRGB_H
