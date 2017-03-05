#ifndef SHADERCACHE_H
#define SHADERCACHE_H

#include "ShaderBinary.h"
#include <unordered_map>

class ShaderCache {

private:

	std::unordered_map<std::string, ProgramBinary> programs;
	std::unordered_map<std::string, ShaderBinary> shaders;

public:


	void addShader(const std::string& code, ShaderBinary& binary) {
		shaders[code] = std::move(binary);
	}

	void addProgram(const std::string& code, ProgramBinary& binary) {
		programs[code] = std::move(binary);
	}


	bool hasShader(const std::string& code) const {
		const auto& it = shaders.find(code);
		return (it != shaders.end());
	}

	bool hasProgram(const std::string& code) const {
		const auto& it = programs.find(code);
		return (it != programs.end());
	}


	const ShaderBinary& getShader(const std::string& code) const {
		const auto& it = shaders.find(code);
		if (it == shaders.end()) {throw Exception("shader not found");}
		return it->second;
	}

	const ProgramBinary& getProgram(const std::string& code) const {
		const auto& it = programs.find(code);
		if (it == programs.end()) {throw Exception("program not found");}
		return it->second;
	}


};

#endif // SHADERCACHE_H
