#ifndef ROWLAYOUT_H
#define ROWLAYOUT_H

#include "Transformable2D.h"
#include "Pos2.h"

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"
#include "../mesh/MeshVertex.h"
#include "../scene/Renderable.h"
#include "../scene/Scene.h"

/**
 * like 9 patch drawable but a little more flexible
 */
class RowLayout : public Renderable, public Transformable2D {

private:

	enum SizeMode {
		FIXED,
		DYNAMIC,
	};

	struct Col {
		float tex_x1;
		float tex_x2;
		float width;		// from config
		float width_calc;	// calculated based on current size
		SizeMode mode;
	};

	struct Row {
		float tex_y1;
		float tex_y2;
		float height;		// from config
		float height_calc;	// calculated based on current size
		SizeMode mode;
		std::vector<Col> cols;
	};

	struct std::vector<Row> rows;


private:


	VAO vao;
	VBOArrayStatic<AttrVertexNormalTexture> vertices;

	Scene* scene;

	Size2 size = Size2(MetricX(0.5, Unit::PERCENT), MetricY(0.5, Unit::PERCENT));

public:

	RowLayout(Scene* scene, Material2* mat) : scene(scene) {

		setMaterial(mat);

		loadLayout();

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

	/** load the layout from the material definition */
	void loadLayout() {

		const PXMLNode& user = material2->getUserNode();

		for (const PXMLNode& sub1 : user.children) {

			// all configured rows
			if (sub1.getName() == "row") {

				Row row;
				const std::pair<float, SizeMode> size = get(sub1.getAttribute("height"));
				row.height = size.first;
				row.mode = size.second;
				row.tex_y1 = get(sub1.getAttribute("y1")).first;
				row.tex_y2 = get(sub1.getAttribute("y2")).first;

				// all of the row's columns
				for (const PXMLNode& sub2 : sub1.children) {
					if (sub2.name == "col") {

						Col col;
						const std::pair<float, SizeMode> size = get(sub2.getAttribute("width"));
						col.width = size.first;
						col.mode = size.second;
						col.tex_x1 = get(sub2.getAttribute("x1")).first;
						col.tex_x2 = get(sub2.getAttribute("x2")).first;
						row.cols.push_back(col);

					}
				}

				this->rows.push_back(row);

			}
		}

		int i = 0; (void) i;

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

		// element size in scene space
		const Vec2 s = size.toVec(scene->getScreenSize());
		const float sx = s.x;
		const float sy = s.y;

		// update row height
		float fixedRowHeight = 0;
		for (Row& row : rows) {

			if (row.mode == SizeMode::FIXED) {
				row.height_calc = row.height * tex_h_px / scn_h_px;			// as is, but from tex-space to scene-space
				fixedRowHeight += row.height_calc;
			}

			// update col width
			float fixedColWidth = 0;
			for (Col& col : row.cols) {
				if (col.mode == SizeMode::FIXED) {
					col.width_calc = col.width * tex_w_px / scn_w_px;		// as is, but from tex-space to scene-space
					fixedColWidth += col.width_calc;
				}
			}

			// distribute remaining width
			float remaningColWidth = sx - fixedColWidth;
			for (Col& col : row.cols) {
				if (col.mode == SizeMode::DYNAMIC) {
					col.width_calc = col.width / 100.0f * remaningColWidth;	// percentage of the remaining size
				}
			}

		}

		// distribute remaining height
		float remainingRowHeight = sy - fixedRowHeight;
		for (Row& row : rows) {
			if (row.mode == SizeMode::DYNAMIC) {
				row.height_calc = row.height / 100.0f * remainingRowHeight;
			}
		}

		vertices.clear();

		// build rectangles
		float curY = 0;
		for (const Row& row : rows) {
			float curX = 0;
			for (const Col& col : row.cols) {

				const float tex_x1 = col.tex_x1;
				const float tex_x2 = col.tex_x2;
				const float tex_y1 = row.tex_y1;
				const float tex_y2 = row.tex_y2;

				const float x1 = curX;
				const float x2 = curX + col.width_calc;
				const float y1 = curY;
				const float y2 = curY + row.height_calc;

				vertices.append(AttrVertexNormalTexture(x1,y1,0.1,	0,0,1,		tex_x1,	tex_y1));
				vertices.append(AttrVertexNormalTexture(x2,y1,0.1,	0,0,1,		tex_x2,	tex_y1));
				vertices.append(AttrVertexNormalTexture(x2,y2,0.1,	0,0,1,		tex_x2,	tex_y2));

				vertices.append(AttrVertexNormalTexture(x1,y1,0.1,	0,0,1,		tex_x1,	tex_y1));
				vertices.append(AttrVertexNormalTexture(x2,y2,0.1,	0,0,1,		tex_x2,	tex_y2));
				vertices.append(AttrVertexNormalTexture(x1,y2,0.1,	0,0,1,		tex_x1,	tex_y2));

				curX += col.width_calc;
			}
			curY += row.height_calc;
		}

		vertices.upload();

		/*
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
*/
	}

/*
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

	*/
	ITexture* getTexture() const {
		return material2->getTextures().all().front().tex;
	}



	std::pair<float, SizeMode> get(const std::string& val) const {

		const size_t posSlash = val.find("/");
		if (posSlash != std::string::npos) {
			const std::string v1 = val.substr(0, posSlash);
			const std::string v2 = val.substr(posSlash+1);
			return std::pair<float, SizeMode>(std::stof(v1) / std::stof(v2), SizeMode::FIXED);
		}

		const size_t posPercent = val.find("%");
		if (posPercent != std::string::npos) {
			const std::string v1 = val.substr(0, posPercent);
			return std::pair<float, SizeMode>(std::stof(v1), SizeMode::DYNAMIC);
		}

		return std::pair<float, SizeMode>(std::stof(val), SizeMode::FIXED);

	}

};


#endif // ROWLAYOUT_H
