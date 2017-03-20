#ifndef OBJIMPORT_H
#define OBJIMPORT_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <unordered_map>

#include "../../gl/gl.h"

#include "../../misc/Tokenizer.h"
#include "../../mesh/MeshVertex.h"
#include "../../math/Math.h"

#include "../../3D/AABB.h"
#include "../../3D/Estimator.h"

#include "../../Debug.h"

#include "../../data/Data.h"
#include "../../data/Resource.h"
#include "../../data/ResourceFactory.h"

#include "Mesh.h"

#include <sstream>

namespace OBJ {

	/**
	 * import static mesh data from .obj files
	 */
	class ImportMesh {

	public:



//		void load(const std::string& file, const bool normalize = false, const bool centerAtOrigin = false) {
//		}

		static Mesh load(const Resource& res, const bool normalize = false, const bool centerAtOrigin = false) {

			Debug("OBJ", "loading resource: " + res.getName());

			const Data d = ResourceFactory::get().get(res);
			return load(d, normalize, centerAtOrigin);

		}

		static Mesh load(const Data& data, const bool normalize = false, const bool centerAtOrigin = false) {

			Debug("OBJ", "loading data ");

			// the to-be-build data-container
			Mesh res;

			std::string str ((char*)data.get(), data.size());
			std::stringstream is(str);
			std::string line;

			// parse all lines within the .obj data
			while(getline(is, line)) {
				parseLine(res, line);
			}

			Debug("OBJ", "got " << res.uniqueVertices.size() << " vertices and " << (res.indices.size()/3) << " triangles");

			// normalize the object? (scale to a max size of 1.0)
			if (normalize)		{doNormalize(res);}

			// put object's center-of-mass to (0,0,0) ?
			if (centerAtOrigin)	{doCenter(res);}

			// done
			return res;

		}





	private:

		/** parse one line of the .obj file */
		static void parseLine(Mesh& res, const std::string& line) {

			if (line.length() < 2) {return;}

			Tokenizer t(line, '\r');
			std::string token = t.getToken(' ');

			if ("v"  == token) {parseVertex(res, t);}
			if ("vt" == token) {parseTexCoord(res, t);}
			if ("vn" == token) {parseNormal(res, t);}
			if ("f"  == token) {parseFace(res, t);}

		}

		static void parseVertex(Mesh& res, Tokenizer& t) {
			const float x = std::stof(t.getToken(' '));
			const float y = std::stof(t.getToken(' '));
			const float z = std::stof(t.getToken(' '));
			res.data.vertices.push_back(Vec3(x,y,z));
		}

		static void parseTexCoord(Mesh& res, Tokenizer& t) {
			const float u = std::stof(t.getToken(' '));
			const float v = std::stof(t.getToken(' '));
			res.data.texCoords.push_back(Vec2(u, -v));
		}

		static void parseNormal(Mesh& res, Tokenizer& t) {
			const float x = std::stof(t.getToken(' '));
			const float y = std::stof(t.getToken(' '));
			const float z = std::stof(t.getToken(' '));
			res.data.normals.push_back(Vec3(x,y,z));
		}

		static void parseFace(Mesh& res, Tokenizer& t) {

			std::vector<int> indices;

			int numVertices = 0;
			while(t.hasNext()) {

				++numVertices;
				const std::string token = t.getToken(' ');
				Tokenizer t2(token);
				const std::string v = t2.getToken('/', false);
				const std::string vt = t2.getToken('/', false);
				const std::string vn = t2.getToken('/', false);

				// create a new vertex/normal/texture combination
				AttrVertexNormalTexture vnt;
				vnt.setVertex(res.data.vertices[std::stoi(v)-1] );
				vnt.setNormal(		(vn.empty()) ? (Vec3(0,0,0)) :	(res.data.normals[std::stoi(vn)-1]) );
				vnt.setTexCoord(	(vt.empty()) ? (Vec2(0,0)) :	(res.data.texCoords[std::stoi(vt)-1]) );

				// update unindexed data-set
				res.meshDataUnindexed.push_back(vnt);

				// update indexed data-set
				auto it = res.uniqueVertices.find(vnt);
				if (it == res.uniqueVertices.end()) {
					res.uniqueVertices[vnt] = res.meshDataIndexed.size();
					indices.push_back(res.meshDataIndexed.size());
					res.meshDataIndexed.push_back(vnt);
				} else {
					indices.push_back(it->second);
				}

			}

			// this will both, create normal triangles and triangulate polygons
			// see: http://www.mathopenref.com/polygontriangles.html
			for (int i = 1; i < (int) indices.size()-1; ++i) {
				res.indices.push_back(indices[0]);
				res.indices.push_back(indices[i]);
				res.indices.push_back(indices[i+1]);
			}

			// sanity check
			//if (numVertices != 3) {throw "this face is not a triangle!";}

		}

		static void doNormalize(Mesh& res) {

			AABB bbox;
			for (const AttrVertexNormalTexture& vnt : res.meshDataIndexed) {bbox.add(vnt.v);}

			const Vec3 diff = bbox.max() - bbox.min();
			const float max = std::max(std::max(diff.x, diff.y), diff.z);

			// adjust vertices
			for (AttrVertexNormalTexture& vnt : res.meshDataUnindexed)	{vnt.v /= max;}
			for (AttrVertexNormalTexture& vnt : res.meshDataIndexed)	{vnt.v /= max;}

		}

		static void doCenter(Mesh& res) {

			Vec3 sum;
			AABB bbox;
			for (const AttrVertexNormalTexture& vnt : res.meshDataIndexed) {
				bbox.add(vnt.v);
				sum += vnt.v;
			}

			Vec3 avg = sum / res.meshDataIndexed.size();

			// adjust vertices
			for (AttrVertexNormalTexture& vnt : res.meshDataUnindexed) {vnt.v -= avg;}
			for (AttrVertexNormalTexture& vnt : res.meshDataIndexed) {vnt.v -= avg;}

		}


	};

}

#endif // OBJIMPORT_H
