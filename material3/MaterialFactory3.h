#ifndef MATERIALFACTORY3_H
#define MATERIALFACTORY3_H


#include "Material3.h"
#include "../lib/tinyxml2/tinyxml2.h"
#include "../Exception.h"
#include "xml.h"
#include "../data/Resource.h"
#include "../data/ResourceFactory.h"
#include "../Debug.h"

#include <unordered_map>

class Components {
public:
	std::unordered_map<std::string, ProvColor*> color;
	std::unordered_map<std::string, ProvUV*> uv;
	inline ProvUV* getUV(const std::string& id) const;
	inline ProvColor* getColor(const std::string& color) const;
	inline void parseColor(Material3* mat, XMLElem* elem);
	inline void parseUV(Material3* mat, XMLElem* elem);
	inline void addDefaults();
};

class Scene;

class MaterialFactory3 {


	static constexpr const char* NAME = "MatFac3";

	Components comp;

	Scene* scene;

	/** temporal storage for UV providers */
	std::unordered_map<std::string, ProvUV*> uv;

public:

	/** ctor */
	MaterialFactory3(Scene* scene) : scene(scene) {
		;
	}

	Material3* load(const Resource& res) {

		Debug(NAME, "loading: " + res.getName());

		Data data = ResourceFactory::get().get(res);

		return load(data, res.getName());

	}

	Material3* load(const Data& data, const std::string& origResourceName = "") {

		Debug(NAME, "loading data: " + std::to_string(data.size()) + " bytes");

		// create a new material
		Material3* mat = new Material3();
		mat->dataPath = getFolder(origResourceName);

		XMLDoc doc;
		//assertOK(doc.LoadFile(file.c_str()), doc, "error while loading file");
		assertOK(doc.Parse((const char*)data.get(), data.size()), doc, "error while parsing XML");

		XMLElem* root = doc.FirstChildElement("material");
		if (root == nullptr) {throw Exception("missing root element");}

		// assign name
		const std::string name = root->Attribute("name");
		mat->name = name;

		// first step, load all needed textures from disk
		loadTextures(mat, root->FirstChildElement("textures"));

		// load all individual components that are needed for the shader (uv-providers, color-providers, ..)
		loadComponents(mat, root->FirstChildElement("components"));

		//		loadModifiers(mat, root->FirstChildElement("modifiers"));
		loadUserValues(mat, root->FirstChildElement("user"));
		loadConfig(mat, root->FirstChildElement("config"));

		check(mat);

		compile(mat);

		/*
		XMLElem* xmlMat = root->FirstChildElement("config");

		setClipping(mat, xmlMat);
		buildLighting(mat, xmlMat);
		buildDiffuse(mat, xmlMat);
		buildShadow(mat, xmlMat);
		buildBumpMap(mat, xmlMat);
		buildSpecular(mat, xmlMat);
		*/

		//buildPixel(mat);

		//compile(mat);

		// TODO
//		Engine::get()->tick();

		return mat;

	}

private:

	inline void loadUserValues(Material3* mat, XMLElem* elem);
	inline void loadTextures(Material3* mat, XMLElem* elem);
	inline void loadComponents(Material3* mat, XMLElem* elem);

	inline void loadModifiers(Material3* mat, XMLElem* elem);
	inline void loadModifiersUV(Material3* mat, XMLElem* elem);
	inline void loadConfig(Material3* mat, XMLElem* elem);

	inline void check(Material3* mat);

	inline void compile(Material3* mat);



	inline void buildDiffuse(Material3* mat, XMLElem* elem);
	inline void buildBumpMap(Material3* mat, XMLElem* elem);

	inline ProvTexture* getTexture(Material3* mat, XMLElem* elem);

private:

	/** get a loaded uv-provider by its name */
	inline ProvUV* getUV(const std::string& name);

	/** get only the folder the given absolute file belongs to */
	static std::string getFolder(const std::string& file) {
		std::size_t pos = file.find_last_of('/');
		return file.substr(0, pos + 1);
	}

	static void assertOK(XMLErr res, XMLDoc& doc, const std::string& txt) {
		if (res != tinyxml2::XMLError::XML_SUCCESS) {
			const std::string err = doc.ErrorName();
			const std::string add = doc.GetErrorStr1();
			throw Exception(txt + ": " + err + " - " + add);
		}
	}

};

#endif // MATERIALFACTORY3_H
