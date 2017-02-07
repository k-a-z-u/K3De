#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <cstdint>
#include <cstdlib>

struct Vec2;
struct Vec3;
struct Vec4;



struct Vec2 {

	union {
		float data[2];
		struct {float x; float y;};
		struct {float u; float v;};
	};


	Vec2() : x(0), y(0) {;}
	Vec2(const float x, const float y) : x(x), y(y) {;}

	Vec2 operator + (const Vec2& o)	const {return Vec2(x+o.x, y+o.y);}
	Vec2 operator - (const Vec2& o)	const {return Vec2(x-o.x, y-o.y);}
	Vec2 operator * (const Vec2& o)	const {return Vec2(x*o.x, y*o.y);}
	Vec2 operator / (const Vec2& o)	const {return Vec2(x/o.x, y/o.y);}

	Vec2 operator / (const float v) const {return Vec2(x/v, y/v);}
	Vec2 operator * (const float v) const {return Vec2(x*v, y*v);}

	bool operator == (const Vec2& o) const {return (o.x == x) && (o.y == y);}
	bool operator != (const Vec2& o) const {return (o.x != x) || (o.y != y);}

	float length() const {return std::sqrt( (x*x) + (y*y) );}

	Vec2 normalized() const {return *this / length();}

	size_t hash() const {return *((uint32_t*)&x) ^ *((uint32_t*)&y);}

};

struct Vec3 {

	union {
		float data[3];
		struct {float x; float y; float z;};
		struct {float r; float g; float b;};
	};

	Vec3() : x(0), y(0), z(0) {;}
	Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {;}

	Vec3&	operator -= (const Vec3& o)	{x-=o.x; y-=o.y; z-=o.z; return *this;}
	Vec3	operator - () const {return Vec3(-x,-y,-z);}


	/** array access operator */
	float& operator [] (const int idx)		{return data[idx];}

	/** const array access operator */
	float operator [] (const int idx) const	{return data[idx];}

	Vec3 operator + (const Vec3& o) const	{return Vec3(x+o.x, y+o.y, z+o.z);}
	Vec3 operator - (const Vec3& o) const	{return Vec3(x-o.x, y-o.y, z-o.z);}
	Vec3 operator * (const Vec3& o) const	{return Vec3(x*o.x, y*o.y, z*o.z);}
	Vec3 operator / (const Vec3& o) const	{return Vec3(x/o.x, y/o.y, z/o.z);}

	Vec3 operator * (const float v) const	{return Vec3(x*v, y*v, z*v);}

	Vec3& operator += (const Vec3& o)		{x+=o.x; y+=o.y; z+=o.z; return *this;}

	Vec3& operator /= (const float v)		{x/=v; y/=v; z/=v; return *this;}
	Vec3& operator *= (const float v)		{x*=v; y*=v; z*=v; return *this;}

	Vec3	operator / (const float v) const {return Vec3(x/v, y/v, z/v);}
	bool operator == (const Vec3& o) const {return (o.x == x) && (o.y == y) && (o.z == z);}

	void set(const float x, const float y, const float z) {this->x = x; this->y = y; this->z = z;}

	float length() const {return std::sqrt( (x*x) + (y*y) + (z*z) );}

	void normalize() { const float l = length(); x/=l; y/=l; z/=l; }
	Vec3 normalized() const {return *this / length();}

	void swapYZ() {std::swap(y,z);}
	Vec3 swappedYZ() const {return Vec3(x,z,y);}


	size_t hash() const {return *((uint32_t*)&x) ^ *((uint32_t*)&y) ^ *((uint32_t*)&z);}



	/** convert this Vec3 to a Vec4 with w=1 */
	inline Vec4 xyz1() const;
	inline Vec2 xy() const;
	inline Vec2 xz() const;

};

struct Vec4 {

	union {
		float data[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};

	Vec4() : x(0), y(0), z(0), w(0) {;}
	Vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {;}

