#ifndef MD5MESH_H
#define MD5MESH_H


#include <KLib/Assertions.h>

#include "MeshVertex.h"
#include "../3D/AABB.h"

#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"
#include "../gl/VAO.h"
#include "../misc/Error.h"

#include "../import/MD5/Animation.h"
#include "../import/MD5/PreparedMesh.h"

#include "IMesh.h"
#include "Transformable.h"


class MD5Mesh : public IMesh, public Transformable {

private:

	friend class MeshFactory;

	VAO vao;
	VBOArrayStatic<AttrTexture> textures;
	VBOArrayDynamic<AttrVertexNormalTangent> vertices;
	VBOArrayIndexStatic indices;

	AABB bbox;

	const MD5::PreparedMesh mesh;
	std::unordered_map<std::string, MD5::Animation> animations;

	struct Animate {
		MD5::Animation* curAni = nullptr;
		Time aniStartTime;
		int curFrameNr = -1;
	} animate;

	struct Attachment {

		IMesh* other;					// to-be-attached mesh
		int otherBoneIdx;				// to-be-attached bone [if any]
		int myBoneIdx;
		AffineTransform boneTransform;	// calculated transformation for the bone

		Attachment(MD5Mesh* other, const int otherBoneIdx, const int myBoneIdx) : other(other), otherBoneIdx(otherBoneIdx), myBoneIdx(myBoneIdx) {;}
		Attachment(IMesh* other, const int myBoneIdx) : other(other), otherBoneIdx(-1), myBoneIdx(myBoneIdx) {;}

		AffineTransform& getOtherTransform() {
			return (dynamic_cast<Transformable*>(other))->getTransform();
		}

	};

	std::vector<Attachment> attachments;


public:

	/** hidden CTOR */
	MD5Mesh(const MD5::PreparedMesh& mesh) : mesh(mesh) {

		configure();

		textures.append( mesh.getTexCoords() );
		textures.upload();

		indices.append( mesh.getIndices() );
		indices.upload();

		vertices.resize(mesh.vertices.size());

	}

	/** add a new, named animation to the mesh */
	void addAnimation(const std::string& name, const MD5::Animation& ani) {
		animations.insert( std::pair<std::string, MD5::Animation>(name, ani) );
	}

	/** set the current animation */
	void setAnimation(const std::string& name) {
		auto it = animations.find(name);
		if (it == animations.end()) {throw Exception("animation " + name + " not found!");}
		animate.curAni = &(it->second);
		animate.aniStartTime = Time::runtime();
		animate.curFrameNr = -1;
	}



	void attach(MD5Mesh* other, const std::string& otherBone, const std::string& myBone) {

		const int otherBoneIdx = other->mesh.skeleton.getBoneIndex(otherBone);
		if (otherBoneIdx == -1) {throw Exception("failed to find other's bone named " + otherBone);}

		const int myBoneIdx = mesh.skeleton.getBoneIndex(myBone);
		if (myBoneIdx == -1) {throw Exception("failed to find my bone named " + myBone);}

		attachments.push_back(Attachment(other, otherBoneIdx, myBoneIdx));

	}

	void attach(IMesh* other, const std::string& myBone) {

		const int myBoneIdx = mesh.skeleton.getBoneIndex(myBone);
		if (myBoneIdx == -1) {throw Exception("failed to find my bone named " + myBone);}

		attachments.push_back(Attachment(other, myBoneIdx));

	}

	int getCurrentFrame() {
		return (animate.curAni) ? (animate.curFrameNr) : (-1);
	}

private:

	/** configure the GPU buffers */
	void configure() {

		// http://stackoverflow.com/questions/14249634/opengl-vaos-and-multiple-buffers
		// answer 1
		vao.bind(); {

			// texture coordinates from the texture buffer
			textures.bind();
			vao.setTexCoords(2, 2*4, 0);		// todo swap indicies [2,3] here and within shaders?

			// vertices, normals and tangents from the vertices buffer
			vertices.bind();
			vao.setVertices(0, 9*4, 0);
			vao.setNormals(1, 9*4, 3*4);
			vao.setTangents(3, 9*4, 6*4);		// todo swap indicies [2,3] here and within shaders?

			// the triangles
			indices.bind();

		} vao.unbind();

	}

public:

	// TODO
	void load() {

//		// update the bbox;
//		for (const AttrVertexNormalTangentTexture& vnt : vertices) {
//			bbox.add(vnt.getVertex());
//		}

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

		MD5::Animation& ani = *animate.curAni;

		const int frameNr = int((ss.getCurrentTime() - animate.aniStartTime).seconds() * ani.fps) % ani.getNumFrames();

		// update needed?
		if (frameNr != animate.curFrameNr) {

			const MD5::AbsoluteFrame& frame = ani.getFrame(frameNr);
			const std::vector<AttrVertexNormalTangent> vnt = mesh.getVertices(frame, true);
			vertices.upload(vnt, false);

			for (Attachment& a : attachments) {

				// bone's transformation
				const Mat4 mBR = frame.bones[a.myBoneIdx].orientation.swappedYZ().toMat4(); //Math::getRotation(a.getTransform().getRotationRad());
				const Mat4 mBT = Math::getTranslation(frame.bones[a.myBoneIdx].pos.swappedYZ());
				a.boneTransform.setManually(mBT*mBR);

				// transformation order: final = this.transform * bone.transform * attachedObject.transform
				a.getOtherTransform().setPost(&a.boneTransform);
				a.boneTransform.setPost(&this->getTransform());

			}

		}

		animate.curFrameNr = frameNr;

	}

};

#endif // MD5MESH_H
