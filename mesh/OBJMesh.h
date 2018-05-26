#ifndef OBJMESH_H
#define OBJMESH_H



#include <KLib/Assertions.h>

#include "MeshVertex.h"
#include "../3D/AABB.h"

#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../gl/VAO.h"
#include "../misc/Error.h"

#include "../import/OBJ/Animation.h"
#include "../import/OBJ/Mesh.h"

#include "IMesh.h"
#include "Transformable.h"


class OBJMesh : public IMesh, public Transformable {

private:

	friend class MeshFactory;

	VAO vao;
	VBOArrayDynamic<AttrVertexNormalTangentTexture> vertices;
	VBOArrayIndexStatic indices;

	AABB bbox;

	/** underlying, parsed mesh data */
	OBJ::Mesh mesh;

	/** named list of animations */
	std::unordered_map<std::string, OBJ::Animation> animations;

	struct Animate {
		OBJ::Animation* curAni = nullptr;
		Time aniStartTime;
		int curFrameNr = -1;
	} animate;



public:

	OBJMesh(const OBJ::Mesh& mesh) : mesh(mesh) {

		configure();

		// triangle indicies stay the same, even for animations
		indices.append( mesh.getIndexedMeshIndices() );
		indices.upload();

		// allocate space for vertices [once]. this data will be overwritten for every frame
		vertices.resize(mesh.getIndexedMeshVertices().size());

		// start with the mesh's skeleton pose
		updateFrame(mesh.getIndexedMeshVerticesWithTangent());

	}

	/** add a new, named animation to the mesh */
	void addAnimation(const std::string& name, const OBJ::Animation& ani) {
		animations.insert( std::pair<std::string, OBJ::Animation>(name, ani) );
	}

	/** select the current animation */
	void setAnimation(const std::string& name) {
		auto it = animations.find(name);
		if (it == animations.end()) {throw Exception("animation " + name + " not found!");}
		animate.curAni = &(it->second);
		animate.aniStartTime = Time::runtime();
		animate.curFrameNr = -1;
	}

	int getCurrentFrame() {
		return (animate.curAni) ? (animate.curFrameNr) : (-1);
	}

	Triangles3 getTriangles() const override {
		throw Exception("not yet implemented");
	}

private:

	/** configure the GPU buffers */
	void configure() {

		// http://stackoverflow.com/questions/14249634/opengl-vaos-and-multiple-buffers
		// answer 1
		vao.bind(); {

			// vertex, normal, texture, tangen
			vertices.bind();
			vao.setVertices(0, 11*4);
			vao.setNormals(1, 11*4, 3*4);
			vao.setTangents(3, 11*4, 6*4);		// todo swap indicies [2,3] here and within shaders?
			vao.setTexCoords(2, 11*4, 9*4);


			// vertices, normals and tangents from the vertices buffer
//			vertices.bind();
//			vao.setVertices(0, 9*4, 0);
//			vao.setNormals(1, 9*4, 3*4);
//			vao.setTangents(3, 9*4, 6*4);		// todo swap indicies [2,3] here and within shaders?

			// NOTE: md5 uses an extra texture-buffer, as those are fixed
			// this is the same for .obj files, but would require further refactoring!
			// TODO LATER
			// texture coordinates from the texture buffer
			//textures.bind();
			//vao.setTexCoords(2, 2*4, 0);		// todo swap indicies [2,3] here and within shaders?

			// the triangles
			indices.bind();

		} vao.unbind();

	}

public:

	// TODO
	void load() {

	}

	const Mat4& getMatrix() const override {
		return transform.getMatrix();
	}

	AffineTransform& getTransform() {
		return transform;
	}

	void onBeforeRender(const SceneState& ss) override {
		updateAnimation(ss);
	}

	void render(const SceneState&, const RenderState&) override {

		if (material) {material->bind();}

		vao.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		Error::assertOK();
		vao.unbind();

		if (material) {material->unbind();}

	}

	AABB getAABBmodel() const override {
		return bbox;
	}

	AABB getAABBworld() const override {
		return bbox.getTransformed(transform.getMatrix());
	}

private:

	void updateAnimation(const SceneState& ss) {

		// currently not animated? -> just skip
		if (!animate.curAni) {return;}

		OBJ::Animation& ani = *animate.curAni;

		const int frameNr = int((ss.getCurrentTime() - animate.aniStartTime).seconds() * ani.fps) % ani.getNumFrames();

		// update needed?
		if (frameNr != animate.curFrameNr) {

			const OBJ::Frame& frame = ani.getFrame(frameNr);
			updateFrame(frame);

		}

		animate.curFrameNr = frameNr;

	}

	void updateFrame(const OBJ::Frame& frame) {

		// TODO: only vertex,normal,tangent as texture is static
		updateFrame(frame.vertices);

	}

	void updateFrame(const std::vector<AttrVertexNormalTangentTexture>& vntt) {

		// TODO: only vertex,normal,tangent as texture is static
		vertices.upload(vntt, false);

	}

};

#endif // OBJMESH_H
