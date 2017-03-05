#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <string>
#include <vector>

#include "Shader.h"
#include "memory.h"

#include "ShaderCache.h"
#include "../data/Resource.h"
#include "../data/ResourceFactory.h"

class ShaderFactory {

private:

	std::vector<std::unique_ptr<Shader>> shader;
	std::string includePath;

	ShaderCache cache;

public:

	/** set the include-path to use */
	void setIncludePath(const std::string& path) {
		this->includePath = path;
	}

	Shader* create(const Resource& vertex, const Resource& fragment) {
		const Data dVertex = ResourceFactory::get().get(vertex);
		const Data dFragment = ResourceFactory::get().get(fragment);
		const std::string sVertex = std::string((const char*)dVertex.get(), dVertex.size());
		const std::string sFragment = std::string((const char*)dFragment.get(), dFragment.size());
		Shader* s = new Shader();
		s->loadVertexShader(sVertex);
		s->loadFragmentShader(sFragment);
		s->link();
		shader.push_back(std::make_unique(s));
		return s;
	}

	/** load shader from GLSL code */
	Shader* createFromCode(const std::string& srcVertex, const std::string& srcPixel, const std::string& srcGeo = "") {

		Shader* s = new Shader();

		const std::string all = srcVertex + srcPixel + srcGeo;
		if (cache.hasProgram(all)) {

			s->loadProgramBinary(cache.getProgram(all));

		} else {

			if (cache.hasShader(srcVertex)) {
				s->loadVertexShader(cache.getShader(srcVertex));
			} else {
				s->loadVertexShader(srcVertex);
			}

			if (cache.hasShader(srcVertex)) {
				s->loadFragmentShader(cache.getShader(srcPixel));
			} else {
				s->loadFragmentShader(srcPixel);
			}

			s->link();
			//ProgramBinary bin = s->getProgramBinary();
			//cache.addProgram(all, bin);

		}

		shader.push_back(std::make_unique(s));

		return s;
	}

//	std::string getCode(const std::string& filename) const {

//		std::string code;
//		std::ifstream file(filename.c_str(), std::ios::in);

//		_assertFalse(file.bad(), "error while opening file " + filename);
//		_assertTrue(file.is_open(), "error while opening file " + filename);

//		std::string line;
//		while(getline(file, line)) {
//			if ("#include" == line.substr(0, 8)) {
//				const std::string subFile = line.substr(9);
//				code += getCode(includePath + subFile);
//			} else {
//				code += line + "\n";
//			}
//		}
//		file.close();

//		return code;

//	}

};

#endif // SHADERFACTORY_H
