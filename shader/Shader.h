#ifndef SHADER_H
#define SHADER_H

#include <KLib/Assertions.h>

#include "../gl/gl.h"
#include "../misc/String.h"

#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "../math/Math.h"
#include "../misc/Error.h"
#include "../gl/UBO.h"
#include "../data/Data.h"

#include "ShaderBinary.h"
#include "ShaderVersion.h"

#include <set>
#include <sstream>
#include <iostream>

#include "Uniform.h"

class Shader {

protected:

	#include "../misc/BindOnce.h"
	#include "Uniforms.h"

	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	GLuint geomentryShaderID;
	GLuint programID;

public:

	friend class ShaderFactory;

	Shader() : vertexShaderID(0), fragmentShaderID(0), geomentryShaderID(0), programID(0) {
		;
	}

public:

	~Shader() {
		if (programID) {glDeleteProgram(programID); programID = 0;}
	}

	/** openGL handle */
	GLuint getProgram() const {
		return programID;
	}

	void loadVertexShader(const std::string& _code) {
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const std::string code = adjustCode(_code);
		Error::assertOK();
		debugCode(code);
		compile(vertexShaderID, code);
		Error::assertOK();
	}
	void loadVertexShader(const ShaderBinary& bin) {
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		Error::assertOK();
		glShaderBinary(1, &vertexShaderID, bin.format, bin.data.get(), bin.data.size());
		Error::assertOK();
	}


	void loadFragmentShader(const std::string& _code) {
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const std::string code = adjustCode(_code);
		Error::assertOK();
		debugCode(code);
		compile(fragmentShaderID, code);
		Error::assertOK();
	}
	void loadFragmentShader(const ShaderBinary& bin) {
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		Error::assertOK();
		glShaderBinary(1, &fragmentShaderID, bin.format, bin.data.get(), bin.data.size());
		Error::assertOK();
	}


	/** not supported by openGl ES! */
	void loadGeometryShader(const std::string& _code) {
		geomentryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		const std::string code = adjustCode(_code);
		Error::assertOK();
		debugCode(code);
		compile(geomentryShaderID, code);
		Error::assertOK();
	}
	void loadGeometryShader(const ShaderBinary& bin) {
		geomentryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		Error::assertOK();
		glShaderBinary(1, &geomentryShaderID, bin.format, bin.data.get(), bin.data.size());
		Error::assertOK();
	}

	void loadProgramBinary(const ProgramBinary& bin) {

		// create a new program
		programID = glCreateProgram();
		_assertNot0(programID, "error while creating program");

		// load
		glProgramBinary(programID, bin.format, bin.data.get(), bin.data.size());
		Error::assertOK();

	}

	ProgramBinary getProgramBinary() {

		// export
		static constexpr int bufSize = 128*1024;
		uint8_t buf[bufSize];
		GLsizei used;
		GLenum format;
		glGetProgramBinary(programID, bufSize, &used, &format, buf);
		Error::assertOK();

		// wrap
		ProgramBinary bin;
		bin.data = Data::wrapManaged(buf, used);
		bin.format = format;
		return bin;

	}

	void link() {

		// create a new program
		programID = glCreateProgram();
		_assertNot0(programID, "error while creating program");

		if (vertexShaderID)		{glAttachShader(programID, vertexShaderID);}
		if (fragmentShaderID)	{glAttachShader(programID, fragmentShaderID);}
		if (geomentryShaderID)	{glAttachShader(programID, geomentryShaderID);}
		Error::assertOK();

		glLinkProgram(programID);
		Error::assertOK();

		// Check the program
		GLint res;
		glGetProgramiv(programID, GL_LINK_STATUS, &res);

		//if (res == GL_FALSE) {
			int logLength;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0) {
				std::string log; log.resize(logLength-1); log.reserve(logLength);
				glGetProgramInfoLog(programID, logLength, NULL, (char*) log.data());
				std::cout << "log: " << log << std::endl;
			}

		//}

		if (res == GL_FALSE) {
			throw Exception("error while linking program");
		}

		cleanup();

		// fetch all active uniforms for this shader
		getAllActiveUniforms();

	}



	void bind() const {
		assertUnbound();
		glUseProgram(programID);
		Error::assertOK();
		setBound(programID);
	}

	void unbind() const {
		glUseProgram(0);
		Error::assertOK();
		setUnbound(programID);
	}

protected:

	std::string adjustCode(const std::string& str) {
		std::string copy = str;
		String::replace(copy, "{VERSION}", ShaderVersion::getGLSL());
		return copy;
	}

	void debugCode(const std::string& str) {

#if defined(WITH_VERBOSE)

		std::stringstream ss;

		int line = 1;
		ss << line << '\t';
		for (char c : str) {
			ss << c;
			if (c == '\n') {ss << ++line << '\t';}
		}

		ss << "\n\n\n";

		std::cout << ss.str() << std::endl;

#else

		(void) str;

#endif

	}

	/** ensure the ID is currently bound */
	inline void assertBound() const {
		_assertTrue(isBound(programID), "bind() the shader first");
	}

	/** ensure the ID is currently unbound */
	inline void assertUnbound() const {
		_assertFalse(isBound(programID), "shader is already bound!");
	}


	/** perform some cleanups */
	void cleanup() {

		glDeleteShader(vertexShaderID); vertexShaderID = 0;
		Error::assertOK();

		glDeleteShader(fragmentShaderID); fragmentShaderID = 0;
		Error::assertOK();

		glDeleteShader(geomentryShaderID); geomentryShaderID = 0;
		Error::assertOK();

	}



	/** compile the given code into the created shader */
	void compile(GLuint shaderID, const std::string& code) {

		GLint res;
		int logLength;

		// compile
		const char* ccode = code.c_str();
		glShaderSource(shaderID, 1, &ccode, NULL);
		Error::assertOK();
		glCompileShader(shaderID);
		Error::assertOK();

		// check
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &res);
		if (res == GL_FALSE) {
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
			std::string log; log.resize(logLength-1); log.reserve(logLength);
			glGetShaderInfoLog(shaderID, logLength, NULL, (char*) log.data());
			std::cout << "log: " << log << std::endl;
			throw "error while compiling shader";
		}

	}


};

#endif // SHADER_H
