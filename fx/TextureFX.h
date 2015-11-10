#ifndef TEXTUREFX_H
#define TEXTUREFX_H

#include "../textures/ITexture.h"
#include "../shader/Shader.h"
#include "../gl/Renderbuffer.h"
#include "../gl/Framebuffer.h"
#include "../gl/VBOArray.h"
#include "../gl/VAO.h"
#include "../mesh/MeshVertex.h"

class TextureFX {

private:

	Shader* shader;
	Framebuffer fb;
	Renderbuffer rb;
	VAO vao;
	VBOArray<VertexNormalTexture> vertices;

public:

	TextureFX() : shader(nullptr) {

		VertexNormalTexture vnt1(Vec3(-1,-1,0.1), Vec3(0,0,0), Vec2(0,0));
		VertexNormalTexture vnt2(Vec3(+1,-1,0.1), Vec3(0,0,0), Vec2(1,0));
		VertexNormalTexture vnt3(Vec3(+1,+1,0.1), Vec3(0,0,0), Vec2(1,1));
		VertexNormalTexture vnt4(Vec3(-1,+1,0.1), Vec3(0,0,0), Vec2(0,1));

		vertices.append(vnt1);
		vertices.append(vnt2);
		vertices.append(vnt3);

		vertices.append(vnt1);
		vertices.append(vnt3);
		vertices.append(vnt4);

		vertices.upload();

		vao.bind();
		vertices.bind();
		vao.setVertices(0, 8*4);
		vao.setNormals(1, 8*4, 3*4);
		vao.setTexCoords(2, 8*4, 6*4);
		vao.unbind();

	}

	void setShader(Shader* shader) {
		this->shader = shader;
	}

	/** process the given texture into the provided output texture */
	void process(Texture* texIn, Texture* texOut, int w, int h) {


		shader->bind();
		texIn->bind(0);

		shader->setInt("tex", 0);

		//fb.attachRenderbufferDepth(&rb, w, h);
		fb.attachTextureColor(0, texOut);

		fb.enable();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();

		fb.detachColorTextures();
		fb.disable();

		texIn->unbind(0);
		shader->unbind();

	}

};

#endif // TEXTUREFX_H
