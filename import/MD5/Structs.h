#ifndef MD5_STRUCTS
#define MD5_STRUCTS

#include "../../math/Math.h"
#include <vector>

// http://www.3dgep.com/loading-and-animating-md5-models-with-opengl/#The_MD5Mesh::PrepareNormals_Method

// notes:
// Each weight is assigned to exactly one joint defined in the model’s “joints” section.

namespace MD5 {

	struct BonePose {
		Vec3 pos;
		Quaternion orientation;
	};

	struct Frame {
		std::vector<float> values;
	};

	struct Bone {
		std::string name;
		int parentIdx;
		uint32_t flags;			// bits that denote which values should be overwritten compared to the BaseFrame: 6 bits pos(x,y,z), orientation(x,y,z)
		uint32_t offset;		// offset to the frame.values pointer
	};

	struct Hierarchy {
		std::vector<Bone> bones;
	};

	struct BaseFrame {
		std::vector<BonePose> bones;
	};

	struct AbsoluteFrame {
		std::vector<BonePose> bones;
	};

	struct Joint {
		std::string name;
		int parentIdx;
		Vec3 pos;
		Quaternion orientation;
	};

	struct Skeleton {

		std::vector<Joint> joints;

		/** convert the base-pose-skeleton into an AbsoluteFrame that can be used for animating the mesh */
		AbsoluteFrame asAbsoluteFrame() const {

			AbsoluteFrame frm;
			frm.bones.resize(joints.size());

			for (size_t i = 0; i < joints.size(); ++i) {
				frm.bones[i].pos = joints[i].pos;
				frm.bones[i].orientation = joints[i].orientation;
			}

			return frm;

		}

		/** get the index for the bone named "name" */
		int getBoneIndex(const std::string name) const {
			for (size_t i = 0; i < joints.size(); ++i) {
				if (joints[i].name == name) {return i;}
			}
			return -1;
		}

	};

	struct VertDesc {

		// the first weight that contributes to this vertex
		uint32_t startWeightIdx;

		// the number of weights that contribute to this vertex [starting at startWeightIdx]
		uint32_t countWeight;

	};

	struct Vert {

		uint32_t idx;

		// the vertices texture coordinate
		Vec2 texCoord;

		// the first weight that contributes to this vertex
		uint32_t startWeightIdx;

		// the number of weights that contribute to this vertex [starting at startWeightIdx]
		uint32_t countWeight;

		// this one is NOT stored within the MD5 data
		// it is calculated once from the initial pose [the skeleton stored within the MD5mesh]
		Vec3 _normalBindPose;

		// same as above, but converted from object-space to joint-local-space
		Vec3 _normalJointRel;

	};

	struct Triangle {
		uint32_t idx;
		uint32_t vertIdx1;
		uint32_t vertIdx2;
		uint32_t vertIdx3;
	};

	struct Weight {
		uint32_t idx;
		uint32_t jointIdx;
		float bias;
		Vec3 pos;
	};



}

#endif // MD5_STRUCTS
