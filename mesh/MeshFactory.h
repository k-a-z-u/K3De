#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <vector>
#

#include "MeshVertex.h"
#include "../misc/Memory.h"
#include "../import/ObjImport.h"

class IMesh;
class IndexedMesh;
class InstanceMesh;

class MeshFactory {

private:

	const char* NAME = "MeshFac";
	std::vector<std::unique_ptr<IMesh>> meshes;

public:

	/** create a new mesh by loading the given file */
	inline IndexedMesh* createMesh(const std::string fileName, const bool normalize = false, const bool centerAtOrigin = false);

	/** create a new instance-wrapper around the given source mesh [allows position,rotation,scale without modifying the mesh] */
	inline InstanceMesh* createNewInstance(IMesh* mesh);

	static std::vector<AttrVertexNormalTexture> getPlaneY(const float y, const float x1, const float z1, const float x2, const float z2, const Vec2 texTiling) {

		std::vector<AttrVertexNormalTexture> vertices;
		AttrVertexNormalTexture v1( x1,y,z1,	0,1,0,	0*texTiling.x,0*texTiling.y);
		AttrVertexNormalTexture v2( x2,y,z1,	0,1,0,	1*texTiling.x,0*texTiling.y);
		AttrVertexNormalTexture v3( x2,y,z2,	0,1,0,	1*texTiling.x,1*texTiling.y);
		AttrVertexNormalTexture v4( x1,y,z2,	0,1,0,	0*texTiling.x,1*texTiling.y);

		// triangle 1
		vertices.push_back(v1);
		vertices.push_back(v3);
		vertices.push_back(v2);

		// triangle 1
		vertices.push_back(v1);
		vertices.push_back(v4);
		vertices.push_back(v3);

		// done
		return vertices;

	}

};

#include "UnindexedMesh.h"
#include "IndexedMesh.h"
#include "InstanceMesh.h"

InstanceMesh* MeshFactory::createNewInstance(IMesh* srcMesh) {

	// create a new instance-wrapper around the given source
	InstanceMesh* mesh = new InstanceMesh(srcMesh);

	// add to the list of meshes [e.g. for scene cleanup]
	meshes.push_back(std::make_unique(mesh));

	// done
	return mesh;

}

IndexedMesh* MeshFactory::createMesh(const std::string fileName, const bool normalize, const bool centerAtOrigin) {

	Debug(NAME, "loading mesh: " + fileName);

	// create a new, empty mesh
	IndexedMesh* mesh = new IndexedMesh();

	// add to the list of meshes [e.g. for scene cleanup]
	meshes.push_back(std::make_unique(mesh));

	// import from .OBJ file
	ObjImport imp;
	imp.load(fileName, normalize, centerAtOrigin);

	// append vertices and faces [=indices]
	mesh->vertices.append(imp.getIndexedMeshVerticesWithTangent());
	mesh->indices.append(imp.getIndexedMeshIndices());

	// upload both
	mesh->vertices.upload();
	mesh->indices.upload();

	// final setup
	mesh->configure();

	// calculate BBox
	for (const AttrVertexNormalTexture& vnt : imp.getIndexedMeshVertices()) {
		mesh->bbox.add(vnt.getVertex());
	}

	// done
	return mesh;

}

#endif // MESHFACTORY_H
