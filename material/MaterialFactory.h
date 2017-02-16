#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "Material2.h"
#include "../lib/tinyxml2/tinyxml2.h"
#include "../Exception.h"
#include "xml.h"
#include "../data/Resource.h"
#include "../data/ResourceFactory.h"

class Scene;

class MaterialFactory {


	const char* NAME = "MatFac2";

	Scene* scene;

public:

	/** ctor */
	MaterialFactory(Scene* scene) : scene(scene) {
		;
	}

//	/** load material from file */
	Material2* load(const std::string& file) {

		Debug(NAME, "loading: " + file);

		// create a new material
		Material2* mat = new Material2();

		// set the data-path to search for included files
		mat->dataPath = getFolder(file);

		XMLDoc doc;
		assertOK(doc.LoadFile(file.c_str()), doc, "error while loading file");

		XMLElem* root = doc.FirstChildElement("material");
		if (root == nullptr) {throw Exception("missing root element");}

		const std::string name = root->Attribute("name");
		mat->paramsFragment.setName(name);
		mat->paramsVertex.setName(name);

		loadTextures(mat, root->FirstChildElement("textures"));
		loadModifiers(mat, root->FirstChildElement("modifiers"));
		loadUserValues(mat, root->FirstChildElement("user"));

		XMLElem* xmlMat = root->FirstChildElement("config");

		setClipping(mat, xmlMat);
		buildLighting(mat, xmlMat);
		buildDiffuse(mat, xmlMat);
		buildShadow(mat, xmlMat);
		buildBumpMap(mat, xmlMat);
		buildSpecular(mat, xmlMat);

		buildPixel(mat);

		compile(mat);

		return mat;

	}

	Material2* load(const Resource& res) {

		Engine::get()->enqueueRender();

		Debug(NAME, "loading: " + res.getName());

		Data data = ResourceFactory::get().get(res);

		// create a new material
		Material2* mat = new Material2();
		mat->dataPath = getFolder(res.getName());

		XMLDoc doc;
		//assertOK(doc.LoadFile(file.c_str()), doc, "error while loading file");
		assertOK(doc.Parse((const char*)data.get(), data.size()), doc, "error while parsing XML");

		XMLElem* root = doc.FirstChildElement("material");
		if (root == nullptr) {throw Exception("missing root element");}

		const std::string name = root->Attribute("name");
		mat->paramsFragment.setName(name);
		mat->paramsVertex.setName(name);

		loadTextures(mat, root->FirstChildElement("textures"));
		loadModifiers(mat, root->FirstChildElement("modifiers"));
		loadUserValues(mat, root->FirstChildElement("user"));

		XMLElem* xmlMat = root->FirstChildElement("config");

		setClipping(mat, xmlMat);
		buildLighting(mat, xmlMat);
		buildDiffuse(mat, xmlMat);
		buildShadow(mat, xmlMat);
		buildBumpMap(mat, xmlMat);
		buildSpecular(mat, xmlMat);

		buildPixel(mat);

		compile(mat);

		return mat;

	}

private:

	inline void loadUserValues(Material2* mat, XMLElem* elem);
	inline void loadTextures(Material2* mat, XMLElem* elem);
	inline void loadModifiers(Material2* mat, XMLElem* elem);

	inline void setClipping(Material2* mat, XMLElem* elem);

	inline void buildLighting(Material2* mat, XMLElem* elem);
	inline void buildDiffuse(Material2* mat, XMLElem* elem);
	inline void buildShadow(Material2* mat, XMLElem* elem);
	inline void buildBumpMap(Material2* mat, XMLElem* elem);
	inline void buildSpecular(Material2* mat, XMLElem* elem);

	inline void buildPixel(Material2* mat);

	inline void compile(Material2* mat);

private:

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

#endif // MATERIALFACTORY_H
