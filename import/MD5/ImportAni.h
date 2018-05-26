#ifndef MD5_MD5ANIIMPORT_H
#define MD5_MD5ANIIMPORT_H

#include <string>
#include <vector>

#include "../../math/Math.h"
#include "../../misc/Splitter.h"

#include "Structs.h"
#include "Animation.h"

#include "../../Assert.h"

#include "../../data/Data.h"
#include "../../data/Resource.h"
#include "../../data/ResourceFactory.h"

namespace MD5 {

	class ImportAnimation {

	private:

		const char* NAME = "MD5ani";
		float scale = 1.0;

	public:

		/** ctor */
		ImportAnimation(const float scale) : scale(scale) {
			;
		}

		/** load an MD5 animation from the given string-data */
		ImportedAnimation load(const Resource& res) {

			Debug(NAME, "loading resource: " + res.getName());

			const Data d = ResourceFactory::get().get(res);
			const std::string dat = d.asString();
			return loadString(dat);

		}

		/** load an MD5 animation from the given string-data */
		ImportedAnimation loadString(const std::string& data) {

			Debug(NAME, "loading string data");

			ImportedAnimation ani(scale);
			std::stringstream ss(data);
			std::string line;
			while(getline(ss, line)) {parseLine(ani, line);}

			//Debug(NAME, "got " << hierarchy.bones.size() << " bones and " << frames.size() << " frames");

			return ani;

		}

		/** load a MD5 animation from the given file */
		ImportedAnimation loadFile(const std::string& file) {

			Debug(NAME, "loading file: " + file);

			ImportedAnimation ani(scale);
			std::ifstream is(file);
			std::string line;
			while(getline(is, line)) {parseLine(ani, line);}

			Assert::isFalse(ani.frames.empty(), "did not find any frames within animation");

			//Debug(NAME, "got " << hierarchy.bones.size() << " bones and " << frames.size() << " frames");

			return ani;

		}

		void setScale(const float scale) {
			this->scale = scale;
		}


	private:

//		int mode = 0;
//		Hierarchy hierarchy;
//		BaseFrame baseFrame;

//		/** all parsed frames */
//		std::vector<Frame> frames;

		void parseLine(ImportedAnimation& dst, const std::string& line) {

			const std::vector<std::string> args = Splitter::split(line);

			// skip empty lines
			if (args.empty()) {return;}

			// mode switch?
			switch(dst.mode) {

			case 0:
				if (args[0] == "hierarchy")	{dst.mode = 1; return;}
				if (args[0] == "baseframe")	{dst.mode = 2; return;}
				if (args[0] == "frame")		{dst.mode = 3; dst.frames.resize(dst.frames.size()+1); return;}
				break;

			case 1:
				if (args[0] == "}") {dst.mode = 0; return;}
				parseHierarchy(dst.hierarchy, line, args);
				break;

			case 2:
				if (args[0] == "}") {dst.mode = 0; return;}
				parseBaseFrame(dst.baseFrame, line, args);
				break;

			case 3:
				if (args[0] == "}") {dst.mode = 0; return;}
				parseFrame(dst.frames.back(), line, args);
				break;

			}

		}

	private:

		/** read the skeleton's hierarchy, flags and index-offsets */
		void parseHierarchy(Hierarchy& dst, const std::string&, const std::vector<std::string>& split) {

			// name parentIDx flags dataOffset
			Bone b;
			b.name = split[0];
			b.parentIdx = std::stoi(split[1]);
			b.flags = std::stoi(split[2]);
			b.offset = std::stoi(split[3]);
			dst.bones.push_back(b);

		}

		/** read the BaseFrame. all other Frames are the BaseFrame + overwritten Values [usually only some values are NOT overwritten] */
		void parseBaseFrame(BaseFrame& dst, const std::string& line, const std::vector<std::string>& split) {

			// ( -0.000000 0.001643 -0.000604 ) ( 0.707107 0.000242 0.707107 )
			if ((split[0] != "(") || (split[4] != ")")) {
				throw Exception("invalid baseframe: " + line);
			}

			// update BaseFrame
			BonePose p;
			p.pos =			Vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
			p.orientation =	Quaternion(std::stof(split[6]), std::stof(split[7]), std::stof(split[8]));

			dst.bones.push_back(p);

		}

		/** parse one frame. one frame consists of 0-6 values pos(xyz), orientation(xyz) and overwrites the base-pose */
		void parseFrame(Frame& dst, const std::string&, const std::vector<std::string>& split) {

			for (const std::string& arg : split) {
				const float val = std::stof(arg);
				dst.values.push_back(val);
			}

		}

	};

}

#endif // MD5_MD5ANIIMPORT_H
