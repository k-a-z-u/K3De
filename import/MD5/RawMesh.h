#ifndef MD5_RAWMESH_H
#define MD5_RAWMESH_H

#include "Structs.h"
#include "PreparedMesh.h"

namespace MD5 {

	/** simply parsed MD5 mesh data */
	struct RawMesh {

		std::vector<Vert> vertices;
		std::vector<Triangle> triangles;
		std::vector<Weight> weights;

		/** prepare the parsed mesh-data using the mesh's base-skeleton */
		PreparedMesh prepare(const Skeleton& s) {

			PreparedMesh out;

			// allocate
			out.normalsInJointSpace.resize(vertices.size());
			out.textureCoordinates.resize(vertices.size());
			out.vertices.resize(vertices.size());

			// copy "as-is"
			out.weights = weights;
			out.skeleton = s;

			// get the skeleton's pose
			const AbsoluteFrame frame = s.asAbsoluteFrame();

			// process each triangle
			for (const Triangle& t : triangles) {

				// construct triangle indices
				out.vertexIndicies.push_back(t.vertIdx1);
				out.vertexIndicies.push_back(t.vertIdx2);
				out.vertexIndicies.push_back(t.vertIdx3);

				// get the triangle's 3 vertices using the skeleton's pose
				const Vec3 a = getVertex(frame, t.vertIdx1);
				const Vec3 b = getVertex(frame, t.vertIdx2);
				const Vec3 c = getVertex(frame, t.vertIdx3);

				// calculate the triangle's normal
				const Vec3 n = cross(c-a, b-a);

				// and the triangle's area
				const float area = Math::getTriangleArea(a, b, c);

				// update the normal's that belong to each vertex
				out.normalsInJointSpace[t.vertIdx1] += n * area;
				out.normalsInJointSpace[t.vertIdx2] += n * area;
				out.normalsInJointSpace[t.vertIdx3] += n * area;

			}

			// process each newly calculated normal
			for (uint32_t i = 0; i < out.normalsInJointSpace.size(); ++i) {

				// 1) ensure each normal has a length of 1
				const Vec3& normal = out.normalsInJointSpace[i].normalized();
				Vec3 resNormal(0,0,0);

				// vertex that corresponds to this normal
				const Vert& vert = vertices[i];

				// 2) convert the normal from object-local [bind-pose] form
				// to joint-local form -> less calculations during the animation process
				// as the normal is only rotated [like the vertices] instead of newly estimated
				float weightSum = 0;
				for (uint32_t j = 0; j < vert.countWeight; ++j) {
					const Weight& weight = weights[vert.startWeightIdx + j];
					const BonePose joint = frame.bones[weight.jointIdx];
					resNormal += ( joint.orientation.inverse().rotate(normal) ) * weight.bias;
					weightSum += weight.bias;
				}

				// sanity check
				_assertNear(weightSum, 1.0, 0.001, "weights do not sum up to 1.0");
				resNormal.normalize();

				Vec3 testNormal(0,0,0);
				for (uint32_t j = 0; j < vert.countWeight; ++j) {
					const Weight& weight = weights[vert.startWeightIdx + j];
					const BonePose joint = frame.bones[weight.jointIdx];
					testNormal += ( joint.orientation.rotate(resNormal) ) * weight.bias;
				}
				Vec3 err = testNormal - normal;
				if (err.length() > 0.01) {
					int x = 0; (void) x;
				}


//				resNormal.normalize();;
//				_assertNear(resNormal.length(), 1.0, 0.01, "calculated normal length is not 1.0");

				// save
				out.normalsInJointSpace[i] = resNormal;

			}

			// process each vertex
			for (const Vert& v : vertices) {

				// set texture coordinates
				out.textureCoordinates[v.idx] = v.texCoord;

				// set vertex parameters
				out.vertices[v.idx].startWeightIdx = v.startWeightIdx;
				out.vertices[v.idx].countWeight = v.countWeight;

			}

			// done
			return out;

		}

	private:

		/** calculate the idx-th vertex' position based on the given skeleton-frame */
		Vec3 getVertex(const AbsoluteFrame& frame, const int idx) const {

			const Vert& vert = vertices[idx];

			Vec3 posSum(0,0,0);

			// calculate the resulting vertex by cumulating all weights that contribute to it
			for (uint32_t i = 0; i < vert.countWeight; ++i) {

				const Weight& weight = weights[vert.startWeightIdx + i];
				const BonePose joint = frame.bones[weight.jointIdx];

				// convert weight's position from joint-local to object-local space
				const Vec3 pos = (joint.pos +	joint.orientation.rotate(weight.pos)) * weight.bias;

				// update
				posSum += pos;

			}

			return posSum;

		}

	};

	struct ImportedMesh {

		int mode = 0;

		// the skeleton for all sub-meshes
		Skeleton skeleton;

		// all parsed raw-meshes
		std::vector<RawMesh> meshes;

		/** get all parsed (sub)meshes "as-is" */
		const std::vector<RawMesh>& getRawMeshes() {return meshes;}

		/** get the whole mesh's skeleton */
		const Skeleton& getSkeleton() {return skeleton;}

		/** get the idx-th pre-processed mesh */
		const PreparedMesh getPreparedMesh(const int idx) {
			return meshes.at(idx).prepare(skeleton);
		}

	};

}

#endif // MD5_RAWMESH_H
