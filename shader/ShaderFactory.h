#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <string>
#include <vector>

#include "Shader.h"
#include "memory.h"


class ShaderFactory {

private:

	std::vector<std::unique_ptr<Shader>> shader;
	std::string includePath;

public:

	/** set the include-path to use */
	void setIncludePath(const std::string& path) {
		this->includePath = path;
	}

	Shader* create(const std::string& fVertex, const std::string& fPixel) {
		Shader* s = new Shader();
		s->loadVertexShader(getCode(includePath + fVertex));
		s->loadFragmentShader(getCode(includePath + fPixel));
		s->link();
		shader.push_back(std::make_unique(s));
		return s;
	}

	/** load shader from GLSL code */
	Shader* createFromCode(const std::string& srcVertex, const std::string& srcPixel) {
		Shader* s = new Shader();
		s->loadVertexShader(srcVertex);
		s->loadFragmentShader(srcPixel);
		s->link();
		shader.push_back(std::make_unique(s));
		return s;
	}

	/** load shader from GLSL code */
	Shader* createFromCode(const std::string& srcVertex, const std::string& srcPixel, const std::string& srcGeo) {
		Shader* s = new Shader();
		s->loadVertexShader(srcVertex);
		s->loadFragmentShader(srcPixel);
		s->loadGeometryShader(srcGeo);		// not supported by opengl es
		s->link();
		shader.push_back(std::make_unique(s));
		return s;
	}

	std::string getCode(const std::string& filename) const {

		std::string code;
		std::ifstream file(filename.c_str(), std::ios::in);

		_assertFalse(file.bad(), "error while opening file " + filename);
		_assertTrue(file.is_open(), "error while opening file " + filename);

		std::string line;
		while(getline(file, line)) {
			if ("#include" == line.substr(0, 8)) {
				const std::string subFile = line.substr(9);
				code += getCode(includePath + subFile);
			} else {
				code += line + "\n";
			}
		}
		file.close();

		return code;

	}

};

#endif // SHADERFACTORY_H
