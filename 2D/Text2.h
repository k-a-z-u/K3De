#ifndef TEXT2_H
#define TEXT2_H

#include <string>
#include <vector>




#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../scene/Renderable.h"
#include "Transformable2D.h"
#include "../scene/Scene.h"
#include "Rect.h"

class Text2 : public Renderable, public Transformable2D {

private:

	VAO vao;
	VBOArrayStatic<AttrVertexTextureColor> vertices;
	Scene* scene;

	std::vector<float> glyphEdges;
	float size;
	Vec4 color;

	std::string text;

public:

//	/** ctor */
//	Text2(Scene* scene, Material2* material, const std::vector<float>& glyphEdges) :
//	    scene(scene), glyphEdges(glyphEdges), pos(0.5, 0.5), size(1.0), color(1,1,1,1) {
//		matrix = Mat4::identity();
//		setMaterial(material);
//	}

	/** ctor */
	Text2(Scene* scene, Material2* material2) :
		scene(scene), size(1.0), color(1,1,1,1) {

		// get font glyps [4 floats] from XML
		const std::string sGlyphs = material2->getUserValues()["glyphs"];
		std::istringstream ss(sGlyphs);
		std::string token;

		// split comma separated string
		while(std::getline(ss, token, ',')) {
			const float val = std::stof(token);
			glyphEdges.push_back(val);
		}

		if (glyphEdges.size() % 4 != 0) {throw "number of glyphs must be multiple of 4";}

		setMaterial(material2);

	}

	const Mat4& getMatrix() const override {
		return Transformable2D::getMatrix();
	}


	/** set the text's color */
	void setColor(const Vec4& color) {
		this->color = color;
		rebuild();
	}

//	/** set the font's position */
//	void setPosition(const Vec2 xy) {
////		if (xy == this->pos) {return;}
////		this->pos = xy;
//		rebuild();
//	}

	/** set the font's size */
	void setFontSize(const float size) {
		this->size = size;
		rebuild();
	}

	/** get the font's size */
	float getFontSize() const {
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

		if (material2) {material2->bind();}

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();

		if (material2) {material2->unbind();}

	}


	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}

private:

	struct Rect {
		float x1;
		float y1;
		float x2;
		float y2;
	};

	void rebuild() {

		// text starts at (0,0) and, depending on chars, lines and size
		// has a corresponding width/height
		// normal affine transforms can be used for positioning
		Vec2 pos(0,0);

		vertices.clear();

		for (char c : text) {

			const Vec2 t1 = getTexCoordStart(c);
			const Vec2 t2 = getTexCoordEnd(c);
			const Vec2 si = getCharSize(c);

			// newline?
			if (c == '\n') {
				pos.x = 0;		// reset x
				pos.y += si.y;	// increment y
				continue;
			}

			// ignore chars [0:31]
			if (c < 32) {continue;}

			const Vec2 v1 = pos;
			const Vec2 v2 = pos + Vec2(si.x, si.y);

			// update size information
			if (v2.x > width_01)	{width_01 = v2.x;}
			if (v2.y > height_01)	{height_01 = v2.y;}

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
		int idx = (c - 32) * 4;
		if (idx < 32) {idx = 32;}	// e.g. return key
		return Vec2(glyphEdges[idx+0], glyphEdges[idx+3]);
	}

	Vec2 getTexCoordEnd(const char c) const {
		int idx = (c - 32) * 4;
		if (idx < 32) {idx = 32;}	// e.g. return key
		return Vec2(glyphEdges[idx+2], glyphEdges[idx+1]);
	}

	Vec2 getCharSize(const char c) const {
		Vec2 v = (getTexCoordEnd(c) - getTexCoordStart(c)) * size;
		return Vec2(std::abs(v.x), std::abs(v.y) * scene->getScreenSize().getAspect());
	}

};


#endif // TEXT2_H
