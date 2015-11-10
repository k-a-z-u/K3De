#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <vector>

#include "MeshVertex.h"
#include "../misc/Memory.h"
#include "../import/ObjImport.h"

class IMesh;
class IndexedMesh;

class MeshFactory {

private:

	std::vector<std::unique_ptr<IMesh>> meshes;

public:

	IndexedMesh* createMesh(const std::string fileName, const bool normalize = false);

	static std::vector<VertexNormalTexture> getPlaneY(const float y, const float x1, const float z1, const float x2, const float z2, const Vec2 texTiling) {

		std::vector<VertexNormalTexture> vertices;
		VertexNormalTexture v1( x1,y,z1,	0,1,0,	0*texTiling.x,0*texTiling.y);
		VertexNormalTexture v2( x2,y,z1,	0,1,0,	1*texTiling.x,0*texTiling.y);
		VertexNormalTexture v3( x2,y,z2,	0,1,0,	1*texTiling.x,1*texTiling.y);
		VertexNormalTexture v4( x1,y,z2,	0,1,0,	0*texTiling.x,1*texTiling.y);

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

IndexedMesh* MeshFactory::createMesh(const std::string fileName, const bool normalize) {

	//tex = TextureFactory::create("/mnt/ssss/3D/checkerboard.png");
//	tex = TextureFactory::create("/mnt/ssss/3D/grass.jpg");

	//Mesh* mesh = new Mesh();
	IndexedMesh* mesh = new IndexedMesh();
//		meshes.push_back(std::make_unique(mesh));

	ObjImport imp;
	imp.load(fileName, normalize);
//		mesh->vertices.append(imp.getUnindexedMeshData());
//		mesh->vertices.upload();
//		mesh->configure();
	mesh->vertices.append(imp.getIndexedMeshVertices());
	mesh->indices.append(imp.getIndexedMeshIndices());
	mesh->vertices.upload();
	mesh->indices.upload();
	mesh->configure();

	for (const VertexNormalTexture& vnt : imp.getIndexedMeshVertices()) {
		mesh->bbox.add(vnt.getVertex());
	}

	return mesh;


}

#endif // MESHFACTORY_H
