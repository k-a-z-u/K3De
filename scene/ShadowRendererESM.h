#ifndef SHADOWRENDERERESM_H
#define SHADOWRENDERERESM_H

#include "ShadowRenderer.h"
#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"
#include "../fx/TextureFX.h"

class Scene;

class ShadowRendererESM : public ShadowRenderer {

private:

	friend class Scene;

	int texW;
	int texH;

	Framebuffer fbShadows;
	Renderbuffer rbShadows;
	Texture* texShadows;
	Texture* texShadowsBlur;
	Shader* sShadowGen;

	Shader* sBlurH;
	Shader* sBlurV;

	Scene* scene;
	TextureFX fx;

	const float exp = 40;

public:

	ShadowRendererESM(Scene* scene);

	/** update the shadow texture */
	void update();

	/** get the rendered shadow texture */
	Texture* getShadowTexture() {return texShadows;}

	/** glsl code to determine the amount of shadowing of the current fragment */
	std::string getShadowAmountCalculationGLSL();

private:

	/** blur the shadow map */
	void blur();

	/** generate GLSL code for horizontal or vertical bluring */
	std::string glslBlur(const int size, const int texSize, const int dir);

	/** generate GLSL vertex-shader-code for bluring */
	std::string glslBlurVertex();

	/** generate GLSL code for horizontal bluring */
	std::string glslBlurH(const int size);

	/** generate GLSL code for vertical bluring */
	std::string glslBlurV(const int size);

	/** generate GLSL code for the shadow-map fragment shader */
	std::string glslShadowMapFragment();

	/** generate GLSL code for the shadow-map vertex shader */
	std::string glslShadowMapVertex();

};

#include "Scene.h"
#include "../Engine.h"

ShadowRendererESM::ShadowRendererESM(Scene* scene) {

	this->scene = scene;
	texW = 512;// Engine::get()->getSettings().screen.width/2;
	texH = 512;// Engine::get()->getSettings().screen.height/2;

	// one channel (red) 32-bit float texture
	texShadows = scene->getTextureFactory().createRenderTexture(texW, texH, GL_R32F);
	texShadows->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texShadows->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
	//texShadows->setAnisotropic(16);

	// one channel (red) 32-bit float texture
	texShadowsBlur = scene->getTextureFactory().createRenderTexture(texW, texH, GL_R32F);
	texShadowsBlur->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texShadowsBlur->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
	//texShadowsBlur->setAnisotropic(16);

	fbShadows.attachRenderbufferDepth(&rbShadows, texW, texH);
	fbShadows.attachTextureColor(0, texShadows);

	// create the shader rendering the shadow-map
	sShadowGen = scene->getShaderFactory().createFromCode(glslShadowMapVertex(), glslShadowMapFragment());

	// create the shaders for hor and ver bluring
	sBlurH = scene->getShaderFactory().createFromCode(glslBlurVertex(), glslBlurH(5));
	sBlurV = scene->getShaderFactory().createFromCode(glslBlurVertex(), glslBlurV(5));

}


void ShadowRendererESM::update() {

	const Vec4 oldClearColor = scene->getClearColor();
	scene->setClearColor( Vec4(1,1,1,1) );

	scene->getCamera().push();

	scene->getCamera().setScreenSize(texW, texH);
	//glCullFace(GL_FRONT);
	scene->setOverwriteShader(sShadowGen);

	scene->getCamera().setPosition(scene->getLight(0).getPosition());
	const float x = 8;
//	scene->getCamera().setOrthographicParams(x,-x, -x,x, 4, 15);
//	scene->getCamera().setUsePerspective(false);

	scene->setShadowPV(scene->getCamera().getPVMatrix());

	fbShadows.enable();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForShadows();
	fbShadows.disable();

	scene->setOverwriteShader(nullptr);
	//glCullFace(GL_BACK);

	blur();

	scene->getCamera().pop();
	texShadows->bind(7);
	scene->setClearColor(oldClearColor);

}

