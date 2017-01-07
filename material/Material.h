#ifndef MATERIAL_H
#define MATERIAL_H

#include "ambient/MaterialAmbient.h"
#include "bump/MaterialBumpMap.h"
#include "specular/MaterialSpecular.h"

#include "../textures/Multitexture.h"

class Scene;
class Shader;

class Material {

protected:

	friend class ShaderCompiler;

	/** whether this material can receive shadows */
	bool receivesShadows;

	/** whether this material casts shadows */
	bool castsShadows;

	/** whether to use lighting or not */
	bool useLigthing;

	/** whether this material can be clipped */
	bool useClipping;

	/** the ambient material (texture, multitexture, color, ...) */
	MaterialAmbient* ambient;

	/** the bump-map material (texture, multitexture, ...) */
	MaterialBumpMap* bumpMap;

	/** the specular material (fixed-value, texture, ...) */
	MaterialSpecular* specular;

	/** used textures */
	Multitexture textures;

public:

	/** ctor */
	Material() : receivesShadows(false), castsShadows(false), useLigthing(true), useClipping(true),
		ambient(nullptr), bumpMap(nullptr), specular(nullptr) {
		;
	}

	/** ctor */
	Material(const bool receivesShadows, const bool castsShadows, const bool useLighting) :
		receivesShadows(receivesShadows), castsShadows(castsShadows), useLigthing(useLighting), useClipping(true),
		ambient(nullptr), bumpMap(nullptr), specular(nullptr) {
		;
	}

	/** whether this material can receive shadows */
	void setReceivesShadows(const bool receives) {this->receivesShadows = receives;}
	bool getReceivesShadows() const {return receivesShadows;}

	/** whether this material casts shadows */
	void setCastsShadows(const bool casts) {this->castsShadows = casts;}
	bool getCastsShadows() const {return castsShadows;}

	/** whether to use lighting for this material */
	void setUseLighting(const bool use) {this->useLigthing = use;}
	bool usesLighting() const {return useLigthing;}

	/** whether this material can be clipped */
	void setUseClipping(const bool use) {this->useClipping = use;}
	bool usesClipping() const {return useClipping;}


	/** get the ambient material */
	MaterialAmbient* getAmbient() const {return ambient;}
	void setAmbient(MaterialAmbient* ambient) {this->ambient = ambient;}

	/** get the bump-map material */
	MaterialBumpMap* getBumpMap() const {return bumpMap;}
	void setBumpMap(MaterialBumpMap* map) {this->bumpMap = map;}

	/** get the specular material */
	MaterialSpecular* getSpecular() const {return specular;}
	void setSpecular(MaterialSpecular* specular) {this->specular = specular;}


	/** whether this material uses normal maps somewhere */
	bool usesNormalMap() const {return bumpMap != nullptr;}

	/** bind the material. before rendering */
	void bind() {
		if (getAmbient())	{getAmbient()->bind();}
		if (getBumpMap())	{getBumpMap()->bind();}
		if (getSpecular())	{getSpecular()->bind();}
		textures.bindAll();
		Error::assertOK();
	}

	/** unbind the material. after rendering */
	void unbind() {
		if (getAmbient())	{getAmbient()->unbind();}
		if (getBumpMap())	{getBumpMap()->unbind();}
		if (getSpecular())	{getSpecular()->unbind();}
		textures.unbindAll();
		Error::assertOK();
	}

};



#endif // MATERIAL_H
