#ifndef TEXT_H
#define TEXT_H

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../scene/Renderable.h"
#include "../textures/Texture2D.h"

class Text : public Renderable {

private:

	VAO vao;
	VBOArray<VertexTexture> vertices;
	Mat4 mat;
	Texture2D* tex;
	Vec2 pos;

public:

	Text(Texture2D* tex, Vec2 pos) : mat(1.0f), tex(tex), pos(pos) {

	}

	void setText(const std::string& text) {

		vertices.clear();

		Vec2 pos = this->pos;
		const Vec2 vs(0.025, 0.05);

		for (char c : text) {

			const Vec2 t1 = getTexCoord(c);
			const Vec2 t2 = t1 + getTexSize();

			const Vec2 v1 = pos;
			const Vec2 v2 = pos + vs;

			VertexTexture vt1(v1.x, v1.y, 0,	t1.x, t2.y);
			VertexTexture vt2(v2.x, v1.y, 0,	t2.x, t2.y);
			VertexTexture vt3(v2.x, v2.y, 0,	t2.x, t1.y);
			VertexTexture vt4(v1.x, v2.y, 0,	t1.x, t1.y);

			vertices.append(vt1);
			vertices.append(vt2);
			vertices.append(vt3);

			vertices.append(vt1);
			vertices.append(vt3);
			vertices.append(vt4);

			// next char
			pos.x += vs.x;

		}

		vertices.upload();

		vao.bind();
		vertices.bind();
		vao.setVertices(0, sizeof(VertexTexture), 0);
		vao.setTexCoords(2, sizeof(VertexTexture), sizeof(Vertex));
		vao.unbind();

	}

	/** render this object */
	virtual void render(const RenderStage& rs) override {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		tex->bind(0);
		vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		vao.unbind();
		tex->unbind(0);
		glDisable(GL_BLEND);

	}

	const Mat4& getMatrix() const override {return mat;}

	virtual bool isVisible(const Mat4& MVP) const override {return true;}

private:


	Vec2 getTexCoord(const char c) {
		const float x = (c % 16) / 16.0f;
		const float y = (c / 16) / 16.0f;
		return Vec2(x,y);
	}
	Vec2 getTexSize() {
		return Vec2(1.0f/16.0f, 1.0f/16.0f);
	}

};

#endif // TEXT_H
