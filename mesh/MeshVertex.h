#ifndef MESHVERTEX_H
#define MESHVERTEX_H

#include "../math/Math.h"

/** represents a 3D vertex */
typedef Vec3 Vertex;

/** represents a 3D normal */
typedef Vec3 Normal;

/** represents a 3D tangent */
typedef Vec3 Tangent;

/** represents a 2D texture coordinate */
typedef Vec2 TexCoord;



struct AttrTexture {

	TexCoord t;

	/** empty ctor */
	AttrTexture() {;}

	/** ctor from vectors */
	AttrTexture(const TexCoord& t) : t(t) {;}

	/** ctor from scalars */
	AttrTexture(const float tu, const float tv) : t(tu,tv) {;}

	void setTexCoord(const Vec2& t)		{this->t = t;}

	const Vec2& getTexCoord() const		{return t;}

	bool operator == (const AttrTexture& o) const {
		return o.t == t;
	}

};

struct AttrVertex {

	Vertex v;

	/** empty ctor */
	AttrVertex() {;}

	/** ctor from vectors */
	AttrVertex(const Vertex& v) : v(v) {;}

	/** ctor from scalars */
	AttrVertex(const float vx, const float vy, const float vz) : v(vx,vy,vz) {;}

	void setVertex(const Vec3& v)		{this->v = v;}

	const Vec3& getVertex() const		{return v;}

	bool operator == (const AttrVertex& o) const {
		return (o.v == v);
	}

};

struct AttrNormal {

	Normal n;

	/** empty ctor */
	AttrNormal() {;}

	/** ctor from vectors */
	AttrNormal(const Normal& n) : n(n) {;}

	/** ctor from scalars */
	AttrNormal(const float nx, const float ny, const float nz) : n(nx,ny,nz) {;}

	void setNormal(const Vec3& n)		{this->n = n;}

	const Vec3& getNormal() const		{return n;}

	bool operator == (const AttrNormal& o) const {
		return (o.n == n);
	}

};

struct AttrTangent {

	Tangent ta;

	/** empty ctor */
	AttrTangent() {;}

	/** ctor from vectors */
	AttrTangent(const Tangent& ta) : ta(ta) {;}

	/** ctor from scalars */
	AttrTangent(const float tx, const float ty, const float tz) : ta(tx, ty, tz) {;}

	void setTangent(const Vec3& ta)		{this->ta = ta;}

	const Vec3& getTangent() const		{return ta;}

	bool operator == (const AttrTangent& o) const {
		return (o.ta == ta);
	}

};

struct AttrColor {

	Vec4 c;

	/** empty ctor */
	AttrColor() {;}

	/** ctor from vectors */
	AttrColor(const Vec4& c) : c(c) {;}

	/** ctor from scalars */
	AttrColor(const float r, const float g, const float b, const float a) : c(r,g,b,a) {;}

	void setColor(const Vec4& c)		{this->c = c;}

	const Vec4& getColor() const		{return c;}

	bool operator == (const AttrColor& o) const {
		return o.c == c;
	}

};





/**
 * represents a combination of: Vertex with Texture-coordinate
 */
struct AttrVertexTexture : public AttrVertex, public AttrTexture {

	// inherited attributes

	/** empty ctor */
	AttrVertexTexture() {;}

	/** ctor from vectors */
	AttrVertexTexture(const Vertex& v, const TexCoord& t) : AttrVertex(v), AttrTexture(t) {;}

	/** ctor from scalars */
	AttrVertexTexture(const float vx, const float vy, const float vz, const float tu, const float tv) :
		AttrVertex(vx,vy,vz), AttrTexture(tu,tv) {
		;
	}

	bool operator == (const AttrVertexTexture& o) const {
		return (o.v == v) && (o.t == t);
	}

};


struct AttrVertexTextureColor : public AttrVertex, public AttrTexture, public AttrColor {

	// inherited attributes

	/** empty ctor */
	AttrVertexTextureColor() {;}

	/** ctor from vectors */
	AttrVertexTextureColor(const Vertex& v, const TexCoord& t, const Vec4& c) : AttrVertex(v), AttrTexture(t), AttrColor(c) {;}

	/** ctor from scalars */
	AttrVertexTextureColor(const float vx, const float vy, const float vz, const float tu, const float tv, const float r, const float g, const float b, const float a) :
		AttrVertex(vx,vy,vz), AttrTexture(tu,tv), AttrColor(r,g,b,a) {
		;
	}

	bool operator == (const AttrVertexTexture& o) const {
		return (o.v == v) && (o.t == t);
	}

};

/**
 * represents a combination of: Vertex with Normal an Texture-coordinate
 */
