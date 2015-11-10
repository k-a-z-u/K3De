#ifndef SHADER_H
#define SHADER_H

#include <KLib/Assertions.h>

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <unordered_map>

#include "../math/Math.h"
#include "../misc/Error.h"
#include "../gl/UBO.h"

class Shader {

private:

	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	GLuint geomentryShaderID;
	GLuint programID;
	mutable std::unordered_map<std::string, int> uniforms;


public:

	friend class ShaderFactory;

	Shader() : vertexShaderID(0), fragmentShaderID(0), geomentryShaderID(0), programID(0) {
		;
	}

public:

	~Shader() {
		if (programID) {glDeleteProgram(programID); programID = 0;}
	}

	void loadVertexShader(const std::string& code) {
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		Error::assertOK();
		std::cout << "----------------------------------" << std::endl;
		std::cout << code << std::endl;
		compile(vertexShaderID, code);
	}

	void loadFragmentShader(const std::string& code) {
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		Error::assertOK();
		std::cout << "----------------------------------" << std::endl;
		std::cout << code << std::endl;
		compile(fragmentShaderID, code);
	}

	void loadGeometryShader(const std::string& code) {
		geomentryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		Error::assertOK();
		std::cout << "----------------------------------" << std::endl;
		std::cout << code << std::endl;
		compile(geomentryShaderID, code);
	}

	void link() {

		// create a new program
		programID = glCreateProgram();
		_assertNot0(programID, "error while creating program");

		if (vertexShaderID)		{glAttachShader(programID, vertexShaderID);}
		if (fragmentShaderID)	{glAttachShader(programID, fragmentShaderID);}
		if (geomentryShaderID)	{glAttachShader(programID, geomentryShaderID);}
		glLinkProgram(programID);
		Error::assertOK();

		// Check the program
		GLint res;
		glGetProgramiv(programID, GL_LINK_STATUS, &res);
		if (res == GL_FALSE) {
			int logLength;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
			std::string log; log.resize(logLength-1); log.reserve(logLength);
			glGetProgramInfoLog(programID, logLength, NULL, (char*) log.data());
			std::cout << "log: " << log << std::endl;
			throw "error while linking program";
		}

		cleanup();

	}

	void bind() const {
		glUseProgram(programID);
		Error::assertOK();
	}

	void unbind() const {
		glUseProgram(0);
		Error::assertOK();
	}

	GLint getCachedUniformLocation(const std::string& name) const {
		auto it = uniforms.find(name);
		if (it == uniforms.end()) {
			const GLint id = glGetUniformLocation(programID, name.c_str());
			Error::assertOK();
			uniforms[name] = id;
			return id;
		} else {
			return it->second;
		}
	}

	/** get the ID for a uniform named "name" */
	GLuint getUniformLocation(const std::string& name) const {
		GLint id = getCachedUniformLocation(name);
		if (id == -1) {throw "uniform not found: " + name;}
		return id;
	}

	/** does the program have a uniform with the given name? */
	bool hasUniform(const std::string& name) const {
		return -1 != getCachedUniformLocation(name);
	}

	/** des the programe have a uniform-block with the given name? */
	bool hasUniformBlock(const std::string& name) const {
		return GL_INVALID_INDEX != glGetUniformBlockIndex(programID, name.c_str());
	}

	void setInt(const std::string& name, const int i) const {
		bind();
		glUniform1i(getUniformLocation(name), i);
		Error::assertOK();
	}

	void setFloat(const std::string& name, const float val) const {
		bind();
		glUniform1f(getUniformLocation(name), val);
		Error::assertOK();
	}

	void setVector(const std::string& name, const Vec2& vec) const {
		bind();
		const GLuint id = getUniformLocation(name);
		glUniform2f(id, vec.x, vec.y);
		Error::assertOK();
	}

	void setVector(const std::string& name, const Vec3& vec) const {
		bind();
		glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
		Error::assertOK();
	}

	void setVar(const std::string& name, UBO& ubo) {
		bind();
		const GLuint idx = glGetUniformBlockIndex(programID, name.c_str());
		glUniformBlockBinding(programID, idx, 0);
		Error::assertOK();
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo.getID());
		Error::assertOK();
	}

	void setMatrix(const std::string& name, const Mat4& mat) const {
		bind();
		const GLuint id = getUniformLocation(name);
		//glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0][0]);
		glUniformMatrix4fv(id, 1, GL_TRUE, mat.data());		// GL_TRUE = transpose from row-major to column-major!
		Error::assertOK();
	}

private:

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
