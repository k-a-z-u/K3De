#ifndef MD5_MD5IMPORT_H
#define MD5_MD5IMPORT_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

#include "../../math/Math.h"
#include "../../Debug.h"
#include "../../misc/Splitter.h"

#include "Structs.h"
#include "RawMesh.h"

// http://tfc.duke.free.fr/coding/md5-specs-en.html
// http://www.3dgep.com/loading-and-animating-md5-models-with-opengl/

namespace MD5 {

	class Import {

	private:


		const char* NAME = "MD5";
		float scale = 1.0;

	public:

		Import(const float scale) : scale(scale) {
			;
		}

		ImportedMesh loadMeshString(const std::string& data) {

			Debug(NAME, "loading string data");

			ImportedMesh imp;
			std::stringstream ss(data);
			std::string line;
			while(getline(ss, line)) {parseLine(imp, line);}

			//Debug(NAME, "got " << joints.size() << " joints, " << vertices.size() << " vertices, " << triangles.size() << " triangles, " << weights.size() << " weights");

			return imp;

		}

		ImportedMesh loadMeshFile(const std::string& file) {

			Debug(NAME, "loading file: " + file);

			ImportedMesh imp;
			std::ifstream is(file);
			std::string line;
			while(getline(is, line)) {parseLine(imp, line);}

			//Debug(NAME, "got " << joints.size() << " joints, " << vertices.size() << " vertices, " << triangles.size() << " triangles, " << weights.size() << " weights");

			return imp;

		}

		void setScale(const float scale) {
			this->scale = scale;
		}


	private:

		void parseLine(ImportedMesh& dst, const std::string& line) {

			const std::vector<std::string> args = Splitter::split(line);

			// skip empty lines
			if (args.empty()) {return;}

			// mode switch?
			if (dst.mode == 0) {
				if (args[0] == "joints")	{dst.mode = 1; return;}
				if (args[0] == "mesh")		{dst.mode = 2; dst.meshes.resize(dst.meshes.size()+1); return;}
			}

			if (dst.mode == 1) {
				if (args[0] == "}") {dst.mode = 0; return;}
				parseJoint(dst.skeleton, line, args);
			}

			if (dst.mode == 2) {
				if (args[0] == "}") {dst.mode = 0; return;}
				parseMesh(dst.meshes.back(), line, args);
			}

		}

		/** parse joint related data */
		void parseJoint(Skeleton& dst, const std::string& line, const std::vector<std::string>& split) {

			// "name" parent ( pos.x pos.y pos.z ) ( orient.x orient.y orient.z )
			if ((split[2] != "(") || (split[6] != ")") || (split[7] != "(") || (split[11] != ")")) {
				throw Exception("invalid joint: " + line);
			}

			Joint j;
			j.name = split[0];
			j.parentIdx = std::stoi(split[1]);
			j.pos = Vec3(std::stof(split[3]), std::stof(split[4]), std::stof(split[5])) * scale;
			j.orientation = Quaternion(std::stof(split[8]), std::stof(split[9]), std::stof(split[10]));

			dst.joints.push_back(j);

		}

		/** parse mesh related data */
		void parseMesh(RawMesh& dst, const std::string& line, const std::vector<std::string>& split) {

			if (split[0] == "vert")		{parseVertex(dst, line, split);}
			if (split[0] == "tri")		{parseTriangle(dst, line, split);}
			if (split[0] == "weight")	{parseWeight(dst, line, split);}

		}

		void parseVertex(RawMesh& dst, const std::string& line, const std::vector<std::string>& split) {

			// vert vertIndex ( s t ) startWeight countWeight
			if ((split[2] != "(") || (split[5] != ")")) {
				throw Exception("invalid vertex: " + line);
			}

			Vert v;
			v.idx = std::stoi(split[1]);
			v.texCoord = Vec2(std::stof(split[3]), std::stof(split[4]));
			v.startWeightIdx = std::stoi(split[6]);
			v.countWeight = std::stoi(split[7]);
			if (dst.vertices.size() != v.idx) {throw Exception("unexpected vertex index");}
			dst.vertices.push_back(v);

		}

		void parseTriangle(RawMesh& dst, const std::string&, const std::vector<std::string>& split) {

			// tri triIndex vertIndex[0] vertIndex[1] vertIndex[2]

			Triangle t;
			t.idx = std::stoi(split[1]);
			t.vertIdx1 = std::stoi(split[2]);
			t.vertIdx2 = std::stoi(split[3]);
			t.vertIdx3 = std::stoi(split[4]);
			if (dst.triangles.size() != t.idx) {throw Exception("unexpected triangle index");}
			dst.triangles.push_back(t);

		}

		void parseWeight(RawMesh& dst, const std::string& line, const std::vector<std::string>& split) {

			// weight weightIndex joint bias ( pos.x pos.y pos.z )
			if ((split[4] != "(") || (split[8] != ")")) {
				throw Exception("invalid vertex: " + line);
			}

			Weight w;
			w.idx = std::stoi(split[1]);
			w.jointIdx = std::stoi(split[2]);
			w.bias = std::stof(split[3]);
			w.pos = Vec3(std::stof(split[5]), std::stof(split[6]), std::stof(split[7])) * scale;

			if (dst.weights.size() != w.idx) {throw Exception("unexpected weight index");}
			dst.weights.push_back(w);

		}

	};

}

#endif // MD5IMPORT_H
