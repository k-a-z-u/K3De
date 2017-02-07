#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <cassert>
#include <vector>

#include "../Exception.h"
#include "../misc/Error.h"

template <typename T, int Mode> class VBOBase {

private:

	GLuint id;

protected:

	/** ctor */
	VBOBase() : id(0) {
		create();
	}

	/** dtor */
	virtual ~VBOBase() {
		cleanup();
	}

	/** create the GPU buffer */
	void create() {
		glGenBuffers(1, &id);
		Error::assertOK();
	}

	/** cleanup the GPU buffer */
	void cleanup() {
		if (id) {
			glDeleteBuffers(1, &id);
			Error::assertOK();
			id = 0;
		}
	}

public:

	/** bind this VBO as vertex-buffer */
	void bind() {
		glBindBuffer(Mode, this->id);
		Error::assertOK();
	}

	/** unbind the vertex-buffer */
	void unbind() {
		glBindBuffer(Mode, 0);
		Error::assertOK();
	}

};


/** dynamic vertex-buffer object */
template <typename T, int Mode> class VBODynamic : public VBOBase<T, Mode> {

private:

	T* data = nullptr;
	size_t numElements = 0;

	GLsync syncObj = 0;

public:

	/** resize the local data */
	void resize(const size_t size) {
		allocateElements(size);
	}

	/** get the data-pointer */
	T* getData() {
		if (!data) {throw Exception("call allocate() first!");}
		return (T*) data;
	}

	T* begin() { return data; }
	T* end()   { return data + numElements; }

	/** array access */
	T& operator [] (const int idx) {return data[idx];}

	/** overwrite! */
	void append(const std::vector<T>& vec) {
		resize(vec.size());
		beginSync();
		memcpy(data, vec.data(), vec.size() * sizeof(T));
		endSync();
	}

	/** allocate a buffer for X elements of type T */
	void allocateElements(const int num) {
		_assertTrue(data == nullptr, "resizing is not yet supported. only one-time alloc works!");
		_assertNot0(num, "can not resize to '0' elements!");
		allocateBytes(num * sizeof(T));
		this->numElements = num;
	}

	/** begin working on a buffer fetched with allocateDynamic() */
	void beginSync() {
		syncObj = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);		Error::assertOK();
		glClientWaitSync(syncObj, GL_SYNC_FLUSH_COMMANDS_BIT, 1);		Error::assertOK();
	}

	/** stop working on a buffer fetched with allocateDynamic() */
	void endSync() {
		glDeleteSync(syncObj);											Error::assertOK();
	}

	/** dummy. dynamic-data does not need uploading */
	void upload() {
		throw Exception("not supported for dynamic VBOs");
	}

	void upload(const void* data, const uint32_t len, const bool sync) {
		_assertNot0(numElements, "allocate data first!");
		_assertEqual(numElements, len/sizeof(T), "upload size does not match allocated size");
		if (sync) {beginSync();}
		memcpy(this->data, data, len);
		if (sync) {endSync();}
	}

	void upload(const std::vector<T>& data, const bool sync) {
		upload(data.data(), data.size()*sizeof(T), sync);
	}

	/** number of contained elements */
	size_t size() const {
		return numElements;
	}

private:

	/** allocate a dynamic memory-region of the given size */
	void allocateBytes(const int size) {
		this->bind();
		const int flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_ARRAY_BUFFER, size, nullptr, flags);
		Error::assertOK();
		uint8_t* res = (uint8_t*) glMapBufferRange(GL_ARRAY_BUFFER, 0, size, flags);
		Error::assertOK();
		data = (T*) res;
	}

};



/** static vertex-buffer object */
template <typename T, int Mode> class VBOStatic : public VBOBase<T, Mode> {

protected:

	/** local (host) data */
	std::vector<T> data;

public:

	/** resize the local data */
	void resize(const size_t size) {
		data.resize(size);
	}

	std::vector<T>& getData() {
		return data;
	}

	decltype(data.begin()) begin() {return data.begin();}
	decltype(data.end()) end() {return data.end();}


	/** delete the local data. the GPU data [committed via upload()] is retained! */
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

	T& operator [] (const int idx) {return data[idx];}

	/** number of contained elements */
	size_t size() const {
		return data.size();
	}

public:

	/** send the STATIC data to the GPU (once) */
	void upload() {
		this->bind();
		glBufferData(Mode, this->data.size()*sizeof(T), this->data.data(), GL_STATIC_DRAW);
		Error::assertOK();
		this->unbind();
	}


};

#endif // VBO_H
