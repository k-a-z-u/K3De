#ifndef MATERIAL3COMPILER_H
#define MATERIAL3COMPILER_H

#include "Material3.h"
#include "../shader/ShaderVersion.h"
#include "../scene/Scene.h"

class Material3Compiler {

private:

	Material3* mat;

public:

	/** ctor */
	Material3Compiler(Material3* mat) : mat(mat) {

	}

	void compileShader(Scene* scene) {

		// get shader code
		const CodePart vert = getVertex();
		const CodePart frag = getFragment();

		std::cout << vert.asString(true) << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << frag.asString(true) << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;

		// create and compile the shader
		Shader* s = scene->getShaderFactory().createFromCode(vert.asString(), frag.asString());
		s->bind();

		// bind normal textures
		for (NamedTexture nt : mat->textures) {
			if (s->hasUniform(nt.name)) {
				const int idx = mat->bindTextures.add(nt.tex);
				s->setInt(nt.name, idx);
			}
		}

		//TODO("TODO: assign fixed texture units for the following textures? less binding/unbinding")

		// additionally bind the shadow textures?
		if (mat->getReceivesShadows() && scene->getShadowRenderer()) {
			for (int i = 0; i < MAX_LIGHTS; ++i) {
				const std::string name = "texShadowMap[" + std::to_string(i) + "]";
				if (s->hasUniform(name)) {
					const int idx = mat->bindTextures.add(scene->getShadowRenderer()->getShadowTexture(i));
					s->setInt(name, idx);
				}
			}
		}

		// additionally bind the water textures? [reflect/refract]
		if (scene->getWaterRenderer()) {
			if (s->hasUniform("texReflect")) {
				const int idx = mat->bindTextures.add(scene->getWaterRenderer()->getReflect());
				s->setInt("texReflect", idx);
			}
			if (s->hasUniform("texRefract")) {
				const int idx = mat->bindTextures.add(scene->getWaterRenderer()->getRefract());
				s->setInt("texRefract", idx);
			}
			if (s->hasUniform("texWaterDepth")) {
				const int idx = mat->bindTextures.add(scene->getWaterRenderer()->getDepth());
				s->setInt("texWaterDepth", idx);
			}
		}


		// assign to the material
		s->unbind();
		mat->shader = s;

	}

	/** generate code */
	CodePart getFragment() {


		CodePart frag;
		frag.header += getHeader();

		// base variables
		frag.addGlobalVariable(VAR_FRAG_IN_VERTEX_UV);
		frag.addGlobalVariable(VAR_FRAG_IN_VERTEX_NORMAL);
		frag.addGlobalVariable(VAR_FRAG_IN_VERTEX_TANGENT);
		frag.addGlobalVariable(VAR_FRAG_IN_VERTEX);
		frag.addGlobalVariable(VAR_FRAG_IN_VERTEX_SCREEN);
		frag.addGlobalVariable(VAR_FRAG_IN_VERTEX_COLOR);
		frag.addGlobalVariable(VAR_FRAG_IN_SURFACE_TO_EYE);

		// uniforms
		frag.addGlobalVariable(VAR_UNIFORM_CLIP_Z);
		frag.addGlobalVariable(VAR_UNIFORM_TIME);
		frag.addGlobalVariable(VAR_UNIFORM_TEX_SHADOW_MAP);	// TODO: move to shadowing?

		// output color
		frag.addGlobalVariable(VAR_FRAG_OUT_COLOR);

		// TODO better way?!
		frag.manualGlobals += VAR_AMBIENT_STRUCT.get();
		frag.manualGlobals += VAR_LIGHT_STRUCT.get();
		frag.manualGlobals += "layout (std140) uniform Lights { Ambient ambient; Light light["+std::to_string(MAX_LIGHTS)+"]; } lights;";


		// add all textures
		for (const NamedTexture& nt : mat->textures) {
			frag.addGlobalVariable(Var(VarScope::UNIFORM, VarType::TEXTURE_2D, VarPrecission::LOW, nt.name));
		}

		// main
		{

			// perform clipping?
			if (mat->clip) {
				frag += mat->clip->getFragment();
			}

			// initial color (no lighting)
			Var vColor = mat->color->getVar();
			frag += mat->color->getFragmentOnce();

			// discard transparent?
			frag.main += "if ("+vColor.name+".a < 0.01) {discard;}";

			// get the model's normal
			Var vNormal = mat->normal->getVar();
			frag += mat->normal->getFragment();

			// apply lighting
			Var vColorLighted = mat->lighting->getVar();
			frag += mat->lighting->getFragment(vColor, vNormal);

			// output color
			frag.main += CodeLine(VAR_FRAG_OUT_COLOR.name + " = " + vColorLighted.name).terminate();
			//frag.main += CodeLine(VAR_FRAG_OUT_COLOR.name + " = vec4(getShadowAmount(texShadowMap[0], 0))").terminate();		// DEBUG SHADOW TEXTURE

			frag.addUsage(VAR_FRAG_OUT_COLOR.name);

		} // end main

		return frag;

	}

