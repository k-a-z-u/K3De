#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include <vector>

#include "Texture.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "AnimatedTexture.h"

#include "../import/ImageFactory.h"
#include "../misc/Error.h"
#include "../misc/Memory.h"

class TextureFactory {

private:

	/** the texture compression format to use */
	static constexpr GLuint COMPR = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

	std::vector<std::unique_ptr<ITexture>> textures;

	bool mipmaps = true;
	float anisotropic = 2;

public:

	/** create a new texture using the given input file */
	Texture2D* create(const std::string& file) {

		// parse the image file
		Image img = ImageFactory::load(file);
		void* data = (void*) img.getData().data();

		// get the texture format
		int formatIn, formatOut;
		switch(img.getFormat()) {
			case ImageFormat::IMAGE_RGB:
				formatIn = GL_RGB;
				formatOut = COMPR;//GL_COMPRESSED_RGB;		// use compression
				break;
			case ImageFormat::IMAGE_RGBA:
				formatIn = GL_RGBA;
				formatOut = COMPR;//GL_COMPRESSED_RGBA;		// use compression
				break;
			case ImageFormat::IMAGE_GREY:
				formatIn = GL_ALPHA;
				formatOut = GL_COMPRESSED_ALPHA;
				break;
			case ImageFormat::IMAGE_GREY_ALPHA:
				formatIn = GL_ALPHA;
				formatOut = COMPR;
				break;
			default: throw "error";
		}

		// create and add a new texture
		Texture2D* tex = new Texture2D();
		textures.push_back(std::make_unique(tex));
		tex->bind(0);

		// load image into the texture
		glTexImage2D(GL_TEXTURE_2D, 0,formatOut, img.getWidth(), img.getHeight(), 0, formatIn, GL_UNSIGNED_BYTE, data);
		Error::assertOK();

		// wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

//		// trilinear filtering???
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// set anisotropic filtering
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);

		// generate mip-maps?
		if (mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
			Error::assertOK();
		}

		return tex;
	}

	/** create a new 2D-array texture using the given input files */
	Texture2DArray* create2DArray(const std::vector<std::string> files) {

		// load all images into a combined memory region
		std::vector<Image> images;
		std::vector<uint8_t> data;
		for (const std::string& file : files) {
			Image img = ImageFactory::load(file);
			images.push_back(img);
			_assertEqual(images.front().getWidth(), img.getWidth(), "image width does not match among all 2D-Array textures");
			_assertEqual(images.front().getHeight(), img.getHeight(), "image height does not match among all 2D-Array textures");
			_assertEqual(images.front().getFormat(), img.getFormat(), "image format does not match among all 2D-Array textures");
			_assertEqual(images.front().getData().size(), img.getData().size(), "image number-of-bytes does not match among all 2D-Array textures");
			data.insert(data.end(), img.getData().begin(), img.getData().end());
		}

		const int w = images.front().getWidth();
		const int h = images.front().getHeight();
		const int layers = files.size();

		// create and add a new texture
		Texture2DArray* tex = new Texture2DArray(layers);
		textures.push_back(std::make_unique(tex));
		tex->bind(0);

//		// allocate memory
//		const int mipMapLevels = 1;

//		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipMapLevels, GL_COMPRESSED_RGB, w, h, layers);
//		Error::assertOK();

		// upload layers
		//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, w, h, layers, GL_RGB, GL_UNSIGNED_BYTE, data.data());
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, COMPR, w, h, layers, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
		Error::assertOK();

		// wrapping
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		Error::assertOK();

		// set anisotropic filtering
		//glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		Error::assertOK();

		// generate mip-maps?
		if (mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			Error::assertOK();
		}

		return tex;
	}

	AnimatedTexture* create(const std::string& base, const std::string& ext, const int from, const int to, const float fps) {
		std::vector<std::string> files;
		for (int i = from; i <= to; ++i) {
			const std::string file = base + std::to_string(i) + ext;
			files.push_back(file);
		}
		return create(files, fps);
	}

	AnimatedTexture* create(const std::vector<std::string> files, const float fps) {

		AnimatedTexture* ani = new AnimatedTexture(fps);
		textures.push_back(std::make_unique(ani));

		for (const std::string& file : files) {
			Texture* tex = create(file);
			ani->add(tex);
		}

		return ani;

	}

	Texture* createRenderTexture(const int w, const int h, const GLuint type = GL_RGB) {

		Texture* tex = new Texture(GL_TEXTURE_2D);
		textures.push_back(std::make_unique(tex));
		tex->bind(0);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0,type, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		Error::assertOK();

//		// Poor filtering. Needed !
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		return tex;

	}

//	Texture* createRenderTextureFloat(const int w, const int h) {

//		Texture* tex = new Texture();
//		textures.push_back(std::make_unique(tex));
//		tex->bind(0);

//		// Give an empty image to OpenGL ( the last "0" )
//		glTexImage2D(GL_TEXTURE_2D, 0,GL_GR, w, h, 0,GL_R, GL_FLOAT, 0);

//		// Poor filtering. Needed !
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//		return tex;

//	}

	Texture* createDepthTexture(const int w, const int h) {

		Texture* tex = new Texture(GL_TEXTURE_2D);
		textures.push_back(std::make_unique(tex));
		tex->bind(0);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		//tex->setFilter(TextureFilter::NEAREST, TextureFilter::NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		return tex;

	}

};

#endif // TEXTUREFACTORY_H
