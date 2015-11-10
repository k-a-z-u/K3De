#ifndef TERRAINFACTORY_H
#define TERRAINFACTORY_H

#include <KLib/Assertions.h>
#include "../math/Math.h"
#include "../mesh/MeshVertex.h"
#include "../import/ImageFactory.h"
#include "Terrain.h"
#include "../math/Math.h"


class TerrainFactory {

	/** state-struct */
	struct TerrainStruct {
		int w;
		int h;
		int xTiles;
		int yTiles;
		Image height;
		Vec3 center;
		Vec3 scale;
		Terrain* terrain;
	};


	static Vec3 getVertex(const int px, const int py, const TerrainStruct& t) {

		const int ox = t.w * t.scale.x;
		const int oy = 1.0 * t.scale.y;
		const int oz = t.h * t.scale.z;

		Vec3 vec;
		vec.y = t.center.y - oy/2 + (t.height.getGrey(px, py) / 255.0f * t.scale.y);
		vec.x = t.center.x - ox/2 + (px * t.scale.x);
		vec.z = t.center.z - oz/2 + (py * t.scale.z);
		return vec;

	}

	/** get the texture-coordinate for the given pixel within the terrain's height map */
	static Vec2 getTexCoord(const int px, const int py, const TerrainStruct& t) {
		const float u = (float)px / (float)t.height.getWidth();
		const float v = (float)py / (float)t.height.getHeight();
		return Vec2(u,v);
	}

	static int getIndex(const int px, const int py, const TerrainStruct& t) {
		return (px) + (py * t.w);
	}



public:

	Terrain* create(const Vec3& center, const Vec3& scale, const Image& heightMap, const int xTiles = 8, const int yTiles = 8) {

		TerrainStruct ts;
		ts.xTiles = xTiles;
		ts.yTiles = yTiles;

		// create a new terrain
		ts.terrain = new Terrain();

		// load the height map
		ts.height = heightMap;
		_assertEqual(ImageFormat::IMAGE_GREY, ts.height.getFormat(), "invalid image format for terrain's height map");

		// set params
		ts.w = ts.height.getWidth();
		ts.h = ts.height.getHeight();
		ts.center = center;
		ts.scale = scale;

		// total number of vertices in x and y direction
		const int xVertices = ts.w;
		const int yVertices = ts.h;

		// create all vertices (the whole terrain)
		std::vector<VertexNormalTexture> vertices;
		for (int py = 0; py < yVertices; ++py) {
			for (int px = 0; px < xVertices; ++px) {
				const Vec3 v = getVertex(px, py, ts);
				const Vec3 n(0,1,0);
				const Vec2 t = getTexCoord(px, py, ts);
				vertices.push_back( VertexNormalTexture(v, n, t) );
				ts.terrain->params.vertices.push_back(v);
			}
		}

//		// calculate normals for each vertex using eigenvalues
//		for (int py = 0; py < ts.h; ++py) {
//			for (int px = 0; px < ts.w; ++px) {

//				// advanced normal calculation
//				EVec3 C = MISC::toEigen(vertices[getIndex(px, py, ts)].getVertex());
//				EMat3 M; M.setZero();
//				for (int yy = -1; yy <= +1; ++yy) {
//					for (int xx = -1; xx <= +1; ++xx) {
//						int x = px+xx; if (x < 0 || x >= ts.w-1) {continue;}
//						int y = py+yy; if (y < 0 || y >= ts.h-1) {continue;}
//						EVec3 V = MISC::toEigen(vertices[getIndex(x, y, ts)].getVertex());
//						EVec3 V2 = V-C;
//						M += V2*V2.transpose();
//					}
//				}

//				EVec3 ev = MISC::getEigenvectorForSmallestEigenvalue(M);
//				if (ev(1) < 0) {ev = -ev;}		// swap?
//				const int idx1 = getIndex(px+0, py+0, ts);
//				vertices[idx1].setNormal(MISC::toGLM(ev));

//			}
//		}

		// calculate normals for each vertex using interpolated normals
		for (int py = 1; py < ts.h-1; ++py) {
			for (int px = 1; px < ts.w-1; ++px) {

				// interpolate the normal for (px,py) using its direct neighbors
				Vec3 vCenter = vertices[getIndex(px, py, ts)].v;
				Vec3 vl = vertices[getIndex(px-1, py  , ts)].v;
				Vec3 vt = vertices[getIndex(px  , py+1, ts)].v;
				Vec3 vr = vertices[getIndex(px+1, py  , ts)].v;
				Vec3 vb = vertices[getIndex(px  , py-1, ts)].v;

				Vec3 n(0,0,0);
				n += Math::cross(vl,vt);
				n += Math::cross(vt,vr);
				n += Math::cross(vr,vb);
				n += Math::cross(vb,vl);
				n = Math::normalize(n);

				vertices[getIndex(px,py,ts)].setNormal(n);

			}
		}

		// now create the terrain parts
		ts.terrain->parts.resize(ts.xTiles * ts.yTiles);
		const int _numX = xVertices / ts.xTiles;
		const int _numY = yVertices / ts.yTiles;

		// fill them with vertices
		for (int y = 0; y < ts.yTiles; ++y) {
			for (int x = 0; x < ts.xTiles; ++x) {

				// connect adjacent areas. (except the last one)
				const int numX = (x < ts.xTiles - 1) ? (_numX + 1) : (_numX);
				const int numY = (y < ts.yTiles - 1) ? (_numY + 1) : (_numY);

				// get the current terrain part
				const int terrainIdx = x + (y*ts.xTiles);
				TerrainPart& part = ts.terrain->parts.at(terrainIdx);

				// append all its vertices
				int sy = y * _numY;
				int sx = x * _numX;
				for (int py = sy; py < sy+numY; ++py) {
					for (int px = sx; px < sx+numX; ++px) {
						const int idx = getIndex(px, py, ts);
						part.vertices.append(vertices[idx]);
					}
				}

				// calculate the vertex-index for a given (x,y)
				auto getIndexInPart = [numX] (const int px, const int py) {return (px) + (py * numX);};

				// connect the part's vertices to triangles
				for (int py = 0; py < numY-1; ++py) {
					for (int px = 0; px < numX-1; ++px) {

						const int idx1 = getIndexInPart(px+0, py+0);
						const int idx2 = getIndexInPart(px+1, py+0);
						const int idx3 = getIndexInPart(px+1, py+1);
						const int idx4 = getIndexInPart(px+0, py+1);

						// first triangle
						part.indices.append(idx1);
						part.indices.append(idx3);
						part.indices.append(idx2);

						// second triangle
						part.indices.append(idx1);
						part.indices.append(idx4);
						part.indices.append(idx3);

					}
				}

				// upload
				part.upload();

			}
		}

		// store params
		ts.terrain->params.size = Vec3(ts.w, 1, ts.h) * scale;
		ts.terrain->params.scale = scale;
		ts.terrain->params.numXVertices = ts.w;
		ts.terrain->params.numYVertices = ts.h;
		ts.terrain->params.numXTiles = xTiles;
		ts.terrain->params.numYTiles = yTiles;

		// done
		return ts.terrain;

	}

};

#endif // TERRAINFACTORY_H
