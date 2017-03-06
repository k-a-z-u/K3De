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

	int texW = 0;
	int texH = 0;

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
	inline Texture* getShadowTexture(const int lightIdx) override {return texShadows[lightIdx];}

	/** update underlying data when the scene was resized */
	inline void resize(const int w, const int h) override;

};

#include "Scene.h"
#include "../Engine.h"

ShadowRendererSimple::ShadowRendererSimple(Scene* scene) {

	this->scene = scene;
	//texW = 512;
	//texH = 512;
	//texW = Engine::get()->getSettings().screen.width;
	//texH = Engine::get()->getSettings().screen.height;

	// ensure all textures are nullptrs
	std::fill(&texShadows[0], &texShadows[MAX_LIGHTS], nullptr);

	resize(1024, 1024);

	//fbShadows.attachTextureDepth(texShadows);

	sShadowGen = scene->getShaderFactory().createFromCode(
		#include "../material/inc/shadowMapVertex.glsl"
		,
		#include "../material/inc/shadowMapFragment.glsl"
	);

}

void ShadowRendererSimple::resize(const int w, const int h) {

	(void) w;
	(void) h;

	// new size. currently we use a fixed size, independent of the scene's size
	const int nTexW = 1024;
	const int nTexH = 1024;

	// not changed? -> skip
	if (nTexW == texW && nTexH == texH) {return;}

	// store new size
	texW = nTexW;
	texH = nTexH;

	// construct one 2D shadow-map texture per light
	// [unfortunately one 3D texture for all lights does NOT work for depth-texture!, color-texture would probably work..]
	for (int i = 0; i < MAX_LIGHTS; ++i) {

		// allocate or resize
		if (!texShadows[i]) {
			texShadows[i] = scene->getTextureFactory().createDepthTexture(texW, texH);
		} else {
			texShadows[i]->bind(0);
			texShadows[i]->resize(texW, texH);
		}

		// create the new texture
		texShadows[i] = scene->getTextureFactory().createDepthTexture(texW, texH);
		texShadows[i]->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
		texShadows[i]->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
		texShadows[i]->unbind(0);

	}

}

void ShadowRendererSimple::update() {

	//glCullFace(GL_FRONT);
	scene->setOverwriteShader(sShadowGen);

	// store current camera
	scene->getCamera().push();

	// switch to shadow's resolution
	scene->getCamera().setScreenSize(texW, texH);

	// enable framebuffer rendering
	fbShadows.bind();

	// only shadow, no color
	fbShadows.noColor();

	// process each potential light
	for (int i = 0; i < MAX_LIGHTS; ++i) {

		// skip inactive lights
		if (!scene->getLight(i).isActive()) {continue;}

		// skip non-shadow-casting lights
		if (!scene->getLight(i).getCastsShadows()) {continue;}

		// setup
		scene->getCamera().setPosition(scene->getLight(i).getPosition());
		scene->setShadowPV(scene->getCamera().getPVMatrix(), i);

		// attach corresponding texture
		fbShadows.attachTextureDepth(texShadows[i]);

		// render
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		scene->renderForShadows();

	}

	// disable framebuffer rendering
	fbShadows.unbind();

	// restore
	scene->setOverwriteShader(nullptr);
	scene->getCamera().pop();

}

#endif // SHADOWRENDERERSIMPLE_H
