#ifndef SHADERPARAMSFRAGMENT_H
#define SHADERPARAMSFRAGMENT_H

#include "ShaderParams.h"
#include "ProgramVariables.h"

class ShaderParamsFragment : public ShaderParams {

public:

	/** ctor */
	ShaderParamsFragment() {

//		// input from vertex
//		addVariable(FRAG_VERTEX_UV,			"in vec2 " FRAG_VERTEX_UV ";");
//		addVariable(FRAG_VERTEX_SCREEN_POS,	"in vec4 " FRAG_VERTEX_SCREEN_POS ";");
//		addVariable(FRAG_VERTEX_WORLD_POS,	"in vec3 " FRAG_VERTEX_WORLD_POS ";");
//		addVariable(FRAG_VERTEX_COLOR,		"in vec4 " FRAG_VERTEX_COLOR ";");
//		addVariable(FRAG_NORMAL_WORLD_POS,	"in vec3 " FRAG_NORMAL_WORLD_POS ";");
//		addVariable(FRAG_TANGENT_WORLD_POS,	"in vec3 " FRAG_TANGENT_WORLD_POS ";");
//		addVariable(FRAG_SURFACE_TO_EYE,	"in vec3 " FRAG_SURFACE_TO_EYE ";");

//		// input from uniforms
//		addVariable(FRAG_CAM_POS,			"uniform vec3 " FRAG_CAM_POS ";");
//		addVariable(FRAG_TIME,				"uniform float " FRAG_TIME ";");

//		// output
//		addVariable("color",				"out vec4 color;", 1);

		// input from vertex
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS2, FRAG_VERTEX_UV));
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS4, FRAG_VERTEX_SCREEN_POS));
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS3, FRAG_VERTEX_WORLD_POS));
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS4, FRAG_VERTEX_COLOR));
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS3, FRAG_NORMAL_WORLD_POS));
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS3, FRAG_TANGENT_WORLD_POS));
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS3, FRAG_SURFACE_TO_EYE));
		addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::POS4, FRAG_SHADOW_MAP_COORD, "[" + std::to_string(MAX_LIGHTS) + "]"));


		// input from uniforms
		addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::POS3, FRAG_CAM_POS));
		addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TIME, FRAG_TIME));

		// output
		addVariable(ShaderVariable(ShaderVariable::Mode::OUTBOUND, ShaderVariable::Type::COLOR4, "color"));
		usedVariable("color");

	}

protected:

	std::string getHeader() const {
		return ShaderParams::getHeader() +
				"precision mediump float;\n" +
				"precision mediump sampler2D;\n" +
				"precision mediump sampler2DArray;\n";
	}

public:

	std::string getCode() const {

		return
			section("fragment shader '" + ShaderParams::name + "'") +
			getHeader() + "\r\n\r\n" +
			getVariables() + "\r\n\r\n" +
			getFunctions() + "\r\n\r\n" +
			getMain();

	}

};

#endif // SHADERPARAMSFRAGMENT_H
