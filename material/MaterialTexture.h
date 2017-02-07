#ifndef MATERIALTEXTURE_H
#define MATERIALTEXTURE_H

#include "MaterialElement.h"
#include "../textures/ITexture.h"

class MaterialTexture {

private:

	std::string name;

	ITexture* texture = nullptr;

	Vec2 tiling;

	int unit = -1;

public:

//	/** ctor */
//	MaterialTexture(const std::string& name) : name(name) {
//		;
//	}

	MaterialTexture(const std::string& name, ITexture* texture, Vec2 tiling = Vec2(1,1)) : name(name), texture(texture), tiling(tiling) {
		;
	}

	//friend class MaterialElement;

	/** get the code needed for the fragment-shader's header */
	std::string getHeaderCode() {
		//return "uniform sampler2D " + name + ";\n" +
		//return ((uvOffset) ? (uvOffset->getHeaderCode(name)) : (""));
		return "";
	}

	void addVariables(ShaderParamsOLD& params) {
		params.addVariable(name, "uniform sampler2D " + name + ";");
		//if (uvOffset) {uvOffset->addVariables(params);}
	}

	/** access the texture's value based on the given uv [which is a function or variable to a vec2()] */
	std::string getValue(const std::string uv) {

		std::string uv2 = uv;
		
//		if (uvOffset) {
//			uv2 = uvOffset->getModified(name, uv2);
//		}
		
		if (tiling != Vec2(1,1)) {
			uv2 += " * vec2(" + std::to_string(tiling.x) + "," + std::to_string(tiling.y) + ")";
		}

		return "texture(" + name + ", " + uv2 + ")";

	}

	void configureShader(Shader* shader, ShaderState& state) {

		this->unit = state.nextFreeTextureSlot;
		++state.nextFreeTextureSlot;

		shader->setInt(name, unit);

	}

	void bind() {
		texture->bind(unit);
	}

	void unbind() {
		texture->unbind(unit);
	}


};

#endif // MATERIALTEXTURE_H
