#ifndef MD5_ANIMATION_H
#define MD5_ANIMATION_H

#include "Structs.h"

namespace MD5 {





	/** an animation consists of several skeleton poses [AbsoluteFrames], one pose per frame */
	struct Animation {

		/** this animation's name */
		std::string name;

		/** animation speed in FPS */
		float fps = 24;

		/** hierarchy of the skeleton used within this animation */
		Hierarchy hierarchy;

		///** the frame all sub-frames were relative to. needed mainly for initial pose */
		//AbsoluteFrame baseFrame;

		/** all frames within this animation */
		std::vector<AbsoluteFrame> frames;


		///** get the animation's base frame */
		//const AbsoluteFrame& getBaseFrame() {return baseFrame;}

		/** get the idx-th frame within this animation */
		const AbsoluteFrame& getFrame(const int idx) {return frames.at(idx);}

		/** get the number of frames for this animation */
		int getNumFrames() const {return frames.size();}

		/** get the index for the bone named "name" */
		int getBoneIndex(const std::string name) const {
			for (size_t i = 0; i < hierarchy.bones.size(); ++i) {
				if (hierarchy.bones[i].name == name) {return i;}
			}
			return -1;
		}

	};

	struct ImportedAnimation {

		int mode;
		float scale;

		ImportedAnimation(const float scale) : mode(0), scale(scale) {;}

		/** hierarchy of the skeleton used within this animation */
		Hierarchy hierarchy;

		/** base frame all other frames are relative to */
		BaseFrame baseFrame;

		/** all frames relative to the base-frame */
		std::vector<Frame> frames;

		/** perform some pre-calculations */
		Animation prepare() const {
			Animation res;
			res.hierarchy = hierarchy;
			//res.baseFrame = toAbsoluteframe(hierarchy, baseFrame, Frame());
			for (const Frame& f : frames) {
				const AbsoluteFrame af = toAbsoluteframe(hierarchy, baseFrame, f);
				res.frames.push_back(af);
			}
			return res;
		}

	private:

		/** convert incomplete, relative frames to complete, absolute frames [see MD5 sources above] */
		const AbsoluteFrame toAbsoluteframe(const Hierarchy& h, const BaseFrame& base, const Frame& f) const {

			const size_t numBones = h.bones.size();

			// copy
			AbsoluteFrame abs;
			abs.bones.resize(numBones);

			// process each bone. those must be sorted by hierarchy!
			for (size_t i = 0; i < numBones; ++i) {

				const Bone& bone = h.bones[i];
				const int parentIdx = bone.parentIdx;

				// start with the BaseFrame's pos/orientation for this bone
				BonePose relPose = base.bones[i];

				// overwrite all values pos(x,y,z), orientation(x,y,z) depending on the bone's flags
				uint32_t offset = bone.offset;
				if (bone.flags &  1) {relPose.pos.x = f.values[offset++];}
				if (bone.flags &  2) {relPose.pos.y = f.values[offset++];}
				if (bone.flags &  4) {relPose.pos.z = f.values[offset++];}
				if (bone.flags &  8) {relPose.orientation.x = f.values[offset++];}
				if (bone.flags & 16) {relPose.orientation.y = f.values[offset++];}
				if (bone.flags & 32) {relPose.orientation.z = f.values[offset++];}
				relPose.orientation.fixW();


				// sanity check
				if (parentIdx >= (int)i) {throw Exception("invalid bone order. must be: all parents first");}

				if (parentIdx == -1) {

					// no parent -> nothing to do
					abs.bones[i].pos = relPose.pos * scale;
					abs.bones[i].orientation = relPose.orientation;

				} else {

					//const Joint& parentJoint = s.joints[s.joints[i].parentIdx];
					const BonePose& parentPose = abs.bones[parentIdx];

					abs.bones[i].pos =			parentPose.pos + parentPose.orientation.rotate(relPose.pos) * scale;
					abs.bones[i].orientation =	(parentPose.orientation * relPose.orientation).normalized();

				}

				// for testing
				//abs.bones[i].pos = s.joints[i].pos;
				//abs.bones[i].orientation = s.joints[i].orientation;

			}

			return abs;


		}

	};



}

#endif // MD5_ANIMATION_H
