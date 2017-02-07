#ifndef MODUVOFFSETTEXTURE_H
#define MODUVOFFSETTEXTURE_H

#include "ModUVOffset.h"
#include <unordered_set>
#include "../../textures/ITexture.h"

/**
 * modify UV texture coordinates based on a given UV-map texture
 */
class ModUVOffsetTexture : public ModUVOffset {

private:

	std::unordered_set<std::string> once;
	ITexture* tex;
	float scale;

	std::string getFuncName() {
		return "getUVOffset";
	}

public:


	ModUVOffsetTexture(ITexture* tex, const float scale) : tex(tex), scale(scale) {
		;
	}

	void addVariables(ShaderParams& params) override {
		if (previous) {previous->addVariables(params);}


		std::stringstream ss;

		ss << "vec2 " << getFuncName() << "(vec2 inp) {\n";
		    if (previous) {
				ss << "\tvec2 uv = " + previous->getModified("uv", "inp") + ";\n";
			} else {
				ss << "\tvec2 uv = inp;\n";
			}
			ss << "\tvec2 offset = texture(texUVOffset, uv).rg;\n";
			ss << "\treturn inp + (offset * " << scale << "f);\n";
		ss << "}\n";

		params.addVariable("texUVOffset", "uniform sampler2D texUVOffset;\n");
		params.addFunction("texUVOffset", ss.str());

	}

	std::string getHeaderCode(const std::string& context) override {

		std::stringstream ss;

		if (previous) {
			ss << previous->getHeaderCode(context);
		}

//		if (once.find("texture") == once.end()) {
//			once.insert("texture");
//			ss << "uniform sampler2D texUVOffset;\n";
//		}

		return ss.str();

	}

	std::string getMainCode(const std::string& context) override {
		return "";
	}

	std::string getModified(const std::string& context, const std::string& inpUV) override {

		return getFuncName() + "(" + inpUV + ")";

	}


};

#endif // MODUVOFFSETTEXTURE_H
