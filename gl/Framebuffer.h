#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "gl.h"

#include <vector>
#include <KLib/Assertions.h>

#include "../misc/Error.h"
#include "../textures/Texture.h"
#include "Renderbuffer.h"

class Framebuffer {

	#include "../misc/BindOnce.h"

	inline void assertBound() const {
		_assertTrue(isBound(fbID), "bind() the Framebuffer first");
	}

	inline void assertUnbound() const {
		_assertFalse(isBound(fbID), "Framebuffer is already bound!");
	}

private:

	GLuint fbID;
	std::vector<GLuint> colorBuffers;

public:

	/** ctor */
	Framebuffer() {
		create();
	}

	/** dtor */
	~Framebuffer() {
		remove();
	}

	void bind() {
		assertUnbound();
		glBindFramebuffer(GL_FRAMEBUFFER, fbID);
		Error::assertOK();
		setBound(fbID);
	}

	void unbind() {
		assertBound();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Error::assertOK();
		setUnbound(fbID);
	}

	/** attach the given texture as the idx-th color-target */
	void attachTextureColor(const unsigned int idx, Texture* texture) {
		assertBound();
		//bind();
		const GLuint id = GL_COLOR_ATTACHMENT0 + idx;
		//glFramebufferTexture(GL_FRAMEBUFFER, id, texture->getID(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, id, GL_TEXTURE_2D, texture->getID(), 0);
		Error::assertOK();
		if (colorBuffers.size() < (idx+1)) {colorBuffers.resize(idx+1);}
		colorBuffers[idx] = id;
		//unbind();
	}

	/** when only depth is rendered, set color-texture to "empty" */
	void noColor() {
		assertBound();
		glDrawBuffer(GL_NONE);
	}

	void detachColorTextures() {
		colorBuffers.clear();
	}

	/** attach the given texture to the depth-channel */
	void attachTextureDepth(Texture* texture) {
		assertBound();
		//bind();
		// todo assert that the texture is a depth texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getID(), 0);
		Error::assertOK();
		//unbind();
	}

	/** attach the given Renderbuffer as depth-buffer of the given size */
	void attachRenderbufferDepth(Renderbuffer* buf, const int w, const int h) {
		assertBound();
		//bind();
		buf->bind();
		buf->setToDepthMode(w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buf->getID());
		Error::assertOK();
		//unbind();
	}

	void setupColorTextures() {

		assertBound();

		if (colorBuffers.empty()) {
			// no color buffers set. just rendering the depth-channel (e.g. shadows)
			glDrawBuffer(GL_NONE);
			Error::assertOK();
		} else {
			// attach all configured color buffers
			glDrawBuffers(colorBuffers.size(), colorBuffers.data());
			Error::assertOK();
		}

		//const GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		//_assertEqual(GL_FRAMEBUFFER_COMPLETE, res, "framebuffer is not complete!");

	}

	void disable() {
		//unbind();
	}

private:

	void create() {
		glGenFramebuffers(1, &fbID);
		Error::assertOK();
	}

	void remove() {
		glDeleteFramebuffers(1, &fbID);
		Error::assertOK();
	}


};

#endif // FRAMEBUFFER_H
