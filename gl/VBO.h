#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <cassert>
#include <vector>

#include "../misc/Error.h"


template <typename T> class VBO {

protected:

	GLuint id;
	std::vector<T> data;

public:

	/** ctor */
	VBO() : id(0) {
		create();
	}

	virtual ~VBO() {
		cleanup();
	}

	void resize(const size_t size) {
		data.resize(size);
	}

	std::vector<T>& getData() {
		return data;
	}

	void clear() {
		data.clear();
	}

	void set(const size_t idx, const T& elem) {
		data[idx] = elem;
	}

	void append(const T* elems, const int num) {
		data.insert(data.begin(), elems, elems+num);
	}

	void append(const std::vector<T>& vec) {
		data.insert(data.begin(), vec.begin(), vec.end());
	}

	void append(const T& elem) {
		data.push_back(elem);
	}

	void append(std::initializer_list<T> lst) {
		data.insert(data.begin(), lst.begin(), lst.end());
	}

//	void append(std::initializer_list<T> elems) {

//	}

	T& operator [] (const int idx) {return data[idx];}

	/** number of contained elements */
	size_t size() const {
		return data.size();
	}

private:

	void create() {
		glGenBuffers(1, &id);
		Error::assertOK();
	}

	void cleanup() {
		if (id) {
			glDeleteBuffers(1, &id);
			Error::assertOK();
			id = 0;
		}
	}

};

#endif // VBO_H