	bool operator == (const Vec4& o) const {return (o.x == x) && (o.y == y) && (o.z == z) && (o.w == w);}

	/** convert this Vec3 to a Vec4 (remove w) */
	inline Vec3 xyz() const;

	/** divide the vector by its w component */
	inline Vec4 divW() const {return Vec4(x/w, y/w, z/w, 1.0f);}

	/** array access operator */
	float& operator [] (const int idx)		{return data[idx];}

	/** const array access operator */
	float operator [] (const int idx) const	{return data[idx];}

	Vec4 operator + (const Vec4& o) const	{return Vec4(x+o.x, y+o.y, z+o.z, w+o.w);}
	Vec4 operator - (const Vec4& o) const	{return Vec4(x-o.x, y-o.y, z-o.z, w-o.w);}
	Vec4 operator * (const Vec4& o) const	{return Vec4(x*o.x, y*o.y, z*o.z, w*o.w);}
	Vec4 operator / (const Vec4& o) const	{return Vec4(x/o.x, y/o.y, z/o.z, w/o.w);}

	Vec4 operator / (const float v) const	{return Vec4(x/v, y/v, z/v, w/v);}

	Vec4& operator /= (const float v)		{x/=v; y/=v; z/=v; w/=w; return *this;}
	Vec4& operator *= (const float v)		{x*=v; y*=v; z*=v; w*=w; return *this;}

	float length() const {return std::sqrt( (x*x) + (y*y) + (z*z) + (w*w) );}

	Vec4 normalized() const {return *this / length();}

};


Vec4 Vec3::xyz1() const {return Vec4(x,y,z,1);}
Vec2 Vec3::xy() const {return Vec2(x,y);}
Vec2 Vec3::xz() const {return Vec2(x,z);}

Vec3 Vec4::xyz() const {return Vec3(x,y,z);}




inline float dot(const Vec2 v1, const Vec2 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

inline float dot(const Vec3 v1, const Vec3 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

inline float dot(const Vec4& v1, const Vec4& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}


inline Vec3 cross(const Vec3 a, const Vec3 b) {
	return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}











template <int size> struct Vector {

	float data[size];

	/** ctor. data is UNINITIALIZED */
	Vector() {;}

	/** ctor. all params receive the same value */
	Vector(const float val) {
		for (int i = 0; i < size; ++i) {data[i] = val;}
	}

	/** ctor. using initializer list */
	Vector(std::initializer_list<float> l) {
		//static_assert(l.size() == 3, "invalid number of arguments");
		memcpy(&data[0], l.begin(), l.size() * sizeof(float));
	}

	/** compare two vectors whether they are equal */
	bool operator == (const Vector<size>& o) const {
		return memcmp(o.data, data, size*sizeof(float)) == 0;
	}

	/** compare two vectors whether they are not equal */
	bool operator != (const Vector<size>& o) const {
		return !(*this == o);
	}

	Vector<size>& operator -= (const Vector<size>& o)	{
		for (int i = 0; i < size; ++i) {data[i] -= o.data[i];}
		return *this;
	}

	Vector<size> operator - (const Vector<size>& o)	const {
		Vector<size> out;
		for (int i = 0; i < size; ++i) {out[i] = data[i] - o.data[i];}
		return out;
	}

	Vector<size> operator - () const {
		Vector<size> out;
		for (int i = 0; i < size; ++i) {out[i] = -data[i];}
		return out;
	}

	Vector<size> operator / (const float v) const {
		Vector<size> out;
		for (int i = 0; i < size; ++i) {out[i] = data[i] / v;}
		return out;
	}

	/** array access operator */
	float& operator [] (const int idx) {return data[idx];}

	size_t hash() const {
		size_t res = 0;
		for (int i = 0; i < size; ++i) {
			const uint32_t tmp = *( (uint32_t*)(&data[i]) );
			res ^= tmp;
		}
		return res;
	}

};



#endif // VECTOR_H
