#ifndef WATER_H
#define WATER_H

#include "../gl/VAO.h"
#include "../gl/VBOArray.h"

#include "../mesh/MeshVertex.h"
#include "../mesh/MeshFactory.h"

#include "../textures/Multitexture.h"

#include "../mesh/IMesh.h"

#include "../shader/Shader.h"

#include "../Engine.h"
#include "../3D/Plane.h"

class Water : public Plane3 {

	VAO vao;
	VBOArrayStatic<AttrVertexNormalTexture> vertices;
	Mat4 mat;
	Multitexture textures;
	AABB bbox;

public:

	/** ctor */
	Water(const Vec3& center, const Vec2 size, const Vec2 texTiling = Vec2(1,1)) : mat(1.0f) {

		//std::vector<AttrVertexNormalTexture> vec = MeshFactory::getPlaneY(center.y, center.x-size.x/2, center.z-size.y/2, center.x+size.x/2, center.z+size.y/2, texTiling);
		//vertices.append(vec);
		//vertices.upload();

		//vao.bind();

		//vertices.bind();
		//vao.setVertices(0, 8*4);
		//vao.setNormals(1, 8*4, 3*4);
		//vao.setTexCoords(2, 8*4, 6*4);

		//vao.unbind();

		addPlaneY(0, -size/2, size/2);
		setPosition(center);

	}

//	void setReflect(ITexture* texReflect) {
//		textures.set(0, texReflect);
//	}
//	void setRefract(ITexture* texRefract) {
//		textures.set(1, texRefract);
//	}
//	void setNormals(ITexture* texNormals) {
//		textures.set(2, texNormals);
//	}
//	void setDuDv(ITexture* texDuDv) {
//		textures.set(3, texDuDv);
//	}

//	void render(const SceneState&, const RenderState&) override {
//		textures.bindAll();
//		//shader->bind();
//		//shader->setVector("viewportSize", Engine::get()->getScene()->getCamera().getScreenSize());
//		vao.bind();
//		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
//		vao.unbind();
//		//shader->unbind();
//		textures.unbindAll();
//	}

//	const Mat4& getMatrix() const override {
//		return mat;
//	}

//	AABB getAABBworld() const override {
//		return bbox;
//	}

//	AABB getAABBmodel() const override {
//		return bbox;
//	}



//	// TODO:
//	virtual bool isVisible(const Mat4& MVP) const override {
//		(void) MVP;
//		return true;
//	}

};

#endif // WATER_H
