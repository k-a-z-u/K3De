#ifndef SHADOWRENDERERSIMPLE_H
#define SHADOWRENDERERSIMPLE_H

#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"
#include "../math/Random.h"
#include <sstream>

class Scene;

class ShadowRendererSimple : public ShadowRenderer {

private:

	friend class Scene;

	int texW;
	int texH;

	Framebuffer fbShadows;
	Texture* texShadows;
	Shader* sShadowGen;

	Scene* scene;

public:

	/** ctor */
	ShadowRendererSimple(Scene* scene);

	void update() override;

	Texture* getShadowTexture() override {return texShadows;}

	std::string getShadowAmountCalculationGLSL() override;

};

#include "Scene.h"
#include "../Engine.h"

ShadowRendererSimple::ShadowRendererSimple(Scene* scene) {

	this->scene = scene;
	texW = 512;//Engine::get()->getSettings().screen.width;
	texH = 512;//Engine::get()->getSettings().screen.height;

	texShadows = scene->getTextureFactory().createDepthTexture(texW, texH);
	texShadows->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texShadows->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);

	fbShadows.attachTextureDepth(texShadows);
	sShadowGen = scene->getShaderFactory().create("shaderShadowMapVertex.glsl", "shaderShadowMapFragment.glsl");

}


void ShadowRendererSimple::update() {

	scene->getCamera().push();

	scene->getCamera().setScreenSize(texW, texH);
	//glCullFace(GL_FRONT);
	scene->setOverwriteShader(sShadowGen);
	scene->getCamera().setPosition(scene->getLight(0).getPosition());
	scene->setShadowPV(scene->getCamera().getPVMatrix());

	fbShadows.enable();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForShadows();
	fbShadows.disable();

	scene->setOverwriteShader(nullptr);
	//glCullFace(GL_BACK);

	scene->getCamera().pop();
	texShadows->bind(7);

}

std::string ShadowRendererSimple::getShadowAmountCalculationGLSL() {
	std::stringstream ss;

	ss << "in vec4 shadowCoord;\n";

	ss << "uniform sampler2D texShadowMap;\n";

	// is the given fragment shadowed?
	ss << "float isShadowed(const vec3 sc) {\n";
	ss << "\tfloat nearest = texture(texShadowMap, sc.st).r;\n";		// smallest distance from this given (x,y) to the light
	ss << "\tfloat curDist = sc.z;\n";									// current distance to the light
	//ss << "\tif(nearest<curDist) {\n";
	//ss << "\t return pow((curDist-nearest), 0.5);\n";
	//ss << "} return 1.0;\n";
	//ss << "\treturn clamp((curDist-nearest), 0,1);\n";
	ss << "\treturn (nearest < curDist) ? (0.25) : (1.0);\n";			// shadowed?
	ss << "}\n";

	const float size = 0.15;
	const int numSamples = 12;

	ss << "float getShadowAmount() {\n";
	ss << "\tif (shadowCoord.w <= 0.0) {return 1.0;}\n";				// if we are behind the light, no shadowing is possible (prevents artifacts)
	ss << "\tvec3 sc = shadowCoord.xyz / shadowCoord.w;\n";
	ss << "\tsc.z -= 0.0002;\n";										// add some clearance to prevent almost-equal-z artifcats
	ss << "\tfloat s = "<<size<<" / shadowCoord.w;\n";						// "smooth" the shadow
	ss << "\tfloat sum = 0;\n";

	for (int i = 0; i < numSamples; ++i) {
		const float x = Random::get(0,1);
		const float y = Random::get(0,1);
		ss << "\tsum += isShadowed(sc + vec3(s*"<<x<<",s*"<<y<<", 0));\n";
	}
	ss << "\tsum += isShadowed(sc + vec3( 0, 0, 0));\n";
	//ss << "\tsum += isShadowed(sc + vec3(+s,+s, 0));\n";
	//ss << "\tsum += isShadowed(sc + vec3(-s,-s, 0));\n";
	//ss << "\tsum += isShadowed(sc + vec3(-s,+s, 0));\n";
	//ss << "\tsum += isShadowed(sc + vec3(+s,-s, 0));\n";
	ss << "\treturn sum/" << numSamples <<";\n";
	ss << "}\n";

	return ss.str();
}

#endif // SHADOWRENDERERSIMPLE_H
