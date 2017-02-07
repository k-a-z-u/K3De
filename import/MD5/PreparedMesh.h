#ifndef MD5_PREPAREDMESH_H
#define MD5_PREPAREDMESH_H

#include <vector>
#include "../../3D/Estimator.h"

#include "Structs.h"

namespace MD5 {

	/** pre-processed MD5 mesh data */
	struct PreparedMesh {

		Skeleton skeleton;
		std::vector<VertDesc> vertices;
		std::vector<Vec3> normalsInJointSpace;
		std::vector<Vec2> textureCoordinates;
		std::vector<uint32_t> vertexIndicies;

		std::vector<Weight> weights;

		struct VN {
			Vec3 vertex;
			Vec3 normal;
			VN(const Vec3 vertex, const Vec3 normal) : vertex(vertex), normal(normal) {;}
		};

		std::vector<AttrTexture> getTexCoords() const {
			std::vector<AttrTexture> res;
			for (const Vec2 v : textureCoordinates) {res.push_back(AttrTexture(v));}
			return res;
		}

		/** adjusted base-vertices depending on the given pose/skeleton */
		std::vector<AttrVertexNormalTangent> getVertices(const AbsoluteFrame& frame, const bool swapYZ) const {

			// output
			std::vector<AttrVertexNormalTangent> res;
			res.resize(vertices.size());

			// process each vertex: calculate its position and normal
			for (size_t i = 0; i < vertices.size(); ++i) {
				const VN vn = getVertexNormal(frame, i);
				if (swapYZ) {
					res[i].setVertex(Vec3(vn.vertex.x, vn.vertex.z, vn.vertex.y)); // swap
					res[i].setNormal(Vec3(vn.normal.x, vn.normal.z, vn.normal.y)); // swap
				} else {
					res[i].setVertex(Vec3(vn.vertex.x, vn.vertex.y, vn.vertex.z));
					res[i].setNormal(Vec3(vn.normal.x, vn.normal.y, vn.normal.z));
				}
			}

			// now we got all vertices, tex-coord and triangles -> estimate tangents
			const std::vector<Vec3> tangents = Estimator::estimateTangents(res, res, getTexCoords(), getIndices());
			for (size_t i = 0; i < vertices.size(); ++i) { res[i].setTangent(tangents[i]); }

			// done
			return res;

		}

		const std::vector<unsigned int>& getIndices() const {
			return vertexIndicies;
		}


	private:

		VN getVertexNormal(const AbsoluteFrame& frame, const int idx) const {

			const VertDesc& vert = vertices[idx];
			const Vec3 srcNormal = normalsInJointSpace[idx];

			Vec3 posSum(0,0,0);
			Vec3 norSum(0,0,0);

			// calculate both, the resulting vertex an normal
			// by cumulating all weights that contribute to this vertex
			float biasSum = 0;
			for (uint32_t i = 0; i < vert.countWeight; ++i) {

				const Weight& weight = weights[vert.startWeightIdx + i];
				const BonePose joint = frame.bones[weight.jointIdx];

				// convert position from joint-local to object-local space
				const Vec3 pos = (joint.pos +	joint.orientation.rotate(weight.pos)) * weight.bias;

				// convert normal from joint-local to object-local space
				const Vec3 nor = (				joint.orientation.rotate(srcNormal)) * weight.bias;

				biasSum += weight.bias;

				// update
				posSum += pos;
				norSum += nor;

			}

			// sanity check
			_assertNear(biasSum, 1.0, 0.0001, "bias does not sum up to 1.0");

			//_assertNear(norSum.length(), 1.0, 0.01, "normal length is too far from 1.0");

			return VN(posSum, norSum);

		}

	};

}

#endif // MD5_PREPAREDMESH_H
