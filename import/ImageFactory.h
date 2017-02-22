#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H


#include "Image.h"
#include <string>


#include "image/TGA.h"
#include "image/JPG.h"
#include "image/PNG.h"

class ImageFactory {

public:

//	static Image load(const std::string& filename) {
//		const std::string ext = getExt(filename);
//		if (ext == "png") {return K3De::PNG::load(d);}
//		if (ext == "jpg") {return K3De::JPG::load(d);}
//		if (ext == "tga") {return K3De::TGA::load(d);}
//		throw Exception("unsupported format for: " + filename);
//	}

	static Image load(const Resource& res) {
		const std::string filename = res.getName();
		const std::string ext = getExt(filename);
		const Data d = ResourceFactory::get().get(res);
		if (ext == "png") {return K3De::PNG::load(d);}
		if (ext == "jpg") {return K3De::JPG::load(d);}
		if (ext == "tga") {return K3De::TGA::load(d);}
		throw Exception("unsupported format for: " + res.getName());
	}

	static void save(const std::string& filename, const Image& img) {
		const std::string ext = getExt(filename);
		if (ext == "jpg") {return K3De::JPG::save(filename, img);}
		if (ext == "tga") {return K3De::TGA::save(filename, img);}
		throw Exception("unsupported format for: " + filename);
	}

private:

	static inline std::string getExt(const std::string& file) {
		return file.substr(file.length() - 3, 3);
	}


};

#endif // IMAGEFACTORY_H
