#ifndef VARTYPE_H
#define VARTYPE_H

#include <string>
#include <vector>
#include "../Exception.h"

enum class VarScope {
	LOCAL,
	INBOUND,
	OUTBOUND,
	UNIFORM,
};

enum class VarType {

	NONE,

	INT,

	FLOAT,

	VEC2,
	VEC3,
	VEC4,

	MAT4,
	MAT3,

	TEXTURE_2D,
	TEXTURE_2D_ARRAY,

};

enum class VarPrecission {

	NONE,
	LOW,
	MEDIUM,
	HIGH,

};



/** typed variable */
struct Var {

	/** the variable's scope */
	VarScope scope;

	/** the variable's type */
	VarType type;

	/** the variable's precision */
	VarPrecission prec = VarPrecission::MEDIUM;

	/** the variable's name */
	std::string name;


	/** fixed location */
	int location = -1;

	/** TODO? */
	bool used = false;



	/** empty ctor */
	Var() : type(VarType::NONE) {
		;
	}

	/** ctor */
	Var(VarScope scope, VarType type, VarPrecission prec, std::string name, int location = -1) : scope(scope), type(type), prec(prec), name(name), location(location) {
		;
	}

	/** get a randomly named variable, starting with a base-name */
	static Var getRandom(VarType type, const std::string& baseName, VarScope scope = VarScope::LOCAL, VarPrecission prec = VarPrecission::MEDIUM) {
		return Var(scope, type, prec, baseName + "_" + getRandomName());
	}

	/** mark as used */
	void setUsed() {
		this->used = true;
	}

	/** returns e.g. "vec2 abc" */
	std::string getDeclaration() const {

		std::string res;

		if (location >= 0) {
			res += "layout(location = " + std::to_string(location) + ") ";
		}

		switch (scope) {
			case VarScope::LOCAL:		break;
			case VarScope::INBOUND:		res += "in "; break;
			case VarScope::OUTBOUND:	res += "out "; break;
			case VarScope::UNIFORM:		res += "uniform "; break;
		}

		switch (prec) {
			case VarPrecission::NONE:	break;
			case VarPrecission::LOW:	res += "lowp "; break;
			case VarPrecission::MEDIUM:	res += "mediump "; break;
			case VarPrecission::HIGH:	res += "highp "; break;
		}

		switch(type) {
			case VarType::NONE:				throw Exception("unsupported variable type. code error");
			case VarType::INT:				res += "int "; break;
			case VarType::FLOAT:			res += "float "; break;
			case VarType::VEC2:				res += "vec2 "; break;
			case VarType::VEC3:				res += "vec3 "; break;
			case VarType::VEC4:				res += "vec4 "; break;
			case VarType::MAT3:				res += "mat3 "; break;
			case VarType::MAT4:				res += "mat4 "; break;
			case VarType::TEXTURE_2D:		res += "sampler2D "; break;
			case VarType::TEXTURE_2D_ARRAY:	res += "sampler2D "; break;
		}

		res += name + " ";
		return res;

	}

	/** get the assigned variable: "vec2 abc = code" */
	std::string getAssigned(const std::string& code) const {
		return code.empty() ? getDeclaration() : getDeclaration() + " = " + code;
	}


	/** generate a random filename */
	static std::string getRandomName() {

		static char chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};


		std::string res;
		for (int i = 0; i < 3; ++i) {
			const int idx = rand() % (26+10);
			res += chars[idx];
		}

		return res;

	}


	bool operator == (const Var& o) const {
		return this->name == o.name;
	}

};

namespace std {
	template <>
	struct hash<Var> {
		size_t operator () (const Var& v) const { return std::hash<std::string>{}(v.name); }
	};
}

struct VarStruct {

	std::string typeName;

	std::vector<Var> variables;

	/** ctor */
	VarStruct(const std::string& typeName, const std::vector<Var>& variables) : typeName(typeName), variables(variables) {;}

//	std::string get() const {
//		std::string res;
//		res += "struct " + typeName + "{\r\n";
//		for (const Var& v : variables) {
//			res += "\t" + v.getDeclaration() + ";\r\n";
//		}
//		res += "}";
//		return res;
//	}

	std::vector<std::string> get() const {
		std::vector<std::string> res;
		res.push_back("struct " + typeName + "{");
		for (const Var& v : variables) {
			res.push_back(v.getDeclaration() + ";");
		}
		res.push_back("};");
		return res;
	}

};

#define VERT_VERTEX_POS			"vertex"
#define VERT_VERTEX_COLOR		"color"
#define VERT_VERTEX_NORMAL		"normal"
#define VERT_VERTEX_TANGENT		"tangent"
#define VERT_VERTEX_UV			"texCoord"

