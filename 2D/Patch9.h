#ifndef PATCH9_H
#define PATCH9_H

#include "Transformable2D.h"
#include "Pos2.h"

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../mesh/MeshVertex.h"
#include "../scene/Renderable.h"
#include "../scene/Scene.h"

/**
 * 9 patch drawable
 */
class Patch9 : public Renderable, public Transformable2D {

public:

	enum Part {

		UPPER_LEFT,
		UPPER_CENTER,
		UPPER_RIGHT,

		CENTER_LEFT,
		CENTER_CENTER,
		CENTER_RIGHT,

		LOWER_LEFT,
		LOWER_CENTER,
		LOWER_RIGHT,

	};

	/**
	 * describes one patch:
	 * exture coordinates
	 */
	struct Patch {
		bool used;
		float tex_x1;
		float tex_y1;
		float tex_x2;
		float tex_y2;
		Patch() : used(false), tex_x1(0), tex_y1(0), tex_x2(0), tex_y2(0) {;}
		Patch(float x1, float y1, float x2, float y2) : used(true), tex_x1(x1), tex_y1(y1), tex_x2(x2), tex_y2(y2) {;}
		float width() const {return tex_x2 - tex_x1;}
		float height() const {return tex_y2 - tex_y1;}
	};

private:

	/** configuration for all patches */
	Patch patches[9];

	VAO vao;
	VBOArrayStatic<AttrVertexNormalTexture> vertices;

	Scene* scene;

	Size2 size = Size2(MetricX(0.5, Unit::PERCENT), MetricY(0.5, Unit::PERCENT));

public:

	Patch9(Scene* scene, Material2* mat) : scene(scene) {

		setMaterial(mat);

		// get patches form XML
		patches[UPPER_LEFT] = parsePatch("patch0");
		patches[UPPER_CENTER] = parsePatch("patch1");
		patches[UPPER_RIGHT] = parsePatch("patch2");

		patches[CENTER_LEFT] = parsePatch("patch3");
		patches[CENTER_CENTER] = parsePatch("patch4");
		patches[CENTER_RIGHT] = parsePatch("patch5");

		patches[LOWER_LEFT] = parsePatch("patch6");
		patches[LOWER_CENTER] = parsePatch("patch7");
		patches[LOWER_RIGHT] = parsePatch("patch8");

		configure();
		build();

	}

//	/** configure the given patch */
//	void set(const Part part, const Patch& patch) {
//		patches[part] = patch;
//	}

	const Mat4& getMatrix() const override {
		return Transformable2D::getMatrix();
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

	/** set the element's size */
	void setSize(const Size2 size) {
		this->size = size;
		build();
	}

private:

	/** parse one patch from xml description */
	Patch parsePatch(const std::string& patchID) {

		const Material2::UserKeyValue kv = material2->getUserValues();
		auto it = kv.find(patchID);
		if (it == kv.end()) {return Patch();}	// empty patch

		const std::string value = it->second;
		std::istringstream ss(value);
		std::string token;

		// split comma separated string
		std::vector<std::string> edges;
		while(std::getline(ss, token, ',')) {
			edges.push_back(token);
		}

		// sanity check
		if (edges.size() != 4) {throw Exception("number of edges must be 4");}

		// OK
		return Patch(getX(edges[0]), getY(edges[1]), getX(edges[2]), getY(edges[3]));

	}

	void configure() {

		vao.bind();

		vertices.bind();
		vao.setVertices(0, 8*4);
		vao.setNormals(1, 8*4, 3*4);
		vao.setTexCoords(2, 8*4, 6*4);

		vao.unbind();

	}



	void build() {

		// scene's size in pixels
		const float scn_w_px = scene->getScreenSize().width;
		const float scn_h_px = scene->getScreenSize().height;

		// texture's size in pixels
		const ITexture* tex = getTexture();
		const float tex_w_px = 512;//tex->getWidth();
		const float tex_h_px = 512;//tex->getHeight();

		// what to do when texture is not yet loaded?

		// clear
		vertices.clear();

		const Vec2 s = size.toVec(scene->getScreenSize());
		const float sx = s.x;
		const float sy = s.y;

		// first row
		// width/height for fixed-size corner elements
		const float w0 = (patches[0].width() * tex_w_px) / scn_w_px;
		const float h0 = (patches[0].height() * tex_h_px) / scn_h_px;
		const float w2 = (patches[2].width() * tex_w_px) / scn_w_px;
		const float h2 = (patches[6].height() * tex_h_px) / scn_h_px;

		addRect(0.0f,		0.0f,	w0,			h0, patches[0]);
		addRect(w0,			0.0f,	sx-w2-w0,	h0, patches[1]);
		addRect(sx-w2,		0.0,	w2,			h0, patches[2]);

		addRect(0.0f,		h0,		w0,			sy-h0-h2, patches[3]);
		addRect(w0,			h0,		sx-w2-w0,	sy-h0-h2, patches[4]);
		addRect(sx-w2,		h0,		w2,			sy-h0-h2, patches[5]);

		addRect(0.0f,		sy-h2,	w0,			h2, patches[6]);
		addRect(w0,			sy-h2,	sx-w2-w0,	h2, patches[7]);
		addRect(sx-w2,		sy-h2,	w2,			h2, patches[8]);

		// upload changes
		vertices.upload();

	}


	void addRect(const float x1, const float y1, const float w, const float h, const Patch& p) {

		const float x2 = x1+w;
		const float y2 = y1+h;

		std::cout << x1 << "," << y1 << " " << x2 << "," << y2 << std::endl;

		vertices.append(AttrVertexNormalTexture(x1,y1,0.1,	0,0,1,		p.tex_x1,	p.tex_y1));
		vertices.append(AttrVertexNormalTexture(x2,y1,0.1,	0,0,1,		p.tex_x2,	p.tex_y1));
		vertices.append(AttrVertexNormalTexture(x2,y2,0.1,	0,0,1,		p.tex_x2,	p.tex_y2));

		vertices.append(AttrVertexNormalTexture(x1,y1,0.1,	0,0,1,		p.tex_x1,	p.tex_y1));
		vertices.append(AttrVertexNormalTexture(x2,y2,0.1,	0,0,1,		p.tex_x2,	p.tex_y2));
		vertices.append(AttrVertexNormalTexture(x1,y2,0.1,	0,0,1,		p.tex_x1,	p.tex_y2));

	}

	ITexture* getTexture() const {
		return material2->getTextures().all().front().tex;
	}

	float getX(const std::string& val) const {
		return get(val);
	}

	float getY(const std::string& val) const {
		return get(val);
	}

	float get(const std::string& val) const {
		const size_t pos = val.find("/");
		if (pos == std::string::npos) {
			return std::stof(val);
		} else {
			std::string v1 = val.substr(0, pos);
			std::string v2 = val.substr(pos+1);
			return std::stof(v1) / std::stof(v2);
		}
	}

};

#endif // PATCH9_H
