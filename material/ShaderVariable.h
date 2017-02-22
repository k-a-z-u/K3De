#ifndef SHADERVARIABLE_H
#define SHADERVARIABLE_H

#include <string>

class ShaderVariable {

public:

	enum class Mode {
		LOCAL,
		INBOUND,
		OUTBOUND,
		UNIFORM,
	};

	// https://developer.apple.com/library/content/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/BestPracticesforShaders/BestPracticesforShaders.html
	enum class Type {

		TEXTURE_2D,
		TEXTURE_2D_ARRAY,

		POS2,
		POS3,
		POS4,

		MAT3,
		MAT4,

		COLOR3,
		COLOR4,

		SHININESS,

		TIME,

	};

private:

	int location;
	Mode mode;
	Type type;
	std::string name;
	std::string add;

public:

	/** ctor */
	ShaderVariable(const int location, const Mode mode, const Type type, const std::string& name, const std::string& add="") :
		location(location), mode(mode), type(type), name(name), add(add) {
		;
	}

	/** ctor */
	ShaderVariable(const Mode mode, const Type type, const std::string& name, const std::string& add="") :
		location(-1), mode(mode), type(type), name(name), add(add) {
		;
	}

	/** local variable ctor */
	ShaderVariable(const Type type, const std::string& name, const std::string& add="") :
		location(-1), mode(Mode::LOCAL), type(type), name(name), add(add) {
		;
	}

	const std::string& getName() const {
		return name;
	}

	/** cast to string */
	operator std::string () const {
		return getDecl();
	}

	std::string getDecl() const {

		std::string decl;

		if (location != -1) {
			decl += "layout(location = " + std::to_string(location) + ") ";
		}

		switch (mode) {
			case Mode::LOCAL:			break;
			case Mode::INBOUND:			decl += "in "; break;
			case Mode::OUTBOUND:		decl += "out "; break;
			case Mode::UNIFORM:			decl += "uniform "; break;
		}

		switch (type) {
			case Type::TEXTURE_2D:			decl += "lowp sampler2D "; break;
			case Type::TEXTURE_2D_ARRAY:	decl += "lowp sampler2D "; break;

			case Type::POS2:				decl += "highp vec2 "; break;
			case Type::POS3:				decl += "highp vec3 "; break;
			case Type::POS4:				decl += "highp vec4 "; break;
			case Type::MAT3:				decl += "mediump mat3 "; break;
			case Type::MAT4:				decl += "mediump mat4 "; break;
			case Type::COLOR3:				decl += "lowp vec3 "; break;
			case Type::COLOR4:				decl += "lowp vec4 "; break;
			case Type::SHININESS:			decl += "lowp float "; break;
			case Type::TIME:				decl += "mediump float "; break;
		}

		decl += name + add;

		return decl;

	}

};

#endif // SHADERVARIABLE_H
