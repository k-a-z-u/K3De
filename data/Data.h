#ifndef DATA_H
#define DATA_H

#include <cstdint>

class Data {

private:

	uint8_t* _data = nullptr;
	size_t _len;
	bool _managed = false;

private:

	/** hidden ctor */
	Data(uint8_t* data, const size_t len, const bool managed) : _data(data), _len(len), _managed(managed) {;}



public:

	/** empty ctor */
	Data() : _data(nullptr), _managed(false) {;}

	/** dtor */
	~Data() {
		freeMe();
	}


	/** allow moving */
	Data(Data&& o) : _data(nullptr), _len(0), _managed(false) {
		this->_data = o._data;			o._data = nullptr;
		this->_len = o._len;			o._len = 0;
		this->_managed = o._managed;	o._managed = false;
	}

	/** no copy */
	Data(const Data& o) = delete;

	/** no copy assign */
	Data& operator = (const Data& o) = delete;

	/** move assignment */
	Data& operator = (Data&& o) {
		if (this != &o) {
			freeMe();
			this->_data = o._data;			o._data = nullptr;
			this->_len = o._len;			o._len = 0;
			this->_managed = o._managed;	o._managed = false;
		}
		return *this;
	}


	/** wrap the given data [unmanaged] */
	static Data wrapUnmanaged(uint8_t* data, const size_t size) {
		return Data(data, size, false);
	}

	/** wrap the given data [managed, free on destruction] */
	static Data wrapManaged(uint8_t* data, const size_t size) {
		return Data(data, size, true);
	}

	/** wrap the given data [managed, free on destruction] */
	static Data copy(uint8_t* data, const size_t size) {
		uint8_t* dst = (uint8_t*) malloc(size);
		memcpy(dst, data, size);
		return Data(dst, size, true);
	}

	/** allocate empty managed data */
	static Data alloc(const size_t size) {
		uint8_t* dst = (uint8_t*) malloc(size);
		return Data(dst, size, true);
	}

private:

	void freeMe() {

		if (_managed && _data) {
			free(_data);
			_data = nullptr;
			_len = 0;
		}

	}


public:

	/** get the underyling data */
	const uint8_t* get() const {return _data;}

	/** get the underyling data */
	uint8_t* get() {return _data;}

	/** get the underlying data's size */
	size_t size() const {return _len;}

};

#endif // DATA_H
