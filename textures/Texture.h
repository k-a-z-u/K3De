#ifndef TEXTURE_H
#define TEXTURE_H

#include "../gl/gl.h"
#include "../misc/Error.h"
#include "ITexture.h"
#include "../math/Vector.h"
#include <set>

/** supported texture filters */
enum class TextureFilter {
	NEAREST,
	LINEAR,
};

/** supported texture wrappings */
enum class TextureWrapping {
	REPEAT,
	CLAMP,
};


class Texture : public ITexture {

private:

	#include "../misc/BindOnceMany.h"

	friend class TextureFactory;

	/** the openGL texture type (1D, 2D, 3D, 2D-Array, ..) */
	GLuint type;

	/** the texture's data format */
	GLuint format;

	/** the openGL texture-id */
	GLuint id;

	/** the texture's width */
	int width;

	/** the texture's height */
	int height;


public:

	/** ctor */
	Texture(const GLuint type, const GLuint format, const int width, const int height) :
		type(type), format(format), id(0), width(width), height(height) {
		create();
	}

	~Texture() {
		remove();
	}


	bool isAlphaOnly() const override {
		return format == GL_COMPRESSED_ALPHA || format == GL_ALPHA;
	}

	int getWidth() const override {
		return width;
	}

	int getHeight() const override {
		return height;
	}

	void bind(const TextureUnit idx) const override {
		assertUnbound(key(idx));
		glActiveTexture(GL_TEXTURE0 + idx);	Error::assertOK();
		glBindTexture(type, id);			Error::assertOK();
		setBound(key(idx));
	}

	void unbind(const TextureUnit idx) const override {
//		assertBound(key(idx));
		glActiveTexture(GL_TEXTURE0 + idx);	Error::assertOK();
		glBindTexture(type, 0);				Error::assertOK();
		setUnbound(key(idx));
	}

	void resize(const int w, const int h) {
//		assertBound(key(idx));
		// TODO: exactly same format as during creation?
		if (format == GL_DEPTH_COMPONENT32F) {
			glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0,				GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			Error::assertOK();
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0,				GL_RED, GL_UNSIGNED_BYTE, 0);
			Error::assertOK();
		}
	}

	/** get the texture's ID */
	GLuint getID() const {
		return id;
	}

	/** set the TextureFilter to use when down/up-scaling this texture */
	void setFilter(const TextureFilter min, const TextureFilter mag) {
		// assert bound!
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, getFilter(mag));	Error::assertOK();
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, getFilter(min));	Error::assertOK();
	}

	/** set the wrapping mode to use for the (s,t) coordinates */
	void setWrapping(const TextureWrapping wrapS, const TextureWrapping wrapT) {
		// assert bound!
		glTexParameteri(type, GL_TEXTURE_WRAP_S, getWrapping(wrapS));
		glTexParameteri(type, GL_TEXTURE_WRAP_T, getWrapping(wrapT));
	}

	/** set anisotropic filtering */
	void setAnisotropic(const float val) {
		// assert bound!
#if defined __APPLE__
		// TODO
#else
		glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, val);
#endif
	}

	/** generate MipMaps for this texture */
	void generateMipMaps() {
		// assert bound!
		glGenerateMipmap(type);
		Error::assertOK();
	}

private:

	/** convert texture-unit + texture-id to a unique key */
	inline unsigned int key(const TextureUnit idx) const {
		return (idx << 16) | id;
	}

	/** ensure the ID is currently bound */
	inline void assertBound(const TextureUnit idx) const {
		_assertTrue(isBound(key(idx)), "bind() the texture first");
	}

	/** ensure the ID is currently unbound */
	inline void assertUnbound(const TextureUnit idx) const {
		_assertFalse(isBound(key(idx)), "texture is already bound!");
	}


	/** convert TextureFilter enum to openGL enum */
	GLenum getFilter(const TextureFilter f) const {
		switch (f) {
			case TextureFilter::NEAREST:	return GL_NEAREST;
			case TextureFilter::LINEAR:		return GL_LINEAR;
			default:						throw "error";
		}
	}

	/** convert TextureWrappiung enum to openGL enum */
	GLenum getWrapping(const TextureWrapping w) const {
		switch (w) {
			case TextureWrapping::CLAMP:	return GL_CLAMP;
			case TextureWrapping::REPEAT:	return GL_REPEAT;
			default:						throw "error";
		}
	}

	void create() {
		glGenTextures(1, &id);			Error::assertOK();
	}

	void remove() {
		glDeleteTextures(1, &id);		Error::assertOK();
	}



};

#endif // TEXTURE_H