#define FRAG_VERTEX_UV			"uv"
#define FRAG_VERTEX_SCREEN_POS	"vertex_S"
#define FRAG_VERTEX_WORLD_POS	"vertex_M"
#define FRAG_VERTEX_COLOR		"color_M"
#define FRAG_NORMAL_WORLD_POS	"normal_M"
#define FRAG_TANGENT_WORLD_POS	"tangent_M"
#define FRAG_SURFACE_TO_EYE		"surfaceToEye_M"
#define FRAG_SHADOW_MAP_COORD	"shadowMapCoord"

#define FRAG_TIME				"time"
#define FRAG_CAM_POS			"camPos"

#define FRAG_TEX_SHADOW_MAP		"texShadowMap[" + std::to_string(MAX_LIGHTS) + "]"

const Var VAR_VERT_IN_VERTEX_POS = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::HIGH, VERT_VERTEX_POS, 0);
const Var VAR_VERT_IN_VERTEX_NORMAL = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::MEDIUM, VERT_VERTEX_NORMAL, 1);
const Var VAR_VERT_IN_VERTEX_UV = Var(VarScope::INBOUND, VarType::VEC2, VarPrecission::MEDIUM, VERT_VERTEX_UV, 2);
const Var VAR_VERT_IN_VERTEX_TANGENT = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::MEDIUM, VERT_VERTEX_TANGENT, 3);
const Var VAR_VERT_IN_VERTEX_COLOR = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::LOW, VERT_VERTEX_COLOR, 4);

const Var VAR_UNIFORM_MAT_PVM = Var(VarScope::UNIFORM, VarType::MAT4, VarPrecission::MEDIUM, "PVM");
const Var VAR_UNIFORM_MAT_M = Var(VarScope::UNIFORM, VarType::MAT4, VarPrecission::MEDIUM, "M");
const Var VAR_UNIFORM_MAT_V = Var(VarScope::UNIFORM, VarType::MAT4, VarPrecission::MEDIUM, "V");
const Var VAR_UNIFORM_MAT_P = Var(VarScope::UNIFORM, VarType::MAT4, VarPrecission::MEDIUM, "P");
const Var VAR_UNIFORM_CAM_POS = Var(VarScope::UNIFORM, VarType::VEC3, VarPrecission::MEDIUM, "camPos");
const Var VAR_UNIFORM_TIME = Var(VarScope::UNIFORM, VarType::FLOAT, VarPrecission::MEDIUM, "time");
const Var VAR_UNIFORM_CLIP_Z = Var(VarScope::UNIFORM, VarType::VEC2, VarPrecission::MEDIUM, "clipZ");

const Var VAR_UNIFORM_TEX_SHADOW_MAP = Var(VarScope::UNIFORM, VarType::TEXTURE_2D, VarPrecission::LOW, FRAG_TEX_SHADOW_MAP);

const Var VAR_VERT_OUT_UV = Var(VarScope::OUTBOUND, VarType::VEC2, VarPrecission::MEDIUM, "uv");
const Var VAR_VERT_OUT_NORMAL = Var(VarScope::OUTBOUND, VarType::VEC3, VarPrecission::MEDIUM, "normal_M");
const Var VAR_VERT_OUT_TANGENT = Var(VarScope::OUTBOUND, VarType::VEC3, VarPrecission::MEDIUM, "tangent_M");
const Var VAR_VERT_OUT_VERTEX = Var(VarScope::OUTBOUND, VarType::VEC3, VarPrecission::MEDIUM, "vertex_M");
const Var VAR_VERT_OUT_VERTEX_SCREEN = Var(VarScope::OUTBOUND, VarType::VEC4, VarPrecission::MEDIUM, "vertex_S");
const Var VAR_VERT_OUT_COLOR = Var(VarScope::OUTBOUND, VarType::VEC4, VarPrecission::LOW, "color_M");
const Var VAR_VERT_OUT_SURFACE_TO_EYE = Var(VarScope::OUTBOUND, VarType::VEC3, VarPrecission::MEDIUM, "surfaceToEye_M");
const Var VAR_VERT_OUT_SHADOW_MAP_COORD = Var(VarScope::OUTBOUND, VarType::VEC4, VarPrecission::HIGH, "shadowMapCoord["+std::to_string(MAX_LIGHTS)+"]");	// TODO



