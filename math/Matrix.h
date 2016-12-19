#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <cstring>
#include <initializer_list>

template <int size> class Matrix {

private:

	union {
		float values[size*size];
		struct {
			Vec4 v0;
			Vec4 v1;
			Vec4 v2;
			Vec4 v3;
		} vectors ;
	};


public:

	/** uninitialized ctor */
	Matrix() {
		;
	}

	/** identity ctor */
	Matrix(const float val) {
		for (int i = 0; i < size*size; ++i) {
			values[i] = (i%(size+1)==0) ? (val) : (0);
		}
	}

	/** get an identity matrix */
	static Matrix<size> identity() {
		return Matrix<size>(1.0f);
	}

	/** create a matrix from raw-row-major data */
	static Matrix<size> fromRawRowMajor(const float data[size*size]) {
		Matrix<size> m;
		memcpy(m.values, data, size*size*sizeof(float));
		return m;
	}

	/** create a matrix from raw-column-major data */
	static Matrix<size> fromRawColumnMajor(const float data[size*size]) {
		Matrix<size> m;
		memcpy(m.values, data, size*size*sizeof(float));
		m.transpose();
		return m;
	}


	/** set the value for the given row/column */
	inline void set(const int col, const int row, const float val) {
		values[getIdx(col,row)] = val;
	}

	/** get the value for the given row/column */
	inline float get(const int col, const int row) const {
		return values[getIdx(col,row)];
	}

	/** transpose the matrix (inplace) */
	void transpose() {
		for (int x = 0; x < size; ++x) {
			for (int y = 0; y < size; ++y) {
				if (y <= x) {continue;}		// process only the upper-right-half
				const float xy = get(x,y);
				const float yx = get(y,x);
				set(x,y,yx);
				set(y,x,xy);
			}
		}
	}

	/** multiply this matrix by the given matrix */
	Matrix<size> operator * (const Matrix<size>& other) const {
		Matrix<size> out;
		for (int x = 0; x < size; ++x) {
			for (int y = 0; y < size; ++y) {
				float val = 0;
				for (int i = 0; i < size; ++i) {
					val += get(i,y) * other.get(x,i);
				}
				out.set(x,y, val);
			}
		}
		return out;
	}

	/** multiply this matrix with the given vector */
	Vec3 operator * (const Vec3& vec) const {
		static_assert(size == 3, "matrix must have a size of 3x3");
		return Vec3(
			vec.x*get(0,0) + vec.y*get(1,0) + vec.z*get(2,0),
			vec.x*get(0,1) + vec.y*get(1,1) + vec.z*get(2,1),
			vec.x*get(0,2) + vec.y*get(1,2) + vec.z*get(2,2)
		);
	}

	/** multiply this matrix with the given vector */
	Vec4 operator * (const Vec4& vec) const {
		static_assert(size == 4, "matrix must have a size of 4x4");
		return Vec4(
			vec.x*get(0,0) + vec.y*get(1,0) + vec.z*get(2,0) + vec.w*get(3,0),
			vec.x*get(0,1) + vec.y*get(1,1) + vec.z*get(2,1) + vec.w*get(3,1),
			vec.x*get(0,2) + vec.y*get(1,2) + vec.z*get(2,2) + vec.w*get(3,2),
			vec.x*get(0,3) + vec.y*get(1,3) + vec.z*get(2,3) + vec.w*get(3,3)
		);
	}

	/** get one row-vector from the matrix */
	Vec3 getRow3(int row) const {
		return Vec3(get(0, row), get(1, row), get(2, row));
	}

	/** get one column-vector from the matrix */
	Vec3 getCol3(int col) const {
		return Vec3(get(col, 0), get(col, 1), get(col, 2));
	}

	/** get access to the matrix' raw data */
	const float* data() const {return values;}

	/** get the size (width/height) of the matrix */
	int getSize() const {return size;}

	/** get the matrix in row-major notation */
	inline Matrix<size> toRowMajor() const {
		return Matrix<size>(*this);
	}

	/** get the matrix in column-major notation */
	inline Matrix<size> toColumnMajor() const {
		Matrix<size> cpy(*this); cpy.transpose(); return cpy;
	}

	/** get the matrix for openGL (in column-major notation) */
	inline Matrix<size> toGL() const {
		return toColumnMajor();
	}

	/** assign initializer list */
	Matrix& operator = (const std::initializer_list<float>& lst) {
		memcpy(values, lst.begin(), size*size*sizeof(float));
		transpose();
		return *this;
	}

private:

	inline int getIdx(const int col, const int row) const {
		return (row*size) + col;
	}

};

typedef Matrix<4> Mat4;
typedef Matrix<3> Mat3;

#endif // MATRIX_H
