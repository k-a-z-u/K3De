#ifndef TEXT2_H
#define TEXT2_H

#include <string>
#include <vector>




#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../scene/Renderable.h"
#include "../scene/Scene.h"

class Text2 : public Renderable {

private:

	VAO vao;
	VBOArrayStatic<AttrVertexTextureColor> vertices;
	Mat4 matrix;
	Scene* scene;

	Material* material;
	std::vector<float> glyphEdges;
	Vec2 pos;
	float size;
	Vec4 color;

	std::string text;

public:

	/** ctor */
	Text2(Scene* scene, Material* material, const std::vector<float>& glyphEdges) :
		scene(scene), material(material), glyphEdges(glyphEdges), pos(0.5, 0.5), size(1.0), color(1,1,1,1) {
		matrix = Mat4::identity();
	}

	/** set the text's color */
	void setColor(const Vec4& color) {
		this->color = color;
		rebuild();
	}

	/** set the font's position */
	void setPosition(const Vec2 xy) {
		if (xy == this->pos) {return;}
		this->pos = xy;
		rebuild();
	}

	/** set the font's size */
	void setSize(const float size) {
		this->size = size;
		rebuild();
	}

	/** get the font's size */
	float getSize() const {
		return this->size;
	}

	/** get the height of one line */
	float getLineHeight() const {
		return getCharSize(' ').y;
	}

	/** set the text to display */
	void setText(const std::string& text) {
		if (text == this->text) {return;}	// only change if needed!
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

			const Vec2 t1 = getTexCoordStart(c);
			const Vec2 t2 = getTexCoordEnd(c);
			const Vec2 si = getCharSize(c);

			// newline?
			if (c == '\n') {
				pos.x = this->pos.x;
				pos.y += si.y;
				continue;
			}

			// ignore chars [0:31]
			if (c < 32) {continue;}

			const Vec2 v1 = pos;
			const Vec2 v2 = pos + Vec2(si.x, si.y);

			const AttrVertexTextureColor vt1(v1.x, v1.y, 0,	t1.x, t2.y, color.r, color.g, color.b, color.a);
			const AttrVertexTextureColor vt2(v2.x, v1.y, 0,	t2.x, t2.y,	color.r, color.g, color.b, color.a);
			const AttrVertexTextureColor vt3(v2.x, v2.y, 0,	t2.x, t1.y,	color.r, color.g, color.b, color.a);
			const AttrVertexTextureColor vt4(v1.x, v2.y, 0,	t1.x, t1.y, color.r, color.g, color.b, color.a);

			vertices.append(vt1);
			vertices.append(vt2);
			vertices.append(vt3);

			vertices.append(vt1);
			vertices.append(vt3);
			vertices.append(vt4);

			// next char
			pos.x += si.x;

		}

		vertices.upload();

		vao.bind(); {
			vertices.bind();
			vao.setVertices(0, sizeof(AttrVertexTextureColor), 0);
			vao.setTexCoords(2, sizeof(AttrVertexTextureColor), sizeof(AttrVertex));
			vao.setColor(4, sizeof(AttrVertexTextureColor), sizeof(AttrVertex) + sizeof(AttrTexture));
		} vao.unbind();

	}

	Vec2 getTexCoordStart(const char c) const {
		const int idx = (c - 32) * 4;
		return Vec2(glyphEdges[idx+0], glyphEdges[idx+3]);
	}

	Vec2 getTexCoordEnd(const char c) const {
		const int idx = (c - 32) * 4;
		return Vec2(glyphEdges[idx+2], glyphEdges[idx+1]);
	}

	Vec2 getCharSize(const char c) const {
		Vec2 v = (getTexCoordEnd(c) - getTexCoordStart(c)) * size;
		return Vec2(std::abs(v.x), std::abs(v.y) * scene->getScreenSize().getAspect());
	}

};


#endif // TEXT2_H
