#ifndef MESHVERTEX_H
#define MESHVERTEX_H

#include "../math/Math.h"

/** represents a 3D vertex */
typedef Vec3 Vertex;

/** represents a 3D normal */
typedef Vec3 Normal;

/** represents a 2D texture coordinate */
typedef Vec2 TexCoord;


/**
 * represents a combination of:
 *	Vertex with Texture-coordinate
 */
struct VertexTexture {

	Vertex v;
	TexCoord t;

	/** empty ctor */
	VertexTexture() {;}

	/** ctor from vectors */
	VertexTexture(const Vertex& v, const TexCoord& t) : v(v), t(t) {;}

	/** ctor from scalars */
	VertexTexture(const float vx, const float vy, const float vz, const float tu, const float tv) :
		v(vx,vy,vz), t(tu,tv) {
		;
	}

	void setVertex(const Vec3& v)		{this->v = v;}
	void setTexCoord(const Vec2& t)		{this->t = t;}

	const Vec3& getVertex() const		{return v;}
	const Vec2& getTexCoord() const		{return t;}

	bool operator == (const VertexTexture& o) const {
		return (o.v == v) && (o.t == t);
	}

};

/**
 * represents a combination of:
 *	Vertex with Normal an Texture-coordinate
 */
struct VertexNormalTexture {

	Vertex v;
	Normal n;
	TexCoord t;

	/** empty ctor */
	VertexNormalTexture() {;}

	/** ctor from vectors */
	VertexNormalTexture(const Vertex& v, const Normal& n, const TexCoord& t) : v(v), n(n), t(t) {;}

	/** ctor from scalars */
	VertexNormalTexture(const float vx, const float vy, const float vz, const float nx, const float ny, const float nz, const float tu, const float tv) :
		v(vx,vy,vz), n(nx,ny,nz), t(tu,tv) {
		;
	}

	void setVertex(const Vec3& v)		{this->v = v;}
	void setNormal(const Vec3& n)		{this->n = n;}
	void setTexCoord(const Vec2& t)		{this->t = t;}

	const Vec3& getVertex() const		{return v;}
	const Vec3& getNormal() const		{return n;}
	const Vec2& getTexCoord() const		{return t;}

	bool operator == (const VertexNormalTexture& o) const {
		return (o.v == v) && (o.n == n) && (o.t == t);
	}

};

namespace std {
	template<> struct hash<VertexNormalTexture> {
		size_t operator() (const VertexNormalTexture& vnt) const {
			return vnt.getVertex().hash() ^ vnt.getNormal().hash() ^ vnt.getTexCoord().hash();
		}
	};
}

#endif // MESHVERTEX_H
