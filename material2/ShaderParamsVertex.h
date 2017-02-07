#ifndef SHADERPARAMSVERTEX_H
#define SHADERPARAMSVERTEX_H


#include "ShaderParams.h"
#include "ProgramVariables.h"

class ShaderParamsVertex : public ShaderParams {

public:

	/** ctor */
	ShaderParamsVertex() {

		addVariable("vertex",	"layout(location = 0) in vec3 " VERT_VERTEX_POS ";");
		addVariable("normal",	"layout(location = 1) in vec3 " VERT_VERTEX_NORMAL ";");
		addVariable("texCoord",	"layout(location = 2) in vec2 " VERT_VERTEX_UV ";");
		addVariable("tangent",	"layout(location = 3) in vec3 " VERT_VERTEX_TANGENT ";");
		addVariable("color",	"layout(location = 4) in vec4 " VERT_VERTEX_COLOR ";");

		addVariable("PVM",		"uniform mat4 PVM;");
		addVariable("M",		"uniform mat4 M;");
		addVariable("V",		"uniform mat4 V;");
		addVariable("P",		"uniform mat4 P;");
		addVariable("camPos",	"uniform vec3 camPos;");

		addVariable(FRAG_VERTEX_UV,			"out vec2 " FRAG_VERTEX_UV ";");
		addVariable(FRAG_NORMAL_WORLD_POS,	"out vec3 " FRAG_NORMAL_WORLD_POS ";");
		addVariable(FRAG_TANGENT_WORLD_POS,	"out vec3 " FRAG_TANGENT_WORLD_POS ";");
		addVariable(FRAG_VERTEX_WORLD_POS,	"out vec3 " FRAG_VERTEX_WORLD_POS ";");
		addVariable(FRAG_SURFACE_TO_EYE,	"out vec3 " FRAG_SURFACE_TO_EYE ";");
		addVariable(FRAG_VERTEX_COLOR,		"out vec4 " FRAG_VERTEX_COLOR ";");

	}

	virtual std::string getHeader() const {
		return ShaderParams::getHeader() +
				"precision mediump float;\n";
	}



	std::string getCode() const {

		return
			section("vertex shader '" + ShaderParams::name + "'") +
			getHeader() + "\r\n\r\n" +
			getVariables() + "\r\n\r\n" +
			getFunctions() + "\r\n\r\n" +
			getMain();

	}

};

#endif // SHADERPARAMSVERTEX_H
