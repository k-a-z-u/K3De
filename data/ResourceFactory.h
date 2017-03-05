#ifndef RESOURCEFACTORY_H
#define RESOURCEFACTORY_H

#include "Resource.h"
#include "Archive.h"
#include <vector>


/**
 * load resources from filesystem or archive files
 * depending on added archives and paths
 */
class ResourceFactory {

private:

	const char* NAME = "ResFac";

	/** all archives to scan for files */
	std::vector<Archive*> archives;

	/** available include paths */
	std::vector<std::string> includePaths;

	/** hidden ctor. singleton */
	ResourceFactory() {;}

public:

	/** singleton */
	static ResourceFactory& get() {
		static ResourceFactory instance;
		return instance;
	}

	/** dtor */
	~ResourceFactory() {
		for (Archive* a : archives) {delete a;}
	}

	/** add a new archive to search for resources */
	void addArchive(const std::string& fileName, const size_t offset = 0) {
		archives.push_back(new Archive(fileName, offset));
	}

	/** add a new include path */
	void addIncludePath(const std::string& path) {
		includePaths.push_back(path);
	}

	/** get the given resource's content */
	Data get(const Resource& r) {

		Debug(NAME, "loading resource " + r.getName());

		// try the name itself as absolute filename
		if (exists(r.getName())) {return readFile(r.getName());}

		// search all include paths
		for (const std::string path : includePaths) {
			const std::string full = path + r.getName();
			if (exists(full)) {return readFile(full);}
		}

		// search within configured archives
		for (Archive* a : archives) {
			if (a->hasFile(r.getName())) {return a->get(r.getName());}
		}

		// err
		throw Exception("could not find resource: " + r.getName());

	}


private:

	inline bool exists (const std::string& name) {
		std::ifstream f(name.c_str(), std::ios::in | std::ios::binary);
		return f.good();
	}

	static Data readFile(const std::string& name) {

		std::ifstream in(name, std::ios::in | std::ios::binary);
		if (!in) {throw Exception("error while opening: " + name);}

		in.seekg(0, std::ios::end);
		const size_t size = in.tellg();
		Data data = Data::alloc(size);
		in.seekg(0, std::ios::beg);
		in.read((char*)data.get(), data.size());
		in.close();

		return data;

	}


};

#endif // RESOURCEFACTORY_H
