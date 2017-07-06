#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "../gl/gl.h"
#include "Shader.h"
#include "../textures/Texture.h"

class ComputeShader : public Shader {

private:

public:


	void loadComputeShader(const std::string& _code) {
		vertexShaderID = glCreateShader(GL_COMPUTE_SHADER);
		const std::string code = adjustCode(_code);
		Error::assertOK();
		debugCode(code);
		compile(vertexShaderID, code);
		Error::assertOK();
	}

	/** bind the given texture to the compute shader. usual format: GL_RGBA8 */
	void bindTexture(const std::string& uniformName, Texture* tex, const GLuint location, const GLenum format) {
		const GLuint level = 0;
		this->setInt(uniformName, location);
		//const GLenum format = GL_RGBA8; //GL_RGBA32F;
		if (tex->getFormat() != format) {std::cerr << "warning: incompatible texture format" << std::endl;}
		glBindImageTexture(location, tex->getID(), level, GL_FALSE, 0, GL_READ_WRITE, format);
		Error::assertOK();
	}

	void dispatch(const GLuint xGroups, const GLuint yGroups, const GLuint zGroups) {

		glDispatchCompute(xGroups, yGroups, zGroups);
		Error::assertOK();

	}



};

#endif // COMPUTESHADER_H