struct AttrVertexNormalTexture : public AttrVertex, public AttrNormal, public AttrTexture {

	// inherited attributes

	/** empty ctor */
	AttrVertexNormalTexture() {;}

	/** ctor from vectors */
	AttrVertexNormalTexture(const Vertex& v, const Normal& n, const TexCoord& t) : AttrVertex(v), AttrNormal(n), AttrTexture(t) {;}

	/** ctor from scalars */
	AttrVertexNormalTexture(const float vx, const float vy, const float vz, const float nx, const float ny, const float nz, const float tu, const float tv) :
		AttrVertex(vx,vy,vz), AttrNormal(nx,ny,nz), AttrTexture(tu,tv) {
		;
	}

	bool operator == (const AttrVertexNormalTexture& o) const {
		return (o.v == v) && (o.n == n) && (o.t == t);
	}

};



/**
 * represents a combination of:
 *	Vertex with Normal an Texture-coordinate
 */
struct AttrVertexNormalTangentTexture : public AttrVertex, public AttrNormal, public AttrTangent, public AttrTexture {

	// inherited attributes

	/** empty ctor */
	AttrVertexNormalTangentTexture() {;}

	/** ctor from vectors */
	AttrVertexNormalTangentTexture(const Vertex& v, const Normal& n, const Tangent& ta, const TexCoord& t) :
		AttrVertex(v), AttrNormal(n), AttrTangent(ta), AttrTexture(t) {;}

	/** ctor from scalars */
	AttrVertexNormalTangentTexture(const float vx, const float vy, const float vz, const float nx, const float ny, const float nz, const float tx, const float ty, const float tz, const float tu, const float tv) :
		AttrVertex(vx,vy,vz), AttrNormal(nx,ny,nz), AttrTangent(tx, ty, tz), AttrTexture(tu,tv) {
		;
	}

	bool operator == (const AttrVertexNormalTangentTexture& o) const {
		return (o.v == v) && (o.n == n) && (o.ta == ta) && (o.t == t);
	}

};

struct AttrVertexNormal : public AttrVertex, public AttrNormal {

	// inherited attributes

	/** empty ctor */
	AttrVertexNormal() {;}

	/** ctor from vectors */
	AttrVertexNormal(const Vertex& v, const Normal& n) :
		AttrVertex(v), AttrNormal(n) {;}

	/** ctor from scalars */
	AttrVertexNormal(const float vx, const float vy, const float vz, const float nx, const float ny, const float nz) :
		AttrVertex(vx,vy,vz), AttrNormal(nx,ny,nz) {
		;
	}

	bool operator == (const AttrVertexNormal& o) const {
		return (o.v == v) && (o.n == n);
	}

};

struct AttrVertexNormalTangent : public AttrVertex, public AttrNormal, public AttrTangent {

	// inherited attributes

	/** empty ctor */
	AttrVertexNormalTangent() {;}

	/** ctor from vectors */
	AttrVertexNormalTangent(const Vertex& v, const Normal& n, const Tangent& ta) :
		AttrVertex(v), AttrNormal(n), AttrTangent(ta) {;}

	/** ctor from scalars */
	AttrVertexNormalTangent(const float vx, const float vy, const float vz, const float nx, const float ny, const float nz, const float tx, const float ty, const float tz) :
		AttrVertex(vx,vy,vz), AttrNormal(nx,ny,nz), AttrTangent(tx, ty, tz) {
		;
	}

	bool operator == (const AttrVertexNormalTangent& o) const {
		return (o.v == v) && (o.n == n) && (o.ta == ta);
	}

};

//struct VertexNormalTangentTextureBone : public VertexNormalTangentTexture {

//	// https://www.khronos.org/opengl/wiki/Skeletal_Animation
//	// http://voxels.blogspot.de/2014/03/skinned-skeletal-animation-tutorial.html
//	// http://stackoverflow.com/questions/4261812/opengl-skeleton-animation#4261887

//	float boneIdx0, boneWeight0;
//	float boneIdx1, boneWeight1;

//	// TODO


//};

namespace std {

	template<> struct hash<AttrVertexNormalTexture> {
		size_t operator() (const AttrVertexNormalTexture& vnt) const {
			return vnt.getVertex().hash() ^ vnt.getNormal().hash() ^ vnt.getTexCoord().hash();
		}
	};

	template<> struct hash<AttrVertexNormalTangentTexture> {
		size_t operator() (const AttrVertexNormalTangentTexture& vnt) const {
			return vnt.getVertex().hash() ^ vnt.getNormal().hash() ^ vnt.getTangent().hash() ^ vnt.getTexCoord().hash();
		}
	};

}

#endif // MESHVERTEX_H
