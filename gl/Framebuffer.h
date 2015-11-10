#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <vector>
#include <KLib/Assertions.h>

#include "../misc/Error.h"
#include "../textures/Texture.h"
#include "Renderbuffer.h"

class Framebuffer {

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
		glBindFramebuffer(GL_FRAMEBUFFER, fbID);
	}

	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/** attach the given texture as the idx-th color-target */
	void attachTextureColor(const int idx, Texture* texture) {
		bind();
		const GLuint id = GL_COLOR_ATTACHMENT0 + idx;
		glFramebufferTexture(GL_FRAMEBUFFER, id, texture->getID(), 0);
		Error::assertOK();
		colorBuffers.push_back(id);
		unbind();
	}

	void detachColorTextures() {
		colorBuffers.clear();
	}

	/** attach the given texture to the depth-channel */
	void attachTextureDepth(Texture* texture) {
		bind();
		// todo assert that the texture is a depth texture
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getID(), 0);
		Error::assertOK();
		unbind();
	}

	/** attach the given Renderbuffer as depth-buffer of the given size */
	void attachRenderbufferDepth(Renderbuffer* buf, const int w, const int h) {
		bind();
		buf->bind();
		buf->setToDepthMode(w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buf->getID());
		Error::assertOK();
		unbind();
	}

	void enable() {

		bind();

		if (colorBuffers.empty()) {
			// no color buffers set. just rendering the depth-channel (e.g. shadows)
			glDrawBuffer(GL_NONE);
		} else {
			// attach all configured color buffers
			glDrawBuffers(colorBuffers.size(), colorBuffers.data());
		}

		Error::assertOK();
		const GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		_assertEqual(GL_FRAMEBUFFER_COMPLETE, res, "framebuffer is not complete!");

	}

	void disable() {
		unbind();
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