const Var VAR_FRAG_IN_VERTEX_UV = Var(VarScope::INBOUND, VarType::VEC2, VarPrecission::MEDIUM, "uv");
const Var VAR_FRAG_IN_VERTEX_NORMAL = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::MEDIUM, "normal_M");
const Var VAR_FRAG_IN_VERTEX_TANGENT = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::MEDIUM, "tangent_M");
const Var VAR_FRAG_IN_VERTEX = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::MEDIUM, "vertex_M");
const Var VAR_FRAG_IN_VERTEX_SCREEN = Var(VarScope::INBOUND, VarType::VEC4, VarPrecission::MEDIUM, "vertex_S");
const Var VAR_FRAG_IN_VERTEX_COLOR = Var(VarScope::INBOUND, VarType::VEC4, VarPrecission::LOW, "color_M");
const Var VAR_FRAG_IN_SURFACE_TO_EYE = Var(VarScope::INBOUND, VarType::VEC3, VarPrecission::MEDIUM, "surfaceToEye_M");
const Var VAR_FRAG_IN_SHADOW_MAP_COORD = Var(VarScope::INBOUND, VarType::VEC4, VarPrecission::HIGH, "shadowMapCoord["+std::to_string(MAX_LIGHTS)+"]");	// TODO


const Var VAR_FRAG_OUT_COLOR = Var(VarScope::OUTBOUND, VarType::VEC4, VarPrecission::LOW, "color");


/** other */

const Var VAR_LIGHT_POS = Var(VarScope::LOCAL, VarType::VEC3, VarPrecission::MEDIUM, "pos");
//const Var VAR_LIGHT_POS_PAD = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "_padPos");
const Var VAR_LIGHT_COLOR = Var(VarScope::LOCAL, VarType::VEC3, VarPrecission::LOW, "color");
const Var VAR_LIGHT_COLOR_PAD = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "_padColor");
const Var VAR_LIGHT_IMPACT = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "impact");
const Var VAR_LIGHT_ATTEN_LIN = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "attenLin");
const Var VAR_LIGHT_ATTEN_QUAD = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "attenQuad");
const Var VAR_LIGHT_FLAGS = Var(VarScope::LOCAL, VarType::INT, VarPrecission::NONE, "flags");
const Var VAR_LIGHT_PV_SHADOW = Var(VarScope::LOCAL, VarType::MAT4, VarPrecission::LOW, "pvShadow");
const Var VAR_LIGHT_TYPE = Var(VarScope::LOCAL, VarType::INT, VarPrecission::NONE, "type");

const Var VAR_LIGHT_PAD1 = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "pad1");
const Var VAR_LIGHT_PAD2 = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "pad2");
const Var VAR_LIGHT_PAD3 = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "pad3");

/*
const VarStruct VAR_LIGHT_STRUCT = VarStruct("Light",
	{VAR_LIGHT_POS, VAR_LIGHT_POS_PAD,
	VAR_LIGHT_COLOR, VAR_LIGHT_COLOR_PAD,
	VAR_LIGHT_IMPACT,
	VAR_LIGHT_ATTEN_LIN, VAR_LIGHT_ATTEN_QUAD,
	VAR_LIGHT_FLAGS,
	VAR_LIGHT_PV_SHADOW}
);
*/

const VarStruct VAR_LIGHT_STRUCT = VarStruct("Light", {
	VAR_LIGHT_POS, VAR_LIGHT_PAD1,
	VAR_LIGHT_COLOR, VAR_LIGHT_PAD2,
	VAR_LIGHT_FLAGS, VAR_LIGHT_TYPE, VAR_LIGHT_ATTEN_LIN, VAR_LIGHT_ATTEN_QUAD,
	VAR_LIGHT_PV_SHADOW
});


const Var VAR_AMBIENT_COLOR = Var(VarScope::LOCAL, VarType::VEC3, VarPrecission::LOW, "color");
const Var VAR_AMBIENT_PAD1 = Var(VarScope::LOCAL, VarType::FLOAT, VarPrecission::LOW, "pad1");


const VarStruct VAR_AMBIENT_STRUCT = VarStruct("Ambient", {
	VAR_AMBIENT_COLOR, VAR_AMBIENT_PAD1
});









//frag.addVariable(Var(VarScope::UNIFORM, VarType::VEC2, VarPrecission::MEDIUM, FRAG_VERTEX_UV));
//frag.addVariable(Var(VarScope::UNIFORM, VarType::VEC4, VarPrecission::HIGH, FRAG_VERTEX_SCREEN_POS));	// ?
//frag.addVariable(Var(VarScope::UNIFORM, VarType::VEC3, VarPrecission::LOW, FRAG_VERTEX_WORLD_POS));





#endif // VARTYPE_H
