#ifndef MATERIAL2_H
#define MATERIAL2_H

#include "../textures/Multitexture.h"
#include "parts/MatTexture.h"
#include "ShaderParamsFragment.h"
#include "ShaderParamsVertex.h"
#include "ElementLookup.h"

class Shader;

namespace MatPart {
	class Modifier;
	class LoadedTexture;
}

class Material2 {

	friend class MaterialFactory;

private:

	/** all textures attached to this material */
	Multitexture bindTextures;

	/** whether this material can receive shadows */
	bool receivesShadows;

	/** whether this material casts shadows */
	bool castsShadows;

	/** whether to use lighting or not */
	bool useLigthing;

	/** does this material use specular lighting? */
	bool useSpecular;

	/** whether this material can be clipped */
	bool useClipping;

	/** the material's shader */
	Shader* shader;

	/** path the material stores all its data at */
	std::string dataPath;

private:

	ShaderParamsVertex paramsVertex;
	ShaderParamsFragment paramsFragment;
	ElementLookup<MatPart::Modifier*> modifiers;
	ElementLookup<MatPart::LoadedTexture> textures;

	std::unordered_map<std::string, std::string> userValues;

public:

	/** get all modifiers used within this material */
	const ElementLookup<MatPart::Modifier*>& getModifiers() const {return modifiers;}
	ElementLookup<MatPart::Modifier*>& getModifiers() {return modifiers;}

	/** get all textures used within this material */
	const ElementLookup<MatPart::LoadedTexture>& getTextures() const {return textures;}
	ElementLookup<MatPart::LoadedTexture>& getTextures() {return textures;}

	/** get all textures used within this material */
	const std::unordered_map<std::string, std::string>& getUserValues() const {return userValues;}
	std::unordered_map<std::string, std::string>& getUserValues() {return userValues;}

	const ShaderParamsFragment& getFragmentParams() const {return paramsFragment;}
	ShaderParamsFragment& getFragmentParams() {return paramsFragment;}

	const ShaderParamsVertex& getVertexParams() const {return paramsVertex;}
	ShaderParamsVertex& getVertexParams() {return paramsVertex;}

	/** whether this material can receive shadows */
	bool getReceivesShadows() const {return receivesShadows;}

	/** whether this material casts shadows */
	bool getCastsShadows() const {return castsShadows;}

	/** whether to use lighting for this material */
	bool usesLighting() const {return useLigthing;}

	/** whether this material can be clipped */
	bool usesClipping() const {return useClipping;}



	/** get the material's shader */
	Shader* getShader() const {return shader;}

	void bind() {
		bindTextures.bindAll();
	}

	void unbind() {
		bindTextures.unbindAll();
	}

};

#endif // MATERIAL2_H
