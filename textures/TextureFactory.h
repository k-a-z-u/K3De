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

	static constexpr const char* NAME = "TexFac";

	/** the texture compression format to use */
#if defined __APPLE__
	static constexpr GLuint COMPR = GL_COMPRESSED_RGBA;
#else
	static constexpr GLuint COMPR = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
#endif

	std::vector<std::unique_ptr<ITexture>> textures;

	bool mipmaps = true;
	int anisotropic = 0;	// not all cards support this

	std::string includePath;

public:

	/** configure using mip maps */
	void setUseMipMaps(const bool use) {
		this->mipmaps = use;
	}

	/** set the include-path to use */
	void setIncludePath(const std::string& path) {
		this->includePath = path;
	}

	/** set anisotropic filtering level. 0 = disabled */
	void setAnisotropicFiltering(const int level) {
		this->anisotropic = level;
	}

	/** number of loaded textures */
	size_t getNumLoaded() const {
		return textures.size();
	}


	/** destroy the given texture and remove it from the factory. the given pointer is NO LONGER VALID */
	void ddestroy(const ITexture* tex) {

		Debug(NAME, "manually destroying 2D texture");

		for (size_t i = 0; i < textures.size(); ++i) {
			if (textures[i].get() == tex) {
				textures.erase(textures.begin() + i);
				return;
			}
		}

		throw Exception("texture does not belong to this factory");

	}

	/** create a new alpha-channel texture using the given input file */
	Texture2D* createAlpha(const std::string& file, bool compressed = true) {

		// parse the image file
		Image img = ImageFactory::load(includePath + file);
		void* data = (void*) img.getData().data();

		const int formatIn = getFormatIn(img);
		const int formatOut = (compressed) ? (GL_COMPRESSED_ALPHA) : (GL_ALPHA);

		return create(data, img.getWidth(), img.getHeight(), formatIn, formatOut);

	}

	/** create a new texture using the given resource */
	Texture2D* create(const Resource res, bool compressed = true, bool mipMaps = true, int reqFormatOut = -1) {

		Debug(NAME, "creating 2D texture from resource");

		// decode the image file
		Image img = ImageFactory::load(res);
		void* data = (void*) img.getData().data();
		const int formatIn = getFormatIn(img);
		const int formatOut = (reqFormatOut == -1) ? (getFormatOut(img, compressed)) : (reqFormatOut);

		Texture2D* tex = new Texture2D(formatOut, -1, -1);
		textures.push_back(std::make_unique(tex));
		upload(tex, data, img.getWidth(), img.getHeight(), formatIn, formatOut, mipMaps);

//		// create and add a new texture
//		Texture2D* tex = new Texture2D(-1, -1);
//		textures.push_back(std::make_unique(tex));

//		auto funcDecode = [this, res, tex, compressed, mipMaps] () {

//			// decode the image file
//			Image img = ImageFactory::load(res);

//			auto funcUpload = [this, tex, img, compressed, mipMaps] () {
//				void* data = (void*) img.getData().data();
//				const int formatIn = getFormatIn(img);
//				const int formatOut = getFormatOut(img, compressed);
//				upload(tex, data, img.getWidth(), img.getHeight(), formatIn, formatOut, mipMaps);
//			};

//			MainLoop::get().add(funcUpload);

//		};

//		GlobalThreadPool::get().add(funcDecode);

		return tex;

	}


	int getFormatIn(const Image& img) const {

		switch(img.getFormat()) {

			// GL_ALPHA and such is deprecated.
			// for grey textures, use one-channel and do the desired conversion within the shader
			case ImageFormat::IMAGE_GREY:
				return GL_RED;

			// same as above: handle actual component mapping within the shader-code
			case ImageFormat::IMAGE_GREY_ALPHA:
				return GL_RG;

		    case ImageFormat::IMAGE_RGB:
			    return GL_RGB;

		    case ImageFormat::IMAGE_RGBA:
			    return GL_RGBA;

		    default:
			    throw Exception("invalid image format");
		}

	}

	int getFormatOut(const Image& img, const bool compressed) const {

		switch(img.getFormat()) {

			// GL_ALPHA and such is deprecated.
			// for grey textures, use one-channel and do the desired conversion within the shader
			case ImageFormat::IMAGE_GREY:
				return (compressed) ? (GL_COMPRESSED_RED) : (GL_RED);

			// same as above: handle actual component mapping within the shader-code
			case ImageFormat::IMAGE_GREY_ALPHA:
				return  (compressed) ? (GL_COMPRESSED_RG) : (GL_RG);

			case ImageFormat::IMAGE_RGB:
				return (compressed) ? (COMPR) : (GL_RGB);

			case ImageFormat::IMAGE_RGBA:
				return (compressed) ? (COMPR) : (GL_RGBA);

			default:
				throw Exception("invalid image format");

		}

	}

	Texture2D* create(const void* data, const int w, const int h, const int formatIn, const int formatOut) {

		Debug(NAME, "creating 2D texture from raw data");

		// create and add a new texture
		Texture2D* tex = new Texture2D(formatOut, w, h);
		textures.push_back(std::make_unique(tex));

		// upload data
		upload(tex, data, w, h, formatIn, formatOut);

		// done
		return tex;

	}

	void upload(Texture* tex, const void* data, const int w, const int h, const int formatIn, const int formatOut, const bool mipmaps = true) {

		// sanity check
		ensureValidFormat(formatOut);

		tex->format = formatOut;
		tex->width = w;
		tex->height = h;

		tex->bind(0);

		// load image into the texture
		glTexImage2D(GL_TEXTURE_2D, 0,formatOut, w, h, 0, formatIn, GL_UNSIGNED_BYTE, data);
		Error::assertOK();

		// wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		Error::assertOK();

		// linear-filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (this->mipmaps && mipmaps) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		// set anisotropic filtering [will fail if unsupported]
		if (anisotropic) {
			#if defined __APPLE__
			#else
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
				Error::assertOK();
			#endif
		}

		// generate mip-maps?
		if (this->mipmaps && mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
			Error::assertOK();
		}

	}


	/** create a new 2D-array texture using the given input files */
	Texture2DArray* create2DArray(const std::vector<std::string> files) {

		Debug(NAME, "creating 2D array-texture");

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
		Texture2DArray* tex = new Texture2DArray(GL_RGB, w, h, layers);
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

	Texture* createRenderTexture(const int w, const int h, const GLuint format = GL_RGB) {

		Debug(NAME, "creating 2D render texture " + std::to_string(w) + " x " + std::to_string(h));

		Texture* tex = new Texture(GL_TEXTURE_2D, format, w, h);
		textures.push_back(std::make_unique(tex));
		tex->bind(0);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
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

		Debug(NAME, "creating 2D depth texture " + std::to_string(w) + " x " + std::to_string(h));
		const GLuint format = GL_DEPTH_COMPONENT32F;

		Texture* tex = new Texture(GL_TEXTURE_2D, format, w, h);
		textures.push_back(std::make_unique(tex));
		tex->bind(0);

		// TODO: performance boost possible?
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		//tex->setFilter(TextureFilter::NEAREST, TextureFilter::NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		return tex;

	}

private:

	void ensureValidFormat(GLenum format) {

		switch (format) {
			case GL_RGBA:
			case GL_RGB:
			case GL_ALPHA:
			case GL_RED:
			case GL_R32F:
			case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			case GL_COMPRESSED_RGBA:
				break;
			default:
				throw Exception("unsupported texture format: " + format);
		}

	}

};

#endif // TEXTUREFACTORY_H
