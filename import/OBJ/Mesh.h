#ifndef MESH_H
#define MESH_H

#include "../../mesh/IMesh.h"
#include "../../math/AffineTransform.h"

#include "../../gl/VAO.h"
#include "../../gl/VBOArray.h"
#include "../../gl/VBOArrayIndex.h"


namespace OBJ {

	/**
	 * parsed mesh.
	 * this data serves as base for adding an OBJMesh to the scene
	 */
	class Mesh {

		friend class ImportMesh;

	private:

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

	public:



		/** get all vertices for this mesh in triangle order. contains duplicate vertices */
		const std::vector<AttrVertexNormalTexture>& getUnindexedMeshData() const {
			return meshDataUnindexed;
		}


		/** get triangle-from-vertices indicies */
		const std::vector<GLuint>& getIndexedMeshIndices() const {
			return indices;
		}

		/** get all (unique) vertices within this mesh */
		const std::vector<AttrVertexNormalTexture>& getIndexedMeshVertices() const {
			return meshDataIndexed;
		}


		/** get all (unique) vertices within this mesh with calculated tangent */
		std::vector<AttrVertexNormalTangentTexture> getIndexedMeshVerticesWithTangent() const {
			std::vector<AttrVertexNormalTangentTexture> res;
			res.reserve(meshDataIndexed.size());
			std::vector<Vec3> tangents = Estimator::estimateTangents(meshDataIndexed, indices);
			for (size_t i = 0; i < meshDataIndexed.size(); ++i) {
				AttrVertexNormalTangentTexture vntt(meshDataIndexed[i].getVertex(), meshDataIndexed[i].getNormal(), tangents[i], meshDataIndexed[i].getTexCoord());
				res.push_back(vntt);
			}
			return res;
		}


	};

}

#endif // MESH_H
