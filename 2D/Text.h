#ifndef TEXT_H
#define TEXT_H

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../scene/Renderable.h"

class Text : public Renderable {

private:

	VAO vao;
	VBOArrayStatic<AttrVertexTexture> vertices;
	Mat4 matrix;
	Material* material;
	std::string text;

	Vec2 pos;
	Vec2 charSize;
	Vec2 stridePercent;

public:

	Text(Material* material) : material(material), pos(0.5f, 0.5f) {
		matrix = Mat4::identity();
		charSize = Vec2(0.1f, 0.2f);
		stridePercent = Vec2(1.0f, 1.0f);
	}

	void setPosition(const Vec2 xy) {
		if (xy == this->pos) {return;}
		this->pos = xy;
		rebuild();
	}

	void setCharSize(const Vec2 charSize) {
		if (charSize == this->charSize) {return;}
		this->charSize = charSize;
		rebuild();
	}

	void setStride(const Vec2 percent) {
		if (percent == this->stridePercent) {return;}
		this->stridePercent = percent;
		rebuild();
	}

	void setText(const std::string& text) {
		if (text == this->text) {return;}
		this->text = text;
		rebuild();
	}

	/** render this object */
	virtual void render(const SceneState&, const RenderState&) override {

		material->bind();
		vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		vao.unbind();
		material->unbind();

	}

	const Mat4& getMatrix() const override {
		return matrix;
	}

	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}

private:

	void rebuild() {

		vertices.clear();

		// start at the given starting position (lower-left)
		Vec2 pos = this->pos;

		for (char c : text) {

			// newline?
			if (c == '\n') {
				pos.x = this->pos.x;
				pos.y += (charSize * stridePercent).y;
				continue;
			}

			const Vec2 t1 = getTexCoord(c);
			const Vec2 t2 = t1 + getTexSize();

			const Vec2 v1 = pos;									// lower left of the letter
			const Vec2 v2 = pos + Vec2(charSize.x, -charSize.y);	// upper right of the letter

			const AttrVertexTexture vt1(v1.x, v1.y, 0,	t1.x, t2.y);
			const AttrVertexTexture vt2(v2.x, v1.y, 0,	t2.x, t2.y);
			const AttrVertexTexture vt3(v2.x, v2.y, 0,	t2.x, t1.y);
			const AttrVertexTexture vt4(v1.x, v2.y, 0,	t1.x, t1.y);

			vertices.append(vt1);
			vertices.append(vt2);
			vertices.append(vt3);

			vertices.append(vt1);
			vertices.append(vt3);
			vertices.append(vt4);

			// next char
			pos.x += (charSize * stridePercent).x;

		}

		vertices.upload();

		vao.bind();
		vertices.bind();
		vao.setVertices(0, sizeof(AttrVertexTexture), 0);
		vao.setTexCoords(2, sizeof(AttrVertexTexture), sizeof(Vertex));
		vao.unbind();

	}

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
