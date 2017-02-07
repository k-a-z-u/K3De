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

	// one shadow texture per light
	Texture* texShadows[MAX_LIGHTS];

	Framebuffer fbShadows;
	Shader* sShadowGen;

	Scene* scene;

public:

	/** ctor */
	inline ShadowRendererSimple(Scene* scene);

	inline void update() override;

	/** get the shadow-map-texture for the given light */
	inline Texture* getShadowTexture(const int lightIdx) {return texShadows[lightIdx];}

	inline std::string getShadowAmountCalculationGLSL() override;

};

#include "Scene.h"
#include "../Engine.h"

ShadowRendererSimple::ShadowRendererSimple(Scene* scene) {

	this->scene = scene;
	//texW = 512;
	//texH = 512;
	texW = 1024;
	texH = 1024;
	//texW = Engine::get()->getSettings().screen.width;
	//texH = Engine::get()->getSettings().screen.height;

	// construct one 2D shadow-map texture per light
	// [unfortunately one 3D texture for all lights does NOT work for depth-texture!, color-texture would probably work..]
	for (int i = 0; i < MAX_LIGHTS; ++i) {
		texShadows[i] = scene->getTextureFactory().createDepthTexture(texW, texH);
		texShadows[i]->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
		texShadows[i]->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
	}

	//fbShadows.attachTextureDepth(texShadows);

	sShadowGen = scene->getShaderFactory().createFromCode(
		#include "../material/inc/shadowMapVertex.glsl"
		,
		#include "../material/inc/shadowMapFragment.glsl"
	);


}


void ShadowRendererSimple::update() {

	//glCullFace(GL_FRONT);
	scene->setOverwriteShader(sShadowGen);

	for (int i = 0; i < MAX_LIGHTS; ++i) {

		// skip inactive lights
		if (!scene->getLight(i).isActive()) {continue;}

		// skip non shadow casting lights
		if (!scene->getLight(i).getCastsShadows()) {continue;}

		// render
		scene->getCamera().push();

		scene->getCamera().setScreenSize(texW, texH);
		scene->getCamera().setPosition(scene->getLight(i).getPosition());
		scene->setShadowPV(scene->getCamera().getPVMatrix(), i);

		// attach corresponding texture
		fbShadows.attachTextureDepth(texShadows[i]);

		fbShadows.enable();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->renderForShadows();
		fbShadows.disable();

		scene->setOverwriteShader(nullptr);
		//glCullFace(GL_BACK);

		scene->getCamera().pop();
		//texShadows->bind(7);		// ???

	}

}

std::string ShadowRendererSimple::getShadowAmountCalculationGLSL() {

	const float size = 0.22f;		// TODO: make flexible?
	//const int numSamples = 16;
	const float darkness = 0.25f;	// darkest possible vlaue

	std::stringstream ss;

	ss << "in vec4 shadowCoord;\n";

	ss << "uniform sampler2D texShadowMap;\n";

	// is the given fragment shadowed?
	ss << "float isShadowed(const vec3 sc) {\n";
		ss << "\tfloat nearest = texture(texShadowMap, sc.st).r;\n";			// smallest distance from this given (x,y) to the light
		ss << "\tfloat curDist = sc.z;\n";										// current distance to the light
		ss << "\treturn (nearest <= curDist) ? ("<<darkness<<") : (1.0f);\n";	// shadowed?
	ss << "}\n";

	ss << "float getShadowDist(const vec3 sc) {\n";
		ss << "\tfloat nearest = texture(texShadowMap, sc.st).r;\n";			// smallest distance from this given (x,y) to the light
		ss << "\tfloat curDist = sc.z;\n";										// current distance to the light
		ss << "\treturn (curDist - nearest);\n";								// shadowed?
	ss << "}\n";



	ss << "float getShadowAmount() {\n"; {

		ss << "\tif (shadowCoord.w <= 0.0) {return 1.0;}\n";							// if we are behind the light, no shadowing is possible (prevents artifacts)

		ss << "\tvec3 sc = shadowCoord.xyz / shadowCoord.w;\n";
		ss << "\tsc.z -= 0.00025f;\n";													// add some clearance to prevent almost-equal-z artifcats



//		ss << "\tfloat dist = getShadowDist(sc);\n";									// get difference between dist-to-light and dist-to-shadow
//		ss << "\tfloat ss = clamp(dist*256.0 / shadowCoord.w, 0.01, 0.5);\n";				// dynamically "smooth" the shadow
//		// distance test
//		ss << "if (dist < 0.0) {return 1.0;}\n";
//		ss << "if (dist < 0.0005) {return 0.0;}\n";
//		ss << "return 0.5;\n";



		ss << "\tfloat s = "<<size<<" / shadowCoord.w;\n";							// "smooth" the shadow

		// fixed scatter pattern looks better than a random pattern
		ss << "\tfloat sum = 0.0f;\n";

//		for (int i = 0; i < 8; ++i) {
//			const float rad = (float)i/8.0f * M_PI * 2;
//			const float ox = std::cos(rad) * 0.1f;
//			const float oy = std::sin(rad) * 0.1f;
//			ss << "\tsum += isShadowed(sc + vec3(" << ox << ", " << oy << ", 0.0f) * s);\n";
//		}

		ss << "\tsum += isShadowed(sc + vec3(-0.1f,  0.0f, 0.0f) * s);\n";
		ss << "\tsum += isShadowed(sc + vec3(+0.1f,  0.0f, 0.0f) * s);\n";
		ss << "\tsum += isShadowed(sc + vec3( 0.0f, +0.1f, 0.0f) * s);\n";
		ss << "\tsum += isShadowed(sc + vec3( 0.0f, -0.1f, 0.0f) * s);\n";

		ss << "\tsum += isShadowed(sc + vec3(-0.2f, +0.2f, 0.0f) * s);\n";
		ss << "\tsum += isShadowed(sc + vec3(+0.2f, +0.2f, 0.0f) * s);\n";
		ss << "\tsum += isShadowed(sc + vec3(-0.2f, -0.2f, 0.0f) * s);\n";
		ss << "\tsum += isShadowed(sc + vec3(+0.2f, -0.2f, 0.0f) * s);\n";

		ss << "\tsum += isShadowed(sc);\n";

		ss << "\treturn sum / 9.0f;\n";


//		ss << "\tfloat s = "<<size<<" / shadowCoord.w;\n";							// "smooth" the shadow
//		ss << "\tfloat sum = 0.0f;\n";

//		for (int i = 0; i < numSamples; ++i) {
//			const float x = Random::get(-1,+1);
//			const float y = Random::get(-1,+1);
//			ss << "\tsum += isShadowed(sc + vec3(s*"<<x<<"f,s*"<<y<<"f, 0.0f));\n";
//		}
//		ss << "\tsum += isShadowed(sc + vec3( 0.0f, 0.0f, 0.0f));\n";
//		ss << "\treturn sum/" << numSamples <<".0f;\n";

	} ss << "}\n";

	return ss.str();
}

#endif // SHADOWRENDERERSIMPLE_H
