#ifndef SPRITE_H
#define SPRITE_H

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../mesh/MeshVertex.h"
#include "../scene/Renderable.h"
#include "Rect.h"

#include <vector>



/** 2D sprite image */
class Sprite2D : public Renderable {

private:

	friend class SpriteFactory;
	friend class PostProcessRenderer;

	VAO vao;
	VBOArrayStatic<AttrVertexNormalTexture> vertices;

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

	void setRect(const Rect& r) {

		const float x1 = r.getUpperLeft().x;
		const float y1 = r.getUpperLeft().y;

		const float x2 = r.getLowerRight().x;
		const float y2 = r.getLowerRight().y;

		setRect(x1,y1, x2,y2);

	}

	void setRect(const float x1, const float y1, const float x2, const float y2) {

		vertices.set(0, AttrVertexNormalTexture(x1,y1,0.1,		0,0,1,	0,0));
		vertices.set(1, AttrVertexNormalTexture(x2,y1,0.1,		0,0,1,	1,0));
		vertices.set(2, AttrVertexNormalTexture(x2,y2,0.1,		0,0,1,	1,1));

		vertices.set(3, AttrVertexNormalTexture(x1,y1,0.1,		0,0,1,	0,0));
		vertices.set(4, AttrVertexNormalTexture(x2,y2,0.1,		0,0,1,	1,1));
		vertices.set(5, AttrVertexNormalTexture(x1,y2,0.1,		0,0,1,	0,1));

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

	void render(const SceneState&, const RenderState&) override {

		if (material) {material->bind();}
		if (material2) {material2->bind();}

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();

		if (material2) {material2->unbind();}
		if (material) {material->unbind();}

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
