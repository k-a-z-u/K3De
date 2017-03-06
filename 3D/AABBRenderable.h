#ifndef AABBRENDERABLE_H
#define AABBRENDERABLE_H

#include "AABB.h"
#include "HasAABB.h"

#include "../mesh/MeshVertex.h"
#include "../gl/VBOArray.h"
#include "../gl/VAO.h"
#include "../scene/Renderable.h"


/**
 * render an axis-aligned bounding box
 */
class AABBRenderable : public Renderable {

	HasAABB* obj;
	VAO vao;
	VBOArrayStatic<Vertex> vertices;
	Mat4 identity;

public:

	AABBRenderable() : identity(1.0f) {

		vao.bind();
		vertices.bind();
		vao.setVertices(0);
		vao.unbind();

	}

	void set(HasAABB* obj) {
		this->obj = obj;
		build();
	}

private:

	void build() {

		const AABB bbox = obj->getAABBworld();

		const Vec3 min = bbox.min();
		const Vec3 max = bbox.max();

		const Vertex v1(min.x, min.y, min.z);
		const Vertex v2(max.x, min.y, min.z);
		const Vertex v3(max.x, max.y, min.z);
		const Vertex v4(min.x, max.y, min.z);

		const Vertex v5(min.x, min.y, max.z);
		const Vertex v6(max.x, min.y, max.z);
		const Vertex v7(max.x, max.y, max.z);
		const Vertex v8(min.x, max.y, max.z);

		std::vector<Vertex> vec = {v1,v2, v2,v3, v3,v4, v4,v1,  v5,v6, v6,v7, v7,v8, v8,v5,  v1,v5, v2,v6, v3,v7, v4,v8};
		vertices.clear();
		vertices.append(vec);
		vertices.upload();

	}

	virtual bool isVisible(const Mat4& MVP) const override {
		(void) MVP;
		return true;
	}

public:

	void render(const SceneState&, const RenderState&) override {

		vao.bind();
		glDrawArrays(GL_LINES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();

	}

	virtual const Mat4& getMatrix() const override {return identity;}

};

#endif // AABBRENDERABLE_H
