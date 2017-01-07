#ifndef SHADERCOMPILER_H
#define SHADERCOMPILER_H

class Material;
class Mat;
class Shader;

class ShaderCompiler {

private:

	/** the material to render */
	Material* mat;

	/** the scene to render within */
	Scene* scene;

public:

	/** compile a shader to render the given material within the given scene */
	static Shader* compileShader(Material* mat, Scene* scene);

private:

	/** hidden ctor */
	ShaderCompiler(Material* mat, Scene* scene);

	/** get the vertex-shader-code */
	std::string getVertexShader();

	/** get the fragment-shader-code */
	std::string getFragmentShader();

	/** get the geometry-shader-code */
	std::string getGeometryShader();

	/** get the shader version to use */
	std::string getShaderVersion();

};

#include "Material.h"
#include "../shader/Shader.h"

ShaderCompiler::ShaderCompiler(Material* mat, Scene* scene) : mat(mat), scene(scene) {
	;
}

std::string ShaderCompiler::getShaderVersion() {

	todo("fetch shader version from gpu");
	// http://stackoverflow.com/questions/27407774/get-supported-glsl-versions

	//return "#version 330 core\n\n";	// 3.3 GL
	//return "#version 130\n\n";			// 3.0 GL

	//shader += "#version 100\n\n";			// 1.00 ES
	return "#version 300 es\n\n precision mediump float;\nprecision lowp sampler2DArray;\n";		// 3.00 ES

}

std::string ShaderCompiler::getVertexShader() {

	std::string shader;
	shader += getShaderVersion();

	shader += "layout(location = 0) in vec3 vertex;\n";
	shader += "layout(location = 1) in vec3 normal;\n";
	shader += "layout(location = 2) in vec2 texCoord;\n";
	shader += "layout(location = 3) in vec3 tangent;\n";
	shader += "layout(location = 4) in vec4 color;\n";

	shader += "uniform mat4 PVM;\n";
	shader += "uniform mat4 M;\n";
	shader += "uniform mat4 V;\n";
	shader += "uniform mat4 P;\n";
	shader += "uniform vec3 camPos;\n";

	shader += "out vec2 uv;\n";
	shader += "out vec3 normal_M;\n";
	shader += "out vec3 tangent_M;\n";
	shader += "out vec3 vertex_M;\n";
	shader += "out vec3 eye_M;\n";
	shader += "out vec4 color_M;\n";

	if (mat->getReceivesShadows()) {
		shader +=
			#include "inc/shadowVertex.glsl"
		;
	}

	if (mat->usesNormalMap()) {
		shader +=
			#include "inc/rotationMatrix.glsl"
		;
		shader +=
			#include "inc/normalMapVertex.glsl"
		;
	}

	shader += "\nvoid main() {\n";

	/** the projection of input vertex */
	shader += "\tgl_Position = PVM * vec4(vertex, 1);\n";

	/** texture coordinates */
	shader += "\tuv = texCoord;\n";

	/** the vertex in wold-space */
	shader += "\tvertex_M = (M * vec4(vertex, 1)).xyz;\n";

	/** the vertex's normal in world-space */
	shader += "\tnormal_M = (mat3(M) * normal);\n";		// normals are NOT translated (only scaled and rotated) -> 3x3 matrix suffices

	/** the vertex's tangent in world-space */
	shader += "\ttangent_M = (mat3(M) * tangent);\n";		// tangents are NOT translated (only scaled and rotated) -> 3x3 matrix suffices

	/** the eye-vector in world-space */
	shader += "\teye_M = normalize(camPos - vertex_M);\n";

	/** the eye-vector in world-space */
	shader += "\tcolor_M = color;\n";

	/** the matrix to rotate a bump-map-normal into the vertex's normal */
	if (mat->usesNormalMap()) {
		shader += "\tnormalRotMat = getNormalRotationMatrix(normal_M, tangent_M);\n";
	}

	/** calculate parameters needed to map shadows onto the object */
	if (mat->getReceivesShadows()) {
		shader += "\tcalcShadowParams();\n";
	}

	shader += "}\n";

	return shader;

}