	CodePart getVertex() {


		CodePart vert;
		vert.header += getHeader();

		// default variables
		vert.addGlobalVariable(VAR_VERT_IN_VERTEX_POS);
		vert.addGlobalVariable(VAR_VERT_IN_VERTEX_NORMAL);
		vert.addGlobalVariable(VAR_VERT_IN_VERTEX_UV);
		vert.addGlobalVariable(VAR_VERT_IN_VERTEX_TANGENT);
		vert.addGlobalVariable(VAR_VERT_IN_VERTEX_COLOR);

		vert.addGlobalVariable(VAR_UNIFORM_MAT_PVM);
		vert.addGlobalVariable(VAR_UNIFORM_MAT_M);
		vert.addGlobalVariable(VAR_UNIFORM_MAT_V);
		vert.addGlobalVariable(VAR_UNIFORM_MAT_P);
		vert.addGlobalVariable(VAR_UNIFORM_CAM_POS);

		vert.addGlobalVariable(VAR_VERT_OUT_UV);
		vert.addGlobalVariable(VAR_VERT_OUT_NORMAL);
		vert.addGlobalVariable(VAR_VERT_OUT_TANGENT);
		vert.addGlobalVariable(VAR_VERT_OUT_VERTEX);
		vert.addGlobalVariable(VAR_VERT_OUT_VERTEX_SCREEN);
		vert.addGlobalVariable(VAR_VERT_OUT_COLOR);
		vert.addGlobalVariable(VAR_VERT_OUT_SURFACE_TO_EYE);

		// TODO better way?!
		vert.manualGlobals += VAR_AMBIENT_STRUCT.get();
		vert.manualGlobals += VAR_LIGHT_STRUCT.get();
		vert.manualGlobals += "layout (std140) uniform Lights { Ambient ambient; Light light["+std::to_string(MAX_LIGHTS)+"]; } lights;";

		// main
		{

			vert.main += "gl_Position = PVM * vec4(vertex, 1.0);";				vert.addUsage("PVM"); vert.addUsage("vertex");

			//vert.main += "gl_Position = V*M * vec4(vertex, 1.0);";
			//vert.addUsage("V"); vert.addUsage("M"); vert.addUsage("vertex");

//			//const float a = atan2(x, y) / (2 * M_PI) + 0.5;
//			//const float b = std::asin(z) / M_PI + 0.5;
//			vert.main += "vec4 xxx = (V*M * vec4(vertex, 1.0)) / 10;";					vert.addUsage("V"); vert.addUsage("M"); vert.addUsage("vertex");
//			vert.main += "gl_Position.x = atan(xxx.x, xxx.z) / (2.0 * 3.14159) * 2.0;";
//			//vert.main += "gl_Position.y = atan(xxx.y, xxx.z) / (2.0 * 3.14159) * 2.0;";
//			//vert.main += "gl_Position.x = acos(xxx.x) / 3.14159 * 2.0;";
//			vert.main += "gl_Position.y = asin(xxx.y) / 3.14159 * 2.0;";
//			vert.main += "gl_Position.z = abs(xxx.z / 100);";
//			vert.main += "gl_Position.w = 1.0;";

			vert.main += "vertex_M = (M * vec4(vertex, 1.0)).xyz;";				vert.addUsage("vertex_M"); vert.addUsage("M"); vert.addUsage("vertex");

			if (!mat->textures.empty()) {
				vert.main += "uv = texCoord;";									vert.addUsage("uv"); vert.addUsage("texCoord");
			}

			if (true) {
				vert.main += "normal_M = normalize(mat3(M) * normal);";			vert.addUsage("normal_M"); vert.addUsage("M"); vert.addUsage("normal");
				vert.main += "surfaceToEye_M = normalize(camPos - vertex_M);";	vert.addUsage("surfaceToEye_M"); vert.addUsage("camPos"); vert.addUsage("vertex_M");
			}

			vert += mat->color->getVertexOnce();

			// lighting stuff
			vert += mat->lighting->getVertex();

			// normal stuff
			vert += mat->normal->getVertex();

			// shadow?
			if (true) {


			}

		} // end main

		return vert;

	}

private:

	std::string getHeader() const {

		std::string head;
		head += ShaderVersion::getGLSL();
		return head;

	}



};

#endif // MATERIAL3COMPILER_H
