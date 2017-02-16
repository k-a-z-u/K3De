#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include "Data.h"
#include "Archive.h"
#include <fstream>
#include "../Exception.h"

class Resource {

private:

	std::string name;

public:

	/** ctor */
	Resource(const std::string name) : name(name) {
		;
	}

	const std::string& getName() const {
		return name;
	}


};

#endif // RESOURCE_H