std::string ShaderCompiler::getFragmentShader() {

	// sanity checks
	if (!mat->getAmbient()) {throw "ambient material missing!";}

	std::string shader;
	shader += getShaderVersion();


	shader += "in vec2 uv;\n";

	shader += "in vec3 vertex_M;\n";
	shader += "in vec3 normal_M;\n";
	shader += "in vec3 tangent_M;\n";
	shader += "in vec3 eye_M;\n";

	shader +=
		#include "inc/lightFragment.glsl"
	;

	if (mat->getReceivesShadows()) {
		_assertNotNull(scene->getShadowRenderer(), "scene does not have a valid ShadowRenderer");
		shader += scene->getShadowRenderer()->getShadowAmountCalculationGLSL();
	}

	// add ambient parts to the header
	shader += mat->getAmbient()->getHeaderCode();

	// add normal-map parts to the header
	if (mat->usesNormalMap()) {
		shader +=
			#include "inc/normalMapFragment.glsl"
		;
		shader += mat->getBumpMap()->getHeaderCode();
	}

	// add specular parts to the header
	if (mat->getSpecular()) {
		shader += mat->getSpecular()->getHeaderCode();
	}

	// add output parts to the header
	shader += "out vec4 color;\n";

	if (mat->usesClipping()) {
		shader +=
			#include "inc/clipFragment.glsl"
		;
	}

	// start the main
	shader += "\nvoid main() {\n";

	// clip vertices and exit if necessary
	if (mat->usesClipping()) {shader += "\tdoClip();\n\n";}

	// ambient color
	shader += mat->getAmbient()->getMainCode();

	// discard (almost) transparent pixels?
	shader += "\tif (ambient.a < 0.01) {discard;}\n";

	// apply shadow
	if (mat->getReceivesShadows()) {
		shader += "\tambient.rgb *= getShadowAmount();\n";
	}

	if (mat->usesLighting()) {

		// calculate fixed light stuff (same for every light)
		if (mat->usesNormalMap()) {
			shader += "\tvec3 bumpMapNormal = " + mat->getBumpMap()->getMainCode(); //getNormalFromBumMap(texNormals, uv);\n";
			shader += "\tvec3 N = normalize(normalRotMat * bumpMapNormal);\n";		// bump-mapped normal
		} else {
			shader += "\tvec3 N = normalize(normal_M);\n";							// simple normal
		}

		shader += "\tvec3 E = normalize(eye_M);\n";

		// calculate variable light stuff (different for every light)
		shader += "\tvec3 lightPos_M = getLightPos(0);\n";
		shader += "\tvec3 lightColor = getLightColor(0);\n";

		shader += "\tvec3 L = normalize(lightPos_M - vertex_M);\n";
		shader += "\tfloat theta = clamp( dot(N, L), 0.0f, 1.0f);\n";

		// use specular lighting?
		if (mat->getSpecular()) {
			shader += "\tvec3 R = reflect(-L, N);\n";
			shader += "\tfloat alpha = clamp( dot(E, R), 0.0f, 1.0f);\n";
			shader += mat->getSpecular()->getMainCode();
		}

		// calculate the final color
		shader += "\tcolor.rgb =\n";

		// self-illuminated ambient
		//shader += "\t\t(ambient.rgb * 0.1)";

		// diffuse color
		shader += "\t\t(ambient.rgb * (lightColor * theta) * 1.0)";

		// specular color
		if (mat->getSpecular()) {
			shader += "+\n\t\t(specularColor * pow(alpha, specularShininess))";
		}

		// done
		shader += ";\n\tcolor.a = ambient.a;\n";

	} else {

		shader += "\tcolor = ambient;\n";

	}

	shader += "}";

	return shader;

}

std::string ShaderCompiler::getGeometryShader() {
	throw "TODO";

	//		std::string geo = R"(

	//					#version 330 core

	//					uniform mat4 M;
	//					uniform mat4 V;
	//					uniform mat4 P;

	//					layout (triangles) in;
	//					layout (triangle_strip, max_vertices = 3) out;

	//					in      vec3  normal_M[];
	//					//float normal_scale = 0.5;

	//					void main() {

	//						int i;

	//						for (i = 0; i < gl_in.length(); ++i) {
	//							gl_Position = gl_in[i].gl_Position;
	//							EmitVertex();
	//						}

	//						gl_Position = gl_in[i].gl_Position;

	//						EndPrimitive();

	//			//			vec4 v0     = vec4(0);//gl_in[0].gl_Position;
	//			//			vec4 v1     = vec4(1);//gl_in[0].gl_Position; //v0 + vec4(normal_M[0] * normal_scale, 0);
	//			//			vec4 v2     = vec4(2);

	//			//			gl_Position = P*V*M * v0;	EmitVertex();
	//			//			gl_Position = P*V*M * v1;	EmitVertex();
	//			//			gl_Position = P*V*M * v2;	EmitVertex();

	//			//			EndPrimitive();

	//			//			// we simply transform and emit the incoming vertex - this is v0 of our
	//			//			// line segment

	//			//			gl_Position = P*V*M * v0;
	//			//			EmitVertex();

	//			//			// we calculate v1 of our line segment
	//			//
	//			//			gl_Position = P*V*M * v1;
	//			//			EmitVertex();

	//			//			EndPrimitive();
	//					}
	//				)";

}

#include "../scene/Scene.h"

/** get a compiled shader for the material from the ctor */
Shader* ShaderCompiler::compileShader(Material* mat, Scene* scene) {

	_assertNotNull(mat, "Material is null");
	_assertNotNull(scene, "Scene is null");

	ShaderCompiler sc(mat, scene);
	const std::string codeVertex = sc.getVertexShader();
	const std::string codeFragment = sc.getFragmentShader();
	//const std::string codeGeometry = sc.getGeometryShader();
	Shader* s = scene->getShaderFactory().createFromCode(codeVertex, codeFragment);
	s->bind();

	ShaderState st;
	mat->getAmbient()->configureShader(s, st);
	if (mat->getBumpMap())		{mat->getBumpMap()->configureShader(s, st);}
	if (mat->getSpecular())		{mat->getSpecular()->configureShader(s, st);}
	if (mat->getReceivesShadows() && scene->getShadowRenderer()) {
		s->setInt("texShadowMap", st.nextFreeTextureSlot);
		mat->textures.set(st.nextFreeTextureSlot, scene->getShadowRenderer()->getShadowTexture());
	}

	return s;

}



#endif // SHADERCOMPILER_H
