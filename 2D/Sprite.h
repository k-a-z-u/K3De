#ifndef SPRITE_H
#define SPRITE_H

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../mesh/MeshVertex.h"
#include "../scene/Renderable.h"
#include "../shader/Shader.h"

#include <vector>

/** 2D sprite image */
class Sprite2D : public Renderable {

private:

	friend class SpriteFactory;

	VAO vao;
	VBOArray<VertexNormalTexture> vertices;

	std::vector<Texture*> textures;
	Mat4 mat;

private:

	/** hidden CTOR */
	Sprite2D() : mat(1.0f) {
		vertices.resize(6);
		//configure();
	}


	void configure() {

		vao.bind();

		vertices.bind();
		vao.setVertices(0, 8*4);
		vao.setNormals(1, 8*4, 3*4);
		vao.setTexCoords(2, 8*4, 6*4);

		vao.unbind();

	}

public:

	void setRect(float x1, float y1, float x2, float y2) {

//		x1 = x1*2-1;
//		x2 = x2*2-1;
//		y1 = y1*2-1;
//		y2 = y2*2-1;

		//vertices.clear();

		vertices.set(0, VertexNormalTexture(x1,y1,0.1,		0,0,1,	0,0));
		vertices.set(1, VertexNormalTexture(x2,y1,0.1,		0,0,1,	1,0));
		vertices.set(2, VertexNormalTexture(x2,y2,0.1,		0,0,1,	1,1));

		vertices.set(3, VertexNormalTexture(x1,y1,0.1,		0,0,1,	0,0));
		vertices.set(4, VertexNormalTexture(x2,y2,0.1,		0,0,1,	1,1));
		vertices.set(5, VertexNormalTexture(x1,y2,0.1,		0,0,1,	0,1));

		vertices.upload();
		configure();

	}

//	/** set the sprite's texture */
//	void setTexture(const size_t idx, Texture* texture) {
//		if (textures.size() < (idx+1)) {textures.resize(idx+1);}
//		textures[idx] = texture;
//	}

//	/** set the sprite's texture */
//	void setTexture(Texture* texture) {
//		const int idx = 0;
//		if (textures.size() < (idx+1)) {textures.resize(idx+1);}
//		textures[idx] = texture;
//	}

	void render(const RenderStage& rs) override {

		(void) rs;

//		for (size_t i = 0; i < textures.size(); ++i) {
//			if (textures[i]) {textures[i]->bind(i);}
//		}

		material->bind();
		//shader->bind();

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();

		//shader->unbind();
		material->unbind();

	}

	const Mat4& getMatrix() const override {
		return mat;
	}

	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}

};

#endif // SPRITE_H