void ShadowRendererESM::blur() {
	fx.setShader(sBlurH);
	fx.process(texShadows, texShadowsBlur, texW, texH);
	fx.setShader(sBlurV);
	fx.process(texShadowsBlur, texShadows, texW, texH);
	//texShadows->generateMipMaps();
}



std::string ShadowRendererESM::glslBlurH(const int size) {
	return glslBlur(size, texW, 0);
}

std::string ShadowRendererESM::glslBlurV(const int size) {
	return glslBlur(size, texH, 1);
}

std::string ShadowRendererESM::glslBlur(const int size, const int texSize, const int dir) {

	std::stringstream ss;
	ss << "#version 330 core\n";
	ss << "in vec2 uv;\n";
	ss << "uniform sampler2D tex;\n";
	ss << "out float color;\n";
	ss << "void main() {\n";
	ss << "\tcolor = 0;\n";

	// linear bluring (non-gaussian)
	const float mul = 1.0 / (2*size+1);

	for (int i = -size; i <= +size; ++i) {
		ss << "\tcolor += texture(tex, uv + vec2(";
		if (dir == 0) {
			ss << i << ".0 / " << float(texSize) << ".0, 0";		// horizontal
		} else {
			ss << "0, " << float(i) << "/" << float(texSize);		// vertical
		}
		ss << ")).r * " << mul << ";\n";
	}

	ss << "}\n";

	std::cout << ss.str() << std::endl;
	return ss.str();

}

std::string ShadowRendererESM::glslBlurVertex() {
	return R"(
		#version 330 core
		layout(location = 0) in vec3 vertex;
		layout(location = 2) in vec2 texCoord;
		out vec2 uv;
		void main(){
			gl_Position = vec4(vertex, 1);
			uv = texCoord;
		}
	)";
}

std::string ShadowRendererESM::glslShadowMapFragment() {
	std::stringstream ss;
	ss << "#version 330 core\n";
	ss << "out float color;\n";
	ss << "in vec4 vertex_P;\n";
	ss << "void main() {\n";
													// gl_FragCoord.z's scale is NOT LINEAR (only when using orthographic projection??)\n";
	ss << "\tfloat depth = gl_FragCoord.z;\n";		// is the same as (vertex_P.z/vertex_P.w*0.5+0.5) ?!
	ss << "\tcolor = exp(" << exp << " * depth);\n";
	ss << "}\n";
	return ss.str();
}

std::string ShadowRendererESM::glslShadowMapVertex() {
	return R"(
		#version 330 core
		layout(location = 0) in vec3 vertex;
		uniform mat4 M;
		uniform mat4 V;
		uniform mat4 P;
		out vec4 vertex_P;
		void main(){
			vertex_P = P*V*M * vec4(vertex, 1);	// the projection of input vertex
			gl_Position = vertex_P;
		}
	)";
}

std::string ShadowRendererESM::getShadowAmountCalculationGLSL() {
	std::stringstream ss;

	ss << "in vec4 shadowCoord;\n";

	ss << "uniform sampler2D texShadowMap;\n";

	ss << "float getShadowAmount() {\n";
	ss << "if (shadowCoord.w <= 0.0) {return 1.0;}\n";										// if we are behind the light, no shadowing is possible (prevents artifacts)
	ss << "vec3 sc = shadowCoord.xyz / shadowCoord.w;\n";									// sc is within range [0,1] as shadowCoord was created using a biased matrix
	ss << "float expOccluderDistanceToLight = texture(texShadowMap, sc.xy).r;\n";			// get exponential distance from the shadowmap
	ss << "if (expOccluderDistanceToLight == 0) {return 1.0;}\n";							// ignore regions filled with the clear-color
	ss << "float curDistToLight = sc.z;\n";													// get the current fragment's distance to the light-source
	ss << "float x = expOccluderDistanceToLight * exp(" << -exp << " * curDistToLight);\n";	// get the shadow amount by comparing both distances
	ss << "return clamp(x, 0, 1);\n";
	ss << "}\n";
	return ss.str();
}


#endif // SHADOWRENDERERESM_H
