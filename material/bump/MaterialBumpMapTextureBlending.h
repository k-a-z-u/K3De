#ifndef MATERIALBUMPMAPTEXTUREBLENDING_H
#define MATERIALBUMPMAPTEXTUREBLENDING_H

#include "MaterialBumpMap.h"
#include "../../textures/Texture2D.h"
#include "../../textures/Texture2DArray.h"
#include "../../textures/Multitexture.h"

#include <sstream>

/**
 * bump-map depends on a blending between several
 * textures using a blend-map
 */
class MaterialBumpMapTextureBlending : public MaterialBumpMap {

private:

	Texture2DArray* texBumpMap;
	std::vector<Vec2> tiles;
	Texture2D* texBumpMapBlend;

	Multitexture textures;

public:

	/** ctor */
	MaterialBumpMapTextureBlending(Texture2DArray* textures, const std::vector<Vec2>& tiles, Texture2D* blendMap)
		: texBumpMap(textures), tiles(tiles), texBumpMapBlend(blendMap) {
		;
	}

	std::string getHeaderCode() override {

		const int num = texBumpMap->getNumLayers();

		std::stringstream ss;
		ss << "uniform sampler2DArray texBumpMap;\n";
		ss << "uniform sampler2D texBumpMapBlend;\n\n";

		ss << "vec3 getBumpMap() {\n";
		//ss << "\tconst vec2 uvScale = vec2(10,200)*0.75;\n";
		ss << "\tfloat t = texture(texBumpMapBlend, uv).a * " << num << " - 0.5;\n";
		ss << "\tvec3 res;\n";
		for (int i = 0; i < num; ++i) {
			ss << "\tres += normalize(texture(texBumpMap, vec3(uv*vec2("<<tiles[i].x<<","<<tiles[i].y<<"), "<< i <<")).rgb - 0.5) * max(1.0-abs(t-"<< i <<"), 0.0);\n";
		}
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

#endif // MATERIALBUMPMAPTEXTUREBLENDING_H
