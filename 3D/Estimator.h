#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include "../math/Math.h"
#include "../mesh/MeshVertex.h"
#include <vector>

class Estimator {

public:

	/** estimate tangent from normal */
	static Vec3 getTangent(const Vec3& normal) {

		// http://www.gamedev.net/topic/552411-calculate-tangent-from-normal/
		// " The key thing is that the tangent is the vector that points in the direction of increasing 'U', and the bi-tangent points in the direction of increasing 'V'."

		return Vec3(normal.y, normal.x, normal.z);

	}

	/** get the tangent for v by examining the other two vertices that are part of the triangle v, v1, v2 */
	static void estimateTangents(AttrVertexNormalTangentTexture& vntt1, AttrVertexNormalTangentTexture& vntt2, AttrVertexNormalTangentTexture& vntt3) {

		// http://gamedev.stackexchange.com/questions/68612/how-to-compute-tangent-and-bitangent-vectors

		const Vec3& v1 = vntt1.getVertex();
		const Vec3& v2 = vntt2.getVertex();
		const Vec3& v3 = vntt3.getVertex();

		const Vec3& n1 = vntt1.getNormal();
		const Vec3& n2 = vntt2.getNormal();
		const Vec3& n3 = vntt3.getNormal();

		const Vec2& w1 = vntt1.getTexCoord();
		const Vec2& w2 = vntt2.getTexCoord();
		const Vec2& w3 = vntt3.getTexCoord();

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		const float r = 1.0F / (s1 * t2 - s2 * t1);
		const Vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		const Vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

		const Vec3 tan1 = sdir;
		const Vec3 tan2 = tdir;

		const Vec3 tangent1 = (tan1 - n1 * dot(n1, tan1)).normalized();
		const Vec3 tangent2 = (tan1 - n2 * dot(n2, tan1)).normalized();
		const Vec3 tangent3 = (tan1 - n3 * dot(n3, tan1)).normalized();

		//  Calculate handedness
		const float _w1 = (dot(cross(n1, tan1), tan2) < 0.0F) ? -1.0f : 1.0f;
		const float _w2 = (dot(cross(n2, tan1), tan2) < 0.0F) ? -1.0f : 1.0f;
		const float _w3 = (dot(cross(n3, tan1), tan2) < 0.0F) ? -1.0f : 1.0f;

		vntt1.setTangent(tangent1*_w1);
		vntt2.setTangent(tangent2*_w2);
		vntt3.setTangent(tangent3*_w3);

	}

	static std::vector<Vec3> estimateTangents(const std::vector<AttrVertexNormalTexture>& verts, const std::vector<unsigned int>& indices) {
		return estimateTangents<AttrVertexNormalTexture, AttrVertexNormalTexture, AttrVertexNormalTexture>(verts, verts, verts, indices);
	}

	template <typename InVerts, typename InTex, typename InNorm> static std::vector<Vec3> estimateTangents(const std::vector<InVerts>& verts, const std::vector<InNorm>& norm, const std::vector<InTex>& tex, const std::vector<unsigned int>& indices) {

		// sanity check
		if (indices.size() % 3 != 0) {throw "error!";}
		const int numTriangles = indices.size() / 3;

		// temporal buffers
		std::vector<Vec3> tan1; tan1.resize(indices.size());
		std::vector<Vec3> tan2; tan2.resize(indices.size());

		// calculate intermediate values based on each triangle [some vertices are part of several triangles]
		for (int triangle = 0; triangle < numTriangles; ++triangle) {

			// indices to the triangle's vertices
			const int i1 = indices[triangle*3 + 0];
			const int i2 = indices[triangle*3 + 1];
			const int i3 = indices[triangle*3 + 2];

			// get only the vertex
			const Vec3& v1 = verts[i1].getVertex();
			const Vec3& v2 = verts[i2].getVertex();
			const Vec3& v3 = verts[i3].getVertex();

			// get only the texture coordinate
			const Vec2& w1 = tex[i1].getTexCoord();
			const Vec2& w2 = tex[i2].getTexCoord();
			const Vec2& w3 = tex[i3].getTexCoord();

			// actual calculation

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			const float r = 1.0F / (s1 * t2 - s2 * t1);
			const Vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			const Vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

			// update intermediate values

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;

		}

		// output tangents
		std::vector<Vec3> out;
		out.resize(verts.size());

		// update each vertex
		for (size_t a = 0; a < verts.size(); ++a) {

			const Vec3 n = norm[a].getNormal();
			const Vec3 t = tan1[a];

			// Gram-Schmidt orthogonalize
			const Vec3 tangent = (t - n * dot(n, t)).normalized();

			// Calculate handedness
			// should be < 0.0f, however, this produces flickering in some parts. so we added a delta
			const float w = (dot(cross(n, t), tan2[a]) < 0.0003f) ? (-1.0f) : (1.0f);

			out[a] = tangent*w;	// normally tangent should be vec4.. is this OK?
//			out[a].setVertex(verts[a].getVertex());
//			out[a].setNormal(verts[a].getNormal());
//			out[a].setTexCoord(tex[a].getTexCoord());
//			out[a].setTangent(tangent * w);

		}

		// done
		return out;

	}



};

#endif // ESTIMATOR_H
