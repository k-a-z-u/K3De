#ifndef OBJIMPORT_H
#define OBJIMPORT_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <unordered_map>

#include "../misc/Tokenizer.h"
#include "../mesh/MeshVertex.h"
#include "../math/Math.h"

#include "../3D/AABB.h"
#include "../3D/Estimator.h"

#include "../Debug.h"

#include "../data/Data.h"
#include <sstream>

class ObjImport {

public:

	struct _Data {
		std::vector<Vec3> vertices;
		std::vector<Vec2> texCoords;
		std::vector<Vec3> normals;
	} data;

	// the assembled mesh data (not using indices -> duplicate vertices)
	std::vector<AttrVertexNormalTexture> meshDataUnindexed;

	// the assembled mesh data (using indices -> unique vertices)
	std::unordered_map<AttrVertexNormalTexture, int> uniqueVertices;
	std::vector<AttrVertexNormalTexture> meshDataIndexed;
	std::vector<GLuint> indices;

	void load(const std::string& file, const bool normalize = false, const bool centerAtOrigin = false) {

		Debug("OBJ", "loading file: " + file);

		std::ifstream is(file);
		std::string line;
		while(getline(is, line)) {parseLine(line);}

		Debug("OBJ", "got " << uniqueVertices.size() << " vertices and " << (indices.size()/3) << " triangles");

		// normalize the object? (scale to a max size of 1.0)
		if (normalize)		{doNormalize();}

		// put object's center-of-mass to (0,0,0) ?
		if (centerAtOrigin)	{doCenter();}

	}

	void load(const Data& data, const bool normalize = false, const bool centerAtOrigin = false) {

		Debug("OBJ", "loading data ");

		std::string str ((char*)data.get(), data.size());
		std::stringstream is(str);
		std::string line;
		while(getline(is, line)) {parseLine(line);}

		Debug("OBJ", "got " << uniqueVertices.size() << " vertices and " << (indices.size()/3) << " triangles");

		// normalize the object? (scale to a max size of 1.0)
		if (normalize)		{doNormalize();}

		// put object's center-of-mass to (0,0,0) ?
		if (centerAtOrigin)	{doCenter();}

	}

	/** get all vertices for this mesh in triangle order. contains duplicate vertices */
	const std::vector<AttrVertexNormalTexture>& getUnindexedMeshData() {
		return meshDataUnindexed;
	}

	/** get all (unique) vertices within this mesh */
	const std::vector<AttrVertexNormalTexture>& getIndexedMeshVertices() {
		return meshDataIndexed;
	}

	/** get all (unique) vertices within this mesh with calculated tangent */
	std::vector<AttrVertexNormalTangentTexture> getIndexedMeshVerticesWithTangent() {
		std::vector<AttrVertexNormalTangentTexture> res;
		res.reserve(meshDataIndexed.size());
		std::vector<Vec3> tangents = Estimator::estimateTangents(meshDataIndexed, indices);
		for (size_t i = 0; i < meshDataIndexed.size(); ++i) {
			AttrVertexNormalTangentTexture vntt(meshDataIndexed[i].getVertex(), meshDataIndexed[i].getNormal(), tangents[i], meshDataIndexed[i].getTexCoord());
			res.push_back(vntt);
		}
		return res;
	}

	/** get the indices to create triangles from the indexed mesh data */
	const std::vector<GLuint> getIndexedMeshIndices() {
		return indices;
	}

private:

	/** parse one line of the .obj file */
	void parseLine(const std::string& line) {

		if (line.length() < 2) {return;}

		Tokenizer t(line, '\r');
		std::string token = t.getToken(' ');

		if ("v"  == token) {parseVertex(t);}
		if ("vt" == token) {parseTexCoord(t);}
		if ("vn" == token) {parseNormal(t);}
		if ("f"  == token) {parseFace(t);}

	}

	void parseVertex(Tokenizer& t) {
		const float x = std::stof(t.getToken(' '));
		const float y = std::stof(t.getToken(' '));
		const float z = std::stof(t.getToken(' '));
		data.vertices.push_back(Vec3(x,y,z));
	}

	void parseTexCoord(Tokenizer& t) {
		const float u = std::stof(t.getToken(' '));
		const float v = std::stof(t.getToken(' '));
		data.texCoords.push_back(Vec2(u, -v));
	}

	void parseNormal(Tokenizer& t) {
		const float x = std::stof(t.getToken(' '));
		const float y = std::stof(t.getToken(' '));
		const float z = std::stof(t.getToken(' '));
		data.normals.push_back(Vec3(x,y,z));
	}

	void parseFace(Tokenizer& t) {

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
			vnt.setVertex( data.vertices[std::stoi(v)-1] );
			vnt.setNormal(		(vn.empty()) ? (Vec3(0,0,0)) :	(data.normals[std::stoi(vn)-1]) );
			vnt.setTexCoord(	(vt.empty()) ? (Vec2(0,0)) :	(data.texCoords[std::stoi(vt)-1]) );

			// update unindexed data-set
			meshDataUnindexed.push_back(vnt);

			// update indexed data-set
			auto it = uniqueVertices.find(vnt);
			if (it == uniqueVertices.end()) {
				uniqueVertices[vnt] = meshDataIndexed.size();
				indices.push_back(meshDataIndexed.size());
				meshDataIndexed.push_back(vnt);
			} else {
				indices.push_back(it->second);
			}

		}

		// this will both, create normal triangles and triangulate polygons
		// see: http://www.mathopenref.com/polygontriangles.html
		for (int i = 1; i < (int) indices.size()-1; ++i) {
			this->indices.push_back(indices[0]);
			this->indices.push_back(indices[i]);
			this->indices.push_back(indices[i+1]);
		}

		// sanity check
		//if (numVertices != 3) {throw "this face is not a triangle!";}

	}

	void doNormalize() {

		AABB bbox;
		for (const AttrVertexNormalTexture& vnt : meshDataIndexed) {bbox.add(vnt.v);}

		const Vec3 diff = bbox.max() - bbox.min();
		const float max = std::max(std::max(diff.x, diff.y), diff.z);

		for (AttrVertexNormalTexture& vnt : meshDataUnindexed) {vnt.v /= max;}
		for (AttrVertexNormalTexture& vnt : meshDataIndexed) {vnt.v /= max;}

	}

	void doCenter() {

		Vec3 sum;
		AABB bbox;
		for (const AttrVertexNormalTexture& vnt : meshDataIndexed) {
			bbox.add(vnt.v);
			sum += vnt.v;
		}

		Vec3 avg = sum / meshDataIndexed.size();

		for (AttrVertexNormalTexture& vnt : meshDataUnindexed) {vnt.v -= avg;}
		for (AttrVertexNormalTexture& vnt : meshDataIndexed) {vnt.v -= avg;}

	}


};

#endif // OBJIMPORT_H
